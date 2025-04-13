/*
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <testbench/main.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

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
    short a : 1;                // lgtm [cpp/ambiguously-signed-bit-field]
    short b : 12;               // lgtm [cpp/ambiguously-signed-bit-field]
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

