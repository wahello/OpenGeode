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

#include <geode/basic/pimpl.h>

#include <geode/mesh/core/mesh_id.h>

#include <geode/model/common.h>

namespace geode
{
    FORWARD_DECLARATION_DIMENSION_CLASS( SurfaceCollection );
    FORWARD_DECLARATION_DIMENSION_CLASS( SurfaceCollectionsBuilder );

    struct uuid;
} // namespace geode

namespace geode
{
    template < index_t dimension >
    class opengeode_model_api SurfaceCollections
    {
        OPENGEODE_DISABLE_COPY( SurfaceCollections );

    public:
        using Builder = SurfaceCollectionsBuilder< dimension >;
        friend Builder;

        class opengeode_model_api SurfaceCollectionRangeBase
        {
        public:
            ~SurfaceCollectionRangeBase();

            bool operator!=(
                const SurfaceCollectionRangeBase& /*unused*/ ) const;

            void operator++();

        protected:
            SurfaceCollectionRangeBase( const SurfaceCollections& collections );
            SurfaceCollectionRangeBase(
                SurfaceCollectionRangeBase&& other ) noexcept;
            SurfaceCollectionRangeBase(
                const SurfaceCollectionRangeBase& other );

        protected:
            IMPLEMENTATION_MEMBER( impl_ );
        };

        class opengeode_model_api SurfaceCollectionRange
            : public SurfaceCollectionRangeBase
        {
        public:
            SurfaceCollectionRange( const SurfaceCollections& boundaries );
            SurfaceCollectionRange( const SurfaceCollectionRange& range );
            ~SurfaceCollectionRange();

            const SurfaceCollectionRange& begin() const;

            const SurfaceCollectionRange& end() const;

            const SurfaceCollection< dimension >& operator*() const;
        };

    public:
        ~SurfaceCollections();

        index_t nb_surface_collections() const;

        const SurfaceCollection< dimension >& surface_collection(
            const uuid& id ) const;

        SurfaceCollectionRange surface_collections() const;

        void save_surface_collections( absl::string_view directory ) const;

    protected:
        SurfaceCollections();
        SurfaceCollections( SurfaceCollections&& other ) noexcept;
        SurfaceCollections& operator=( SurfaceCollections&& other ) noexcept;

    private:
        class ModifiableSurfaceCollectionRange
            : public SurfaceCollectionRangeBase
        {
        public:
            ModifiableSurfaceCollectionRange(
                const SurfaceCollections& boundaries );
            ModifiableSurfaceCollectionRange(
                const ModifiableSurfaceCollectionRange& range );
            ~ModifiableSurfaceCollectionRange();

            const ModifiableSurfaceCollectionRange& begin() const;

            const ModifiableSurfaceCollectionRange& end() const;

            SurfaceCollection< dimension >& operator*() const;
        };

    private:
        const uuid& create_surface_collection();

        void create_surface_collection( uuid surface_collection_id );

        void delete_surface_collection(
            const SurfaceCollection< dimension >& boundary );

        void load_surface_collections( absl::string_view directory );

        ModifiableSurfaceCollectionRange modifiable_surface_collections();

        SurfaceCollection< dimension >& modifiable_surface_collection(
            const uuid& id );

    private:
        IMPLEMENTATION_MEMBER( impl_ );
    };
    ALIAS_2D_AND_3D( SurfaceCollections );
} // namespace geode
