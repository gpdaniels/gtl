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
#ifndef BENCHMARK_TESTS_HPP
#define BENCHMARK_TESTS_HPP

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <chrono>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <thread>
#include <utility>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

template <typename type>
void do_not_optimise_away(type&& value) {
    // To prevent value being optimised away it needs to be used somwhere.
    // When using the value it must not impact the benchmark being performed.
    // Therefore, use the value inside a never executed if block.
    // However, compilers are smart enough that using an if(false) block is not enough.
    // An if block is required that will never execute and complex enough that the compiler cannot remove it.
    // Enter std::thread::id, the compiler cannot know that the current thread id will never match std::thread::id().
    static std::thread::id thread_id = std::this_thread::get_id();
    if (thread_id == std::thread::id()) {
        // Once inside the if block we must now "use" the function and value.
        // Copy the raw data of the value.
        char buffer_value[sizeof(type)] = {};
        std::memcpy(buffer_value, &value, sizeof(type));
        // Print it all out.
        for (const char character : buffer_value) {
            putchar(character);
        }
        // To sanity check that this block of code is never reached, abort.
        std::abort();
    }
}

template <typename type>
void do_not_optimise_away(std::function<type(void)>&& function) {
    // Call function and get returned value.
    volatile type value = function();

    // To prevent value and function being optimised away they must be used somwhere.
    // When using the value and function they must not impact the benchmark being performed.
    // Therefore, use the value and function inside a never executed if block.
    // However, compilers are smart enough that using an if(false) block is not enough.
    // An if block is required that will never execute and complex enough that the compiler cannot remove it.
    // Enter std::thread::id, the compiler cannot know that the current thread id will never match std::thread::id().
    static std::thread::id thread_id = std::this_thread::get_id();
    if (thread_id == std::thread::id()) {
        // Once inside the if block we must now "use" the function and value.
        // Copy the raw data of the value.
        char buffer_value[sizeof(type)] = {};
        std::memcpy(buffer_value, &value, sizeof(type));
        // Print it all out.
        for (const char character : buffer_value) {
            putchar(character);
        }
        // Copy the raw data of the function.
        char buffer_function[sizeof(std::function<type(void)>)] = {};
        std::memcpy(buffer_function, &function, sizeof(std::function<type(void)>));
        // Print it all out.
        for (const char character : buffer_function) {
            putchar(character);
        }
        // To sanity check that this block of code is never reached, abort.
        std::abort();
    }
}

template <>
void do_not_optimise_away(std::function<void(void)>&& function);

// Simple benchmarking function
template <typename type = void>
std::pair<double, unsigned long long int> benchmark(std::function<type(void)>&& testFunction, unsigned long long int minimum_iterations = 1, double minimum_runtime = 0.0) {
    // Warmup
    do_not_optimise_away(std::forward<std::function<type(void)>>(testFunction));

    // Monitoring variables.
    std::chrono::steady_clock::time_point Start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point End = Start;
    unsigned long long int iterations = 0;

    // Testing
    while ((iterations < minimum_iterations) || (std::chrono::duration<double>(End - Start).count() < minimum_runtime)) {

        do_not_optimise_away(std::forward<std::function<type(void)>>(testFunction));

        ++iterations;
        End = std::chrono::steady_clock::now();
    }

    return { std::chrono::duration<double, std::nano>(End - Start).count() / static_cast<double>(iterations), iterations };
}

#endif // BENCHMARK_TESTS_HPP
