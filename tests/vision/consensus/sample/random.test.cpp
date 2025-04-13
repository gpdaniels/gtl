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

#include <vision/consensus/sample/random>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(random, constructor, empty) {
    {
        gtl::random<1> random;
        testbench::do_not_optimise_away(random);
    }
    {
        gtl::random<2> random;
        testbench::do_not_optimise_away(random);
    }
    {
        gtl::random<5> random;
        testbench::do_not_optimise_away(random);
    }
    {
        gtl::random<100> random;
        testbench::do_not_optimise_away(random);
    }
}

TEST(random, function, sample) {
    {
        gtl::random<1> random;
        std::size_t indices[1] = { 123456 };
        random.prepare(1);
        random.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        random.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
        random.sample(&indices[0]);
        REQUIRE(indices[0] == 0);
    }
    {
        gtl::random<5> random;
        std::size_t indices[5] = { 1111, 2222, 3333, 4444, 5555 };
        random.prepare(6);
        random.sample(&indices[0]);
        REQUIRE(indices[0] == 1);
        REQUIRE(indices[1] == 3);
        REQUIRE(indices[2] == 2);
        REQUIRE(indices[3] == 4);
        REQUIRE(indices[4] == 5);
        random.sample(&indices[0]);
        REQUIRE(indices[0] == 2);
        REQUIRE(indices[1] == 1);
        REQUIRE(indices[2] == 0);
        REQUIRE(indices[3] == 5);
        REQUIRE(indices[4] == 3);
    }
}
