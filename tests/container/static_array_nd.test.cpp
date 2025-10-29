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
#include <testbench/data.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <container/static_array_nd>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(static_array_nd, traits, standard) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type) -> void {
            using type = typename decltype(test_type)::type;
            REQUIRE((std::is_pod<gtl::static_array_nd<type>>::value == true), "Expected std::is_pod to be true.");
            REQUIRE((std::is_trivial<gtl::static_array_nd<type>>::value == true), "Expected std::is_trivial to be true.");
            REQUIRE((std::is_trivially_copyable<gtl::static_array_nd<type>>::value == true), "Expected std::is_trivially_copyable to be true.");
            REQUIRE((std::is_standard_layout<gtl::static_array_nd<type>>::value == true), "Expected std::is_standard_layout to be true.");
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1) -> void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    REQUIRE((std::is_pod<gtl::static_array_nd<type, value1>>::value == true), "Expected std::is_pod to be true.");
                    REQUIRE((std::is_trivial<gtl::static_array_nd<type, value1>>::value == true), "Expected std::is_trivial to be true.");
                    REQUIRE((std::is_trivially_copyable<gtl::static_array_nd<type, value1>>::value == true), "Expected std::is_trivially_copyable to be true.");
                    REQUIRE((std::is_standard_layout<gtl::static_array_nd<type, value1>>::value == true), "Expected std::is_standard_layout to be true.");
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2) -> void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            REQUIRE((std::is_pod<gtl::static_array_nd<type, value1, value2>>::value == true), "Expected std::is_pod to be true.");
                            REQUIRE((std::is_trivial<gtl::static_array_nd<type, value1, value2>>::value == true), "Expected std::is_trivial to be true.");
                            REQUIRE((std::is_trivially_copyable<gtl::static_array_nd<type, value1, value2>>::value == true), "Expected std::is_trivially_copyable to be true.");
                            REQUIRE((std::is_standard_layout<gtl::static_array_nd<type, value1, value2>>::value == true), "Expected std::is_standard_layout to be true.");
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3) -> void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    REQUIRE((std::is_pod<gtl::static_array_nd<type, value1, value2, value3>>::value == true), "Expected std::is_pod to be true.");
                                    REQUIRE((std::is_trivial<gtl::static_array_nd<type, value1, value2, value3>>::value == true), "Expected std::is_trivial to be true.");
                                    REQUIRE((std::is_trivially_copyable<gtl::static_array_nd<type, value1, value2, value3>>::value == true), "Expected std::is_trivially_copyable to be true.");
                                    REQUIRE((std::is_standard_layout<gtl::static_array_nd<type, value1, value2, value3>>::value == true), "Expected std::is_standard_layout to be true.");
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(static_array_nd, constructor, empty) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type) -> void {
            using type = typename decltype(test_type)::type;
            gtl::static_array_nd<type> static_array_nd_0d;
            testbench::do_not_optimise_away(static_array_nd_0d);
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1) -> void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::static_array_nd<type, value1> static_array_nd_1d;
                    testbench::do_not_optimise_away(static_array_nd_1d);
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2) -> void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::static_array_nd<type, value1, value2> static_array_nd_2d;
                            testbench::do_not_optimise_away(static_array_nd_2d);
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3) -> void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::static_array_nd<type, value1, value2, value3> static_array_nd_3d;
                                    testbench::do_not_optimise_away(static_array_nd_3d);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(static_array_nd, function, dimensions) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type) -> void {
            using type = typename decltype(test_type)::type;
            gtl::static_array_nd<type> static_array_nd_0d;
            REQUIRE(static_array_nd_0d.dimensions() == 0, "static_array_nd_0d.dimensions() = %lld, expected %lld", static_array_nd_0d.dimensions(), 0ull);
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1) -> void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::static_array_nd<type, value1> static_array_nd_1d;
                    REQUIRE(static_array_nd_1d.dimensions() == 1, "static_array_nd_1d.dimensions() = %lld, expected %lld", static_array_nd_1d.dimensions(), 1ull);
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2) -> void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::static_array_nd<type, value1, value2> static_array_nd_2d;
                            REQUIRE(static_array_nd_2d.dimensions() == 2, "static_array_nd_2d.dimensions() = %lld, expected %lld", static_array_nd_2d.dimensions(), 2ull);
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3) -> void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::static_array_nd<type, value1, value2, value3> static_array_nd_3d;
                                    REQUIRE(static_array_nd_3d.dimensions() == 3, "static_array_nd_3d.dimensions() = %lld, expected %lld", static_array_nd_3d.dimensions(), 3ull);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(static_array_nd, function, size) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type) -> void {
            using type = typename decltype(test_type)::type;
            gtl::static_array_nd<type> static_array_nd_0d;
            REQUIRE(static_array_nd_0d.size() == 0);
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1) -> void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::static_array_nd<type, value1> static_array_nd_1d;
                    REQUIRE(static_array_nd_1d.size() == value1);
                    REQUIRE(static_array_nd_1d.size(0) == value1);
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2) -> void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::static_array_nd<type, value1, value2> static_array_nd_2d;
                            REQUIRE(static_array_nd_2d.size() == value1);
                            REQUIRE(static_array_nd_2d.size(0) == value1);
                            REQUIRE(static_array_nd_2d.size(1) == value2);
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3) -> void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::static_array_nd<type, value1, value2, value3> static_array_nd_3d;
                                    REQUIRE(static_array_nd_3d.size() == value1);
                                    REQUIRE(static_array_nd_3d.size(0) == value1);
                                    REQUIRE(static_array_nd_3d.size(1) == value2);
                                    REQUIRE(static_array_nd_3d.size(2) == value3);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(static_array_nd, constructor, list_initialiser) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10>>(
        [](auto test_type, auto value_1) -> void {
            using type = typename decltype(test_type)::type;
            using type_value1 = decltype(value_1);
            constexpr static const unsigned long long value1 = type_value1::value;
            for (const type& value : testbench::test_data<type>()) {
                gtl::static_array_nd<type, value1> static_array_nd_1d = { { value } };
                testbench::do_not_optimise_away(static_array_nd_1d);
            }
            testbench::test_template<testbench::value_collection<1, 10>>(
                [](auto value_2) -> void {
                    using type_value2 = decltype(value_2);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    constexpr static const unsigned long long value2 = type_value2::value;
                    for (const type& value : testbench::test_data<type>()) {
                        gtl::static_array_nd<type, value1, value2> static_array_nd_2d = { { { { value } } } };
                        testbench::do_not_optimise_away(static_array_nd_2d);
                    }
                    testbench::test_template<testbench::value_collection<1, 10>>(
                        [](auto value_3) -> void {
                            using type_value3 = decltype(value_3);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            constexpr static const unsigned long long value3 = type_value3::value;
                            for (const type& value : testbench::test_data<type>()) {
                                gtl::static_array_nd<type, value1, value2, value3> static_array_nd_3d = { { { { { { value } } } } } };
                                testbench::do_not_optimise_away(static_array_nd_3d);
                            }
                        }
                    );
                }
            );
        }
    );
}

