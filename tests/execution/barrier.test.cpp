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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
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

TEST(barrier, evaluate, set_trigger_count_with_two_threads) {
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

TEST(barrier, evaluate, trigger_with_two_threads) {
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

TEST(barrier, evaluate, sync_with_two_threads) {
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
