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
#include <benchmark.tests.hpp>
#include <macro.tests.hpp>

#include <semaphore>

#include <condition_variable>
#include <mutex>
#include <type_traits>

TEST(traits, standard) {
    REQUIRE((sizeof(gtl::semaphore<std::mutex, std::condition_variable>) >= 1), "sizeof(gtl::semaphore) = %ld, expected >= %lld", sizeof(gtl::semaphore<std::mutex, std::condition_variable>), 1ull);

    REQUIRE((std::is_pod<gtl::semaphore<std::mutex, std::condition_variable>>::value == false), "Expected std::is_pod to be false.");

    REQUIRE((std::is_trivial<gtl::semaphore<std::mutex, std::condition_variable>>::value == false), "Expected std::is_trivial to be false.");

    REQUIRE((std::is_trivially_copyable<gtl::semaphore<std::mutex, std::condition_variable>>::value == false), "Expected std::is_trivially_copyable to be false.");

    REQUIRE((std::is_standard_layout<gtl::semaphore<std::mutex, std::condition_variable>>::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore;
    DoNotOptimiseAway(semaphore);
}

TEST(constructor, value) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore(1);
    semaphore.wait();
}

TEST(function, wait) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore(1);
    semaphore.wait();
}

TEST(function, try_wait) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore(1);
    REQUIRE(semaphore.try_wait() == true, "Expected try_wait to succeed on a semaphore with count 1.");
}

TEST(function, notify_and_wait) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore;
    semaphore.notify();
    semaphore.wait();
}

TEST(evaluation, mutex_and_condition_variable) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore;
    DoNotOptimiseAway(semaphore);
}

TEST(evaluation, recursive_mutex_and_condition_variable_any) {
    gtl::semaphore<std::recursive_mutex, std::condition_variable_any> semaphore;
    DoNotOptimiseAway(semaphore);
}
