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
#include <require.tests.hpp>

#include <execution/barrier>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <atomic>
#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(barrier, traits, standard) {
    REQUIRE(std::is_pod<gtl::barrier>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::barrier>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::barrier>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::barrier>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(barrier, constructor, empty) {
    gtl::barrier barrier;
    testbench::do_not_optimise_away(barrier);
}

TEST(barrier, constructor, value) {
    gtl::barrier barrier(1);
    testbench::do_not_optimise_away(barrier);
}

TEST(barrier, function, get_and_set_trigger_count) {
    gtl::barrier barrier;
    REQUIRE(barrier.get_trigger_count() == 0, "Expected the trigger count of an empty barrier to be %d, not %lld", 0, barrier.get_trigger_count());
    barrier.set_trigger_count(1);
    REQUIRE(barrier.get_trigger_count() == 1, "Expected the trigger count of an empty barrier to be %d, not %lld", 1, barrier.get_trigger_count());
}

TEST(barrier, function, get_waiting_count) {
    gtl::barrier barrier;
    REQUIRE(barrier.get_waiting_count() == 0, "Expected the waiting count of an empty barrier to be %d, not %lld", 0, barrier.get_waiting_count());
}

TEST(barrier, function, trigger) {
    gtl::barrier barrier;
    barrier.trigger();
}

TEST(barrier, function, sync) {
    gtl::barrier barrier;
    barrier.sync();
}

TEST(barrier, evaluation, set_trigger_count_with_two_threads) {
    gtl::barrier barrier(2);

    std::atomic<int> result = 0;

    REQUIRE(result == 0, "Expected result to be set to 0 not '%d' before anything.", result.load());

    std::thread thread([&barrier, &result](){

        result = 1;

        REQUIRE(result == 1, "Expected result to be set to 1 not '%d' before thread sync.", result.load());

        barrier.sync();

        result = 2;

        REQUIRE(result == 2, "Expected result to be set to 2 not '%d' after thread sync.", result.load());
    });

    barrier.set_trigger_count(1);

    thread.join();

    REQUIRE(result == 2, "Expected result to be set to 1 not '%d' after join.", result.load());
}

TEST(barrier, evaluation, trigger_with_two_threads) {
    gtl::barrier barrier(2);

    std::atomic<int> result = 0;

    REQUIRE(result == 0, "Expected result to be set to 0 not '%d' before anything.", result.load());

    std::thread thread([&barrier, &result](){

        result = 1;

        REQUIRE(result == 1, "Expected result to be set to 1 not '%d' before thread sync.", result.load());

        barrier.sync();

        result = 2;

        REQUIRE(result == 2, "Expected result to be set to 2 not '%d' after thread sync.", result.load());
    });

    barrier.trigger();

    thread.join();

    REQUIRE(result == 2, "Expected result to be set to 1 not '%d' after join.", result.load());
}

TEST(barrier, evaluation, sync_with_two_threads) {
    gtl::barrier barrier(2);

    std::atomic<int> result = 0;

    REQUIRE(result == 0, "Expected result to be set to 0 not '%d' before anything.", result.load());

    std::thread thread([&barrier, &result](){

        result = 1;

        REQUIRE(result == 1, "Expected result to be set to 1 not '%d' before thread sync.", result.load());

        barrier.sync();

        result = 2;

        REQUIRE(result == 2, "Expected result to be set to 2 not '%d' after thread sync.", result.load());
    });

    barrier.sync();

    REQUIRE(result > 0, "Expected result to be greater than zero not '%d' after sync.", result.load());

    thread.join();

    REQUIRE(result == 2, "Expected result to be set to 1 not '%d' after join.", result.load());
}
