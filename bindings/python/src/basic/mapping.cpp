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

#include "../common.hpp"

#include <geode/basic/mapping.hpp>
#include <geode/basic/uuid.hpp>

#define PYTHON_MAPPING( type )                                                 \
    const auto bijective##type = absl::StrCat( "BijectiveMapping", #type );    \
    pybind11::class_< BijectiveMapping< type > >(                              \
        module, bijective##type.c_str() )                                      \
        .def( pybind11::init<>() )                                             \
        .def( pybind11::init( []( BijectiveMapping< type >& mapping ) {        \
            return BijectiveMapping< type >{ std::move( mapping ) };           \
        } ) )                                                                  \
        .def( "has_mapping_input",                                             \
            &BijectiveMapping< type >::has_mapping_input )                     \
        .def( "has_mapping_output",                                            \
            &BijectiveMapping< type >::has_mapping_output )                    \
        .def( "in2out", &BijectiveMapping< type >::in2out )                    \
        .def( "out2in", &BijectiveMapping< type >::out2in );                   \
                                                                               \
    const auto generic##type = absl::StrCat( "GenericMapping", #type );        \
    pybind11::class_< GenericMapping< type > >(                                \
        module, generic##type.c_str() )                                        \
        .def( pybind11::init<>() )                                             \
        .def( pybind11::init( []( GenericMapping< type >& mapping ) {          \
            return GenericMapping< type >{ std::move( mapping ) };             \
        } ) )                                                                  \
        .def(                                                                  \
            "has_mapping_input", &GenericMapping< type >::has_mapping_input )  \
        .def( "has_mapping_output",                                            \
            &GenericMapping< type >::has_mapping_output )                      \
        .def( "in2out", &GenericMapping< type >::in2out )                      \
        .def( "out2in", &GenericMapping< type >::out2in )

namespace geode
{
    void define_mapping( pybind11::module& module )
    {
        PYTHON_MAPPING( uuid );
    }
} // namespace geode
