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
#include <macro.tests.hpp>
#include <template.tests.hpp>

#include <type_id>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::type_id) >= 1, "sizeof(gtl::type_id) = %ld, expected >= %lld", sizeof(gtl::type_id), 1ull);

    REQUIRE(std::is_pod<gtl::type_id>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::type_id>::value == false, "Expected std::is_trivial to be false.");

    #if defined(__clang__)
        REQUIRE(std::is_trivially_copyable<gtl::type_id>::value == true, "Expected std::is_trivially_copyable to be true.");
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER))
        REQUIRE(std::is_trivially_copyable<gtl::type_id>::value == true, "Expected std::is_trivially_copyable to be true.");
    #elif defined(_MSC_VER)
        REQUIRE(std::is_trivially_copyable<gtl::type_id>::value == false, "Expected std::is_trivially_copyable to be false.");
    #endif

    REQUIRE(std::is_standard_layout<gtl::type_id>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(constructor, type) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_id type_id(type{});
            do_not_optimise_away(type_id);
        }
    );
}

TEST(function, id) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_id type_id(type{});
            unsigned long long int id = type_id.id();
            do_not_optimise_away(id);
        }
    );
}

TEST(operator, unsigned_long_long_int) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_id type_id(type{});
            unsigned long long int id = type_id;
            do_not_optimise_away(id);
        }
    );
}

TEST(operator, equality) {
    test_template<test_types>(
        [](auto test_type1)->void {
            using type1 = typename decltype(test_type1)::type;
            bool match_found = false;
            test_template<test_types>(
                [&match_found](auto test_type2)->void {
                    using type2 = typename decltype(test_type2)::type;
                    if (gtl::type_id(type1{}) == gtl::type_id(type2{})) {
                        REQUIRE(match_found == false);
                        match_found = true;
                    }
                }
            );
            REQUIRE(match_found == true);
        }
    );
}

TEST(operator, inequality) {
    test_template<test_types>(
        [](auto test_type1)->void {
            using type1 = typename decltype(test_type1)::type;
            bool match_found = false;
            test_template<test_types>(
                [&match_found](auto test_type2)->void {
                    using type2 = typename decltype(test_type2)::type;
                    if (gtl::type_id(type1{}) == gtl::type_id(type2{})) {
                        REQUIRE(match_found == false);
                        match_found = true;
                    }
                }
            );
            REQUIRE(match_found == true);
        }
    );
}
