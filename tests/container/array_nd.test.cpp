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

#include <container/array_nd>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(array_nd, traits, standard) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE((std::is_pod<gtl::array_nd<type> >::value == false), "Expected std::is_pod to be false.");
            REQUIRE((std::is_trivial<gtl::array_nd<type> >::value == false), "Expected std::is_trivial to be false.");
            REQUIRE((std::is_trivially_copyable<gtl::array_nd<type> >::value == true), "Expected std::is_trivially_copyable to be true.");
            REQUIRE((std::is_standard_layout<gtl::array_nd<type> >::value == true), "Expected std::is_standard_layout to be true.");
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    REQUIRE((std::is_pod<gtl::array_nd<type, value1> >::value == false), "Expected std::is_pod to be false.");
                    REQUIRE((std::is_trivial<gtl::array_nd<type, value1> >::value == false), "Expected std::is_trivial to be false.");
                    REQUIRE((std::is_trivially_copyable<gtl::array_nd<type, value1> >::value == (value1!=0)), "Expected std::is_trivially_copyable to be %d.", (value1==0));
                    REQUIRE((std::is_standard_layout<gtl::array_nd<type, value1> >::value == true), "Expected std::is_standard_layout to be true.");
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            REQUIRE((std::is_pod<gtl::array_nd<type, value1, value2> >::value == false), "Expected std::is_pod to be false.");
                            REQUIRE((std::is_trivial<gtl::array_nd<type, value1, value2> >::value == false), "Expected std::is_trivial to be false.");
                            REQUIRE((std::is_trivially_copyable<gtl::array_nd<type, value1, value2> >::value == ((value1!=0) && (value2!=0))), "Expected std::is_trivially_copyable to be %d.", ((value1==0) && (value2==0)));
                            REQUIRE((std::is_standard_layout<gtl::array_nd<type, value1, value2> >::value == true), "Expected std::is_standard_layout to be true.");
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    REQUIRE((std::is_pod<gtl::array_nd<type, value1, value2, value3> >::value == false), "Expected std::is_pod to be false.");
                                    REQUIRE((std::is_trivial<gtl::array_nd<type, value1, value2, value3> >::value == false), "Expected std::is_trivial to be false.");
                                    REQUIRE((std::is_trivially_copyable<gtl::array_nd<type, value1, value2, value3> >::value == ((value1!=0) && (value2!=0) && (value3!=0))), "Expected std::is_trivially_copyable to be %d.", ((value1==0) && (value2==0) && (value3==0)));
                                    REQUIRE((std::is_standard_layout<gtl::array_nd<type, value1, value2, value3> >::value == true), "Expected std::is_standard_layout to be true.");
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(array_nd, constructor, static) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            testbench::do_not_optimise_away(array_nd_0d);
            testbench::test_template<testbench::value_collection<1, 10>>(
                [](auto value_1)->void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::array_nd<type, value1> array_nd_1d;
                    testbench::do_not_optimise_away(array_nd_1d);
                    testbench::test_template<testbench::value_collection<1, 10>>(
                        [](auto value_2)->void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::array_nd<type, value1, value2> array_nd_2d;
                            testbench::do_not_optimise_away(array_nd_2d);
                            testbench::test_template<testbench::value_collection<1, 10>>(
                                [](auto value_3)->void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                    testbench::do_not_optimise_away(array_nd_3d);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(array_nd, constructor, dynamic) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            testbench::do_not_optimise_away(array_nd_0d);
            testbench::test_template<testbench::value_collection<1, 10>>(
                [](auto value_1)->void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::array_nd<type, 0> array_nd_1d(value1);
                    testbench::do_not_optimise_away(array_nd_1d);
                    testbench::test_template<testbench::value_collection<1, 10>>(
                        [](auto value_2)->void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::array_nd<type, 0, 0> array_nd_2d(value1, value2);
                            testbench::do_not_optimise_away(array_nd_2d);
                            testbench::test_template<testbench::value_collection<1, 10>>(
                                [](auto value_3)->void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::array_nd<type, 0, 0, 0> array_nd_3d(value1, value2, value3);
                                    testbench::do_not_optimise_away(array_nd_3d);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(array_nd, function, dimensions) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            REQUIRE(array_nd_0d.dimensions() == 0, "array_nd_0d.dimensions() = %lld, expected %lld", array_nd_0d.dimensions(), 0ull);
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::array_nd<type, value1> array_nd_1d;
                    REQUIRE(array_nd_1d.dimensions() == 1, "array_nd_1d.dimensions() = %lld, expected %lld", array_nd_1d.dimensions(), 1ull);
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::array_nd<type, value1, value2> array_nd_2d;
                            REQUIRE(array_nd_2d.dimensions() == 2, "array_nd_2d.dimensions() = %lld, expected %lld", array_nd_2d.dimensions(), 2ull);
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                    REQUIRE(array_nd_3d.dimensions() == 3, "array_nd_3d.dimensions() = %lld, expected %lld", array_nd_3d.dimensions(), 3ull);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(array_nd, function, size) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            REQUIRE(array_nd_0d.size() == 0);
            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    using type_value1 = decltype(value_1);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    gtl::array_nd<type, value1> array_nd_1d;
                    REQUIRE(array_nd_1d.size() == value1);
                    REQUIRE(array_nd_1d.size(0) == value1);
                    testbench::test_template<testbench::value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            using type_value2 = decltype(value_2);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            gtl::array_nd<type, value1, value2> array_nd_2d;
                            REQUIRE(array_nd_2d.size() == value1 * value2);
                            REQUIRE(array_nd_2d.size(0) == value1);
                            REQUIRE(array_nd_2d.size(1) == value2);
                            testbench::test_template<testbench::value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    using type_value3 = decltype(value_3);
                                    constexpr static const unsigned long long value1 = type_value1::value;
                                    constexpr static const unsigned long long value2 = type_value2::value;
                                    constexpr static const unsigned long long value3 = type_value3::value;
                                    gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                    REQUIRE(array_nd_3d.size() == value1 * value2 * value3);
                                    REQUIRE(array_nd_3d.size(0) == value1);
                                    REQUIRE(array_nd_3d.size(1) == value2);
                                    REQUIRE(array_nd_3d.size(2) == value3);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(array_nd, operator, parenthesis) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10>>(
        [](auto test_type, auto value_1)->void {
            using type = typename decltype(test_type)::type;
            using type_value1 = decltype(value_1);
            constexpr static const unsigned long long value1 = type_value1::value;
            for (const type& value : testbench::test_data<type>()) {
                gtl::array_nd<type, value1> array_nd_1d;
                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                    array_nd_1d(index1) = value;
                    REQUIRE(testbench::is_value_equal(array_nd_1d(index1), value));
                }
            }
            testbench::test_template<testbench::value_collection<1, 10>>(
                [](auto value_2)->void {
                    using type_value2 = decltype(value_2);
                    constexpr static const unsigned long long value1 = type_value1::value;
                    constexpr static const unsigned long long value2 = type_value2::value;
                    for (const type& value : testbench::test_data<type>()) {
                        gtl::array_nd<type, value1, value2> array_nd_2d;
                        for (unsigned int index1 = 0; index1 < value1; ++index1) {
                            for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                array_nd_2d(index1, index2) = value;
                                REQUIRE(testbench::is_value_equal(array_nd_2d(index1, index2), value));
                            }
                        }
                    }
                    testbench::test_template<testbench::value_collection<1, 10>>(
                        [](auto value_3)->void {
                            using type_value3 = decltype(value_3);
                            constexpr static const unsigned long long value1 = type_value1::value;
                            constexpr static const unsigned long long value2 = type_value2::value;
                            constexpr static const unsigned long long value3 = type_value3::value;
                            for (const type& value : testbench::test_data<type>()) {
                                gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                                    for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                        for (unsigned int index3 = 0; index3 < value3; ++index3) {
                                            array_nd_3d(index1, index2, index3) = value;
                                            REQUIRE(testbench::is_value_equal(array_nd_3d(index1, index2, index3), value));
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
