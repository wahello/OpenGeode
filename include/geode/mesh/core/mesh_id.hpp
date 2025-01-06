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

#pragma once

#include <string>

#include <absl/hash/hash.h>

#include <bitsery/brief_syntax/string.h>

#include <geode/basic/named_type.hpp>

namespace geode
{
    struct MeshImplTag
    {
    };
    /*!
     * Strong type for a mesh data structure
     */
    using MeshImpl = NamedType< std::string, MeshImplTag >;

    struct MeshTypeTag
    {
    };
    /*!
     * Strong type for a mesh type
     */
    using MeshType = NamedType< std::string, MeshTypeTag >;
} // namespace geode

namespace std
{
    template <>
    struct hash< geode::MeshImpl >
    {
        std::size_t operator()( const geode::MeshImpl& impl ) const
        {
            return absl::Hash< std::string >{}( impl.get() );
        }
    };

    template <>
    struct hash< geode::MeshType >
    {
        std::size_t operator()( const geode::MeshType& type ) const
        {
            return absl::Hash< std::string >{}( type.get() );
        }
    };
} // namespace std
