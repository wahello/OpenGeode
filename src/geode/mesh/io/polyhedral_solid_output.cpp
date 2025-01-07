/*
 * Copyright (c) 2019 - 2025 Geode-solutions
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

#include <geode/mesh/io/polyhedral_solid_output.hpp>

#include <string>
#include <string_view>
#include <vector>

#include <absl/strings/str_cat.h>

#include <geode/basic/detail/geode_output_impl.hpp>
#include <geode/basic/io.hpp>
#include <geode/basic/logger.hpp>

#include <geode/mesh/core/polyhedral_solid.hpp>
#include <geode/mesh/io/vertex_set_output.hpp>

namespace geode
{
    template < index_t dimension >
    std::vector< std::string > save_polyhedral_solid(
        const PolyhedralSolid< dimension >& polyhedral_solid,
        std::string_view filename )
    {
        const auto type = absl::StrCat( "PolyhedralSolid", dimension, "D" );
        try
        {
            return detail::geode_object_output_impl<
                PolyhedralSolidOutputFactory< dimension > >(
                type, polyhedral_solid, filename );
        }
        catch( const OpenGeodeException& e )
        {
            Logger::error( e.what() );
            print_available_extensions<
                PolyhedralSolidOutputFactory< dimension > >( type );
            Logger::info( "Other extensions are available in parent classes." );
            print_available_extensions< VertexSetOutputFactory >( "VertexSet" );
            throw OpenGeodeException{ "Cannot save PolyhedralSolid in file: ",
                filename };
        }
    }

    template < index_t dimension >
    bool is_polyhedral_solid_saveable(
        const PolyhedralSolid< dimension >& polyhedral_solid,
        std::string_view filename )
    {
        const auto output = detail::geode_object_output_writer<
            PolyhedralSolidOutputFactory< dimension > >( filename );
        return output->is_saveable( polyhedral_solid );
    }

    template std::vector< std::string > opengeode_mesh_api
        save_polyhedral_solid( const PolyhedralSolid< 3 >&, std::string_view );

    template bool opengeode_mesh_api is_polyhedral_solid_saveable(
        const PolyhedralSolid< 3 >&, std::string_view );
} // namespace geode
