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

#pragma once
#ifndef GTL_BENCHMARK_TESTS_HPP
#define GTL_BENCHMARK_TESTS_HPP

#include "abort.tests.hpp"
#include "optimise.tests.hpp"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <chrono>
#include <functional>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

namespace testbench {
    template <typename type>
    double benchmark(std::function<type()>&& testFunction, unsigned long long int minimum_iterations = 1) {
        // Warmup
        do_not_optimise_away(std::forward<std::function<type()>>(testFunction));

        // Monitoring variables.
        std::chrono::steady_clock::time_point Start = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point End = Start;
        unsigned long long int iterations = 0;

        // Testing
        while (iterations < minimum_iterations) {

            do_not_optimise_away(std::forward<std::function<type()>>(testFunction));

            ++iterations;
            End = std::chrono::steady_clock::now();
        }

        return std::chrono::duration<double, std::nano>(End - Start).count() / static_cast<double>(iterations);
    }

    template <typename type>
    unsigned long long int benchmark(std::function<type()>&& testFunction, double minimum_runtime) {
        // Warmup
        do_not_optimise_away(std::forward<std::function<type()>>(testFunction));

        // Monitoring variables.
        std::chrono::steady_clock::time_point Start = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point End = Start;
        unsigned long long int iterations = 0;

        // Testing
        while (std::chrono::duration<double>(End - Start).count() < minimum_runtime) {

            do_not_optimise_away(std::forward<std::function<type()>>(testFunction));

            ++iterations;
            End = std::chrono::steady_clock::now();
        }

        return iterations;
    }
}

#endif // GTL_BENCHMARK_TESTS_HPP
