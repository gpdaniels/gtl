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

#include "benchmark.tests.hpp"
#include "abort.tests.hpp"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <chrono>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

namespace testbench {
    long long int get_timestamp_nanoseconds();
    long long int get_timestamp_nanoseconds() {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }

    double get_difference_seconds(long long int start_nanoseconds, long long int end_nanoseconds);
    double get_difference_seconds(long long int start_nanoseconds, long long int end_nanoseconds) {
        long long int difference_nanoseconds = end_nanoseconds - start_nanoseconds;
        return static_cast<double>(difference_nanoseconds) / 1e9;
    }

    double benchmark(lambda<void()>&& testFunction, unsigned long long int minimum_iterations, double minimum_runtime) {
        // Warmup
        testFunction();

        // Monitoring variables.
        long long int start_nanoseconds = get_timestamp_nanoseconds();
        long long int end_nanoseconds = start_nanoseconds;
        unsigned long long int iterations = 0;

        // Testing
        while ((iterations < minimum_iterations) || (get_difference_seconds(start_nanoseconds, end_nanoseconds) < minimum_runtime)) {

            testFunction();

            ++iterations;
            end_nanoseconds = get_timestamp_nanoseconds();
        }

        return get_difference_seconds(start_nanoseconds, end_nanoseconds) / static_cast<double>(iterations);
    }
}
