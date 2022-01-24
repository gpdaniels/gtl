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

#include "benchmark.tests.hpp"
#include "abort.tests.hpp"
#include "optimise.tests.hpp"

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
        do_not_optimise_away(testFunction);

        // Monitoring variables.
        long long int start_nanoseconds = get_timestamp_nanoseconds();
        long long int end_nanoseconds = start_nanoseconds;
        unsigned long long int iterations = 0;

        // Testing
        while ((iterations < minimum_iterations) || (get_difference_seconds(start_nanoseconds, end_nanoseconds) < minimum_runtime)) {

            do_not_optimise_away(testFunction);

            ++iterations;
            end_nanoseconds = get_timestamp_nanoseconds();
        }

        return get_difference_seconds(start_nanoseconds, end_nanoseconds) / static_cast<double>(iterations);
    }
}
