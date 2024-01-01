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

#include <main.tests.hpp>
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <debug/short_file>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(short_file, evaluate, macro) {
    PRINT("SHORT: %s\n", GTL_SHORT_FILE);
    PRINT("LONG:  %s\n", __FILE__);
    REQUIRE(testbench::is_string_same(__FILE__, "short_file.test.cpp") == false);
    REQUIRE(testbench::is_string_same(GTL_SHORT_FILE, "short_file.test.cpp"));
}
