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
#include <data.tests.hpp>
#include <macro.tests.hpp>
#include <template.tests.hpp>

#include <any>

#include <type_traits>

TEST(traits, sizeof) {
    REQUIRE(sizeof(gtl::any) >= 1, "sizeof(gtl::any) = %ld, expected >= %lld", sizeof(gtl::any), 1ull);
}

TEST(traits, is_not_pod) {
    REQUIRE(std::is_pod<gtl::any>::value == false, "Expected std::is_pod to be false.");
}

TEST(traits, is_not_trivial) {
    REQUIRE(std::is_trivial<gtl::any>::value == false, "Expected std::is_trivial to be false.");
}

TEST(traits, is_not_trivially_copyable) {
    REQUIRE(std::is_trivially_copyable<gtl::any>::value == false, "Expected std::is_trivially_copyable to be false.");
}

TEST(traits, is_standard_layout) {
    REQUIRE(std::is_standard_layout<gtl::any>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::any any;
    UNOPTIMISED(any);
}

TEST(constructor, value) {
    gtl::any any(1);
    UNOPTIMISED(any);
}

TEST(evaluation, any) {
    gtl::any any(1);
    REQUIRE(static_cast<int>(any) == 1, "Unexpected value '%d' in any, expected %d.", static_cast<int>(any), 1);

    any = 1.0;
    REQUIRE(static_cast<int>(any) != 1, "Unexpected value '%d' in any, expected %f.", static_cast<int>(any), 1.0);
    REQUIRE(static_cast<double>(any) == 1.0,  "Unexpected value '%f' in any, expected %f.", static_cast<double>(any), 1.0);

    auto strcmp = [](const char* LHS, const char* RHS) -> bool {
        while (*LHS && (*LHS++==*RHS++));
        return *LHS == *RHS;
    };

    const char* string = "Hello world!";
    any = string;
    REQUIRE(strcmp(static_cast<const char*>(any), string) == true, "gtl::any = '%s', expected '%s'", static_cast<char*>(any), string);
}
