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

#include <testbench/main.tests.hpp>

#include <testbench/require.tests.hpp>

#include <utility/pimpl.test.hpp>
#include <utility/pimpl>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

template <>
class test_pimpl::pimpl::implementation final {
public:
    int value;

public:
    ~implementation() {
    }

    implementation(int input_value)
        : value(input_value) {
    }
};

test_pimpl::~test_pimpl() {
}

test_pimpl::test_pimpl(int input_value)
    : pimpl(input_value) {
}

int test_pimpl::return_value() const {
    return (*this)->value;
}

namespace {
    int tracked_constructed = 0;
    int tracked_copy_constructed = 0;
    int tracked_move_constructed = 0;
    int tracked_copy_assigned = 0;
    int tracked_move_assigned = 0;
    int tracked_destructed = 0;

    void tracked_reset() {
        tracked_constructed = 0;
        tracked_copy_constructed = 0;
        tracked_move_constructed = 0;
        tracked_copy_assigned = 0;
        tracked_move_assigned = 0;
        tracked_destructed = 0;
    }
}

template <>
class test_pimpl_tracked::pimpl::implementation final {
public:
    int* value;

public:
    ~implementation() {
        delete this->value;
        ++tracked_destructed;
    }

    implementation(int input_value)
        : value(new int(input_value)) {
        ++tracked_constructed;
    }

    implementation(const implementation& other)
        : value(other.value ? new int(*other.value) : nullptr) {
        ++tracked_copy_constructed;
    }

    implementation(implementation&& other)
        : value(other.value) {
        other.value = nullptr;
        ++tracked_move_constructed;
    }

    implementation& operator=(const implementation& other) {
        if (this != &other) {
            delete this->value;
            this->value = other.value ? new int(*other.value) : nullptr;
        }
        ++tracked_copy_assigned;
        return *this;
    }

    implementation& operator=(implementation&& other) {
        if (this != &other) {
            delete this->value;
            this->value = other.value;
            other.value = nullptr;
        }
        ++tracked_move_assigned;
        return *this;
    }
};

test_pimpl_tracked::~test_pimpl_tracked() = default;

test_pimpl_tracked::test_pimpl_tracked(int input_value)
    : pimpl(input_value) {
}

test_pimpl_tracked::test_pimpl_tracked(const test_pimpl_tracked& other) = default;

test_pimpl_tracked::test_pimpl_tracked(test_pimpl_tracked&& other) = default;

test_pimpl_tracked& test_pimpl_tracked::operator=(const test_pimpl_tracked& other) = default;

test_pimpl_tracked& test_pimpl_tracked::operator=(test_pimpl_tracked&& other) = default;

int test_pimpl_tracked::return_value() const {
    return (*this)->value ? *(*this)->value : -1;
}

TEST(pimpl, traits, standard) {
    REQUIRE(std::is_pod<test_pimpl>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<test_pimpl>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<test_pimpl>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<test_pimpl>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(pimpl, evaluate, return_value) {
    test_pimpl pimpl(1);
    REQUIRE(pimpl.return_value() == 1);
}

TEST(pimpl, evaluate, copying) {
    tracked_reset();
    {
        test_pimpl_tracked pimpl1(1);
        REQUIRE(tracked_constructed == 1);
        REQUIRE(pimpl1.return_value() == 1);

        // Copy constructing must use the copy constructor of the implementation.
        test_pimpl_tracked pimpl2(pimpl1);
        REQUIRE(tracked_copy_constructed == 1);
        REQUIRE(pimpl1.return_value() == 1);
        REQUIRE(pimpl2.return_value() == 1);

        const test_pimpl_tracked& pimpl1_const = pimpl1;
        test_pimpl_tracked pimpl3(pimpl1_const);
        REQUIRE(tracked_copy_constructed == 2);
        REQUIRE(pimpl1.return_value() == 1);
        REQUIRE(pimpl3.return_value() == 1);

        // Copy assigning must use the copy assignment operator of the implementation.
        test_pimpl_tracked pimpl4(4);
        REQUIRE(tracked_constructed == 2);
        REQUIRE(pimpl4.return_value() == 4);
        pimpl2 = pimpl4;
        REQUIRE(tracked_copy_assigned == 1);
        REQUIRE(pimpl2.return_value() == 4);
        REQUIRE(pimpl4.return_value() == 4);

        // Copy assigning from itself must keep the implementation.
        const test_pimpl_tracked& pimpl4_const = pimpl4;
        pimpl4 = pimpl4_const;
        REQUIRE(tracked_copy_assigned == 2);
        REQUIRE(pimpl4.return_value() == 4);

        REQUIRE(tracked_move_constructed == 0);
        REQUIRE(tracked_move_assigned == 0);
        REQUIRE(tracked_destructed == 0);
    }

    // Every implementation that was constructed must have been destructed.
    REQUIRE(tracked_constructed + tracked_copy_constructed + tracked_move_constructed == tracked_destructed, "Expected %d == %d", tracked_constructed + tracked_copy_constructed + tracked_move_constructed, tracked_destructed);
}

TEST(pimpl, evaluate, moving) {
    tracked_reset();
    {
        test_pimpl_tracked pimpl1(1);
        REQUIRE(tracked_constructed == 1);
        REQUIRE(pimpl1.return_value() == 1);

        // Move constructing must use the move constructor of the implementation.
        test_pimpl_tracked pimpl2(static_cast<test_pimpl_tracked&&>(pimpl1));
        REQUIRE(tracked_move_constructed == 1);
        REQUIRE(pimpl1.return_value() == -1);
        REQUIRE(pimpl2.return_value() == 1);

        // Move assigning must use the move assignment operator of the implementation.
        test_pimpl_tracked pimpl3(3);
        REQUIRE(tracked_constructed == 2);
        REQUIRE(pimpl3.return_value() == 3);
        pimpl1 = static_cast<test_pimpl_tracked&&>(pimpl3);
        REQUIRE(tracked_move_assigned == 1);
        REQUIRE(pimpl1.return_value() == 3);
        REQUIRE(pimpl3.return_value() == -1);

        REQUIRE(tracked_copy_constructed == 0);
        REQUIRE(tracked_copy_assigned == 0);
        REQUIRE(tracked_destructed == 0);
    }

    // Every implementation that was constructed must have been destructed.
    REQUIRE(tracked_constructed + tracked_copy_constructed + tracked_move_constructed == tracked_destructed, "Expected %d == %d", tracked_constructed + tracked_copy_constructed + tracked_move_constructed, tracked_destructed);
}
