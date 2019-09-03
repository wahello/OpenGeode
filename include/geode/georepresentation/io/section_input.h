/*
 * Copyright (c) 2019 Geode-solutions
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

#pragma once

#include <geode/basic/factory.h>

#include <geode/georepresentation/builder/section_builder.h>
#include <geode/georepresentation/common.h>

#include <geode/mesh/io/vertex_set_input.h>

namespace geode
{
    class Section;
    class SectionBuilder;
} // namespace geode

namespace geode
{
    /*!
     * API function for loading a Section.
     * The adequate loader is called depending on the filename extension.
     * @param[out] section Loaded Section.
     * @param[in] filename Path to the file to load.
     */
    void opengeode_georepresentation_api load_section(
        Section& section, const std::string& filename );

    class opengeode_georepresentation_api SectionInput : public Input
    {
    protected:
        SectionInput( Section& section, std::string filename );

        Section& section()
        {
            return section_;
        }

    private:
        Section& section_;
    };

    using SectionInputFactory =
        Factory< std::string, SectionInput, Section&, std::string >;
} // namespace geode