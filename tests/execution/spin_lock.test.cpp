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

#include <execution/spin_lock>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <mutex>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(spin_lock, traits, standard) {
    REQUIRE(std::is_pod<gtl::spin_lock>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::spin_lock>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::spin_lock>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::spin_lock>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(spin_lock, constructor, empty) {
    gtl::spin_lock spin_lock;
    testbench::do_not_optimise_away(spin_lock);
}

TEST(spin_lock, function, lock_and_unlock) {
    gtl::spin_lock spin_lock;
    spin_lock.lock();
    spin_lock.unlock();
}

TEST(spin_lock, function, try_lock_and_unlock) {
    gtl::spin_lock spin_lock;
    REQUIRE(spin_lock.try_lock() == true, "Expected the newly constructed spin_lock to be lockable.");
    spin_lock.unlock();
}

TEST(spin_lock, evaluation, lock_guard) {
    gtl::spin_lock spin_lock;
    {
        std::lock_guard<gtl::spin_lock> lock_guard(spin_lock);
        testbench::do_not_optimise_away(lock_guard);
        REQUIRE(spin_lock.try_lock() == false, "Expected the newly constructed lock_guard to lock the spin_lock.");
    }
    REQUIRE(spin_lock.try_lock() == true, "Expected the destructed lock_guard to unlock the spin_lock.");
    spin_lock.unlock();
}

TEST(spin_lock, evaluation, unique_lock) {
    gtl::spin_lock spin_lock;
    {
        std::unique_lock<gtl::spin_lock> unique_lock(spin_lock);
        testbench::do_not_optimise_away(unique_lock);
        REQUIRE(spin_lock.try_lock() == false, "Expected the newly constructed unique_lock to lock the spin_lock.");
        spin_lock.unlock();
        REQUIRE(spin_lock.try_lock() == true, "Expected the unique_lock be unlockable.");
    }
    REQUIRE(spin_lock.try_lock() == true, "Expected the destructed unique_lock to unlock the spin_lock.");
    spin_lock.unlock();
}
