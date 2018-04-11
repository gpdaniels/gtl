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

#include <array_nd>

#include <type_traits>

TEST(traits, sizeof) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE(sizeof(gtl::array_nd<type>) >= 1, "sizeof(gtl::array_nd<type>) = %ld, expected >= %lld", sizeof(gtl::array_nd<type>), 1ull);
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    REQUIRE(sizeof(gtl::array_nd<type, value1>) >= sizeof(type) * value1, "sizeof(gtl::array_nd<type, value1>) = %ld, expected >= %lld", sizeof(gtl::array_nd<type, value1>), sizeof(type) * value1);
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            REQUIRE(sizeof(gtl::array_nd<type, value1, value2>) >= sizeof(type) * value1 * value2, "sizeof(gtl::array_nd<type, value1, value2>) = %ld, expected >= %lld", sizeof(gtl::array_nd<type, value1, value2>), sizeof(type) * value1 * value2);
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
                                    REQUIRE(sizeof(gtl::array_nd<type, value1, value2, value3>) >= sizeof(type) * value1 * value2 * value3, "sizeof(gtl::array_nd<type, value1, value2, value3>) = %ld, expected >= %lld", sizeof(gtl::array_nd<type, value1, value2, value3>), sizeof(type) * value1 * value2 * value3);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(traits, is_pod) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE((std::is_pod<gtl::array_nd<type> >::value == true), "Expected std::is_pod to be true.");
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    REQUIRE((std::is_pod<gtl::array_nd<type, value1> >::value == true), "Expected std::is_pod to be true.");
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            REQUIRE((std::is_pod<gtl::array_nd<type, value1, value2> >::value == true), "Expected std::is_pod to be true.");
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
                                    REQUIRE((std::is_pod<gtl::array_nd<type, value1, value2, value3> >::value == true), "Expected std::is_pod to be true.");
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(traits, is_trivial) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE((std::is_trivial<gtl::array_nd<type> >::value == true), "Expected std::is_trivial to be true.");
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    REQUIRE((std::is_trivial<gtl::array_nd<type, value1> >::value == true), "Expected std::is_trivial to be true.");
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            REQUIRE((std::is_trivial<gtl::array_nd<type, value1, value2> >::value == true), "Expected std::is_trivial to be true.");
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
                                    REQUIRE((std::is_trivial<gtl::array_nd<type, value1, value2, value3> >::value == true), "Expected std::is_trivial to be true.");
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(traits, is_trivially_copyable) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE((std::is_trivially_copyable<gtl::array_nd<type> >::value == true), "Expected std::is_trivially_copyable to be true.");
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    REQUIRE((std::is_trivially_copyable<gtl::array_nd<type, value1> >::value == true), "Expected std::is_trivially_copyable to be true.");
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            REQUIRE((std::is_trivially_copyable<gtl::array_nd<type, value1, value2> >::value == true), "Expected std::is_trivially_copyable to be true.");
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
                                    REQUIRE((std::is_trivially_copyable<gtl::array_nd<type, value1, value2, value3> >::value == true), "Expected std::is_trivially_copyable to be true.");
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(traits, is_standard_layout) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            REQUIRE((std::is_standard_layout<gtl::array_nd<type> >::value == true), "Expected std::is_standard_layout to be true.");
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    REQUIRE((std::is_standard_layout<gtl::array_nd<type, value1> >::value == true), "Expected std::is_standard_layout to be true.");
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            REQUIRE((std::is_standard_layout<gtl::array_nd<type, value1, value2> >::value == true), "Expected std::is_standard_layout to be true.");
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
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

TEST(constructor, empty) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            UNOPTIMISED(array_nd_0d);
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    gtl::array_nd<type, value1> array_nd_1d;
                    UNOPTIMISED(array_nd_1d);
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            gtl::array_nd<type, value1, value2> array_nd_2d;
                            UNOPTIMISED(array_nd_2d);
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
                                    gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                    UNOPTIMISED(array_nd_3d);
                                }
                            );
                        }
                    );
                }
            );
        }
    );
}

TEST(function, dimensions) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            REQUIRE(array_nd_0d.dimensions() == 0, "array_nd_0d.dimensions() = %lld, expected %lld", array_nd_0d.dimensions(), 0ull);
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    gtl::array_nd<type, value1> array_nd_1d;
                    REQUIRE(array_nd_1d.dimensions() == 1, "array_nd_1d.dimensions() = %lld, expected %lld", array_nd_1d.dimensions(), 1ull);
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            gtl::array_nd<type, value1, value2> array_nd_2d;
                            REQUIRE(array_nd_2d.dimensions() == 2, "array_nd_2d.dimensions() = %lld, expected %lld", array_nd_2d.dimensions(), 2ull);
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
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

