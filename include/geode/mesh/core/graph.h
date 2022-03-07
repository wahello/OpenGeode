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

#pragma once

#include <vector>

#include <absl/container/inlined_vector.h>

#include <geode/basic/bitsery_archive.h>
#include <geode/basic/passkey.h>

#include <geode/mesh/common.h>
#include <geode/mesh/core/vertex_set.h>

namespace geode
{
    class AttributeManager;
    class GraphBuilder;
} // namespace geode

namespace geode
{
    /*!
     * This struct represents a graph edge endpoint
     */
    struct opengeode_mesh_api EdgeVertex
    {
        EdgeVertex() = default;
        EdgeVertex( index_t edge_id_in, local_index_t vertex_id_in )
            : edge_id( edge_id_in ), vertex_id( vertex_id_in )
        {
        }
        bool operator==( const EdgeVertex& other ) const
        {
            return edge_id == other.edge_id && vertex_id == other.vertex_id;
        }
        bool operator!=( const EdgeVertex& other ) const
        {
            return !( *this == other );
        }
        EdgeVertex opposite() const
        {
            const local_index_t opposite = vertex_id == 0 ? 1 : 0;
            return { edge_id, opposite };
        }
        template < typename Archive >
        void serialize( Archive& archive );

        /*!
         * Index of the edge
         */
        index_t edge_id{ NO_ID };
        /*!
         * Identification of the edge endpoint (0 or 1)
         */
        local_index_t vertex_id{ NO_LID };
    };

    using EdgesAroundVertex = absl::InlinedVector< EdgeVertex, 2 >;

    /*!
     * Interface class to represent a Graph.
     * A graph is a set of vertices linked by edges
     */
    class opengeode_mesh_api Graph : public VertexSet
    {
        PASSKEY( GraphBuilder, GraphKey );

    public:
        /*!
         * Create a new Graph using default data structure.
         */
        static std::unique_ptr< Graph > create();

        /*!
         * Create a new Graph using a specified data structure.
         * @param[in] impl Data structure implementation
         */
        static std::unique_ptr< Graph > create( const MeshImpl& impl );

        static MeshType type_name_static()
        {
            return MeshType{ "Graph" };
        }

        std::unique_ptr< Graph > clone() const;

        ~Graph();

        /*!
         * Return the vertex index corresponding to an edge endpoint
         * @param[in] edge_vertex Edge endpoint
         */
        index_t edge_vertex( const EdgeVertex& edge_vertex ) const;

        /*!
         * Return the indices of the two edge vertices
         * @param[in] edge_id Index of the edge
         */
        std::array< index_t, 2 > edge_vertices( index_t edge_id ) const;

        index_t nb_edges() const;

        /*!
         * Access to the manager of attributes associated with edges.
         */
        AttributeManager& edge_attribute_manager() const;

        /*!
         * Get all edge endpoints corresponding to a given vertex
         * @param[in] vertex_id Index of the vertex
         */
        const EdgesAroundVertex& edges_around_vertex( index_t vertex_id ) const;

        bool is_vertex_isolated( index_t vertex_id ) const;

        absl::optional< index_t > edge_from_vertices(
            index_t v0, index_t v1 ) const;

        void set_edges_around_vertex(
            index_t vertex_id, EdgesAroundVertex edges, GraphKey );

        void associate_edge_vertex_to_vertex(
            const EdgeVertex& edge_vertex, index_t vertex_id, GraphKey );

        void disassociate_edge_vertex_to_vertex(
            const EdgeVertex& edge_vertex, GraphKey );

    protected:
        Graph();
        Graph( Graph&& );

    private:
        friend class bitsery::Access;
        template < typename Archive >
        void serialize( Archive& archive );
        virtual index_t get_edge_vertex(
            const EdgeVertex& edge_vertex ) const = 0;

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
} // namespace geode
