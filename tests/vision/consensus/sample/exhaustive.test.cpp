/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <vision/consensus/sample/exhaustive>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(exhaustive, constructor, empty) {
    {
        gtl::exhaustive<1> exhaustive;
        testbench::do_not_optimise_away(exhaustive);
    }
    {
        gtl::exhaustive<2> exhaustive;
        testbench::do_not_optimise_away(exhaustive);
    }
    {
        gtl::exhaustive<5> exhaustive;
        testbench::do_not_optimise_away(exhaustive);
    }
    {
        gtl::exhaustive<100> exhaustive;
        testbench::do_not_optimise_away(exhaustive);
    }
}

TEST(exhaustive, function, sample) {
    {
        gtl::exhaustive<1> exhaustive;
        size_t indices[1] = { 123456 };
        exhaustive.prepare(1);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
    }
    {
        gtl::exhaustive<5> exhaustive;
        size_t indices[5] = { 1111, 2222, 3333, 4444, 5555 };
        exhaustive.prepare(6);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        REQUIRE(indices[1] == 1);
        REQUIRE(indices[2] == 2);
        REQUIRE(indices[3] == 3);
        REQUIRE(indices[4] == 4);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        REQUIRE(indices[1] == 1);
        REQUIRE(indices[2] == 2);
        REQUIRE(indices[3] == 3);
        REQUIRE(indices[4] == 5);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        REQUIRE(indices[1] == 1);
        REQUIRE(indices[2] == 2);
        REQUIRE(indices[3] == 4);
        REQUIRE(indices[4] == 5);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        REQUIRE(indices[1] == 1);
        REQUIRE(indices[2] == 3);
        REQUIRE(indices[3] == 4);
        REQUIRE(indices[4] == 5);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        REQUIRE(indices[1] == 2);
        REQUIRE(indices[2] == 3);
        REQUIRE(indices[3] == 4);
        REQUIRE(indices[4] == 5);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 1);
        REQUIRE(indices[1] == 2);
        REQUIRE(indices[2] == 3);
        REQUIRE(indices[3] == 4);
        REQUIRE(indices[4] == 5);
        exhaustive.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        REQUIRE(indices[1] == 1);
        REQUIRE(indices[2] == 2);
        REQUIRE(indices[3] == 3);
        REQUIRE(indices[4] == 4);
    }
}
