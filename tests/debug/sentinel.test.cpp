/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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

#include <debug/sentinel>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(sentinel, traits, standard) {
    REQUIRE(std::is_pod<gtl::sentinel<>>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::sentinel<>>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::sentinel<>>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::sentinel<>>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(sentinel, constructor, empty) {
    gtl::sentinel<> sentinel;
    testbench::do_not_optimise_away(sentinel);
}
