/*
 * Copyright (c) 2019 - 2022 Geode-solutions
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <geode/model/helpers/detail/cut_along_internal_lines.h>

#include <async++.h>

#include <geode/basic/pimpl_impl.h>

#include <geode/geometry/point.h>

#include <geode/mesh/builder/surface_mesh_builder.h>
#include <geode/mesh/core/edged_curve.h>
#include <geode/mesh/core/surface_mesh.h>

#include <geode/model/helpers/component_mesh_edges.h>
#include <geode/model/mixin/core/line.h>
#include <geode/model/mixin/core/surface.h>
#include <geode/model/representation/builder/brep_builder.h>
#include <geode/model/representation/builder/section_builder.h>
#include <geode/model/representation/core/brep.h>
#include <geode/model/representation/core/section.h>

namespace geode
{
    namespace detail
    {
        template < typename Model, typename ModelBuilder, index_t dimension >
        class CutAlongInternalLines< Model, ModelBuilder, dimension >::Impl
        {
            using MCVmapping = std::pair< MeshComponentVertex, index_t >;
            using MCVmappings = std::vector< MCVmapping >;
            using Task = async::task< MCVmappings >;
            struct SurfaceInfo
            {
                SurfaceInfo( index_t nb_vertices )
                    : polygon_vertices( nb_vertices )
                {
                }
                absl::FixedArray< PolygonsAroundVertex > polygon_vertices;
                std::vector< index_t > vertices_to_check;
            };

        public:
            Impl( Model& model ) : model_( model ), builder_{ model } {}

            void cut()
            {
                absl::FixedArray< Task > tasks( model_.nb_surfaces() );
                index_t count{ 0 };
                for( const auto& surface : model_.surfaces() )
                {
                    tasks[count++] = async::spawn( [this, &surface] {
                        return split_points( surface );
                    } );
                }
                async::when_all( tasks.begin(), tasks.end() )
                    .then(
                        [this]( async::task< std::vector< Task > > all_task ) {
                            for( auto& task : all_task.get() )
                            {
                                update_unique_vertices( task.get() );
                            }
                        } )
                    .wait();
            }

            void cut_surface( const Surface< dimension >& surface )
            {
                const auto mapping = split_points( surface );
                update_unique_vertices( mapping );
            }

        private:
            void update_unique_vertices( const MCVmappings& mapping )
            {
                for( const auto& mcv : mapping )
                {
                    builder_.set_unique_vertex( mcv.first, mcv.second );
                }
            }

            MCVmappings split_points( const Surface< dimension >& surface )
            {
                auto builder = builder_.surface_mesh_builder( surface.id() );
                remove_adjacencies_along_internal_lines( surface, *builder );
                return duplicate_points( surface, *builder );
            }

            MCVmappings duplicate_points( const Surface< dimension >& surface,
                SurfaceMeshBuilder< dimension >& builder )
            {
                const auto& mesh = surface.mesh();
                const auto info = compute_surface_info( mesh );
                MCVmappings mapping;
                mapping.reserve( info.vertices_to_check.size() );
                for( const auto vertex_id : info.vertices_to_check )
                {
                    auto polygons_around =
                        mesh.polygons_around_vertex( vertex_id );
                    const auto& polygon_vertices =
                        info.polygon_vertices[vertex_id];
                    OPENGEODE_ASSERT(
                        polygons_around.size() <= polygon_vertices.size(),
                        "[CutAlongInternalLines] Wrong size comparison" );
                    auto nb_polygons_around = polygons_around.size();
                    PolygonsAroundVertex total_polygons;
                    while( nb_polygons_around != polygon_vertices.size() )
                    {
                        for( auto& polygon : polygons_around )
                        {
                            total_polygons.emplace_back( std::move( polygon ) );
                        }
                        mapping.emplace_back(
                            process_component( surface, mesh, builder,
                                vertex_id, total_polygons, polygon_vertices ) );
                        polygons_around =
                            mesh.polygons_around_vertex( vertex_id );
                        nb_polygons_around += polygons_around.size();
                    }
                }
                return mapping;
            }

            SurfaceInfo compute_surface_info(
                const SurfaceMesh< dimension >& mesh ) const
            {
                SurfaceInfo info{ mesh.nb_vertices() };
                std::vector< bool > vertex_to_check(
                    mesh.nb_vertices(), false );
                for( const auto p : Range{ mesh.nb_polygons() } )
                {
                    for( const auto v :
                        LRange{ mesh.nb_polygon_vertices( p ) } )
                    {
                        PolygonVertex vertex{ p, v };
                        const auto vertex_id = mesh.polygon_vertex( vertex );
                        if( mesh.is_edge_on_border( vertex ) )
                        {
                            vertex_to_check[vertex_id] = true;
                        }
                        info.polygon_vertices[vertex_id].emplace_back(
                            std::move( vertex ) );
                    }
                }
                for( const auto v : Indices{ vertex_to_check } )
                {
                    if( vertex_to_check[v] )
                    {
                        info.vertices_to_check.push_back( v );
                    }
                }
                return info;
            }

            void remove_adjacencies_along_internal_lines(
                const Surface< dimension >& surface,
                SurfaceMeshBuilder< dimension >& builder )
            {
                std::vector< PolygonEdge > edges;
                for( const auto& line : model_.internal_lines( surface ) )
                {
                    const auto& mesh = line.mesh();
                    for( const auto e : Range{ mesh.nb_edges() } )
                    {
                        const auto model_edges =
                            component_mesh_edges( model_, line, e );
                        const auto& surface_edges = model_edges.surface_edges;
                        const auto it = surface_edges.find( surface.id() );
                        if( it == surface_edges.end() )
                        {
                            continue;
                        }
                        for( auto& edge : it->second )
                        {
                            edges.emplace_back( std::move( edge ) );
                        }
                    }
                }
                for( const auto& edge : edges )
                {
                    builder.unset_polygon_adjacent( edge );
                }
            }

            MCVmapping process_component( const Surface< dimension >& surface,
                const SurfaceMesh< dimension >& mesh,
                SurfaceMeshBuilder< dimension >& builder,
                index_t vertex_id,
                const PolygonsAroundVertex& total_polygons,
                const PolygonsAroundVertex& polygon_vertices )
            {
                const auto new_vertex_id =
                    builder.create_point( mesh.point( vertex_id ) );
                builder.replace_vertex( vertex_id, new_vertex_id );
                for( const auto& polygon_vertex : polygon_vertices )
                {
                    if( absl::c_find( total_polygons, polygon_vertex )
                        == total_polygons.end() )
                    {
                        builder.associate_polygon_vertex_to_vertex(
                            polygon_vertex, vertex_id );
                        break;
                    }
                }
                OPENGEODE_ASSERT(
                    !mesh.polygons_around_vertex( vertex_id ).empty(),
                    "[ModelFromMeshBuilder::cut_surface_by_lines] Lost "
                    "polygon around vertex" );
                const auto uid = model_.unique_vertex(
                    { surface.component_id(), vertex_id } );
                return { MeshComponentVertex{
                             surface.component_id(), new_vertex_id },
                    uid };
            }

        private:
            const Model& model_;
            ModelBuilder builder_;
        };

        template < typename Model, typename ModelBuilder, index_t dimension >
        CutAlongInternalLines< Model, ModelBuilder, dimension >::
            CutAlongInternalLines( Model& model )
            : impl_( model )
        {
        }

        template < typename Model, typename ModelBuilder, index_t dimension >
        CutAlongInternalLines< Model, ModelBuilder, dimension >::
            ~CutAlongInternalLines()
        {
        }

        template < typename Model, typename ModelBuilder, index_t dimension >
        void CutAlongInternalLines< Model, ModelBuilder, dimension >::
            cut_all_surfaces()
        {
            impl_->cut();
        }

        template < typename Model, typename ModelBuilder, index_t dimension >
        void CutAlongInternalLines< Model, ModelBuilder, dimension >::
            cut_surface( const Surface< dimension >& surface )
        {
            impl_->cut_surface( surface );
        }

        template class opengeode_model_api
            CutAlongInternalLines< Section, SectionBuilder, 2 >;
        template class opengeode_model_api
            CutAlongInternalLines< BRep, BRepBuilder, 3 >;
    } // namespace detail
} // namespace geode