TEST(function, size) {
    test_template<test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            gtl::array_nd<type> array_nd_0d;
            REQUIRE(array_nd_0d.size() == 0);
            test_template<value_collection<0, 1, 10>>(
                [](auto value_1)->void {
                    constexpr static const unsigned long long value1 = decltype(value_1)::value;
                    gtl::array_nd<type, value1> array_nd_1d;
                    REQUIRE(array_nd_1d.size() == value1);
                    REQUIRE(array_nd_1d.size(0) == value1);
                    test_template<value_collection<0, 1, 10>>(
                        [](auto value_2)->void {
                            constexpr static const unsigned long long value2 = decltype(value_2)::value;
                            gtl::array_nd<type, value1, value2> array_nd_2d;
                            REQUIRE(array_nd_2d.size() == value1);
                            REQUIRE(array_nd_2d.size(0) == value1);
                            REQUIRE(array_nd_2d.size(1) == value2);
                            test_template<value_collection<0, 1, 10>>(
                                [](auto value_3)->void {
                                    constexpr static const unsigned long long value3 = decltype(value_3)::value;
                                    gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                    REQUIRE(array_nd_3d.size() == value1);
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

TEST(constructor, list_initialiser) {
    test_template<test_types, value_collection<1, 10>>(
        [](auto test_type, auto value_1)->void {
            using type = typename decltype(test_type)::type;
            constexpr static const unsigned long long value1 = decltype(value_1)::value;
            for (const type& value : test_data<type>()) {
                gtl::array_nd<type, value1> array_nd_1d = {{ value }};
                UNOPTIMISED(array_nd_1d);
            }
            test_template<value_collection<1, 10>>(
                [](auto value_2)->void {
                    constexpr static const unsigned long long value2 = decltype(value_2)::value;
                    for (const type& value : test_data<type>()) {
                        gtl::array_nd<type, value1, value2> array_nd_2d = {{{ value }}};
                        UNOPTIMISED(array_nd_2d);
                    }
                    test_template<value_collection<1, 10>>(
                        [](auto value_3)->void {
                            constexpr static const unsigned long long value3 = decltype(value_3)::value;
                            for (const type& value : test_data<type>()) {
                                gtl::array_nd<type, value1, value2, value3> array_nd_3d = {{{{{ value }}}}};
                                UNOPTIMISED(array_nd_3d);
                            }
                        }
                    );
                }
            );
        }
    );
}

TEST(operator, subscript) {
    test_template<test_types, value_collection<1, 10>>(
        [](auto test_type, auto value_1)->void {
            using type = typename decltype(test_type)::type;
            constexpr static const unsigned long long value1 = decltype(value_1)::value;
            for (const type& value : test_data<type>()) {
                gtl::array_nd<type, value1> array_nd_1d;
                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                    array_nd_1d[index1] = value;
                    REQUIRE(array_nd_1d[index1] == value);
                }
            }
            test_template<value_collection<1, 10>>(
                [](auto value_2)->void {
                    constexpr static const unsigned long long value2 = decltype(value_2)::value;
                    for (const type& value : test_data<type>()) {
                        gtl::array_nd<type, value1, value2> array_nd_2d;
                        for (unsigned int index1 = 0; index1 < value1; ++index1) {
                            for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                array_nd_2d[index1][index2] = value;
                                REQUIRE(array_nd_2d[index1][index2] == value);
                            }
                        }
                    }
                    test_template<value_collection<1, 10>>(
                        [](auto value_3)->void {
                            constexpr static const unsigned long long value3 = decltype(value_3)::value;
                            for (const type& value : test_data<type>()) {
                                gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                                    for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                        for (unsigned int index3 = 0; index3 < value3; ++index3) {
                                            array_nd_3d[index1][index2][index3] = value;
                                            REQUIRE(array_nd_3d[index1][index2][index3] == value);
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

TEST(operator, parenthesis) {
    test_template<test_types, value_collection<1, 10>>(
        [](auto test_type, auto value_1)->void {
            using type = typename decltype(test_type)::type;
            constexpr static const unsigned long long value1 = decltype(value_1)::value;
            for (const type& value : test_data<type>()) {
                gtl::array_nd<type, value1> array_nd_1d;
                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                    array_nd_1d(index1) = value;
                    REQUIRE(array_nd_1d(index1) == value);
                }
            }
            test_template<value_collection<1, 10>>(
                [](auto value_2)->void {
                    constexpr static const unsigned long long value2 = decltype(value_2)::value;
                    for (const type& value : test_data<type>()) {
                        gtl::array_nd<type, value1, value2> array_nd_2d;
                        for (unsigned int index1 = 0; index1 < value1; ++index1) {
                            for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                array_nd_2d(index1, index2) = value;
                                REQUIRE(array_nd_2d(index1, index2) == value);
                            }
                        }
                    }
                    test_template<value_collection<1, 10>>(
                        [](auto value_3)->void {
                            constexpr static const unsigned long long value3 = decltype(value_3)::value;
                            for (const type& value : test_data<type>()) {
                                gtl::array_nd<type, value1, value2, value3> array_nd_3d;
                                for (unsigned int index1 = 0; index1 < value1; ++index1) {
                                    for (unsigned int index2 = 0; index2 < value2; ++index2) {
                                        for (unsigned int index3 = 0; index3 < value3; ++index3) {
                                            array_nd_3d(index1, index2, index3) = value;
                                            REQUIRE(array_nd_3d(index1, index2, index3) == value);
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

