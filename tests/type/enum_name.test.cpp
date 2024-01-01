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
#include <comparison.tests.hpp>
#include <data.tests.hpp>
#include <require.tests.hpp>

#include <type/enum_name>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

enum class test_enum {
    first,
    second = 123456,
    third
};

TEST(enum_name, traits, standard) {
    REQUIRE((std::is_pod<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_pod to be true.");

    REQUIRE((std::is_trivial<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_trivial to be true.");

    REQUIRE((std::is_trivially_copyable<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_trivially_copyable to be true.");

    REQUIRE((std::is_standard_layout<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(enum_name, constructor, empty) {
    gtl::enum_name<test_enum, test_enum::first> enum_name;
    testbench::do_not_optimise_away(enum_name);
}

TEST(enum_name, function, name) {
    #if defined(__clang__) || defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ >= 9))
        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name(), "test_enum::first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "test_enum::first");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name(), "test_enum::second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "test_enum::second");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name(), "test_enum::third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "test_enum::third");
    #elif (defined(__GNUC__)  && (__GNUC__ <= 8))
        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name(), "(test_enum)0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "(test_enum)0");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name(), "(test_enum)123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "(test_enum)123456");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name(), "(test_enum)123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "(test_enum)123457");
    #endif
}

TEST(enum_name, function, name_type) {
    gtl::enum_name<test_enum, test_enum::first> enum_name1;
    REQUIRE(testbench::is_string_same(enum_name1.name_type(), "test_enum") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_type(), "test_enum");

    gtl::enum_name<test_enum, test_enum::second> enum_name2;
    REQUIRE(testbench::is_string_same(enum_name2.name_type(), "test_enum") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_type(), "test_enum");

    gtl::enum_name<test_enum, test_enum::third> enum_name3;
    REQUIRE(testbench::is_string_same(enum_name3.name_type(), "test_enum") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_type(), "test_enum");
}

TEST(enum_name, function, name_value) {
    #if defined(__clang__) || defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ >= 9))
        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name_value(), "first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "first");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name_value(), "second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "second");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name_value(), "third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "third");
    #elif (defined(__GNUC__)  && (__GNUC__ <= 8))
        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name_value(), "0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "0");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name_value(), "123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "123456");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name_value(), "123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "123457");
    #endif
}

namespace test_namespace {
    enum class test_enum {
        first,
        second = 123456,
        third
    };
}

TEST(enum_name, function, name_with_namespaces) {
    #if defined(__clang__) || defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ >= 9))
        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name(), "test_namespace::test_enum::first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "test_namespace::test_enum::first");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name(), "test_namespace::test_enum::second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "test_namespace::test_enum::second");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name(), "test_namespace::test_enum::third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "test_namespace::test_enum::third");
    #elif (defined(__GNUC__)  && (__GNUC__ <= 8))
        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name(), "(test_namespace::test_enum)0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "(test_namespace::test_enum)0");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name(), "(test_namespace::test_enum)123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "(test_namespace::test_enum)123456");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name(), "(test_namespace::test_enum)123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "(test_namespace::test_enum)123457");
    #endif
}

TEST(enum_name, function, name_type_with_namespaces) {
    gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
    REQUIRE(testbench::is_string_same(enum_name1.name_type(), "test_namespace::test_enum") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_type(), "test_namespace::test_enum");

    gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
    REQUIRE(testbench::is_string_same(enum_name2.name_type(), "test_namespace::test_enum") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_type(), "test_namespace::test_enum");

    gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
    REQUIRE(testbench::is_string_same(enum_name3.name_type(), "test_namespace::test_enum") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_type(), "test_namespace::test_enum");
}

TEST(enum_name, function, name_value_with_namespaces) {
    #if defined(__clang__) || defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ >= 9))
        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name_value(), "first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "first");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name_value(), "second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "second");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name_value(), "third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "third");
    #elif (defined(__GNUC__)  && (__GNUC__ <= 8))
        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(testbench::is_string_same(enum_name1.name_value(), "0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "0");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(testbench::is_string_same(enum_name2.name_value(), "123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "123456");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(testbench::is_string_same(enum_name3.name_value(), "123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "123457");
    #endif
}
