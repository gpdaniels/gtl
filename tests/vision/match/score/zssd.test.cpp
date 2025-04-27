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

#include <vision/match/score/zssd>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

constexpr static const unsigned int data_width = 5;
constexpr static const unsigned int data_height = 5;

constexpr static const unsigned char data_empty[data_height][data_width] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

constexpr static const unsigned char data_chequer1[data_height][data_width] = {
    { 0, 255, 0, 255, 0 },
    { 255, 0, 255, 0, 255 },
    { 0, 255, 0, 255, 0 },
    { 255, 0, 255, 0, 255 },
    { 0, 255, 0, 255, 0 }
};

constexpr static const unsigned char data_chequer2[data_height][data_width] = {
    { 255, 0, 255, 0, 255 },
    { 0, 255, 0, 255, 0 },
    { 255, 0, 255, 0, 255 },
    { 0, 255, 0, 255, 0 },
    { 255, 0, 255, 0, 255 }
};

constexpr static const unsigned char data_full[data_height][data_width] = {
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 },
    { 255, 255, 255, 255, 255 }
};

constexpr static const unsigned char data_x_fill[data_height][data_width] = {
    { 0, 64, 128, 192, 255 },
    { 0, 64, 128, 192, 255 },
    { 0, 64, 128, 192, 255 },
    { 0, 64, 128, 192, 255 },
    { 0, 64, 128, 192, 255 }
};

constexpr static const unsigned char data_y_fill[data_height][data_width] = {
    { 0, 0, 0, 0, 0 },
    { 64, 64, 64, 64, 64 },
    { 128, 128, 128, 128, 128 },
    { 192, 192, 192, 192, 192 },
    { 255, 255, 255, 255, 255 }
};

constexpr static const unsigned char data_x_empty[data_height][data_width] = {
    { 255, 192, 128, 64, 0 },
    { 255, 192, 128, 64, 0 },
    { 255, 192, 128, 64, 0 },
    { 255, 192, 128, 64, 0 },
    { 255, 192, 128, 64, 0 }
};

constexpr static const unsigned char data_y_empty[data_height][data_width] = {
    { 255, 255, 255, 255, 255 },
    { 192, 192, 192, 192, 192 },
    { 128, 128, 128, 128, 128 },
    { 64, 64, 64, 64, 64 },
    { 0, 0, 0, 0, 0 }
};

constexpr static const unsigned char data_xy_fill[data_height][data_width] = {
    { 0, 32, 64, 96, 128 },
    { 32, 64, 96, 128, 160 },
    { 64, 96, 128, 160, 192 },
    { 96, 128, 160, 192, 224 },
    { 128, 160, 192, 224, 255 }
};

constexpr static const unsigned char data_xy_empty[data_height][data_width] = {
    { 255, 224, 192, 160, 128 },
    { 224, 192, 160, 128, 96 },
    { 192, 160, 128, 96, 64 },
    { 160, 128, 96, 64, 32 },
    { 128, 96, 64, 32, 0 }
};

template <unsigned long long value1, unsigned long long value2>
void test(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]) {
    REQUIRE(
        testbench::is_value_approx(gtl::zssd<value1, value2>(&lhs[0][0], data_width, &rhs[0][0], data_width), results[value2 - 1][value1 - 1], 1E-6f),
        "ncc<%llu, %llu> = %f (!= %f)",
        value1,
        value2,
        static_cast<double>(gtl::zssd<value1, value2>(&lhs[0][0], data_width, &rhs[0][0], data_width)),
        static_cast<double>(results[value2 - 1][value1 - 1]));
}

void test_set(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]);

void test_set(const unsigned char lhs[data_height][data_width], const unsigned char rhs[data_height][data_width], const float results[data_height][data_width]) {
    testbench::test_template<testbench::value_collection<1, 2, 3, 4, 5>>(
        [&lhs, &rhs, &results](auto value_1) -> void {
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
                [&lhs, &rhs, &results](auto value_2) -> void {
                    using type_value2 = decltype(value_2);
                    constexpr static const unsigned long long value2 = type_value2::value;
                    test<value1, value2>(lhs, rhs, results);
                });
#endif
        });
}

TEST(zssd, function, empty_and_full) {
    const float results_empty_empty[data_height][data_width] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };
    test_set(data_empty, data_empty, results_empty_empty);

    const float results_empty_full[data_height][data_width] = {
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 }
    };
    test_set(data_empty, data_full, results_empty_full);
    test_set(data_full, data_empty, results_empty_full);
}

