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

#include <geode/mesh/io/polygonal_surface_input.h>

#include <absl/strings/ascii.h>

#include <geode/basic/filename.h>
#include <geode/basic/identifier_builder.h>
#include <geode/basic/timer.h>

#include <geode/mesh/core/mesh_factory.h>
#include <geode/mesh/core/polygonal_surface.h>

namespace geode
{
    template < index_t dimension >
    std::unique_ptr< PolygonalSurface< dimension > > load_polygonal_surface(
        const MeshImpl& impl, absl::string_view filename )
    {
        try
        {
            Timer timer;
            const auto extension =
                absl::AsciiStrToLower( extension_from_filename( filename ) );
            OPENGEODE_EXCEPTION(
                PolygonalSurfaceInputFactory< dimension >::has_creator(
                    extension ),
                "Unknown extension: ", extension );
            auto input = PolygonalSurfaceInputFactory< dimension >::create(
                extension, filename );
            auto polygonal_surface = input->read( impl );
            if( polygonal_surface->name() == Identifier::DEFAULT_NAME )
            {
                IdentifierBuilder{ *polygonal_surface }.set_name(
                    filename_without_extension( filename ) );
            }
            Logger::info( "PolygonalSurface", dimension, "D loaded from ",
                filename, " in ", timer.duration() );
            Logger::info( "PolygonalSurface", dimension,
                "D has: ", polygonal_surface->nb_vertices(), " vertices, ",
                polygonal_surface->nb_polygons(), " polygons" );
            return polygonal_surface;
        }
        catch( const OpenGeodeException& e )
        {
            Logger::error( e.what() );
            throw OpenGeodeException{
                "Cannot load PolygonalSurface from file: ", filename
            };
        }
    }

    template < index_t dimension >
    std::unique_ptr< PolygonalSurface< dimension > > load_polygonal_surface(
        absl::string_view filename )
    {
        return load_polygonal_surface< dimension >(
            MeshFactory::default_impl(
                PolygonalSurface< dimension >::type_name_static() ),
            filename );
    }

    template std::unique_ptr< PolygonalSurface< 2 > > opengeode_mesh_api
        load_polygonal_surface( const MeshImpl&, absl::string_view );
    template std::unique_ptr< PolygonalSurface< 3 > > opengeode_mesh_api
        load_polygonal_surface( const MeshImpl&, absl::string_view );

    template std::unique_ptr< PolygonalSurface< 2 > >
        opengeode_mesh_api load_polygonal_surface( absl::string_view );
    template std::unique_ptr< PolygonalSurface< 3 > >
        opengeode_mesh_api load_polygonal_surface( absl::string_view );
} // namespace geode
