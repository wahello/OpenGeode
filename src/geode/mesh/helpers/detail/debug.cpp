/*
 * Copyright (c) 2019 - 2024 Geode-solutions
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

#include <geode/mesh/helpers/detail/debug.h>

#include <absl/strings/string_view.h>

#include <geode/geometry/basic_objects/segment.h>
#include <geode/geometry/basic_objects/tetrahedron.h>
#include <geode/geometry/basic_objects/triangle.h>

#include <geode/mesh/builder/edged_curve_builder.h>
#include <geode/mesh/builder/triangulated_surface_builder.h>
#include <geode/mesh/core/edged_curve.h>
#include <geode/mesh/core/tetrahedral_solid.h>
#include <geode/mesh/core/triangulated_surface.h>
#include <geode/mesh/io/edged_curve_output.h>
#include <geode/mesh/io/triangulated_surface_output.h>

namespace geode
{
    namespace detail
    {
        template < index_t dimension >
        void save_segment(
            const Segment< dimension >& segment, absl::string_view suffix )
        {
            auto curve = geode::EdgedCurve< dimension >::create();
            auto builder =
                geode::EdgedCurveBuilder< dimension >::create( *curve );
            const auto& vertices = segment.vertices();
            builder->create_point( vertices[0] );
            builder->create_point( vertices[1] );
            builder->create_edge( 0, 1 );
            geode::save_edged_curve( *curve,
                absl::StrCat( "segment_", suffix, ".og_edc", dimension, "d" ) );
        }

        template < index_t dimension >
        void save_triangle(
            const Triangle< dimension >& triangle, absl::string_view suffix )
        {
            auto surface = geode::TriangulatedSurface< dimension >::create();
            auto builder =
                geode::TriangulatedSurfaceBuilder< dimension >::create(
                    *surface );
            const auto& vertices = triangle.vertices();
            builder->create_point( vertices[0] );
            builder->create_point( vertices[1] );
            builder->create_point( vertices[2] );
            builder->create_triangle( { 0, 1, 2 } );
            geode::save_triangulated_surface(
                *surface, absl::StrCat( "triangle_", suffix, ".og_tsf",
                              dimension, "d" ) );
        }

        void save_tetrahedron(
            const Tetrahedron& tetrahedron, absl::string_view suffix )
        {
            auto tetra = geode::TriangulatedSurface3D::create();
            auto builder =
                geode::TriangulatedSurfaceBuilder3D::create( *tetra );
            const auto& vertices = tetrahedron.vertices();
            builder->create_point( vertices[0] );
            builder->create_point( vertices[1] );
            builder->create_point( vertices[2] );
            builder->create_point( vertices[3] );
            builder->create_triangle( { 0, 1, 2 } );
            builder->create_triangle( { 0, 1, 3 } );
            builder->create_triangle( { 0, 3, 2 } );
            builder->create_triangle( { 3, 1, 2 } );
            geode::save_triangulated_surface(
                *tetra, absl::StrCat( "tet_", suffix, ".og_tsf3d" ) );
        }

        void save_tetrahedra( const TetrahedralSolid3D& solid,
            absl::Span< const index_t > ids,
            absl::string_view suffix )
        {
            for( const auto id : ids )
            {
                save_tetrahedron(
                    solid.tetrahedron( id ), absl::StrCat( id, suffix ) );
            }
        }

        template void opengeode_mesh_api save_segment(
            const Segment< 2 >&, absl::string_view );
        template void opengeode_mesh_api save_segment(
            const Segment< 3 >&, absl::string_view );

        template void opengeode_mesh_api save_triangle(
            const Triangle< 2 >&, absl::string_view );
        template void opengeode_mesh_api save_triangle(
            const Triangle< 3 >&, absl::string_view );
    } // namespace detail
} // namespace geode
