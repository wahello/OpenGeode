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

#pragma once

#include <string_view>

#include <geode/basic/factory.hpp>
#include <geode/basic/input.hpp>

#include <geode/mesh/common.hpp>
#include <geode/mesh/core/mesh_id.hpp>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( EdgedCurve );
} // namespace geode

namespace geode
{
    /*!
     * API function for loading an EdgedCurve.
     * The adequate loader is called depending on the filename extension.
     * @param[in] impl Data structure implementation.
     * @param[in] filename Path to the file to load.
     */
    template < index_t dimension >
    [[nodiscard]] std::unique_ptr< EdgedCurve< dimension > > load_edged_curve(
        const MeshImpl& impl, std::string_view filename );

    /*!
     * API function for loading an EdgedCurve.
     * The adequate loader is called depending on the filename extension.
     * Default data structure implémentation is used.
     * @param[in] filename Path to the file to load.
     */
    template < index_t dimension >
    [[nodiscard]] std::unique_ptr< EdgedCurve< dimension > > load_edged_curve(
        std::string_view filename );

    template < index_t dimension >
    class EdgedCurveInput
        : public Input< std::unique_ptr< EdgedCurve< dimension > >, MeshImpl >
    {
    public:
        using Base =
            Input< std::unique_ptr< EdgedCurve< dimension > >, MeshImpl >;
        using typename Base::InputData;
        using typename Base::MissingFiles;

    protected:
        explicit EdgedCurveInput( std::string_view filename ) : Base{ filename }
        {
        }
    };

    template < index_t dimension >
    [[nodiscard]] typename EdgedCurveInput< dimension >::MissingFiles
        check_edged_curve_missing_files( std::string_view filename );

    template < index_t dimension >
    [[nodiscard]] bool is_edged_curve_loadable( std::string_view filename );

    template < index_t dimension >
    using EdgedCurveInputFactory =
        Factory< std::string, EdgedCurveInput< dimension >, std::string_view >;
    ALIAS_2D_AND_3D( EdgedCurveInputFactory );
} // namespace geode
