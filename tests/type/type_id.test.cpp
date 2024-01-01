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
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <type/type_id>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(type_id, traits, standard) {
    REQUIRE(std::is_pod<gtl::type_id>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::type_id>::value == false, "Expected std::is_trivial to be false.");

    #if defined(__clang__)
        REQUIRE(std::is_trivially_copyable<gtl::type_id>::value == true, "Expected std::is_trivially_copyable to be true.");
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
        REQUIRE(std::is_trivially_copyable<gtl::type_id>::value == true, "Expected std::is_trivially_copyable to be true.");
    #elif defined(_MSC_VER)
        #if 0
            // Not reliable across compiler / os.
            REQUIRE(std::is_trivially_copyable<gtl::type_id>::value == false, "Expected std::is_trivially_copyable to be false.");
        #endif
    #endif

    REQUIRE(std::is_standard_layout<gtl::type_id>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(type_id, constructor, type) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_id type_id(type{});
            testbench::do_not_optimise_away(type_id);
        }
    );
}

TEST(type_id, function, id) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_id type_id(type{});
            unsigned long long int id = type_id.id();
            testbench::do_not_optimise_away(id);
        }
    );
}

TEST(type_id, operator, unsigned_long_long_int) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::type_id type_id(type{});
            unsigned long long int id = type_id;
            testbench::do_not_optimise_away(id);
        }
    );
}

TEST(type_id, operator, equality) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type1)->void {
            using type1 = typename decltype(test_type1)::type;
            bool match_found = false;
            testbench::test_template<testbench::test_types>(
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

TEST(type_id, operator, inequality) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type1)->void {
            using type1 = typename decltype(test_type1)::type;
            bool match_found = false;
            testbench::test_template<testbench::test_types>(
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
