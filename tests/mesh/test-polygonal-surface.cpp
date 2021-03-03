/*
 * Copyright (c) 2019 - 2021 Geode-solutions
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

#include <geode/basic/attribute_manager.h>
#include <geode/basic/logger.h>

#include <geode/geometry/bounding_box.h>
#include <geode/geometry/point.h>
#include <geode/geometry/vector.h>

#include <geode/mesh/builder/geode_polygonal_surface_builder.h>
#include <geode/mesh/builder/surface_edges_builder.h>
#include <geode/mesh/core/geode_polygonal_surface.h>
#include <geode/mesh/core/polygonal_surface.h>
#include <geode/mesh/core/surface_edges.h>
#include <geode/mesh/io/polygonal_surface_input.h>
#include <geode/mesh/io/polygonal_surface_output.h>

#include <geode/tests/common.h>

void test_create_vertices( const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    builder.create_point( { { 0.1, 0.2, 0.3 } } );
    builder.create_point( { { 2.1, 9.4, 6.7 } } );
    builder.create_point( { { 7.5, 5.2, 6.3 } } );
    builder.create_point( { { 8.1, 1.4, 4.7 } } );
    builder.create_point( { { 4.7, 2.1, 1.3 } } );
    builder.create_point( { { 9.3, 5.3, 6.7 } } );
    builder.create_point( { { 7.5, 4.2, 2.8 } } );
    OPENGEODE_EXCEPTION( polygonal_surface.isolated_vertex( 0 ),
        "[Test] Vertices should be isolated before polygons creation" );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_vertices() == 7,
        "[Test] PolygonalSurface should have 7 vertices" );
}

void test_bounding_box( const geode::PolygonalSurface3D& polygonal_surface )
{
    geode::Point3D answer_min{ { 0.1, 0.2, 0.3 } };
    geode::Point3D answer_max{ { 9.3, 9.4, 6.7 } };
    OPENGEODE_EXCEPTION( polygonal_surface.bounding_box().min() == answer_min,
        "[Test] Wrong computation of bounding box (min)" );
    OPENGEODE_EXCEPTION( polygonal_surface.bounding_box().max() == answer_max,
        "[Test] Wrong computation of bounding box (max)" );
}

void test_create_vertex_attribute(
    const geode::PolygonalSurface3D& polygonal_surface )
{
    auto attribute =
        polygonal_surface.vertex_attribute_manager()
            .find_or_create_attribute< geode::VariableAttribute,
                geode::PolygonEdge >( "test", geode::PolygonEdge{} );
    for( const auto v : geode::Range{ polygonal_surface.nb_vertices() } )
    {
        attribute->set_value( v, geode::PolygonEdge{ v, 0 } );
        OPENGEODE_EXCEPTION( geode::PolygonVertex{} != attribute->value( v ),
            "[Test] PolygonalSurface attribute assignation is not correct" );
    }
}

void test_permutation( const geode::PolygonalSurface3D& surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    std::vector< geode::index_t > vertex_permutation{ 4, 2, 6, 1, 5, 0, 3 };
    builder.permute_vertices( vertex_permutation );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 0 } ) == 5,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 1 } ) == 3,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 2 } ) == 1,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 0 } ) == 3,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 1 } ) == 6,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 2 } ) == 0,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 3 } ) == 1,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 0 } ) == 3,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 1 } ) == 4,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 2 } ) == 2,
        "[Test] Wrong PolygonVertex after vertex permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 3 } ) == 6,
        "[Test] Wrong PolygonVertex after vertex permute" );

    std::vector< geode::index_t > polygon_permutation{ 2, 0, 1 };
    builder.permute_polygons( polygon_permutation );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 0 } ) == 3,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 1 } ) == 4,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 2 } ) == 2,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 0, 3 } ) == 6,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 0 } ) == 5,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 1 } ) == 3,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 1, 2 } ) == 1,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 0 } ) == 3,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 1 } ) == 6,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 2 } ) == 0,
        "[Test] Wrong PolygonVertex after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_vertex( { 2, 3 } ) == 1,
        "[Test] Wrong PolygonVertex after polygon permute" );

    OPENGEODE_EXCEPTION( surface.polygon_adjacent( { 0, 3 } ) == 2,
        "[Test] Wrong Adjacency after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_adjacent( { 2, 0 } ) == 0,
        "[Test] Wrong Adjacency after polygon permute" );
    OPENGEODE_EXCEPTION( surface.polygon_adjacent( { 1, 1 } ) == 2,
        "[Test] Wrong Adjacency after polygon permute" );
    OPENGEODE_EXCEPTION( !surface.polygon_adjacent( { 1, 0 } ),
        "[Test] Wrong Adjacency after polygon permute" );

    const auto polygons_3 = surface.polygons_around_vertex( 3 );
    OPENGEODE_EXCEPTION( polygons_3.size() == 3,
        "[Test] Wrong polygons_3 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_3[0].polygon_id == 0,
        "[Test] Wrong polygons_3 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_3[0].vertex_id == 0,
        "[Test] Wrong polygons_3 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_3[1].polygon_id == 2,
        "[Test] Wrong polygons_3 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_3[1].vertex_id == 0,
        "[Test] Wrong polygons_3 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_3[2].polygon_id == 1,
        "[Test] Wrong polygons_3 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_3[2].vertex_id == 1,
        "[Test] Wrong polygons_3 after polygon permute" );

    const auto polygons_6 = surface.polygons_around_vertex( 6 );
    OPENGEODE_EXCEPTION( polygons_6.size() == 2,
        "[Test] Wrong polygons_6 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_6[0].polygon_id == 0,
        "[Test] Wrong polygons_6 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_6[0].vertex_id == 3,
        "[Test] Wrong polygons_6 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_6[1].polygon_id == 2,
        "[Test] Wrong polygons_6 after polygon permute" );
    OPENGEODE_EXCEPTION( polygons_6[1].vertex_id == 1,
        "[Test] Wrong polygons_6 after polygon permute" );
}

void test_delete_vertex( const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    std::vector< bool > to_delete( polygonal_surface.nb_vertices(), false );
    to_delete.front() = true;
    builder.delete_vertices( to_delete );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_vertices() == 6,
        "[Test] PolygonalSurface should have 6 vertices" );
    const geode::Point3D answer{ { 2.1, 9.4, 6.7 } };
    OPENGEODE_EXCEPTION( polygonal_surface.point( 2 ) == answer,
        "[Test] PolygonalSurface vertex coordinates are not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_polygons() == 2,
        "[Test] PolygonalSurface should have 2 polygons" );
    OPENGEODE_EXCEPTION( !polygonal_surface.polygon_adjacent( { 1, 2 } ),
        "[Test] PolygonalSurface adjacent index is not correct" );
    builder.edges_builder().delete_isolated_edges();
    OPENGEODE_EXCEPTION( polygonal_surface.edges().nb_edges() == 7,
        "[Test] PolygonalSurface should have 7 edges" );

    const auto attribute = polygonal_surface.edges()
                               .edge_attribute_manager()
                               .find_attribute< geode::index_t >( "test" );
    OPENGEODE_EXCEPTION( attribute->value( 4 ) == 6,
        "[Test] Update of edge attributes after "
        "vertex deletion is not correct (value of 4)" );
    OPENGEODE_EXCEPTION( attribute->value( 5 ) == 7,
        "[Test] Update of edge attributes after "
        "vertex deletion is not correct (value of 5)" );
    OPENGEODE_EXCEPTION( attribute->value( 6 ) == 8,
        "[Test] Update of edge attributes after "
        "vertex deletion is not correct (value of 6)" );
    OPENGEODE_EXCEPTION(
        polygonal_surface.edges().edge_from_vertices(
            polygonal_surface.polygon_edge_vertices( { 1, 0 } ) )
            == 0,
        "[Test] Edge index for PolygonEdge( 1, 0 ) is not correct" );
    OPENGEODE_EXCEPTION(
        polygonal_surface.edges().edge_from_vertices(
            polygonal_surface.polygon_edge_vertices( { 1, 2 } ) )
            == 2,
        "[Test] Edge index for PolygonEdge( 1, 3 ) is not correct" );
}

void test_create_polygons( const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    builder.create_polygon( { 0, 1, 2 } );
    builder.create_polygon( { 1, 3, 4, 2 } );
    builder.create_polygon( { 1, 5, 6, 3 } );
    OPENGEODE_EXCEPTION( !polygonal_surface.isolated_vertex( 0 ),
        "[Test] Vertices should not be isolated after polygons creation" );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_polygons() == 3,
        "[Test] PolygonalSurface should have 3 polygons" );
    OPENGEODE_EXCEPTION( polygonal_surface.edges().nb_edges() == 9,
        "[Test] PolygonalSurface should have 9 edges" );
}

void test_create_edge_attribute(
    const geode::PolygonalSurface3D& polygonal_surface )
{
    auto attribute = polygonal_surface.edges()
                         .edge_attribute_manager()
                         .find_or_create_attribute< geode::VariableAttribute,
                             geode::index_t >( "test", geode::NO_ID );
    for( const auto e : geode::Range{ polygonal_surface.edges().nb_edges() } )
    {
        attribute->set_value( e, e );
    }
}

void test_polygon_adjacencies(
    const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    builder.compute_polygon_adjacencies();
    OPENGEODE_EXCEPTION( !polygonal_surface.polygon_adjacent( { 0, 0 } ),
        "[Test] PolygonalSurface adjacent index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_adjacent( { 0, 1 } ) == 1,
        "[Test] PolygonalSurface adjacent index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_adjacent( { 1, 3 } ) == 0,
        "[Test] PolygonalSurface adjacent index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_adjacent_edge( { 0, 1 } )
                             == geode::PolygonEdge( 1, 3 ),
        "[Test] PolygonalSurface adjacent index is not correct" );
    OPENGEODE_EXCEPTION(
        polygonal_surface.edges().edge_from_vertices(
            polygonal_surface.polygon_edge_vertices( { 0, 1 } ) )
            == polygonal_surface.edges().edge_from_vertices(
                polygonal_surface.polygon_edge_vertices( { 1, 3 } ) ),
        "[Test] PolygonalSurface edge indices is not correct" );

    OPENGEODE_EXCEPTION( !polygonal_surface.polygon_adjacent( { 2, 0 } ),
        "[Test] PolygonalSurface adjacent index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_adjacent( { 2, 3 } ) == 1,
        "[Test] PolygonalSurface adjacent index is not correct" );

    OPENGEODE_EXCEPTION(
        polygonal_surface.polygons_around_vertex( 1 ).size() == 3,
        "[Test] PolygonalSurface should have 3 polygons around this vertex" );
}

void test_polygon_edges_on_borders(
    const geode::PolygonalSurface3D& polygonal_surface )
{
    const auto result = polygonal_surface.polygon_edges_on_border( 0 );
    OPENGEODE_EXCEPTION( result.size() == 2,
        "[Test] Number of polygon edges on border index is not correct" );
    OPENGEODE_EXCEPTION(
        absl::c_find( result, geode::PolygonEdge( 0, 0 ) ) != result.end()
            && absl::c_find( result, geode::PolygonEdge( 0, 2 ) )
                   != result.end(),
        "[Test] Polygon edge indices on border index is not correct" );
}

void test_previous_next_on_border(
    const geode::PolygonalSurface3D& polygonal_surface )
{
    OPENGEODE_EXCEPTION( polygonal_surface.previous_on_border( { 0, 0 } )
                             == geode::PolygonEdge( 0, 2 ),
        "[Test] Previous edge on border index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.next_on_border( { 0, 0 } )
                             == geode::PolygonEdge( 2, 0 ),
        "[Test] Next edge on border index is not correct" );
}

void test_polygon_edge_requests(
    const geode::PolygonalSurface3D& polygonal_surface )
{
    OPENGEODE_EXCEPTION( polygonal_surface.edge_length(
                             polygonal_surface.edges().edge_vertices( 0 ) )
                             == std::sqrt( 2 * 2 + 9.2 * 9.2 + 6.4 * 6.4 ),
        "[Test] Polygon edge length is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.edge_barycenter(
                             polygonal_surface.edges().edge_vertices( 0 ) )
                             == geode::Point3D( { 1.1, 4.8, 3.5 } ),
        "[Test] Polygon edge barycenter is not correct" );
    OPENGEODE_EXCEPTION(
        polygonal_surface.polygon_edge_vertex( { 0, 0 }, 1 ) == 1,
        "[Test] Polygon edge vertex index is not correct" );
    auto polygon_edge = polygonal_surface.polygon_edge_from_vertices( 3, 5 );
    OPENGEODE_EXCEPTION(
        !polygon_edge, "[Test] Polygon edge from vertices is not correct" );
    polygon_edge = polygonal_surface.polygon_edge_from_vertices( 0, 1 );
    OPENGEODE_EXCEPTION(
        polygon_edge, "[Test] Polygon edge from vertices is not correct" );
    OPENGEODE_EXCEPTION( polygon_edge == geode::PolygonEdge( 0, 0 ),
        "[Test] Polygon edge from vertices is not correct" );
    polygon_edge = polygonal_surface.polygon_edge_from_vertices( 1, 0 );
    OPENGEODE_EXCEPTION(
        !polygon_edge, "[Test] Polygon edge from vertices is not correct" );
}

void test_delete_polygon( const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    std::vector< bool > to_delete( polygonal_surface.nb_polygons(), false );
    to_delete.front() = true;
    builder.delete_polygons( to_delete );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_polygons() == 1,
        "[Test] PolygonalSurface should have 1 polygon" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_vertex( { 0, 0 } ) == 4,
        "[Test] PolygonalSurface edge vertex index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_vertex( { 0, 1 } ) == 2,
        "[Test] PolygonalSurface edge vertex index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_vertex( { 0, 2 } ) == 0,
        "[Test] PolygonalSurface edge vertex index is not correct" );
    OPENGEODE_EXCEPTION( polygonal_surface.edges().isolated_edge( 0 ),
        "[Test] Edge should be isolated after polygon deletion" );
    builder.edges_builder().delete_isolated_edges();
    OPENGEODE_EXCEPTION( polygonal_surface.edges().nb_edges() == 3,
        "[Test] PolygonalSurface should have  edges" );
    OPENGEODE_EXCEPTION( !polygonal_surface.edges().isolated_edge( 0 ),
        "[Test] Edge should not be isolated after isolated egde deletion" );

    const auto attribute = polygonal_surface.edges()
                               .edge_attribute_manager()
                               .find_attribute< geode::index_t >( "test" );
    OPENGEODE_EXCEPTION( attribute->value( 0 ) == 0,
        "[Test] Update of edge attributes after "
        "polygon deletion is not correct" );
    OPENGEODE_EXCEPTION( attribute->value( 1 ) == 1,
        "[Test] Update of edge attributes after "
        "polygon deletion is not correct" );
    OPENGEODE_EXCEPTION( attribute->value( 2 ) == 2,
        "[Test] Update of edge attributes after "
        "polygon deletion is not correct" );
    OPENGEODE_EXCEPTION(
        polygonal_surface.edges().edge_from_vertices(
            polygonal_surface.polygon_edge_vertices( { 0, 0 } ) )
            == 0,
        "[Test] Edge index for PolygonEdge( 0, 0 ) is not correct" );
    OPENGEODE_EXCEPTION(
        polygonal_surface.edges().edge_from_vertices(
            polygonal_surface.polygon_edge_vertices( { 0, 2 } ) )
            == 2,
        "[Test] Edge index for PolygonEdge( 0, 2 ) is not correct" );
}

void test_polygon_barycenter(
    const geode::PolygonalSurface3D& polygonal_surface )
{
    const geode::Point3D answer( { 5.6, 4.525, 4.75 } );
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_barycenter( 1 ) == answer,
        "[Test] PolygonalSurface polygon barycenter is not correct" );
}

void test_polygon_area()
{
    auto polygonal_surface = geode::PolygonalSurface2D::create(
        geode::OpenGeodePolygonalSurface2D::impl_name_static() );
    auto builder =
        geode::PolygonalSurfaceBuilder2D::create( *polygonal_surface );
    const double a{ 6.0 };
    const double b{ 8.0 };
    const double c{ 4.0 };

    builder->create_point( { { 0.0, 0.0 } } );
    builder->create_point( { { a, 0.0 } } );
    builder->create_point( { { b, c } } );
    builder->create_point( { { 0.0, c } } );

    builder->create_polygon( { 0, 1, 2, 3 } );

    const double answer{ 28. };
    OPENGEODE_EXCEPTION( polygonal_surface->polygon_area( 0 ) == answer,
        "[Test] PolygonalSurface polygon area is not correct" );
}

void test_polygon_normal()
{
    auto polygonal_surface = geode::PolygonalSurface3D::create(
        geode::OpenGeodePolygonalSurface3D::impl_name_static() );
    auto builder =
        geode::PolygonalSurfaceBuilder3D::create( *polygonal_surface );
    const double a{ 6.0 };
    const double b{ 8.0 };
    const double c{ 4.0 };

    builder->create_point( { { 0.0, 0.0, 0.0 } } );
    builder->create_point( { { a, 0.0, 0.0 } } );
    builder->create_point( { { b, c, 0.0 } } );
    builder->create_point( { { 0.0, c, 0.0 } } );

    builder->create_polygon( { 0, 1, 2, 3 } );

    const geode::Vector3D answer{ { 0., 0., 1. } };
    OPENGEODE_EXCEPTION( polygonal_surface->polygon_normal( 0 ) == answer,
        "[Test] PolygonalSurface polygon normal is not correct" );
}

void test_polygon_vertex_normal()
{
    auto polygonal_surface = geode::PolygonalSurface3D::create(
        geode::OpenGeodePolygonalSurface3D::impl_name_static() );
    auto builder =
        geode::PolygonalSurfaceBuilder3D::create( *polygonal_surface );

    builder->create_point( { { 0.0, 0.0, 0.0 } } );
    builder->create_point( { { 1.0, 0.0, 0.0 } } );
    builder->create_point( { { 0.0, 1.0, 1.0 } } );
    builder->create_point( { { 0.0, -1.0, 1.0 } } );

    builder->create_polygon( { 0, 1, 2 } );
    builder->create_polygon( { 0, 3, 1 } );
    builder->compute_polygon_adjacencies();

    const geode::Vector3D answer{ { 0., 0., 1. } };
    OPENGEODE_EXCEPTION(
        polygonal_surface->polygon_vertex_normal( 0 ) == answer,
        "[Test] PolygonalSurface polygon vertex normal is not correct" );
}

void test_io( const geode::PolygonalSurface3D& polygonal_surface,
    const std::string& filename )
{
    geode::save_polygonal_surface( polygonal_surface, filename );
    geode::load_polygonal_surface< 3 >( filename );
    auto new_polygonal_surface = geode::load_polygonal_surface< 3 >(
        geode::OpenGeodePolygonalSurface3D::impl_name_static(), filename );

    OPENGEODE_EXCEPTION( new_polygonal_surface->nb_vertices() == 7,
        "[Test] Reloaded PolygonalSurface should have 7 vertices" );
    OPENGEODE_EXCEPTION( new_polygonal_surface->edges().nb_edges() == 9,
        "[Test] Reloaded PolygonalSurface should have 9 edges" );
    OPENGEODE_EXCEPTION( new_polygonal_surface->nb_polygons() == 3,
        "[Test] Reloaded PolygonalSurface should have 3 polygons" );
    OPENGEODE_EXCEPTION(
        new_polygonal_surface->edges().edge_from_vertices( { 1, 0 } )
            == polygonal_surface.edges().edge_from_vertices( { 1, 0 } ),
        "[Test] Reloaded PolygonalSurface has wrong polygon edge index" );
    const auto attribute = new_polygonal_surface->edges()
                               .edge_attribute_manager()
                               .find_attribute< geode::index_t >( "test" );
    for( const auto e :
        geode::Range{ new_polygonal_surface->edges().nb_edges() } )
    {
        OPENGEODE_EXCEPTION( attribute->value( e ) == e,
            "[Test] Reloaded PolygonalSurface has "
            "wrong attributes on its edges" );
    }
}

void test_backward_io( const std::string& filename )
{
    const auto new_polygonal_surface = geode::load_polygonal_surface< 3 >(
        geode::OpenGeodePolygonalSurface3D::impl_name_static(), filename );

    OPENGEODE_EXCEPTION( new_polygonal_surface->nb_vertices() == 7,
        "[Test] Reloaded PolygonalSurface should have 7 vertices" );
    OPENGEODE_EXCEPTION( new_polygonal_surface->edges().nb_edges() == 9,
        "[Test] Reloaded PolygonalSurface should have 9 edges" );
    OPENGEODE_EXCEPTION( new_polygonal_surface->nb_polygons() == 3,
        "[Test] Reloaded PolygonalSurface should have 3 polygons" );

    OPENGEODE_EXCEPTION(
        new_polygonal_surface->polygons_around_vertex( 1 ).size() == 3,
        "[Test] Backward polygons around failed" );
    OPENGEODE_EXCEPTION(
        new_polygonal_surface->polygons_around_vertex( 2 ).size() == 2,
        "[Test] Backward polygons around failed" );
}

void test_clone( const geode::PolygonalSurface3D& polygonal_surface )
{
    const auto polygonal_surface2 = polygonal_surface.clone();
    OPENGEODE_EXCEPTION( polygonal_surface2->nb_vertices() == 6,
        "[Test] PolygonalSurface2 should have 6 vertices" );
    OPENGEODE_EXCEPTION( polygonal_surface2->edges().nb_edges() == 3,
        "[Test] PolygonalSurface2 should have 3 edges" );
    OPENGEODE_EXCEPTION( polygonal_surface2->nb_polygons() == 1,
        "[Test] PolygonalSurface2 should have 1 polygon" );

    const auto attribute2 = polygonal_surface2->vertex_attribute_manager()
                                .find_attribute< geode::PolygonEdge >( "test" );
    std::vector< geode::PolygonEdge > att_answer{ { 2, 0 }, { 6, 0 }, { 1, 0 },
        { 5, 0 }, { 0, 0 }, { 3, 0 } };
    for( const auto v : geode::Range{ polygonal_surface2->nb_vertices() } )
    {
        const geode::PolygonEdge answer{ v, 0 };
        OPENGEODE_EXCEPTION( attribute2->value( v ) == att_answer[v],
            "[Test] PolygonalSurface2 attribute is not correct" );
    }
}

void test_set_polygon_vertex(
    const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    builder.set_polygon_vertex( { 0, 2 }, 1 );
    builder.edges_builder().delete_isolated_edges();
    OPENGEODE_EXCEPTION( polygonal_surface.polygon_vertex( { 0, 2 } ) == 1,
        "[Test] PolygonVertex after set_polygon_vertex is wrong" );

    const auto vertices = polygonal_surface.edges().edge_vertices( 1 );
    OPENGEODE_EXCEPTION( vertices[0] == 1 && vertices[1] == 4,
        "[Test] Edge vertices after set_polygon_vertex is wrong" );
}

void test_replace_vertex( const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    const auto new_id = builder.create_vertex();
    const auto polygons_around = polygonal_surface.polygons_around_vertex( 1 );
    builder.replace_vertex( 1, new_id );
    for( const auto& pv : polygons_around )
    {
        OPENGEODE_EXCEPTION( polygonal_surface.polygon_vertex( pv ) == new_id,
            "[Test] PolygonVertex after replace_vertex is wrong" );
    }
    OPENGEODE_EXCEPTION( polygonal_surface.isolated_vertex( 1 ),
        "[Test] Isolated vertex after replace_vertex is wrong" );
    builder.replace_vertex( new_id, 1 );
    for( const auto& pv : polygons_around )
    {
        OPENGEODE_EXCEPTION( polygonal_surface.polygon_vertex( pv ) == 1,
            "[Test] PolygonVertex after second replace_vertex is wrong" );
    }
    builder.delete_isolated_vertices();
    OPENGEODE_EXCEPTION( polygonal_surface.nb_vertices() == new_id,
        "[Test] Revert after replace_vertex is wrong" );
}

void test_delete_all( const geode::PolygonalSurface3D& polygonal_surface,
    geode::PolygonalSurfaceBuilder3D& builder )
{
    std::vector< bool > to_delete( polygonal_surface.nb_polygons(), true );
    builder.delete_polygons( to_delete );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_vertices() == 6,
        "[Test] PolygonalSurface should have 6 vertices" );
    OPENGEODE_EXCEPTION( polygonal_surface.nb_polygons() == 0,
        "[Test] PolygonalSurface should have 0 polygon" );
    OPENGEODE_EXCEPTION( polygonal_surface.polygons_around_vertex( 0 ).empty(),
        "[Test] No more polygon around vertices" );

    builder.delete_isolated_vertices();
    builder.edges_builder().delete_isolated_edges();
    OPENGEODE_EXCEPTION( polygonal_surface.nb_vertices() == 0,
        "[Test] PolygonalSurface should have 0 vertex" );
    OPENGEODE_EXCEPTION( polygonal_surface.edges().nb_edges() == 0,
        "[Test] PolygonalSurface should have 0 edge" );
}

void test_non_manifold_surface()
{
    auto surface = geode::PolygonalSurface3D::create(
        geode::OpenGeodePolygonalSurface3D::impl_name_static() );
    auto builder = geode::PolygonalSurfaceBuilder3D::create( *surface );
    builder->create_vertices( 10 );
    builder->create_polygon( { 0, 1, 2, 3 } );
    builder->create_polygon( { 2, 3, 4, 5 } );
    builder->create_polygon( { 7, 6, 3, 2 } );
    builder->create_polygon( { 9, 2, 3, 8 } );
    builder->compute_polygon_adjacencies();
    for( const auto p : geode::Range{ surface->nb_polygons() } )
    {
        for( const auto e : geode::LRange{ surface->nb_polygon_edges( p ) } )
        {
            geode::PolygonEdge edge{ p, e };
            OPENGEODE_EXCEPTION( surface->is_edge_on_border( edge ),
                "[Test] Wrong adjacency computations on non-manifold surface" );
        }
    }
}

void test()
{
    auto polygonal_surface = geode::PolygonalSurface3D::create(
        geode::OpenGeodePolygonalSurface3D::impl_name_static() );
    polygonal_surface->enable_edges();
    auto builder =
        geode::PolygonalSurfaceBuilder3D::create( *polygonal_surface );

    test_create_vertices( *polygonal_surface, *builder );
    test_bounding_box( *polygonal_surface );
    test_create_vertex_attribute( *polygonal_surface );
    test_create_polygons( *polygonal_surface, *builder );
    test_create_edge_attribute( *polygonal_surface );
    test_polygon_adjacencies( *polygonal_surface, *builder );
    test_polygon_edges_on_borders( *polygonal_surface );
    test_previous_next_on_border( *polygonal_surface );
    test_polygon_edge_requests( *polygonal_surface );
    test_polygon_barycenter( *polygonal_surface );
    test_polygon_area();
    test_polygon_normal();
    test_polygon_vertex_normal();

    test_io( *polygonal_surface,
        absl::StrCat( "test.", polygonal_surface->native_extension() ) );
    test_backward_io( absl::StrCat(
        geode::data_path, "test_v7.", polygonal_surface->native_extension() ) );

    test_permutation( *polygonal_surface, *builder );
    test_replace_vertex( *polygonal_surface, *builder );
    test_delete_vertex( *polygonal_surface, *builder );
    test_delete_polygon( *polygonal_surface, *builder );
    test_clone( *polygonal_surface );
    test_set_polygon_vertex( *polygonal_surface, *builder );
    test_delete_all( *polygonal_surface, *builder );

    test_non_manifold_surface();
}

OPENGEODE_TEST( "polygonal-surface" )