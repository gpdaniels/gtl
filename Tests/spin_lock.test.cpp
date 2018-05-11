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
#include <macro.tests.hpp>

#include <spin_lock>

#include <mutex>
#include <type_traits>

TEST(traits, sizeof) {
    REQUIRE(sizeof(gtl::spin_lock) >= 1, "sizeof(gtl::spin_lock) = %ld, expected >= %lld", sizeof(gtl::spin_lock), 1ull);
}

TEST(traits, is_not_pod) {
    REQUIRE(std::is_pod<gtl::spin_lock>::value == false, "Expected std::is_pod to be false.");
}

TEST(traits, is_not_trivial) {
    REQUIRE(std::is_trivial<gtl::spin_lock>::value == false, "Expected std::is_trivial to be false.");
}

TEST(traits, is_not_trivially_copyable) {
    REQUIRE(std::is_trivially_copyable<gtl::spin_lock>::value == false, "Expected std::is_trivially_copyable to be false.");
}

TEST(traits, is_standard_layout) {
    REQUIRE(std::is_standard_layout<gtl::spin_lock>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::spin_lock spin_lock;
    UNOPTIMISED(spin_lock);
}

TEST(function, lock_and_unlock) {
    gtl::spin_lock spin_lock;
    spin_lock.lock();
    spin_lock.unlock();
}

TEST(function, try_lock_and_unlock) {
    gtl::spin_lock spin_lock;
    REQUIRE(spin_lock.try_lock() == true, "Expected the newly constructed spin_lock to be lockable.");
    spin_lock.unlock();
}

TEST(evaluation, lock_guard) {
    gtl::spin_lock spin_lock;
    {
        std::lock_guard<gtl::spin_lock> lock_guard(spin_lock);
        UNUSED(lock_guard);
        REQUIRE(spin_lock.try_lock() == false, "Expected the newly constructed lock_guard to lock the spin_lock.");
    }
    REQUIRE(spin_lock.try_lock() == true, "Expected the destructed lock_guard to unlock the spin_lock.");
    spin_lock.unlock();
}

TEST(evaluation, unique_lock) {
    gtl::spin_lock spin_lock;
    {
        std::unique_lock<gtl::spin_lock> unique_lock(spin_lock);
        UNUSED(unique_lock);
        REQUIRE(spin_lock.try_lock() == false, "Expected the newly constructed unique_lock to lock the spin_lock.");
        spin_lock.unlock();
        REQUIRE(spin_lock.try_lock() == true, "Expected the unique_lock be unlockable.");
    }
    REQUIRE(spin_lock.try_lock() == true, "Expected the destructed unique_lock to unlock the spin_lock.");
    spin_lock.unlock();
}