TEST(zssd, function, same_lhs_and_rhs) {
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

TEST(zssd, function, checker) {
    const float results_empty_checker[data_height][data_width] = {
        { 0.0f, 32512.5f, 43350.0f, 65025.0f, 78030.000000f },
        { 32512.5f, 65025.0f, 97537.5f, 130050.0f, 162562.500000f },
        { 43350.0f, 97537.5f, 144500.0f, 195075.0f, 242760.000000f },
        { 65025.0f, 130050.0f, 195075.0f, 260100.0f, 325125.000000f },
        { 78030.0f, 162562.5f, 242760.0f, 325125.0f, 405755.968750f }
    };
    test_set(data_empty, data_chequer1, results_empty_checker);
    test_set(data_chequer1, data_empty, results_empty_checker);

    const float results_full_checker[data_height][data_width] = {
        { 0.0f, 32512.5f, 43350.0f, 65025.0f, 78030.000000f },
        { 32512.5f, 65025.0f, 97537.5f, 130050.0f, 162562.500000f },
        { 43350.0f, 97537.5f, 144500.0f, 195075.0f, 242760.000000f },
        { 65025.0f, 130050.0f, 195075.0f, 260100.0f, 325125.000000f },
        { 78030.0f, 162562.5f, 242760.0f, 325125.0f, 405755.968750f }
    };
    test_set(data_full, data_chequer1, results_full_checker);
    test_set(data_chequer1, data_full, results_full_checker);

    test_set(data_empty, data_chequer2, results_full_checker);
    test_set(data_chequer2, data_empty, results_full_checker);

    test_set(data_full, data_chequer2, results_empty_checker);
    test_set(data_chequer2, data_full, results_empty_checker);

    const float results_checker_checker[data_height][data_width] = {
        { 0.0f, 255.0f * 255.0f * 2.0f, 173400.0000f, 255.0f * 255.0f * 4.0f, 312120.000f },
        { 130050.0f, 255.0f * 255.0f * 4.0f, 390150.0000f, 255.0f * 255.0f * 8.0f, 650250.000f },
        { 173400.0f, 255.0f * 255.0f * 6.0f, 577999.9375f, 255.0f * 255.0f * 12.0f, 971040.000f },
        { 260100.0f, 255.0f * 255.0f * 8.0f, 780300.0000f, 255.0f * 255.0f * 16.0f, 1300500.000f },
        { 312120.0f, 255.0f * 255.0f * 10.0f, 971040.0000f, 255.0f * 255.0f * 20.0f, 1623023.875f }
    };
    test_set(data_chequer1, data_chequer2, results_checker_checker);
    test_set(data_chequer2, data_chequer1, results_checker_checker);
}

TEST(zssd, function, gradient_1d) {
    const float results_gradient_x[data_height][data_width] = {
        { 0.0f, 8064.5f, 32512.667969f, 81536.75f, 162818.0f },
        { 0.0f, 16129.0f, 65025.335938f, 163073.50f, 325636.0f },
        { 0.0f, 24193.5f, 97538.000000f, 244610.25f, 488454.0f },
        { 0.0f, 32258.0f, 130050.664062f, 326147.00f, 651272.0f },
        { 0.0f, 40322.5f, 162563.328125f, 407683.75f, 814090.0f }
    };
    test_set(data_x_empty, data_x_fill, results_gradient_x);

    const float results_gradient_y[data_height][data_width] = {
        { 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f },
        { 8064.500000f, 16129.000000f, 24193.500000f, 32258.000000f, 40322.500000f },
        { 32512.667969f, 65025.335938f, 97538.000000f, 130050.664062f, 162563.328125f },
        { 81536.750000f, 163073.500000f, 244610.250000f, 326147.000000f, 407683.750000f },
        { 162818.000000f, 325636.000000f, 488454.000000f, 651272.000000f, 814090.000000f }
    };
    test_set(data_y_empty, data_y_fill, results_gradient_y);
}

TEST(zssd, function, gradient_2d) {
    const float results_gradient[data_height][data_width] = {
        { 0.000000f, 1984.500000f, 8064.666504f, 20288.750000f, 40704.796875f },
        { 1984.500000f, 8064.750000f, 22336.832031f, 48896.875000f, 91840.898438f },
        { 8064.666504f, 22336.832031f, 48896.886719f, 93888.921875f, 163456.937500f },
        { 20288.750000f, 48896.875000f, 93888.921875f, 163456.890625f, 265792.968750f },
        { 40704.796875f, 91840.898438f, 163456.937500f, 265792.968750f, 408578.000000f }
    };
    test_set(data_xy_empty, data_xy_fill, results_gradient);
    test_set(data_xy_fill, data_xy_empty, results_gradient);
}
