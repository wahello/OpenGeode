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

#pragma once

#include <absl/container/inlined_vector.h>
#include <absl/types/optional.h>

#include <geode/basic/pimpl.h>

#include <geode/mesh/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( Point );
    FORWARD_DECLARATION_DIMENSION_CLASS( BoundingBox );
    class AttributeManager;
} // namespace geode

namespace geode
{
    template < index_t dimension >
    using GridCellIndex = std::array< index_t, dimension >;
    ALIAS_2D_AND_3D( GridCellIndex );

    template < index_t dimension >
    using GridVertexIndex = std::array< index_t, dimension >;
    ALIAS_2D_AND_3D( GridVertexIndex );

    template < index_t dimension >
    using GridCellIndices =
        absl::InlinedVector< GridCellIndex< dimension >, 1 << dimension >;
    ALIAS_2D_AND_3D( GridCellIndices );

    /*!
     * Interface class to represent regular grids.
     */
    template < index_t dimension >
    class RegularGrid
    {
        OPENGEODE_DISABLE_COPY( RegularGrid );
        friend class bitsery::Access;

    public:
        using Index OPENGEODE_MESH_DEPRECATED = GridCellIndex< dimension >;
        using Indices OPENGEODE_MESH_DEPRECATED = GridCellIndices< dimension >;

        RegularGrid( Point< dimension > origin,
            std::array< index_t, dimension > cells_number,
            std::array< double, dimension > cells_size );

        RegularGrid( Point< dimension > origin,
            std::array< index_t, dimension > cells_number,
            double cells_size );

        RegularGrid( RegularGrid&& );

        ~RegularGrid();

        absl::string_view native_extension() const
        {
            return native_extension_static();
        }

        static absl::string_view native_extension_static()
        {
            static const auto ext = absl::StrCat( "og_rgd", dimension, "d" );
            return ext;
        }

        const Point< dimension >& origin() const;

        index_t nb_cells() const;

        index_t nb_cells( index_t direction ) const;

        double cell_size( index_t direction ) const;

        index_t cell_index( const GridCellIndex< dimension >& index ) const;

        GridCellIndex< dimension > cell_index( index_t index ) const;

        absl::optional< GridCellIndex< dimension > > next_cell(
            const GridCellIndex< dimension >& index, index_t direction ) const;

        absl::optional< GridCellIndex< dimension > > previous_cell(
            const GridCellIndex< dimension >& index, index_t direction ) const;

        index_t nb_vertices() const;

        index_t nb_vertices( index_t direction ) const;

        GridVertexIndex< dimension > vertex_index( index_t index ) const;

        index_t vertex_index( const GridVertexIndex< dimension >& index ) const;

        absl::optional< GridVertexIndex< dimension > > next_vertex(
            const GridVertexIndex< dimension >& index,
            index_t direction ) const;

        absl::optional< GridVertexIndex< dimension > > previous_vertex(
            const GridVertexIndex< dimension >& index,
            index_t direction ) const;

        /*!
         * Return the cell(s) containing the query point
         * @param[in] query Position of point
         * @return No, one index or several cell indices that contain the query
         * point.
         * @detail When query point is geometrically near to cell limit, several
         * cell indices are returned: they correspond the potential cells that
         * may contain the point.
         */
        absl::optional< GridCellIndices< dimension > > cell(
            const Point< dimension >& query ) const;

        Point< dimension > point(
            const GridVertexIndex< dimension >& index ) const;

        AttributeManager& cell_attribute_manager() const;

        AttributeManager& vertex_attribute_manager() const;

        /*!
         * Compute the bounding box of the grid
         */
        BoundingBox< dimension > bounding_box() const;

        RegularGrid< dimension > clone() const;

    private:
        RegularGrid();

        template < typename Archive >
        void serialize( Archive& archive );

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
    ALIAS_2D_AND_3D( RegularGrid );
} // namespace geode
