/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <vision/match_distance/sad>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

constexpr static const unsigned int data_width = 5;
constexpr static const unsigned int data_height = 5;

constexpr static const unsigned char data_empty[data_height][data_width] = {
    {   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0 },
    {   0,   0,   0,   0,   0 }
};

constexpr static const unsigned char data_chequer1[data_height][data_width] = {
    {   0, 255,   0, 255,   0 },
    { 255,   0, 255,   0, 255 },
    {   0, 255,   0, 255,   0 },
    { 255,   0, 255,   0, 255 },
    {   0, 255,   0, 255,   0 }
};

constexpr static const unsigned char data_chequer2[data_height][data_width] = {
    { 255,   0, 255,   0, 255 },
    {   0, 255,   0, 255,   0 },
    { 255,   0, 255,   0, 255 },
    {   0, 255,   0, 255,   0 },
    { 255,   0, 255,   0, 255 }
};

constexpr static const unsigned char data_full[data_height][data_width] = {
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 }
};

constexpr static const unsigned char data_x_fill[data_height][data_width] = {
    {   0,  64, 128, 192, 255 },
    {   0,  64, 128, 192, 255 },
    {   0,  64, 128, 192, 255 },
    {   0,  64, 128, 192, 255 },
    {   0,  64, 128, 192, 255 }
};

constexpr static const unsigned char data_y_fill[data_height][data_width] = {
    {   0,   0,   0,   0,   0 },
    {  64,  64,  64,  64,  64 },
    { 128, 128, 128, 128, 128 },
    { 192, 192, 192, 192, 192 },
    { 255, 255, 255, 255, 255 }
};

constexpr static const unsigned char data_x_empty[data_height][data_width] = {
    { 255, 192, 128,  64,   0 },
    { 255, 192, 128,  64,   0 },
    { 255, 192, 128,  64,   0 },
    { 255, 192, 128,  64,   0 },
    { 255, 192, 128,  64,   0 }
};

constexpr static const unsigned char data_y_empty[data_height][data_width] = {
    { 255, 255, 255, 255, 255 },
    { 192, 192, 192, 192, 192 },
    { 128, 128, 128, 128, 128 },
    {  64,  64,  64,  64,  64 },
    {   0,   0,   0,   0,   0 }
};

constexpr static const unsigned char data_xy_fill[data_height][data_width] = {
    {   0,  32,  64,  96, 128 },
    {  32,  64,  96, 128, 160 },
    {  64,  96, 128, 160, 192 },
    {  96, 128, 160, 192, 224 },
    { 128, 160, 192, 224, 255 }
};

constexpr static const unsigned char data_xy_empty[data_height][data_width] = {
    { 255, 224, 192, 160, 128 },
    { 224, 192, 160, 128,  96 },
    { 192, 160, 128,  96,  64 },
    { 160, 128,  96,  64,  32 },
    { 128,  96,  64,  32,   0 }
};

void test_set(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]);
void test_set(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]) {
    testbench::test_template<testbench::value_collection<1, 2, 3, 4, 5>>(
        [&lhs, &rhs, &results](auto value_1)->void {
            using type_value1 = decltype(value_1);
            constexpr static const unsigned long long value1 = type_value1::value;
            testbench::test_template<testbench::value_collection<1, 2, 3, 4, 5>>(
                [&lhs, &rhs, &results](auto value_2)->void {
                    using type_value2 = decltype(value_2);
                    constexpr static const unsigned long long value2 = type_value2::value;
                    REQUIRE(
                        testbench::is_value_approx(gtl::sad<value1, value2>(&lhs[0][0], data_width, &rhs[0][0], data_width), results[value2 - 1][value1 - 1], 1E-6f),
                        "sad<%llu, %llu> = %f (!= %f)",
                        value1, value2,
                        static_cast<double>(gtl::sad<value1, value2>(&lhs[0][0], data_width, &rhs[0][0], data_width)),
                        static_cast<double>(results[value2 - 1][value1 - 1])
                    );
                }
            );
        }
    );
}

TEST(sad, function, empty_and_full) {
    const float results_empty_empty[data_height][data_width] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };
    test_set(data_empty, data_empty, results_empty_empty);

    const float results_empty_full[data_height][data_width] = {
        { 255*1, 255*2, 255*3, 255*4, 255*5 },
        { 255*2, 255*4, 255*6, 255*8, 255*10 },
        { 255*3, 255*6, 255*9, 255*12, 255*15 },
        { 255*4, 255*8, 255*12, 255*16, 255*20 },
        { 255*5, 255*10, 255*15, 255*20, 255*25 }
    };
    test_set(data_empty, data_full, results_empty_full);
    test_set(data_full, data_empty, results_empty_full);
}

