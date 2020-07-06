/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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
#include <require.tests.hpp>

#include <string/string_literal_encryption>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(string_literal_encryption, traits, standard) {
    REQUIRE((std::is_pod<gtl::string_literal_encryption>::value == true));

    REQUIRE((std::is_trivial<gtl::string_literal_encryption>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::string_literal_encryption>::value == true));

    REQUIRE((std::is_standard_layout<gtl::string_literal_encryption>::value == true));
}

TEST(string_literal_encryption, constructor, empty) {
    gtl::string_literal_encryption string_literal_encryption;
    testbench::do_not_optimise_away(string_literal_encryption);
}

