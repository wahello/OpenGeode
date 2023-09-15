/*
 * Copyright (c) 2019 - 2023 Geode-solutions
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

#include <geode/mesh/io/point_set_input.h>

#include <geode/basic/detail/geode_input_impl.h>

#include <geode/mesh/core/mesh_factory.h>
#include <geode/mesh/core/point_set.h>

namespace geode
{
    template < index_t dimension >
    std::unique_ptr< PointSet< dimension > > load_point_set(
        const MeshImpl& impl, absl::string_view filename )
    {
        try
        {
            const auto type = absl::StrCat( "PointSet", dimension, "D" );
            auto point_set = detail::geode_object_input_impl<
                PointSetInputFactory< dimension > >( type, filename, impl );
            Logger::info(
                type, " has: ", point_set->nb_vertices(), " vertices" );
            return point_set;
        }
        catch( const OpenGeodeException& e )
        {
            Logger::error( e.what() );
            throw OpenGeodeException{ "Cannot load PointSet from file: ",
                filename };
        }
    }

    template < index_t dimension >
    std::unique_ptr< PointSet< dimension > > load_point_set(
        absl::string_view filename )
    {
        return load_point_set< dimension >(
            MeshFactory::default_impl(
                PointSet< dimension >::type_name_static() ),
            filename );
    }

    template < index_t dimension >
    typename PointSetInput< dimension >::MissingFiles
        check_point_set_missing_files( absl::string_view filename )
    {
        const auto input = detail::geode_object_input_reader<
            PointSetInputFactory< dimension > >( filename );
        return input->check_missing_files();
    }

    template std::unique_ptr< PointSet< 2 > > opengeode_mesh_api load_point_set(
        const MeshImpl&, absl::string_view );
    template std::unique_ptr< PointSet< 3 > > opengeode_mesh_api load_point_set(
        const MeshImpl&, absl::string_view );

    template std::unique_ptr< PointSet< 2 > > opengeode_mesh_api load_point_set(
        absl::string_view );
    template std::unique_ptr< PointSet< 3 > > opengeode_mesh_api load_point_set(
        absl::string_view );

    template PointSetInput< 2 >::MissingFiles opengeode_mesh_api
        check_point_set_missing_files< 2 >( absl::string_view );
    template PointSetInput< 3 >::MissingFiles opengeode_mesh_api
        check_point_set_missing_files< 3 >( absl::string_view );
} // namespace geode
