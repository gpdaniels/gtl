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
    REQUIRE(sizeof(gtl::enum_name<test_enum, test_enum::first>) >= 1, "sizeof(gtl::enum_name<test_enum>) = %ld, expected >= %lld", sizeof(gtl::enum_name<test_enum, test_enum::first>), 1ull);

    REQUIRE((std::is_pod<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_pod to be true.");

    REQUIRE((std::is_trivial<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_trivial to be true.");

    REQUIRE((std::is_trivially_copyable<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_trivially_copyable to be true.");

    REQUIRE((std::is_standard_layout<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(enum_name, constructor, empty) {
    gtl::enum_name<test_enum, test_enum::first> enum_name;
    do_not_optimise_away(enum_name);
}

TEST(enum_name, function, name) {
    #if defined(__clang__) || defined(_MSC_VER)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name(), "test_enum::first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "test_enum::first");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name(), "test_enum::second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "test_enum::second");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name(), "test_enum::third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "test_enum::third");
    #elif defined(__GNUC__)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name(), "(test_enum)0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "(test_enum)0");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name(), "(test_enum)123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "(test_enum)123456");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name(), "(test_enum)123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "(test_enum)123457");
    #endif
}

TEST(enum_name, function, name_type) {
    auto strcmp = [](const char* LHS, const char* RHS) -> bool {
        while (*LHS && (*LHS == *RHS)) {
            ++LHS;
            ++RHS;
        }
        return *LHS == *RHS;
    };

    gtl::enum_name<test_enum, test_enum::first> enum_name1;
    REQUIRE(strcmp(enum_name1.name_type(), "test_enum") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_type(), "test_enum");

    gtl::enum_name<test_enum, test_enum::second> enum_name2;
    REQUIRE(strcmp(enum_name2.name_type(), "test_enum") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_type(), "test_enum");

    gtl::enum_name<test_enum, test_enum::third> enum_name3;
    REQUIRE(strcmp(enum_name3.name_type(), "test_enum") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_type(), "test_enum");
}

TEST(enum_name, function, name_value) {
    #if defined(__clang__) || defined(_MSC_VER)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name_value(), "first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "first");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name_value(), "second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "second");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name_value(), "third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "third");
    #elif defined(__GNUC__)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name_value(), "0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "0");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name_value(), "123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "123456");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name_value(), "123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "123457");
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
    #if defined(__clang__) || defined(_MSC_VER)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name(), "test_namespace::test_enum::first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "test_namespace::test_enum::first");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name(), "test_namespace::test_enum::second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "test_namespace::test_enum::second");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name(), "test_namespace::test_enum::third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "test_namespace::test_enum::third");
    #elif defined(__GNUC__)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name(), "(test_namespace::test_enum)0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "(test_namespace::test_enum)0");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name(), "(test_namespace::test_enum)123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name(), "(test_namespace::test_enum)123456");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name(), "(test_namespace::test_enum)123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name(), "(test_namespace::test_enum)123457");
    #endif
}

TEST(enum_name, function, name_type_with_namespaces) {
    auto strcmp = [](const char* LHS, const char* RHS) -> bool {
        while (*LHS && (*LHS == *RHS)) {
            ++LHS;
            ++RHS;
        }
        return *LHS == *RHS;
    };

    gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
    REQUIRE(strcmp(enum_name1.name_type(), "test_namespace::test_enum") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_type(), "test_namespace::test_enum");

    gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
    REQUIRE(strcmp(enum_name2.name_type(), "test_namespace::test_enum") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_type(), "test_namespace::test_enum");

    gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
    REQUIRE(strcmp(enum_name3.name_type(), "test_namespace::test_enum") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_type(), "test_namespace::test_enum");
}

TEST(enum_name, function, name_value_with_namespaces) {
    #if defined(__clang__) || defined(_MSC_VER)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name_value(), "first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "first");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name_value(), "second") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "second");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name_value(), "third") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "third");
    #elif defined(__GNUC__)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS == *RHS)) {
                ++LHS;
                ++RHS;
            }
            return *LHS == *RHS;
        };

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name_value(), "0") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name_value(), "0");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name_value(), "123456") == true, "gtl::test_enum<test_enum, second>::name() = '%s', expected '%s'", enum_name2.name_value(), "123456");

        gtl::enum_name<test_namespace::test_enum, test_namespace::test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name_value(), "123457") == true, "gtl::test_enum<test_enum, third>::name() = '%s', expected '%s'", enum_name3.name_value(), "123457");
    #endif
}
