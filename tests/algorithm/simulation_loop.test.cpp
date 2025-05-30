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

#include <testbench/require.tests.hpp>

#include <algorithm/simulation_loop>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(simulation_loop, traits, standard) {
    REQUIRE((std::is_pod<gtl::simulation_loop<100>>::value == false), "Expected std::is_pod to be false.");

    REQUIRE((std::is_trivial<gtl::simulation_loop<100>>::value == false), "Expected std::is_trivial to be false.");

    REQUIRE((std::is_trivially_copyable<gtl::simulation_loop<100>>::value == true), "Expected std::is_trivially_copyable to be true.");

    REQUIRE((std::is_standard_layout<gtl::simulation_loop<100>>::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(simulation_loop, constructor, empty) {
    gtl::simulation_loop<100> simulation_loop;
}

TEST(simulation_loop, function, update) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.update() == false);
    while (!simulation_loop.update())
        ;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    REQUIRE(std::chrono::nanoseconds(end - start).count() >= 1000000, "Failed %lld !>= %lld", static_cast<long long int>((end - start).count()), 10000000ll);
}

TEST(simulation_loop, function, get_tick_rate) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_tick_rate() == 100);
}

TEST(simulation_loop, function, get_tick_step) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_tick_step() == std::chrono::milliseconds(10));
}

TEST(simulation_loop, function, get_current_tick) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.update() == false);
    REQUIRE(simulation_loop.get_current_tick() == 0);
    while (!simulation_loop.update())
        ;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    REQUIRE(std::chrono::nanoseconds(end - start).count() >= 10000000, "Failed %lld !>= %lld", static_cast<long long int>((end - start).count()), 10000000ll);
    REQUIRE(simulation_loop.get_current_tick() == 1);
}

TEST(simulation_loop, function, get_lag_duration) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_lag_duration() == std::chrono::milliseconds(0));
}

TEST(simulation_loop, function, get_alpha) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_alpha<float>() == 0.0f);
}

TEST(simulation_loop, evaluate, game_loop) {
    gtl::simulation_loop<100> simulation_loop;

    std::chrono::steady_clock::time_point last_tick = std::chrono::steady_clock::now();
    double average_tick_rate = 10.0;

    // Simulate a small number of ticks.
    while (simulation_loop.get_current_tick() < 50) {
        // Add input to buffered input.
        // Required for an actual game loop, not in this test.

        // Check if state should be stepped.
        while (simulation_loop.update()) {
            // Update state a fixed timestep using input buffered so far.
            // Required for an actual game loop, not in this test.

            // Evaluate simulation loop timings.
            std::chrono::steady_clock::time_point now_tick = std::chrono::steady_clock::now();
            std::chrono::steady_clock::duration elapsed_tick_time = now_tick - last_tick;
            last_tick = now_tick;
            average_tick_rate += static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_tick_time).count());
        }

        // Render using the buffered input thus far and the current alpha value for interpolating the current state forwards.
        // Required for an actual game loop, not in this test.

        // PRINT("Alpha: %f\n", simulation_loop.get_alpha<float>());
    }

    average_tick_rate /= static_cast<double>(simulation_loop.get_current_tick());
    PRINT("Desired Tick Rate: %f\n", 100.0);
    PRINT("Average Tick Rate: %f\n", 1000.0 / average_tick_rate);

    REQUIRE(9.0 < average_tick_rate && average_tick_rate < 11.0);
}
