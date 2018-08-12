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
#include <macro.tests.hpp>

#include <enum_name>

#include <type_traits>

enum class test_enum {
    first,
    second = 123456,
    third
};

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::enum_name<test_enum, test_enum::first>) >= 1, "sizeof(gtl::enum_name<test_enum>) = %ld, expected >= %lld", sizeof(gtl::enum_name<test_enum, test_enum::first>), 1ull);

    REQUIRE((std::is_pod<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_pod to be true.");

    REQUIRE((std::is_trivial<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_trivial to be true.");

    REQUIRE((std::is_trivially_copyable<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_trivially_copyable to be true.");

    REQUIRE((std::is_standard_layout<gtl::enum_name<test_enum, test_enum::first> >::value == true), "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::enum_name<test_enum, test_enum::first> enum_name;
    DoNotOptimiseAway(enum_name);
}

TEST(function, name) {
    #if defined(__clang__) || defined(_MSC_VER)
        auto strcmp = [](const char* LHS, const char* RHS) -> bool {
            while (*LHS && (*LHS++==*RHS++));
            return *LHS == *RHS;
        };

        gtl::enum_name<test_enum, test_enum::first> enum_name1;
        REQUIRE(strcmp(enum_name1.name(), "first") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name1.name(), "first");

        gtl::enum_name<test_enum, test_enum::second> enum_name2;
        REQUIRE(strcmp(enum_name2.name(), "second") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name2.name(), "second");

        gtl::enum_name<test_enum, test_enum::third> enum_name3;
        REQUIRE(strcmp(enum_name3.name(), "third") == true, "gtl::test_enum<test_enum, first>::name() = '%s', expected '%s'", enum_name3.name(), "third");
    #endif
}

