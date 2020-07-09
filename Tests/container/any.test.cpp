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
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <data.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <container/any>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(any, traits, standard) {
    REQUIRE(std::is_pod<gtl::any>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::any>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::any>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::any>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(any, constructor, empty) {
    gtl::any any;
    testbench::do_not_optimise_away(any);
}

TEST(any, constructor, value) {
    gtl::any any(1);
    testbench::do_not_optimise_away(any);
}

TEST(any, evaluation, any) {
    gtl::any any(1);
    REQUIRE(static_cast<int>(any) == 1, "Unexpected value '%d' in any, expected %d.", static_cast<int>(any), 1);

    any = 1.0;
    REQUIRE(static_cast<int>(any) != 1, "Unexpected value '%d' in any, expected %f.", static_cast<int>(any), 1.0);
    REQUIRE(static_cast<double>(any) == 1.0,  "Unexpected value '%f' in any, expected %f.", static_cast<double>(any), 1.0);

    const char* string = "Hello world!";
    any = string;
    REQUIRE(testbench::is_string_same(static_cast<const char*>(any), string) == true, "gtl::any = '%s', expected '%s'", static_cast<char*>(any), string);
}
