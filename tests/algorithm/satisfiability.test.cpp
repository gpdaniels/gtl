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

#include <algorithm/satisfiability>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(satisfiability, constructor, empty) {
    gtl::satisfiability satisfiability;
    testbench::do_not_optimise_away(satisfiability);
}

TEST(satisfiability, evaluate, empty) {
    gtl::satisfiability satisfiability;
    REQUIRE(satisfiability.solve());
}

TEST(satisfiability, evaluate, valid) {
    {
        gtl::satisfiability satisfiability(1);
        satisfiability.add_clause({1});
        REQUIRE(satisfiability.solve());
        REQUIRE(satisfiability.get_value(1) == true);
    }
    {
        gtl::satisfiability satisfiability(1);
        satisfiability.add_clause({-1});
        REQUIRE(satisfiability.solve());
        REQUIRE(satisfiability.get_value(1) == false);
    }
    {
        gtl::satisfiability satisfiability(2);
        satisfiability.add_clause({1, 2});
        satisfiability.add_clause({-1});
        satisfiability.add_clause({-1, 2});
        REQUIRE(satisfiability.solve());
        REQUIRE(satisfiability.get_value(1) == false);
        REQUIRE(satisfiability.get_value(2) == true);
    }
}

TEST(satisfiability, evaluate, invalid) {
    {
        gtl::satisfiability satisfiability(1);
        satisfiability.add_clause({1});
        satisfiability.add_clause({-1});
        REQUIRE(satisfiability.solve() == false);
    }
}
