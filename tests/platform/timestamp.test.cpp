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
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <platform/timestamp>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(timestamp, evaluate, differece) {
    long long int timestamp1 = gtl::timestamp();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    long long int timestamp2 = gtl::timestamp();
    REQUIRE(timestamp2 > timestamp1);
    REQUIRE(testbench::is_value_approx(timestamp1, timestamp2 - 1000000ll, 200000ll), "Excessive timestamp difference: %lld - %lld = %lld", timestamp2, timestamp1, timestamp2 - timestamp1);
}

