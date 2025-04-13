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

#include <type/standard>

TEST(standard, traits, standard) {
    REQUIRE(sizeof(gtl::u8) == 1);
    REQUIRE(sizeof(gtl::u16) == 2);
    REQUIRE(sizeof(gtl::u32) == 4);
    REQUIRE(sizeof(gtl::u64) == 8);

    REQUIRE(sizeof(gtl::s8) == 1);
    REQUIRE(sizeof(gtl::s16) == 2);
    REQUIRE(sizeof(gtl::s32) == 4);
    REQUIRE(sizeof(gtl::s64) == 8);

    REQUIRE(sizeof(gtl::f32) == 4);
    REQUIRE(sizeof(gtl::f64) == 8);
}
