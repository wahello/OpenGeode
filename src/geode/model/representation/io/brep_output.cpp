/*
 * Copyright (c) 2019 - 2020 Geode-solutions
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

#include <geode/model/representation/io/brep_output.h>

#include <geode/model/representation/core/brep.h>

namespace geode
{
    void save_brep( const BRep& brep, absl::string_view filename )
    {
        try
        {
            const auto output = BRepOutputFactory::create(
                extension_from_filename( filename ).data(), brep, filename );
            output->write();
            Logger::info( "BRep saved in ", filename );
        }
        catch( const OpenGeodeException& e )
        {
            Logger::error( e.what() );
            throw OpenGeodeException{ "Cannot save BRep in file: ", filename };
        }
    }

    BRepOutput::BRepOutput( const BRep& brep, absl::string_view filename )
        : Output( filename ), brep_( brep )
    {
    }
} // namespace geode