TEST(sad, function, same_lhs_and_rhs) {
    const float results_same[data_height][data_width] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };
    test_set(data_full, data_full, results_same);
    test_set(data_x_empty, data_x_empty, results_same);
    test_set(data_y_empty, data_y_empty, results_same);
    test_set(data_xy_empty, data_xy_empty, results_same);

    test_set(data_chequer1, data_chequer1, results_same);
    test_set(data_xy_fill, data_xy_fill, results_same);
    test_set(data_x_fill, data_x_fill, results_same);
    test_set(data_y_fill, data_y_fill, results_same);
}

TEST(sad, function, checker) {
    const float results_empty_checker[data_height][data_width] = {
        { 255*0, 255*1, 255*1, 255*2, 255*2 },
        { 255*1, 255*2, 255*3, 255*4, 255*5 },
        { 255*1, 255*3, 255*4, 255*6, 255*7 },
        { 255*2, 255*4, 255*6, 255*8, 255*10 },
        { 255*2, 255*5, 255*7, 255*10, 255*12 }
    };
    test_set(data_empty, data_chequer1, results_empty_checker);
    test_set(data_chequer1, data_empty, results_empty_checker);

    const float results_full_checker[data_height][data_width] = {
        { 255*1, 255*1, 255*2, 255*2, 255*3 },
        { 255*1, 255*2, 255*3, 255*4, 255*5 },
        { 255*2, 255*3, 255*5, 255*6, 255*8 },
        { 255*2, 255*4, 255*6, 255*8, 255*10 },
        { 255*3, 255*5, 255*8, 255*10, 255*13 }
    };
    test_set(data_full, data_chequer1, results_full_checker);
    test_set(data_chequer1, data_full, results_full_checker);

    test_set(data_empty, data_chequer2, results_full_checker);
    test_set(data_chequer2, data_empty, results_full_checker);

    test_set(data_full, data_chequer2, results_empty_checker);
    test_set(data_chequer2, data_full, results_empty_checker);

    const float results_checker_checker[data_height][data_width] = {
        { 255*1, 255*2, 255*3, 255*4, 255*5 },
        { 255*2, 255*4, 255*6, 255*8, 255*10 },
        { 255*3, 255*6, 255*9, 255*12, 255*15 },
        { 255*4, 255*8, 255*12, 255*16, 255*20 },
        { 255*5, 255*10, 255*15, 255*20, 255*25 }
    };
    test_set(data_chequer1, data_chequer2, results_checker_checker);
    test_set(data_chequer2, data_chequer1, results_checker_checker);
}

TEST(sad, function, gradient_1d) {
    const float results_gradient_x[data_height][data_width] = {
        { 255*1, 383*1, 383*1, 511*1, 766*1 },
        { 255*2, 383*2, 383*2, 511*2, 766*2 },
        { 255*3, 383*3, 383*3, 511*3, 766*3 },
        { 255*4, 383*4, 383*4, 511*4, 766*4 },
        { 255*5, 383*5, 383*5, 511*5, 766*5 }
    };
    test_set(data_x_empty, data_x_fill, results_gradient_x);

    const float results_gradient_y[data_height][data_width] = {
        { 255*1, 255*2, 255*3, 255*4, 255*5 },
        { 383*1, 383*2, 383*3, 383*4, 383*5 },
        { 383*1, 383*2, 383*3, 383*4, 383*5 },
        { 511*1, 511*2, 511*3, 511*4, 511*5 },
        { 766*1, 766*2, 766*3, 766*4, 766*5 }
    };
    test_set(data_y_empty, data_y_fill, results_gradient_y);
}

TEST(sad, function, gradient_2d) {
    const float results_gradient[data_height][data_width] = {
        { 255, 447, 575, 639, 639 },
        { 447, 767, 959, 1023, 1087 },
        { 575, 959, 1151, 1279, 1471 },
        { 639, 1023, 1279, 1535, 1919 },
        { 639, 1087, 1471, 1919, 2558 }
    };
    test_set(data_xy_empty, data_xy_fill, results_gradient);
    test_set(data_xy_fill, data_xy_empty, results_gradient);
}
