/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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

#include <vision/match_distance/zncc>

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

template <unsigned long long value1, unsigned long long value2>
void test(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]) {
    REQUIRE(
        testbench::is_value_approx(gtl::zncc<value1, value2>(&lhs[0][0], data_width, &rhs[0][0], data_width), results[value2 - 1][value1 - 1], 1E-6f),
        "ncc<%llu, %llu> = %f (!= %f)",
        value1, value2,
        static_cast<double>(gtl::zncc<value1, value2>(&lhs[0][0], data_width, &rhs[0][0], data_width)),
        static_cast<double>(results[value2 - 1][value1 - 1])
    );
}

void test_set(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]);
void test_set(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]) {
    testbench::test_template<testbench::value_collection<1, 2, 3, 4, 5>>(
        [&lhs, &rhs, &results](auto value_1)->void {
            using type_value1 = decltype(value_1);
            constexpr static const unsigned long long value1 = type_value1::value;
            static_cast<void>(value1);
            #if defined(_MSC_VER)
                test<value1, 1>(lhs, rhs, results);
                test<value1, 2>(lhs, rhs, results);
                test<value1, 3>(lhs, rhs, results);
                test<value1, 4>(lhs, rhs, results);
                test<value1, 5>(lhs, rhs, results);
            #else
                testbench::test_template<testbench::value_collection<1, 2, 3, 4, 5>>(
                    [&lhs, &rhs, &results](auto value_2)->void {
                        using type_value2 = decltype(value_2);
                        constexpr static const unsigned long long value2 = type_value2::value;
                        test<value1, value2>(lhs, rhs, results);
                    }
                );
            #endif
        }
    );
}

TEST(zncc, function, empty_and_full) {
    const float results[data_height][data_width] = {
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f }
    };
    test_set(data_empty, data_empty, results);
    test_set(data_empty, data_full, results);
    test_set(data_full, data_empty, results);
}

TEST(zncc, function, same_lhs_and_rhs) {
    const float results_same[data_height][data_width] = {
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f }
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

TEST(zncc, function, checker) {
    const float results_zeros[data_height][data_width] = {
        { -1.000000f,  0.000000f,  0.000000f,  0.000000f,  0.000000f },
        {  0.000000f,  0.000000f,  0.000000f,  0.000000f,  0.000000f },
        {  0.000000f,  0.000000f,  0.000000f,  0.000000f,  0.000000f },
        {  0.000000f,  0.000000f,  0.000000f,  0.000000f,  0.000000f },
        {  0.000000f,  0.000000f,  0.000000f,  0.000000f,  0.000000f }
    };
    test_set(data_empty, data_chequer1, results_zeros);
    test_set(data_chequer1, data_empty, results_zeros);

    test_set(data_full, data_chequer1, results_zeros);
    test_set(data_chequer1, data_full, results_zeros);

    test_set(data_empty, data_chequer2, results_zeros);
    test_set(data_chequer2, data_empty, results_zeros);

    test_set(data_full, data_chequer2, results_zeros);
    test_set(data_chequer2, data_full, results_zeros);

    const float results_anti_correlate[data_height][data_width] = {
        { -1.000000f,  1.000000f,  1.000000f,  1.000000f,  1.000000f },
        {  1.000000f,  1.000000f,  1.000000f,  1.000000f,  1.000000f },
        {  1.000000f,  1.000000f,  1.000000f,  1.000000f,  1.000000f },
        {  1.000000f,  1.000000f,  1.000000f,  1.000000f,  1.000000f },
        {  1.000000f,  1.000000f,  1.000000f,  1.000000f,  1.000000f }
    };

    test_set(data_chequer1, data_chequer2, results_anti_correlate);
    test_set(data_chequer2, data_chequer1, results_anti_correlate);
}

TEST(zncc, function, gradient_1d) {
    const float results_gradient_x[data_height][data_width] = {
        { -1.000000f,  1.000000f,  0.999990f,  0.999993f,  0.999985f },
        { -1.000000f,  1.000000f,  0.999990f,  0.999993f,  0.999985f },
        { -1.000000f,  1.000000f,  0.999990f,  0.999993f,  0.999985f },
        { -1.000000f,  1.000000f,  0.999990f,  0.999993f,  0.999985f },
        { -1.000000f,  1.000000f,  0.999990f,  0.999993f,  0.999985f }
    };
    test_set(data_x_empty, data_x_fill, results_gradient_x);

    const float results_gradient_y[data_height][data_width] = {
        { -1.000000f, -1.000000f, -1.000000f, -1.000000f, -1.000000f },
        {  1.000000f,  1.000000f,  1.000000f,  1.000000f,  1.000000f },
        {  0.999990f,  0.999990f,  0.999990f,  0.999990f,  0.999990f },
        {  0.999993f,  0.999993f,  0.999993f,  0.999993f,  0.999993f },
        {  0.999985f,  0.999985f,  0.999985f,  0.999985f,  0.999985f }
    };
    test_set(data_y_empty, data_y_fill, results_gradient_y);
}

TEST(zncc, function, gradient_2d) {
    const float results_gradient[data_height][data_width] = {
        { -1.000000f,  1.000000f,  0.999958f,  0.999970f,  0.999980f },
        {  1.000000f,  0.999937f,  0.999962f,  0.999978f,  0.999986f },
        {  0.999958f,  0.999962f,  0.999977f,  0.999986f,  0.999991f },
        {  0.999970f,  0.999978f,  0.999986f,  0.999991f,  0.999994f },
        {  0.999980f,  0.999986f,  0.999991f,  0.999994f,  0.999991f }
    };
    test_set(data_xy_empty, data_xy_fill, results_gradient);
    test_set(data_xy_fill, data_xy_empty, results_gradient);
}
