/*
The MIT License
Copyright (c) 2022 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
*/

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <utility/sizeof_bitfield>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

enum class asd : unsigned long long int {
    zero,
    one
};

struct test {
    short a : 1;
    short b : 12;
    unsigned short au : 1;
    unsigned short bu : 12;
    asd x : 2;
};

TEST(sizeof_bitfield, evaluate, size) {
    constexpr auto a3 = GTL_SIZEOF_BITFIELD(test, a);
    constexpr auto b3 = GTL_SIZEOF_BITFIELD(test, au);
    static_assert(a3 == 1);
    static_assert(b3 == 1);
    constexpr auto a4 = GTL_SIZEOF_BITFIELD(test, b);
    constexpr auto b4 = GTL_SIZEOF_BITFIELD(test, bu);
    static_assert(a4 == 12);
    static_assert(b4 == 12);
    constexpr auto x = GTL_SIZEOF_BITFIELD(test, x);
    static_assert(x == 2);

}

// TODO: Implement tests for sizeof_bitfield.

