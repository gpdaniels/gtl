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
#include <require.tests.hpp>
#include <utility/pimpl.test.hpp>

#include <utility/pimpl>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
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
