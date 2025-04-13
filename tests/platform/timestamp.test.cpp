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

#include <platform/timestamp>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <chrono>
#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(timestamp, evaluate, differece) {
    std::chrono::high_resolution_clock::time_point outer1 = std::chrono::high_resolution_clock::now();
    long long int timestamp1 = gtl::timestamp();
    std::chrono::high_resolution_clock::time_point inner1 = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::chrono::high_resolution_clock::time_point inner2 = std::chrono::high_resolution_clock::now();
    long long int timestamp2 = gtl::timestamp();
    std::chrono::high_resolution_clock::time_point outer2 = std::chrono::high_resolution_clock::now();

    REQUIRE(timestamp2 > timestamp1);

    long long int minimum_time = std::chrono::duration_cast<std::chrono::nanoseconds>(inner2 - inner1).count();
    long long int maximum_time = std::chrono::duration_cast<std::chrono::nanoseconds>(outer2 - outer1).count();
    long long int timestamp_time = timestamp2 - timestamp1;

    REQUIRE(maximum_time > minimum_time);

    REQUIRE(timestamp_time >= minimum_time);
    REQUIRE(timestamp_time <= maximum_time);
}

