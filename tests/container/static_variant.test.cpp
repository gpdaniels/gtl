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

#include <testbench/comparison.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <container/static_variant>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(static_variant, traits, standard) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type_1) -> void {
            using type1 = typename decltype(test_type_1)::type;
            REQUIRE((std::is_pod<gtl::static_variant<type1>>::value == false), "Expected std::is_pod to be false.");
            REQUIRE((std::is_trivial<gtl::static_variant<type1>>::value == false), "Expected std::is_trivial to be false.");
            REQUIRE((std::is_trivially_copyable<gtl::static_variant<type1>>::value == false), "Expected std::is_trivially_copyable to be false.");
            REQUIRE((std::is_standard_layout<gtl::static_variant<type1>>::value == true), "Expected std::is_standard_layout to be true.");
            testbench::test_template<testbench::test_types>(
                [](auto test_type_2) -> void {
                    using type2 = typename decltype(test_type_2)::type;
                    REQUIRE((std::is_pod<gtl::static_variant<type1, type2>>::value == false), "Expected std::is_pod to be false.");
                    REQUIRE((std::is_trivial<gtl::static_variant<type1, type2>>::value == false), "Expected std::is_trivial to be false.");
                    REQUIRE((std::is_trivially_copyable<gtl::static_variant<type1, type2>>::value == false), "Expected std::is_trivially_copyable to be false.");
                    REQUIRE((std::is_standard_layout<gtl::static_variant<type1, type2>>::value == true), "Expected std::is_standard_layout to be true.");
                }
            );
        }
    );
}

TEST(static_variant, constructor, empty) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type_1) -> void {
            using type1 = typename decltype(test_type_1)::type;
            gtl::static_variant<type1> variant1;
            testbench::do_not_optimise_away(variant1);
            testbench::test_template<testbench::test_types>(
                [](auto test_type_2) -> void {
                    using type2 = typename decltype(test_type_2)::type;
                    gtl::static_variant<type1, type2> variant2;
                    testbench::do_not_optimise_away(variant2);
                }
            );
        }
    );
}

TEST(static_variant, constructor, value) {
    gtl::static_variant<int, float> static_variant_e;
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);
}

TEST(static_variant, function, empty) {
    gtl::static_variant<int, float> static_variant_e;
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(static_variant_e.empty() == true);
    REQUIRE(static_variant_i.empty() == false);
    REQUIRE(static_variant_f.empty() == false);
}

TEST(static_variant, function, is) {
    gtl::static_variant<int, float> static_variant_e;
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(static_variant_e.is<int>() == false);
    REQUIRE(static_variant_e.is<float>() == false);
    REQUIRE(static_variant_i.is<int>() == true);
    REQUIRE(static_variant_i.is<float>() == false);
    REQUIRE(static_variant_f.is<int>() == false);
    REQUIRE(static_variant_f.is<float>() == true);

    static_variant_i = 1.0f;
    static_variant_f = 1234;

    REQUIRE(static_variant_e.is<int>() == false);
    REQUIRE(static_variant_e.is<float>() == false);
    REQUIRE(static_variant_i.is<int>() == false);
    REQUIRE(static_variant_i.is<float>() == true);
    REQUIRE(static_variant_f.is<int>() == true);
    REQUIRE(static_variant_f.is<float>() == false);
}

TEST(static_variant, function, as) {
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(testbench::is_value_equal(static_variant_i.as<int>(), 1234));
    REQUIRE(testbench::is_value_equal(static_variant_i.as<float>(), 1234.0f));
    REQUIRE(testbench::is_value_equal(static_variant_f.as<int>(), 1));
    REQUIRE(testbench::is_value_equal(static_variant_f.as<float>(), 1.0f));
}

TEST(static_variant, function, get) {
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(testbench::is_value_equal(static_variant_i.get<int>(), 1234));
    REQUIRE(testbench::is_value_equal(static_variant_f.get<float>(), 1.0f));
}

TEST(static_variant, function, set) {
    gtl::static_variant<int, float> static_variant_i(1234);
    gtl::static_variant<int, float> static_variant_f(1.0f);

    REQUIRE(testbench::is_value_equal(static_variant_i.get<int>(), 1234));
    REQUIRE(testbench::is_value_equal(static_variant_f.get<float>(), 1.0f));

    static_variant_i.set<int>(4321);
    static_variant_f.set<float>(0.1f);

    REQUIRE(testbench::is_value_equal(static_variant_i.get<int>(), 4321));
    REQUIRE(testbench::is_value_equal(static_variant_f.get<float>(), 0.1f));
}
