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

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <thread>
#include <utility>

template <typename type>
void DoNotOptimiseAway(type&& value) {
    // To prevent value being optimised away it needs to be used somwhere.
    // When using the value it must not impact the benchmark being performed.
    // Therefore, use the value inside a never executed if block.
    // However, compilers are smart enough that using an if(false) block is not enough.
    // An if block is required that will never execute and complex enough that the compiler cannot remove it.
    // Enter std::thread::id, the compiler cannot know that the current thread id will never match std::thread::id().
    static std::thread::id thread_id = std::this_thread::get_id();
    if (thread_id == std::thread::id()) {
        // Once inside the if block we must now "use" the value, start by taking a pointer to the value.
        const void* value_pointer = &value;
        // Reinterpret the pointer as a char and print it to stdout.
        putchar(reinterpret_cast<volatile const char&>(value_pointer));
        // To sanity check that this block of code is never reached, abort.
        std::abort();
    }
}

template <typename type>
void DoNotOptimizeAway(std::function<type(void)>&& function) {
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
        // Once inside the if block we must now "use" the function and value, start by taking a pointer to the value and function.
        const void* value_and_function_pointer = &value + &function;
        // Reinterpret the pointer as a char and print it to stdout.
        putchar(reinterpret_cast<volatile const char&>(value_and_function_pointer));
        // To sanity check that this block of code is never reached, abort.
        std::abort();
    }
}

template <>
void DoNotOptimizeAway(std::function<void(void)>&& function);

#endif // BENCHMARK_TESTS_HPP