TEST(static_array_nd, operator, subscript) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10>>(
        [](auto test_type, auto value_1) -> void {
            using type = typename decltype(test_type)::type;
            using type_value1 = decltype(value_1);
            constexpr static const unsigned long long value1 = type_value1::value;
            for (const type& value : testbench::test_data<type>()) {
                gtl::static_array_nd<type, value1> static_array_nd_1d;
                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                    static_array_nd_1d[index1] = value;
                    REQUIRE(testbench::is_value_equal(static_array_nd_1d[index1], value));
                }
            }
            testbench::test_template<testbench::value_collection<1, 10>>(
                [](auto value_2) -> void {
                    using type_value2 = decltype(value_2);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    constexpr static const unsigned long long value2 = type_value2::value;
                    for (const type& value : testbench::test_data<type>()) {
                        gtl::static_array_nd<type, value1, value2> static_array_nd_2d;
                        for (unsigned int index1 = 0; index1 < value1; ++index1) {
                            for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                static_array_nd_2d[index1][index2] = value;
                                REQUIRE(testbench::is_value_equal(static_array_nd_2d[index1][index2], value));
                            }
                        }
                    }
                    testbench::test_template<testbench::value_collection<1, 10>>(
                        [](auto value_3) -> void {
                            using type_value3 = decltype(value_3);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            constexpr static const unsigned long long value3 = type_value3::value;
                            for (const type& value : testbench::test_data<type>()) {
                                gtl::static_array_nd<type, value1, value2, value3> static_array_nd_3d;
                                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                                    for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                        for (unsigned int index3 = 0; index3 < value3; ++index3) {
                                            static_array_nd_3d[index1][index2][index3] = value;
                                            REQUIRE(testbench::is_value_equal(static_array_nd_3d[index1][index2][index3], value));
                                        }
                                    }
                                }
                            }
                        }
                    );
                }
            );
        }
    );
}

TEST(static_array_nd, operator, parenthesis) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10>>(
        [](auto test_type, auto value_1) -> void {
            using type = typename decltype(test_type)::type;
            using type_value1 = decltype(value_1);
            constexpr static const unsigned long long value1 = type_value1::value;
            for (const type& value : testbench::test_data<type>()) {
                gtl::static_array_nd<type, value1> static_array_nd_1d;
                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                    static_array_nd_1d(index1) = value;
                    REQUIRE(testbench::is_value_equal(static_array_nd_1d(index1), value));
                }
            }
            testbench::test_template<testbench::value_collection<1, 10>>(
                [](auto value_2) -> void {
                    using type_value2 = decltype(value_2);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    constexpr static const unsigned long long value2 = type_value2::value;
                    for (const type& value : testbench::test_data<type>()) {
                        gtl::static_array_nd<type, value1, value2> static_array_nd_2d;
                        for (unsigned int index1 = 0; index1 < value1; ++index1) {
                            for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                static_array_nd_2d(index1, index2) = value;
                                REQUIRE(testbench::is_value_equal(static_array_nd_2d(index1, index2), value));
                            }
                        }
                    }
                    testbench::test_template<testbench::value_collection<1, 10>>(
                        [](auto value_3) -> void {
                            using type_value3 = decltype(value_3);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            constexpr static const unsigned long long value3 = type_value3::value;
                            for (const type& value : testbench::test_data<type>()) {
                                gtl::static_array_nd<type, value1, value2, value3> static_array_nd_3d;
                                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                                    for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                        for (unsigned int index3 = 0; index3 < value3; ++index3) {
                                            static_array_nd_3d(index1, index2, index3) = value;
                                            REQUIRE(testbench::is_value_equal(static_array_nd_3d(index1, index2, index3), value));
                                        }
                                    }
                                }
                            }
                        }
                    );
                }
            );
        }
    );
}
