/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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

#include <execution/semaphore>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <condition_variable>
#include <mutex>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(semaphore, traits, standard) {
    REQUIRE((sizeof(gtl::semaphore<std::mutex, std::condition_variable>) >= 1), "sizeof(gtl::semaphore) = %ld, expected >= %lld", sizeof(gtl::semaphore<std::mutex, std::condition_variable>), 1ull);

    REQUIRE((std::is_pod<gtl::semaphore<std::mutex, std::condition_variable>>::value == false), "Expected std::is_pod to be false.");

    REQUIRE((std::is_trivial<gtl::semaphore<std::mutex, std::condition_variable>>::value == false), "Expected std::is_trivial to be false.");

    REQUIRE((std::is_trivially_copyable<gtl::semaphore<std::mutex, std::condition_variable>>::value == false), "Expected std::is_trivially_copyable to be false.");

    REQUIRE((std::is_standard_layout<gtl::semaphore<std::mutex, std::condition_variable>>::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(semaphore, constructor, empty) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore;
    testbench::do_not_optimise_away(semaphore);
}

TEST(semaphore, constructor, value) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore(1);
    semaphore.wait();
}

TEST(semaphore, function, wait) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore(1);
    semaphore.wait();
}

TEST(semaphore, function, try_wait) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore(1);
    REQUIRE(semaphore.try_wait() == true, "Expected try_wait to succeed on a semaphore with count 1.");
}

TEST(semaphore, function, notify_and_wait) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore;
    semaphore.notify();
    semaphore.wait();
}

TEST(semaphore, evaluate, mutex_and_condition_variable) {
    gtl::semaphore<std::mutex, std::condition_variable> semaphore;
    testbench::do_not_optimise_away(semaphore);
}

TEST(semaphore, evaluate, recursive_mutex_and_condition_variable_any) {
    gtl::semaphore<std::recursive_mutex, std::condition_variable_any> semaphore;
    testbench::do_not_optimise_away(semaphore);
}
