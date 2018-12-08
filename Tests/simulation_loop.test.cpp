/*
The MIT License
Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
*/

#include <main.tests.hpp>
#include <require.tests.hpp>

#include <simulation_loop>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::simulation_loop<100>) >= 24, "sizeof(gtl::simulation_loop) = %ld, expected >= %lld", sizeof(gtl::simulation_loop<100>), 24ull);

    REQUIRE((std::is_pod<gtl::simulation_loop<100>>::value == false), "Expected std::is_pod to be false.");

    REQUIRE((std::is_trivial<gtl::simulation_loop<100>>::value == false), "Expected std::is_trivial to be false.");

    REQUIRE((std::is_trivially_copyable<gtl::simulation_loop<100>>::value == true), "Expected std::is_trivially_copyable to be true.");

    REQUIRE((std::is_standard_layout<gtl::simulation_loop<100>>::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::simulation_loop<100> simulation_loop;
}

TEST(function, update) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.update() == false);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    REQUIRE(simulation_loop.update());
}

TEST(function, get_tick_rate) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_tick_rate() == 100);
}

TEST(function, get_tick_step) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_tick_step() == std::chrono::milliseconds(10));
}

TEST(function, get_current_tick) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.update() == false);
    REQUIRE(simulation_loop.get_current_tick() == 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    REQUIRE(simulation_loop.update() == true);
    REQUIRE(simulation_loop.get_current_tick() == 1);
}

TEST(function, get_lag_duration) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_lag_duration() == std::chrono::milliseconds(0));
}

TEST(function, get_alpha) {
    gtl::simulation_loop<100> simulation_loop;
    REQUIRE(simulation_loop.get_alpha<float>() == 0.0f);
}

TEST(evaluation, game_loop) {

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

        //PRINT("Alpha: %f\n", simulation_loop.get_alpha<float>());
    }

    average_tick_rate /= static_cast<double>(simulation_loop.get_current_tick());
    PRINT("Desired Tick Rate: %f\n", 100.0);
    PRINT("Average Tick Rate: %f\n", 1000.0 / average_tick_rate);

    REQUIRE(9.0 < average_tick_rate && average_tick_rate < 11.0);
}
