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
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <math/symbolic>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(symbolic, types, inputs) {
    gtl::symbolic::parameter<0> parameter;
    gtl::symbolic::variable<1> variable;
    gtl::symbolic::constant<gtl::symbolic::integer<5>> constant;

    testbench::do_not_optimise_away(parameter);
    testbench::do_not_optimise_away(variable);
    testbench::do_not_optimise_away(constant);
}

TEST(symbolic, types, basic_operations) {
    gtl::symbolic::parameter<0> parameter;

    auto equation_plus_op       = gtl::symbolic::plus_op<decltype(parameter)>{};
    auto equation_minus_op      = gtl::symbolic::minus_op<decltype(parameter)>{};
    auto equation_add_op        = gtl::symbolic::add_op<decltype(parameter), decltype(parameter)>{};
    auto equation_subtract_op   = gtl::symbolic::subtract_op<decltype(parameter), decltype(parameter)>{};
    auto equation_multiply_op   = gtl::symbolic::multiply_op<decltype(parameter), decltype(parameter)>{};
    auto equation_divide_op     = gtl::symbolic::divide_op<decltype(parameter), decltype(parameter)>{};

    testbench::do_not_optimise_away(equation_plus_op);
    testbench::do_not_optimise_away(equation_minus_op);
    testbench::do_not_optimise_away(equation_add_op);
    testbench::do_not_optimise_away(equation_subtract_op);
    testbench::do_not_optimise_away(equation_multiply_op);
    testbench::do_not_optimise_away(equation_divide_op);
}

TEST(symbolic, types, basic_operation_overloads) {
    gtl::symbolic::parameter<0> parameter;

    auto equation_plus_symbol       = +parameter;
    auto equation_minus_symbol      = -parameter;
    auto equation_add_symbol        = parameter + parameter;
    auto equation_subtract_symbol   = parameter - parameter;
    auto equation_multiply_symbol   = parameter * parameter;
    auto equation_divide_symbol     = parameter / parameter;

    testbench::do_not_optimise_away(equation_plus_symbol);
    testbench::do_not_optimise_away(equation_minus_symbol);
    testbench::do_not_optimise_away(equation_add_symbol);
    testbench::do_not_optimise_away(equation_subtract_symbol);
    testbench::do_not_optimise_away(equation_multiply_symbol);
    testbench::do_not_optimise_away(equation_divide_symbol);
}

TEST(symbolic, types, math_operations) {
    gtl::symbolic::parameter<0> parameter;

    auto equation_sign  = sign(parameter);
    auto equation_abs   = abs(parameter);
    auto equation_ceil  = ceil(parameter);
    auto equation_floor = floor(parameter);
    auto equation_exp   = exp(parameter);
    auto equation_log   = log(parameter);
    auto equation_exp2  = exp2(parameter);
    auto equation_log2  = log2(parameter);
    auto equation_sin   = sin(parameter);
    auto equation_asin  = asin(parameter);
    auto equation_cos   = cos(parameter);
    auto equation_acos  = acos(parameter);
    auto equation_tan   = tan(parameter);
    auto equation_atan  = atan(parameter);
    auto equation_sinh  = sinh(parameter);
    auto equation_asinh = asinh(parameter);
    auto equation_cosh  = cosh(parameter);
    auto equation_acosh = acosh(parameter);
    auto equation_tanh  = tanh(parameter);
    auto equation_atanh = atanh(parameter);
    auto equation_sqrt  = sqrt(parameter);
    auto equation_cbrt  = cbrt(parameter);
    auto equation_min   = min(parameter, parameter);
    auto equation_max   = max(parameter, parameter);
    auto equation_hypot = hypot(parameter, parameter);
    auto equation_atan2 = atan2(parameter, parameter);
    auto equation_pow   = pow(parameter, parameter);

    testbench::do_not_optimise_away(equation_sign);
    testbench::do_not_optimise_away(equation_abs);
    testbench::do_not_optimise_away(equation_ceil);
    testbench::do_not_optimise_away(equation_floor);
    testbench::do_not_optimise_away(equation_exp);
    testbench::do_not_optimise_away(equation_log);
    testbench::do_not_optimise_away(equation_exp2);
    testbench::do_not_optimise_away(equation_log2);
    testbench::do_not_optimise_away(equation_sin);
    testbench::do_not_optimise_away(equation_asin);
    testbench::do_not_optimise_away(equation_cos);
    testbench::do_not_optimise_away(equation_acos);
    testbench::do_not_optimise_away(equation_tan);
    testbench::do_not_optimise_away(equation_atan);
    testbench::do_not_optimise_away(equation_sinh);
    testbench::do_not_optimise_away(equation_asinh);
    testbench::do_not_optimise_away(equation_cosh);
    testbench::do_not_optimise_away(equation_acosh);
    testbench::do_not_optimise_away(equation_tanh);
    testbench::do_not_optimise_away(equation_atanh);
    testbench::do_not_optimise_away(equation_sqrt);
    testbench::do_not_optimise_away(equation_cbrt);
    testbench::do_not_optimise_away(equation_min);
    testbench::do_not_optimise_away(equation_max);
    testbench::do_not_optimise_away(equation_hypot);
    testbench::do_not_optimise_away(equation_atan2);
    testbench::do_not_optimise_away(equation_pow);
}

///////////////////////////////////////////////////////////////////////////////

TEST(symbolic, solve, inputs) {
    gtl::symbolic::parameter<0> parameter;
    gtl::symbolic::variable<1> variable;
    gtl::symbolic::constant<gtl::symbolic::integer<54321>> constant;

    REQUIRE(parameter.solve<int>(12345) == 12345);
    REQUIRE(parameter.solve<int>(12345, 67890) == 12345);
    REQUIRE(variable.solve<int>(12345, 67890) == 67890);
    REQUIRE(variable.solve<int>(12345, 67890, 9876) == 67890);
    REQUIRE(constant.solve<int>(12345) == 54321);
    REQUIRE(constant.solve<int>(12345, 67890) == 54321);
    REQUIRE(constant.solve<int>(12345, 67890, 9876) == 54321);
}

TEST(symbolic, solve, basic_operations) {
    gtl::symbolic::parameter<0> parameter;

    auto equation_plus_op       = gtl::symbolic::plus_op<decltype(parameter)>{};
    auto equation_minus_op      = gtl::symbolic::minus_op<decltype(parameter)>{};
    auto equation_add_op        = gtl::symbolic::add_op<decltype(parameter), decltype(parameter)>{};
    auto equation_subtract_op   = gtl::symbolic::subtract_op<decltype(parameter), decltype(parameter)>{};
    auto equation_multiply_op   = gtl::symbolic::multiply_op<decltype(parameter), decltype(parameter)>{};
    auto equation_divide_op     = gtl::symbolic::divide_op<decltype(parameter), decltype(parameter)>{};

    REQUIRE(equation_plus_op.solve<int>(123) == (+123));
    REQUIRE(equation_minus_op.solve<int>(123) == (-123));
    REQUIRE(equation_add_op.solve<int>(123) == (123 + 123));
    REQUIRE(equation_subtract_op.solve<int>(123) == (123 - 123));
    REQUIRE(equation_multiply_op.solve<int>(123) == (123 * 123));
    REQUIRE(equation_divide_op.solve<int>(123) == (123 / 123));
}

TEST(symbolic, solve, basic_operation_overloads) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    REQUIRE((+parameter0).solve<int>(512) == (+512));
    REQUIRE((-parameter0).solve<int>(512) == (-512));
    REQUIRE((parameter0 + parameter1).solve<int>(512, 128) == (512 + 128));
    REQUIRE((parameter0 - parameter1).solve<int>(512, 128) == (512 - 128));
    REQUIRE((parameter0 * parameter1).solve<int>(512, 128) == (512 * 128));
    REQUIRE((parameter0 / parameter1).solve<int>(512, 128) == (512 / 128));
}

TEST(symbolic, solve, math_operation_sign) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(sign(parameter0).solve<int>(1) == 1);
    REQUIRE(sign(parameter0).solve<int>(0) == 0);
    REQUIRE(sign(parameter0).solve<int>(-1) == -1);

    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(std::numeric_limits<float>::max()), 1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(2134567), 1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(1), 1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(std::numeric_limits<float>::min()), 1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(0), 0.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(-std::numeric_limits<float>::min()), -1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(-1), -1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(-56), -1.0f));
    REQUIRE(testbench::is_value_equal(sign(parameter0).solve<float>(-std::numeric_limits<float>::max()), -1.0f));
}

TEST(symbolic, solve, math_operation_abs) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(abs(parameter0).solve<int>(1) == 1);
    REQUIRE(abs(parameter0).solve<int>(0) == 0);
    REQUIRE(abs(parameter0).solve<int>(-1) == 1);

    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(std::numeric_limits<float>::max()), std::numeric_limits<float>::max()));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(2134567), 2134567.0f));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(1), 1.0f));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(std::numeric_limits<float>::min()), std::numeric_limits<float>::min()));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(0), 0.0f));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(-std::numeric_limits<float>::min()), std::numeric_limits<float>::min()));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(-1), 1.0f));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(-56), 56.0f));
    REQUIRE(testbench::is_value_equal(abs(parameter0).solve<float>(-std::numeric_limits<float>::max()), std::numeric_limits<float>::max()));
}

TEST(symbolic, solve, math_operation_ceil) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(ceil(parameter0).solve<int>(1) == 1);
    REQUIRE(ceil(parameter0).solve<int>(0) == 0);
    REQUIRE(ceil(parameter0).solve<int>(-1) == -1);

    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(std::numeric_limits<float>::max()), std::ceil(std::numeric_limits<float>::max())));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(2134567), 2134567.0f));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(1), 1.0f));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(std::numeric_limits<float>::min()), std::ceil(std::numeric_limits<float>::min())));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(0), 0.0f));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(-std::numeric_limits<float>::min()), std::ceil(-std::numeric_limits<float>::min())));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(-1), -1.0f));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(-56), -56.0f));
    REQUIRE(testbench::is_value_equal(ceil(parameter0).solve<float>(-std::numeric_limits<float>::max()), std::ceil(-std::numeric_limits<float>::max())));
}

TEST(symbolic, solve, math_operation_floor) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(floor(parameter0).solve<int>(1) == 1);
    REQUIRE(floor(parameter0).solve<int>(0) == 0);
    REQUIRE(floor(parameter0).solve<int>(-1) == -1);

    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(std::numeric_limits<float>::max()), std::floor(std::numeric_limits<float>::max())));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(2134567), 2134567.0f));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(1), 1.0f));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(std::numeric_limits<float>::min()), std::floor(std::numeric_limits<float>::min())));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(0), 0.0f));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(-std::numeric_limits<float>::min()), std::floor(-std::numeric_limits<float>::min())));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(-1), -1.0f));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(-56), -56.0f));
    REQUIRE(testbench::is_value_equal(floor(parameter0).solve<float>(-std::numeric_limits<float>::max()), std::floor(-std::numeric_limits<float>::max())));
}

TEST(symbolic, solve, math_operation_exp) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(exp(parameter0).solve<float>(37.0f), std::exp(37.0f)));
    REQUIRE(testbench::is_value_equal(exp(parameter0).solve<float>(1.0f), std::exp(1.0f)));
    REQUIRE(testbench::is_value_equal(exp(parameter0).solve<float>(0.0f), std::exp(0.0f)));
    REQUIRE(testbench::is_value_equal(exp(parameter0).solve<float>(-1.0f), std::exp(-1.0f)));
    REQUIRE(testbench::is_value_equal(exp(parameter0).solve<float>(-56.0f), std::exp(-56.0f)));
}

TEST(symbolic, solve, math_operation_log) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(log(parameter0).solve<float>(2134567.0f), std::log(2134567.0f)));
    REQUIRE(testbench::is_value_equal(log(parameter0).solve<float>(1.0f), std::log(1.0f)));
    REQUIRE(testbench::is_value_equal(log(parameter0).solve<float>(0.01f), std::log(0.01f)));
}

TEST(symbolic, solve, math_operation_exp2) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(exp2(parameter0).solve<float>(37.0f), std::exp2(37.0f)));
    REQUIRE(testbench::is_value_equal(exp2(parameter0).solve<float>(1.0f), std::exp2(1.0f)));
    REQUIRE(testbench::is_value_equal(exp2(parameter0).solve<float>(0.0f), std::exp2(0.0f)));
    REQUIRE(testbench::is_value_equal(exp2(parameter0).solve<float>(-1.0f), std::exp2(-1.0f)));
    REQUIRE(testbench::is_value_equal(exp2(parameter0).solve<float>(-56.0f), std::exp2(-56.0f)));
}

TEST(symbolic, solve, math_operation_log2) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(log2(parameter0).solve<float>(2134567.0f), std::log2(2134567.0f)));
    REQUIRE(testbench::is_value_equal(log2(parameter0).solve<float>(1.0f), std::log2(1.0f)));
    REQUIRE(testbench::is_value_equal(log2(parameter0).solve<float>(0.01f), std::log2(0.01f)));
}

TEST(symbolic, solve, math_operation_sin) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(sin(parameter0).solve<float>(2134567.0f), std::sin(2134567.0f)));
    REQUIRE(testbench::is_value_equal(sin(parameter0).solve<float>(1.0f), std::sin(1.0f)));
    REQUIRE(testbench::is_value_equal(sin(parameter0).solve<float>(0.0f), std::sin(0.0f)));
    REQUIRE(testbench::is_value_equal(sin(parameter0).solve<float>(-1.0f), std::sin(-1.0f)));
    REQUIRE(testbench::is_value_equal(sin(parameter0).solve<float>(-56.0f), std::sin(-56.0f)));
}

TEST(symbolic, solve, math_operation_asin) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(asin(parameter0).solve<float>(1.0f), std::asin(1.0f)));
    REQUIRE(testbench::is_value_equal(asin(parameter0).solve<float>(0.5f), std::asin(0.5f)));
    REQUIRE(testbench::is_value_equal(asin(parameter0).solve<float>(0.0f), std::asin(0.0f)));
    REQUIRE(testbench::is_value_equal(asin(parameter0).solve<float>(-0.5f), std::asin(-0.5f)));
    REQUIRE(testbench::is_value_equal(asin(parameter0).solve<float>(-1.0f), std::asin(-1.0f)));
}

TEST(symbolic, solve, math_operation_cos) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(cos(parameter0).solve<float>(2134567.0f), std::cos(2134567.0f)));
    REQUIRE(testbench::is_value_equal(cos(parameter0).solve<float>(1.0f), std::cos(1.0f)));
    REQUIRE(testbench::is_value_equal(cos(parameter0).solve<float>(0.0f), std::cos(0.0f)));
    REQUIRE(testbench::is_value_equal(cos(parameter0).solve<float>(-1.0f), std::cos(-1.0f)));
    REQUIRE(testbench::is_value_equal(cos(parameter0).solve<float>(-56.0f), std::cos(-56.0f)));
}

TEST(symbolic, solve, math_operation_acos) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(acos(parameter0).solve<float>(1.0f), std::acos(1.0f)));
    REQUIRE(testbench::is_value_equal(acos(parameter0).solve<float>(0.5f), std::acos(0.5f)));
    REQUIRE(testbench::is_value_equal(acos(parameter0).solve<float>(0.0f), std::acos(0.0f)));
    REQUIRE(testbench::is_value_equal(acos(parameter0).solve<float>(-0.5f), std::acos(-0.5f)));
    REQUIRE(testbench::is_value_equal(acos(parameter0).solve<float>(-1.0f), std::acos(-1.0f)));
}

TEST(symbolic, solve, math_operation_tan) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(tan(parameter0).solve<float>(2134567.0f), std::tan(2134567.0f)));
    REQUIRE(testbench::is_value_equal(tan(parameter0).solve<float>(1.0f), std::tan(1.0f)));
    REQUIRE(testbench::is_value_equal(tan(parameter0).solve<float>(0.0f), std::tan(0.0f)));
    REQUIRE(testbench::is_value_equal(tan(parameter0).solve<float>(-1.0f), std::tan(-1.0f)));
    REQUIRE(testbench::is_value_equal(tan(parameter0).solve<float>(-56.0f), std::tan(-56.0f)));
}

TEST(symbolic, solve, math_operation_atan) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(atan(parameter0).solve<float>(1.0f), std::atan(1.0f)));
    REQUIRE(testbench::is_value_equal(atan(parameter0).solve<float>(0.5f), std::atan(0.5f)));
    REQUIRE(testbench::is_value_equal(atan(parameter0).solve<float>(0.0f), std::atan(0.0f)));
    REQUIRE(testbench::is_value_equal(atan(parameter0).solve<float>(-0.5f), std::atan(-0.5f)));
    REQUIRE(testbench::is_value_equal(atan(parameter0).solve<float>(-1.0f), std::atan(-1.0f)));
}

TEST(symbolic, solve, math_operation_sinh) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(sinh(parameter0).solve<float>(45.0f), std::sinh(45.0f)));
    REQUIRE(testbench::is_value_equal(sinh(parameter0).solve<float>(1.0f), std::sinh(1.0f)));
    REQUIRE(testbench::is_value_equal(sinh(parameter0).solve<float>(0.0f), std::sinh(0.0f)));
    REQUIRE(testbench::is_value_equal(sinh(parameter0).solve<float>(-1.0f), std::sinh(-1.0f)));
    REQUIRE(testbench::is_value_equal(sinh(parameter0).solve<float>(-56.0f), std::sinh(-56.0f)));
}

TEST(symbolic, solve, math_operation_asinh) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(asinh(parameter0).solve<float>(1.0f), std::asinh(1.0f)));
    REQUIRE(testbench::is_value_equal(asinh(parameter0).solve<float>(0.5f), std::asinh(0.5f)));
    REQUIRE(testbench::is_value_equal(asinh(parameter0).solve<float>(0.0f), std::asinh(0.0f)));
    REQUIRE(testbench::is_value_equal(asinh(parameter0).solve<float>(-0.5f), std::asinh(-0.5f)));
    REQUIRE(testbench::is_value_equal(asinh(parameter0).solve<float>(-1.0f), std::asinh(-1.0f)));
}

TEST(symbolic, solve, math_operation_cosh) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(cosh(parameter0).solve<float>(45.0f), std::cosh(45.0f)));
    REQUIRE(testbench::is_value_equal(cosh(parameter0).solve<float>(1.0f), std::cosh(1.0f)));
    REQUIRE(testbench::is_value_equal(cosh(parameter0).solve<float>(0.0f), std::cosh(0.0f)));
    REQUIRE(testbench::is_value_equal(cosh(parameter0).solve<float>(-1.0f), std::cosh(-1.0f)));
    REQUIRE(testbench::is_value_equal(cosh(parameter0).solve<float>(-56.0f), std::cosh(-56.0f)));
}

TEST(symbolic, solve, math_operation_acosh) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(acosh(parameter0).solve<float>(8456.5f), std::acosh(8456.5f)));
    REQUIRE(testbench::is_value_equal(acosh(parameter0).solve<float>(34.0f), std::acosh(34.0f)));
    REQUIRE(testbench::is_value_equal(acosh(parameter0).solve<float>(5.0f), std::acosh(5.0f)));
    REQUIRE(testbench::is_value_equal(acosh(parameter0).solve<float>(1.0f), std::acosh(1.0f)));
}

TEST(symbolic, solve, math_operation_tanh) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(tanh(parameter0).solve<float>(2134567.0f), std::tanh(2134567.0f)));
    REQUIRE(testbench::is_value_equal(tanh(parameter0).solve<float>(1.0f), std::tanh(1.0f)));
    REQUIRE(testbench::is_value_equal(tanh(parameter0).solve<float>(0.0f), std::tanh(0.0f)));
    REQUIRE(testbench::is_value_equal(tanh(parameter0).solve<float>(-1.0f), std::tanh(-1.0f)));
    REQUIRE(testbench::is_value_equal(tanh(parameter0).solve<float>(-56.0f), std::tanh(-56.0f)));
}

TEST(symbolic, solve, math_operation_atanh) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(atanh(parameter0).solve<float>(0.5f), std::atanh(0.5f)));
    REQUIRE(testbench::is_value_equal(atanh(parameter0).solve<float>(0.0001f), std::atanh(0.0001f)));
    REQUIRE(testbench::is_value_equal(atanh(parameter0).solve<float>(0.0f), std::atanh(0.0f)));
    REQUIRE(testbench::is_value_equal(atanh(parameter0).solve<float>(-0.0001f), std::atanh(-0.0001f)));
    REQUIRE(testbench::is_value_equal(atanh(parameter0).solve<float>(-0.5f), std::atanh(-0.5f)));
}

TEST(symbolic, solve, math_operation_sqrt) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(sqrt(parameter0).solve<float>(2134567.0f), std::sqrt(2134567.0f)));
    REQUIRE(testbench::is_value_equal(sqrt(parameter0).solve<float>(56.0f), std::sqrt(56.0f)));
    REQUIRE(testbench::is_value_equal(sqrt(parameter0).solve<float>(1.0f), std::sqrt(1.0f)));
    REQUIRE(testbench::is_value_equal(sqrt(parameter0).solve<float>(0.0001f), std::sqrt(0.0001f)));
    REQUIRE(testbench::is_value_equal(sqrt(parameter0).solve<float>(0.0f), std::sqrt(0.0f)));
}

TEST(symbolic, solve, math_operation_cbrt) {
    gtl::symbolic::parameter<0> parameter0;

    REQUIRE(testbench::is_value_equal(cbrt(parameter0).solve<float>(2134567.0f), std::cbrt(2134567.0f)));
    REQUIRE(testbench::is_value_equal(cbrt(parameter0).solve<float>(56.0f), std::cbrt(56.0f)));
    REQUIRE(testbench::is_value_equal(cbrt(parameter0).solve<float>(1.0f), std::cbrt(1.0f)));
    REQUIRE(testbench::is_value_equal(cbrt(parameter0).solve<float>(0.0001f), std::cbrt(0.0001f)));
    REQUIRE(testbench::is_value_equal(cbrt(parameter0).solve<float>(0.0f), std::cbrt(0.0f)));
}

TEST(symbolic, solve, math_operation_min) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    REQUIRE(testbench::is_value_equal(min(parameter0, parameter1).solve<float>(2134567.0f, 2134567.0f), 2134567.0f));
    REQUIRE(testbench::is_value_equal(min(parameter0, parameter1).solve<float>(std::numeric_limits<float>::max(), 2134567.0f), 2134567.0f));
    REQUIRE(testbench::is_value_equal(min(parameter0, parameter1).solve<float>(1.01f, 0.01f), 0.01f));
    REQUIRE(testbench::is_value_equal(min(parameter0, parameter1).solve<float>(0.0f, 0.0f), 0.0f));
    REQUIRE(testbench::is_value_equal(min(parameter0, parameter1).solve<float>(-1.0f, -345.0f), -345.0f));
    REQUIRE(testbench::is_value_equal(min(parameter0, parameter1).solve<float>(134.23f, -56.0f), -56.0f));
}

TEST(symbolic, solve, math_operation_max) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    REQUIRE(testbench::is_value_equal(max(parameter0, parameter1).solve<float>(2134567.0f, 2134567.0f), 2134567.0f));
    REQUIRE(testbench::is_value_equal(max(parameter0, parameter1).solve<float>(std::numeric_limits<float>::min(), 2134567.0f), 2134567.0f));
    REQUIRE(testbench::is_value_equal(max(parameter0, parameter1).solve<float>(1.01f, 0.01f), 1.01f));
    REQUIRE(testbench::is_value_equal(max(parameter0, parameter1).solve<float>(0.0f, 0.0f), 0.0f));
    REQUIRE(testbench::is_value_equal(max(parameter0, parameter1).solve<float>(-1.0f, -345.0f), -1.0f));
    REQUIRE(testbench::is_value_equal(max(parameter0, parameter1).solve<float>(134.23f, -56.0f), 134.23f));
}

TEST(symbolic, solve, math_operation_hypot) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    REQUIRE(testbench::is_value_equal(hypot(parameter0, parameter1).solve<float>(2134567.0f, 2134567.0f), std::hypot(2134567.0f, 2134567.0f)));
    REQUIRE(testbench::is_value_equal(hypot(parameter0, parameter1).solve<float>(567.5f, 2134567.0f), std::hypot(567.5f, 2134567.0f)));
    REQUIRE(testbench::is_value_equal(hypot(parameter0, parameter1).solve<float>(1.01f, 0.01f), std::hypot(1.01f, 0.01f)));
    REQUIRE(testbench::is_value_equal(hypot(parameter0, parameter1).solve<float>(0.0f, 0.0f), 0.0f));
}

TEST(symbolic, solve, math_operation_atan2) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    REQUIRE(testbench::is_value_equal(atan2(parameter0, parameter1).solve<float>(2134567.0f, 2134567.0f), std::atan2(2134567.0f, 2134567.0f)));
    REQUIRE(testbench::is_value_equal(atan2(parameter0, parameter1).solve<float>(567.5f, 2134567.0f), std::atan2(567.5f, 2134567.0f)));
    REQUIRE(testbench::is_value_equal(atan2(parameter0, parameter1).solve<float>(1.01f, 0.01f), std::atan2(1.01f, 0.01f)));
    REQUIRE(testbench::is_value_equal(atan2(parameter0, parameter1).solve<float>(0.0f, 0.0f), 0.0f));
}

TEST(symbolic, solve, math_operation_pow) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    REQUIRE(testbench::is_value_equal(pow(parameter0, parameter1).solve<float>(2134567.0f, 2.0f), std::pow(2134567.0f, 2.0f)));
    REQUIRE(testbench::is_value_equal(pow(parameter0, parameter1).solve<float>(567.5f, 0.5f), std::pow(567.5f, 0.5f)));
    REQUIRE(testbench::is_value_equal(pow(parameter0, parameter1).solve<float>(1.01f, 0.01f), std::pow(1.01f, 0.01f)));
    REQUIRE(testbench::is_value_equal(pow(parameter0, parameter1).solve<float>(14.0f, 0.0f), 1.0f));
    REQUIRE(testbench::is_value_equal(pow(parameter0, parameter1).solve<float>(0.0f, 235.0f), 0.0f));
}

///////////////////////////////////////////////////////////////////////////////

TEST(symbolic, simplify, remove_plus_on_zero) {

    auto equation = +gtl::symbolic::zero{};
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, remove_minus_on_zero) {

    auto equation = -gtl::symbolic::zero{};
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_minus) {
    gtl::symbolic::constant<gtl::symbolic::integer<3996>> constant;

    auto equation = -constant;
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::constant<gtl::symbolic::integer<-3996>>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_add) {
    gtl::symbolic::constant<gtl::symbolic::integer<3996>> constant0;
    gtl::symbolic::constant<gtl::symbolic::integer<37>> constant1;

    auto equation = constant0 + constant1;
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::constant<gtl::symbolic::integer<3996 + 37>>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_subtract) {
    gtl::symbolic::constant<gtl::symbolic::integer<3996>> constant0;
    gtl::symbolic::constant<gtl::symbolic::integer<37>> constant1;

    auto equation = constant0 - constant1;
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::constant<gtl::symbolic::integer<3996 - 37>>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_multiply) {
    gtl::symbolic::constant<gtl::symbolic::integer<3996>> constant0;
    gtl::symbolic::constant<gtl::symbolic::integer<37>> constant1;

    auto equation = constant0 * constant1;
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::constant<gtl::symbolic::integer<3996 * 37>>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_divide) {
    gtl::symbolic::constant<gtl::symbolic::integer<3996>> constant0;
    gtl::symbolic::constant<gtl::symbolic::integer<37>> constant1;

    auto equation = constant0 / constant1;
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::constant<gtl::symbolic::integer<3996 / 37>>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_sign) {
    gtl::symbolic::constant<gtl::symbolic::integer<123>> constant;

    auto equation = sign(constant);
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::one{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_abs) {
    gtl::symbolic::constant<gtl::symbolic::integer<-123>> constant;

    auto equation = abs(constant);
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::constant<gtl::symbolic::integer<123>>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_ceil) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = ceil(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_floor) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = floor(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_exp) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = exp(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_log) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = log(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_exp2) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = exp2(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_log2) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = log2(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_sin) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = sin(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_asin) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = asin(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_cos) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = cos(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_acos) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = acos(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_tan) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = tan(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_atan) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = atan(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_sinh) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = sinh(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_asinh) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = asinh(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_cosh) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = cosh(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_acosh) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = acosh(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_tanh) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = tanh(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_atanh) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = atanh(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_sqrt) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = sqrt(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_cbrt) {
    //gtl::symbolic::constant<???> constant;

    //auto equation = cbrt(constant);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_min) {
    gtl::symbolic::constant<gtl::symbolic::integer<123>> constant0;
    gtl::symbolic::constant<gtl::symbolic::integer<-123>> constant1;

    auto equation = min(constant0, constant1);
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = constant1;

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_max) {
    gtl::symbolic::constant<gtl::symbolic::integer<123>> constant0;
    gtl::symbolic::constant<gtl::symbolic::integer<-123>> constant1;

    auto equation = max(constant0, constant1);
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = constant0;

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_hypot) {
    //gtl::symbolic::constant<???> constant0;
    //gtl::symbolic::constant<???> constant1;

    //auto equation = hypot(constant0, constant1);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_atan2) {
    //gtl::symbolic::constant<???> constant0;
    //gtl::symbolic::constant<???> constant1;

    //auto equation = atan2(constant0, constant1);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, constants_pow) {
    //gtl::symbolic::constant<???> constant0;
    //gtl::symbolic::constant<???> constant1;

    //auto equation = pow(constant0, constant1);
    //auto simplified = gtl::symbolic::simplify_fully(equation);
    //auto result = gtl::symbolic::constant<???>{};

    //REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, remove_zeros_add) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    auto equation = gtl::symbolic::zero{} + parameter0 + gtl::symbolic::zero{} + parameter1 + gtl::symbolic::zero{} + gtl::symbolic::zero{};
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = parameter0 + parameter1;

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, remove_zeros_subtract) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    auto equation = gtl::symbolic::zero{} - parameter0 - gtl::symbolic::zero{} - parameter1 - gtl::symbolic::zero{} - gtl::symbolic::zero{};
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = gtl::symbolic::zero{} - parameter0 - parameter1;

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, remove_zeros_multiply) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    auto equation = gtl::symbolic::zero{} * parameter0 + gtl::symbolic::zero{} + parameter1 + gtl::symbolic::zero{} * gtl::symbolic::zero{};
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = parameter1;

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

TEST(symbolic, simplify, remove_zeros_divide) {
    gtl::symbolic::parameter<0> parameter0;
    gtl::symbolic::parameter<1> parameter1;

    auto equation = gtl::symbolic::zero{} / parameter0 + gtl::symbolic::zero{} + parameter1 + gtl::symbolic::zero{} / gtl::symbolic::one{};
    auto simplified = gtl::symbolic::simplify_fully(equation);
    auto result = parameter1;

    REQUIRE((gtl::symbolic::is_same_type<decltype(simplified), decltype(result)>));
}

///////////////////////////////////////////////////////////////////////////////

template <
    typename equation_type
>
inline double numerical_derivative(
    equation_type equation,
    double value,
    double delta = 1e-8
) {
    const double measurment_origin = equation.template solve<double>(value - 0.5 * delta, 0);
    const double measurment_offset = equation.template solve<double>(value + 0.5 * delta, 0);
    return (measurment_offset - measurment_origin) / delta;
}

TEST(symbolic, derive, inputs) {
    gtl::symbolic::parameter<0> parameter;
    gtl::symbolic::variable<1> variable;
    gtl::symbolic::constant<gtl::symbolic::integer<5>> constant;

    auto equation_parameter = parameter;
    auto equation_variable = variable;
    auto equation_constant = constant;

    auto derived_parameter = gtl::symbolic::derive_to_depth<0, 0>(equation_parameter);
    auto derived_variable = gtl::symbolic::derive_to_depth<0, 0>(equation_variable);
    auto derived_constant = gtl::symbolic::derive_to_depth<0, 0>(equation_constant);

    auto result_parameter = gtl::symbolic::one{};
    auto result_variable = gtl::symbolic::zero{};
    auto result_constant = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_parameter), decltype(result_parameter)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_variable), decltype(result_variable)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_constant), decltype(result_constant)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_parameter, -1.0), derived_parameter.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_variable,  -1.0), derived_variable.solve<double> (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_constant,  -1.0), derived_constant.solve<double> (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_parameter,  0.0), derived_parameter.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_variable,   0.0), derived_variable.solve<double> ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_constant,   0.0), derived_constant.solve<double> ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_parameter,  1.0), derived_parameter.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_variable,   1.0), derived_variable.solve<double> ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_constant,   1.0), derived_constant.solve<double> ( 1.0), 1e-5));
}

TEST(symbolic, derive, basic_operations) {
    gtl::symbolic::parameter<0> parameter;

    auto equation_plus_op       = gtl::symbolic::plus_op<decltype(parameter)>{};
    auto equation_minus_op      = gtl::symbolic::minus_op<decltype(parameter)>{};
    auto equation_add_op        = gtl::symbolic::add_op<decltype(parameter), decltype(parameter)>{};
    auto equation_subtract_op   = gtl::symbolic::subtract_op<decltype(parameter), decltype(parameter)>{};
    auto equation_multiply_op   = gtl::symbolic::multiply_op<decltype(parameter), decltype(parameter)>{};
    auto equation_divide_op     = gtl::symbolic::divide_op<decltype(parameter), decltype(parameter)>{};

    auto derived_plus_op       = gtl::symbolic::derive_to_depth<0, 0>(equation_plus_op);
    auto derived_minus_op      = gtl::symbolic::derive_to_depth<0, 0>(equation_minus_op);
    auto derived_add_op        = gtl::symbolic::derive_to_depth<0, 0>(equation_add_op);
    auto derived_subtract_op   = gtl::symbolic::derive_to_depth<0, 0>(equation_subtract_op);
    auto derived_multiply_op   = gtl::symbolic::derive_to_depth<0, 0>(equation_multiply_op);
    auto derived_divide_op     = gtl::symbolic::derive_to_depth<0, 0>(equation_divide_op);

    auto result_plus_op       = gtl::symbolic::one{};
    auto result_minus_op      = gtl::symbolic::minus_one{};
    auto result_add_op        = gtl::symbolic::two{};
    auto result_subtract_op   = gtl::symbolic::zero{};
    auto result_multiply_op   = gtl::symbolic::two{} * parameter;
    auto result_divide_op     = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_plus_op), decltype(result_plus_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_minus_op), decltype(result_minus_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_add_op), decltype(result_add_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_subtract_op), decltype(result_subtract_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_multiply_op), decltype(result_multiply_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_divide_op), decltype(result_divide_op)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_plus_op,       -1.0), derived_plus_op.solve<double>        (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_minus_op,      -1.0), derived_minus_op.solve<double>       (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_add_op,        -1.0), derived_add_op.solve<double>         (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_subtract_op,   -1.0), derived_subtract_op.solve<double>    (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_multiply_op,   -1.0), derived_multiply_op.solve<double>    (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_divide_op,     -1.0), derived_divide_op.solve<double>      (-1.0), 1e-5));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_plus_op,        0.0), derived_plus_op.solve<double>        ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_minus_op,       0.0), derived_minus_op.solve<double>       ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_add_op,         0.0), derived_add_op.solve<double>         ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_subtract_op,    0.0), derived_subtract_op.solve<double>    ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_multiply_op,    0.0), derived_multiply_op.solve<double>    ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_divide_op,      0.0), derived_divide_op.solve<double>      ( 0.0), 1e-5));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_plus_op,        1.0), derived_plus_op.solve<double>        ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_minus_op,       1.0), derived_minus_op.solve<double>       ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_add_op,         1.0), derived_add_op.solve<double>         ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_subtract_op,    1.0), derived_subtract_op.solve<double>    ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_multiply_op,    1.0), derived_multiply_op.solve<double>    ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_divide_op,      1.0), derived_divide_op.solve<double>      ( 1.0), 1e-5));
}

TEST(symbolic, derive, basic_operation_overloads) {
    gtl::symbolic::parameter<0> parameter;

    auto equation_plus_op       = +parameter;
    auto equation_minus_op      = -parameter;
    auto equation_add_op        = parameter + parameter;
    auto equation_subtract_op   = parameter - parameter;
    auto equation_multiply_op   = parameter * parameter;
    auto equation_divide_op     = parameter / parameter;

    auto derived_plus_op       = gtl::symbolic::derive_to_depth<0, 0>(equation_plus_op);
    auto derived_minus_op      = gtl::symbolic::derive_to_depth<0, 0>(equation_minus_op);
    auto derived_add_op        = gtl::symbolic::derive_to_depth<0, 0>(equation_add_op);
    auto derived_subtract_op   = gtl::symbolic::derive_to_depth<0, 0>(equation_subtract_op);
    auto derived_multiply_op   = gtl::symbolic::derive_to_depth<0, 0>(equation_multiply_op);
    auto derived_divide_op     = gtl::symbolic::derive_to_depth<0, 0>(equation_divide_op);

    auto result_plus_op       = gtl::symbolic::one{};
    auto result_minus_op      = gtl::symbolic::minus_one{};
    auto result_add_op        = gtl::symbolic::two{};
    auto result_subtract_op   = gtl::symbolic::zero{};
    auto result_multiply_op   = gtl::symbolic::two{} * parameter;
    auto result_divide_op     = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_plus_op), decltype(result_plus_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_minus_op), decltype(result_minus_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_add_op), decltype(result_add_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_subtract_op), decltype(result_subtract_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_multiply_op), decltype(result_multiply_op)>));
    REQUIRE((gtl::symbolic::is_same_type<decltype(derived_divide_op), decltype(result_divide_op)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_plus_op,       -1.0), derived_plus_op.solve<double>        (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_minus_op,      -1.0), derived_minus_op.solve<double>       (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_add_op,        -1.0), derived_add_op.solve<double>         (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_subtract_op,   -1.0), derived_subtract_op.solve<double>    (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_multiply_op,   -1.0), derived_multiply_op .solve<double>   (-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_divide_op,     -1.0), derived_divide_op.solve<double>      (-1.0), 1e-5));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_plus_op,        0.0), derived_plus_op.solve<double>        ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_minus_op,       0.0), derived_minus_op.solve<double>       ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_add_op,         0.0), derived_add_op.solve<double>         ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_subtract_op,    0.0), derived_subtract_op.solve<double>    ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_multiply_op,    0.0), derived_multiply_op .solve<double>   ( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_divide_op,      0.0), derived_divide_op.solve<double>      ( 0.0), 1e-5));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_plus_op,        1.0), derived_plus_op.solve<double>        ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_minus_op,       1.0), derived_minus_op.solve<double>       ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_add_op,         1.0), derived_add_op.solve<double>         ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_subtract_op,    1.0), derived_subtract_op.solve<double>    ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_multiply_op,    1.0), derived_multiply_op .solve<double>   ( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation_divide_op,      1.0), derived_divide_op.solve<double>      ( 1.0), 1e-5));
}

TEST(symbolic, derive, math_operation_sign) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = sign(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.1), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.1), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
}

TEST(symbolic, derive, math_operation_abs) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = abs(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::sign_op<decltype(parameter)>{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
}

TEST(symbolic, derive, math_operation_ceil) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = ceil(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.5), derived.solve<double>(-1.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.1), derived.solve<double>(-0.1), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.1), derived.solve<double>( 0.1), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.5), derived.solve<double>( 1.5), 1e-5));
}

TEST(symbolic, derive, math_operation_floor) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = floor(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::zero{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.5), derived.solve<double>(-1.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.1), derived.solve<double>(-0.1), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.1), derived.solve<double>( 0.1), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.5), derived.solve<double>( 1.5), 1e-5));
}

TEST(symbolic, derive, math_operation_exp) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = exp(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = exp(parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -2.0), derived.solve<double>(-2.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  2.0), derived.solve<double>( 2.0), 1e-5));
}

TEST(symbolic, derive, math_operation_log) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = log(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / parameter;

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 0.01), derived.solve<double>(0.01), 1e-4));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 0.10), derived.solve<double>(0.10), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 1.00), derived.solve<double>(1.00), 1e-5));
}

TEST(symbolic, derive, math_operation_exp2) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = exp2(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = log(gtl::symbolic::two{}) * exp2(parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -2.0), derived.solve<double>(-2.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  2.0), derived.solve<double>( 2.0), 1e-5));
}

TEST(symbolic, derive, math_operation_log2) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = log2(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / (log(gtl::symbolic::two{}) * parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 0.01), derived.solve<double>(0.01), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 0.10), derived.solve<double>(0.10), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 1.00), derived.solve<double>(1.00), 1e-5));
}

TEST(symbolic, derive, math_operation_sin) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = sin(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = cos(parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_asin) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = asin(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / sqrt(gtl::symbolic::one{} - pow(parameter, gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
}

TEST(symbolic, derive, math_operation_cos) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = cos(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = -sin(parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_acos) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = acos(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::minus_one{} / sqrt(gtl::symbolic::one{} - pow(parameter, gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
}

TEST(symbolic, derive, math_operation_tan) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = tan(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / pow(cos(parameter), gtl::symbolic::two{});

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_atan) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = atan(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / (gtl::symbolic::one{} + pow(parameter, gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
}

TEST(symbolic, derive, math_operation_sinh) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = sinh(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = cosh(parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_asinh) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = asinh(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / sqrt(pow(parameter, gtl::symbolic::two{}) + gtl::symbolic::one{});

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>), "%s == %s\n", derived.to_string().c_str(), result.to_string().c_str());

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_cosh) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = cosh(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = sinh(parameter);

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_acosh) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = acosh(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / sqrt(pow(parameter, gtl::symbolic::two{}) - gtl::symbolic::one{});

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 25.0), derived.solve<double>(25.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 11.0), derived.solve<double>(11.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 10.0), derived.solve<double>(10.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.1), derived.solve<double>( 1.1), 1e-5));
}

TEST(symbolic, derive, math_operation_tanh) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = tanh(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / (pow(cosh(parameter), gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_atanh) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = atanh(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / (gtl::symbolic::one{} - pow(parameter, gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
}

TEST(symbolic, derive, math_operation_sqrt) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = sqrt(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / (gtl::symbolic::two{} * sqrt(parameter));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 25.0), derived.solve<double>(25.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 11.0), derived.solve<double>(11.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 10.0), derived.solve<double>(10.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
}

TEST(symbolic, derive, math_operation_cbrt) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = cbrt(parameter);
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::one{} / (gtl::symbolic::three{} * pow(cbrt(parameter), gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>), "%s == %s\n", derived.to_string().c_str(), result.to_string().c_str());

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 25.0), derived.solve<double>(25.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 11.0), derived.solve<double>(11.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, 10.0), derived.solve<double>(10.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
}

TEST(symbolic, derive, math_operation_min) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = min((gtl::symbolic::two{} * parameter), gtl::symbolic::three{});
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = ((sign((gtl::symbolic::two{} * parameter) - gtl::symbolic::three{}) * gtl::symbolic::minus_two{}) + gtl::symbolic::two{}) / gtl::symbolic::two{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_max) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = max((gtl::symbolic::two{} * parameter), gtl::symbolic::three{});
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = ((sign((gtl::symbolic::two{} * parameter) - gtl::symbolic::three{}) * gtl::symbolic::two{}) + gtl::symbolic::two{}) / gtl::symbolic::two{};

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_hypot) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = hypot((gtl::symbolic::two{} * parameter), gtl::symbolic::three{});
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = ((gtl::symbolic::two{} * parameter * gtl::symbolic::two{}) / sqrt(pow(gtl::symbolic::two{} * parameter, gtl::symbolic::two{}) + pow(gtl::symbolic::three{}, gtl::symbolic::two{})));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_atan2) {
    gtl::symbolic::parameter<0> parameter;

    auto equation   = atan2((gtl::symbolic::two{} * parameter), gtl::symbolic::three{});
    auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
    auto result     = gtl::symbolic::constant<gtl::symbolic::integer<6>>{} / (pow(gtl::symbolic::two{} * parameter, gtl::symbolic::two{}) + pow(gtl::symbolic::three{}, gtl::symbolic::two{}));

    REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>));

    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.1), derived.solve<double>( 0.1), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.1), derived.solve<double>(-0.1), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
    REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-5));
}

TEST(symbolic, derive, math_operation_pow) {
    {
        gtl::symbolic::parameter<0> parameter;

        auto equation   = pow((gtl::symbolic::two{} * parameter), gtl::symbolic::three{});
        auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
        auto result     = gtl::symbolic::three{} * pow(gtl::symbolic::two{} * parameter, gtl::symbolic::two{}) * gtl::symbolic::two{};

        REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>), "%s == %s\n", derived.to_string().c_str(), result.to_string().c_str());

        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-4));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.0), derived.solve<double>( 0.0), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.5), derived.solve<double>(-0.5), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -0.9), derived.solve<double>(-0.9), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -1.0), derived.solve<double>(-1.0), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation, -5.0), derived.solve<double>(-5.0), 1e-4));
    }
    {
        gtl::symbolic::parameter<0> parameter;

        auto equation   = pow(parameter, parameter);
        auto derived    = gtl::symbolic::derive_to_depth<0, 0>(equation);
        auto result     = pow(parameter, parameter) * (log(parameter) + gtl::symbolic::one{});

        REQUIRE((gtl::symbolic::is_same_type<decltype(derived), decltype(result)>), "%s == %s\n", derived.to_string().c_str(), result.to_string().c_str());

        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  5.0), derived.solve<double>( 5.0), 1e-3));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  1.0), derived.solve<double>( 1.0), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.9), derived.solve<double>( 0.9), 1e-5));
        REQUIRE(testbench::is_value_approx(numerical_derivative(equation,  0.5), derived.solve<double>( 0.5), 1e-5));
    }
}

///////////////////////////////////////////////////////////////////////////////

TEST(symbolic, types, matrices) {
    auto mat2x3 = gtl::symbolic::matrix<2,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>>{};
    auto mat3x2 = gtl::symbolic::matrix<3,2,gtl::symbolic::parameter<6>,gtl::symbolic::parameter<7>,gtl::symbolic::parameter<8>,gtl::symbolic::parameter<9>,gtl::symbolic::parameter<10>,gtl::symbolic::parameter<11>>{};

    testbench::do_not_optimise_away(mat2x3);
    testbench::do_not_optimise_away(mat3x2);
}

TEST(symbolic, solve, matrix_add_scalar) {
    auto mat2x3 = gtl::symbolic::matrix<2,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>>{};

    auto result = gtl::symbolic::one{} + mat2x3 + gtl::symbolic::parameter<6>{};

    auto result00 = gtl::symbolic::get_cell<0,0>(result);
    auto result01 = gtl::symbolic::get_cell<0,1>(result);
    auto result02 = gtl::symbolic::get_cell<0,2>(result);
    auto result10 = gtl::symbolic::get_cell<1,0>(result);
    auto result11 = gtl::symbolic::get_cell<1,1>(result);
    auto result12 = gtl::symbolic::get_cell<1,2>(result);

    REQUIRE(result00.solve<int>(0,1,2,3,4,5,6) == 1 + 0 + 6);
    REQUIRE(result01.solve<int>(0,1,2,3,4,5,6) == 1 + 1 + 6);
    REQUIRE(result02.solve<int>(0,1,2,3,4,5,6) == 1 + 2 + 6);
    REQUIRE(result10.solve<int>(0,1,2,3,4,5,6) == 1 + 3 + 6);
    REQUIRE(result11.solve<int>(0,1,2,3,4,5,6) == 1 + 4 + 6);
    REQUIRE(result12.solve<int>(0,1,2,3,4,5,6) == 1 + 5 + 6);
}

TEST(symbolic, solve, matrix_subtract_scalar) {
    auto mat2x3 = gtl::symbolic::matrix<2,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>>{};

    auto result = gtl::symbolic::one{} - mat2x3 - gtl::symbolic::parameter<6>{};

    auto result00 = gtl::symbolic::get_cell<0,0>(result);
    auto result01 = gtl::symbolic::get_cell<0,1>(result);
    auto result02 = gtl::symbolic::get_cell<0,2>(result);
    auto result10 = gtl::symbolic::get_cell<1,0>(result);
    auto result11 = gtl::symbolic::get_cell<1,1>(result);
    auto result12 = gtl::symbolic::get_cell<1,2>(result);

    REQUIRE(result00.solve<int>(0,1,2,3,4,5,6) == 1 - 0 - 6);
    REQUIRE(result01.solve<int>(0,1,2,3,4,5,6) == 1 - 1 - 6);
    REQUIRE(result02.solve<int>(0,1,2,3,4,5,6) == 1 - 2 - 6);
    REQUIRE(result10.solve<int>(0,1,2,3,4,5,6) == 1 - 3 - 6);
    REQUIRE(result11.solve<int>(0,1,2,3,4,5,6) == 1 - 4 - 6);
    REQUIRE(result12.solve<int>(0,1,2,3,4,5,6) == 1 - 5 - 6);
}

TEST(symbolic, solve, matrix_multiply_scalar) {
    auto mat2x3 = gtl::symbolic::matrix<2,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>>{};

    auto result = gtl::symbolic::two{} * mat2x3 * gtl::symbolic::parameter<6>{};

    auto result00 = gtl::symbolic::get_cell<0,0>(result);
    auto result01 = gtl::symbolic::get_cell<0,1>(result);
    auto result02 = gtl::symbolic::get_cell<0,2>(result);
    auto result10 = gtl::symbolic::get_cell<1,0>(result);
    auto result11 = gtl::symbolic::get_cell<1,1>(result);
    auto result12 = gtl::symbolic::get_cell<1,2>(result);

    REQUIRE(result00.solve<int>(0,1,2,3,4,5,6) == 2 * 0 * 6);
    REQUIRE(result01.solve<int>(0,1,2,3,4,5,6) == 2 * 1 * 6);
    REQUIRE(result02.solve<int>(0,1,2,3,4,5,6) == 2 * 2 * 6);
    REQUIRE(result10.solve<int>(0,1,2,3,4,5,6) == 2 * 3 * 6);
    REQUIRE(result11.solve<int>(0,1,2,3,4,5,6) == 2 * 4 * 6);
    REQUIRE(result12.solve<int>(0,1,2,3,4,5,6) == 2 * 5 * 6);
}

TEST(symbolic, solve, matrix_add) {
    auto mat2x2a = gtl::symbolic::matrix<2,2,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>>{};
    auto mat2x2b = gtl::symbolic::matrix<2,2,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>,gtl::symbolic::parameter<6>,gtl::symbolic::parameter<7>>{};

    auto result = mat2x2a + mat2x2b;

    auto result00 = gtl::symbolic::get_cell<0,0>(result);
    auto result01 = gtl::symbolic::get_cell<0,1>(result);
    auto result10 = gtl::symbolic::get_cell<1,0>(result);
    auto result11 = gtl::symbolic::get_cell<1,1>(result);

    REQUIRE(result00.solve<int>(0,1,2,3,4,5,6,7) == 0 + 4);
    REQUIRE(result01.solve<int>(0,1,2,3,4,5,6,7) == 1 + 5);
    REQUIRE(result10.solve<int>(0,1,2,3,4,5,6,7) == 2 + 6);
    REQUIRE(result11.solve<int>(0,1,2,3,4,5,6,7) == 3 + 7);
}

TEST(symbolic, solve, matrix_subtract) {
    auto mat2x2a = gtl::symbolic::matrix<2,2,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>>{};
    auto mat2x2b = gtl::symbolic::matrix<2,2,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>,gtl::symbolic::parameter<6>,gtl::symbolic::parameter<7>>{};

    auto result = mat2x2a - mat2x2b;

    auto result00 = gtl::symbolic::get_cell<0,0>(result);
    auto result01 = gtl::symbolic::get_cell<0,1>(result);
    auto result10 = gtl::symbolic::get_cell<1,0>(result);
    auto result11 = gtl::symbolic::get_cell<1,1>(result);

    REQUIRE(result00.solve<int>(0,1,2,3,4,5,6,7) == 0 - 4);
    REQUIRE(result01.solve<int>(0,1,2,3,4,5,6,7) == 1 - 5);
    REQUIRE(result10.solve<int>(0,1,2,3,4,5,6,7) == 2 - 6);
    REQUIRE(result11.solve<int>(0,1,2,3,4,5,6,7) == 3 - 7);
}

TEST(symbolic, solve, matrix_multiply) {
    auto mat2x3 = gtl::symbolic::matrix<2,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>>{};
    auto mat3x2 = gtl::symbolic::matrix<3,2,gtl::symbolic::parameter<6>,gtl::symbolic::parameter<7>,gtl::symbolic::parameter<8>,gtl::symbolic::parameter<9>,gtl::symbolic::parameter<10>,gtl::symbolic::parameter<11>>{};

    auto mat2x2 = mat2x3 * mat3x2;

    auto result00 = gtl::symbolic::get_cell<0,0>(mat2x2);
    auto result01 = gtl::symbolic::get_cell<0,1>(mat2x2);
    auto result10 = gtl::symbolic::get_cell<1,0>(mat2x2);
    auto result11 = gtl::symbolic::get_cell<1,1>(mat2x2);

    REQUIRE(result00.solve<int>(0,1,2,3,4,5,6,7,8,9,10,11) == 28);
    REQUIRE(result01.solve<int>(0,1,2,3,4,5,6,7,8,9,10,11) == 100);
    REQUIRE(result10.solve<int>(0,1,2,3,4,5,6,7,8,9,10,11) == 31);
    REQUIRE(result11.solve<int>(0,1,2,3,4,5,6,7,8,9,10,11) == 112);
}

TEST(symbolic, solve, matrix_transpose) {
    {
        auto mat2x3 = gtl::symbolic::matrix<2,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>,gtl::symbolic::parameter<4>,gtl::symbolic::parameter<5>>{};
        auto mat3x2 = gtl::symbolic::transpose(mat2x3);

        REQUIRE((gtl::symbolic::get_cell<0,0>(mat3x2).solve<int>(0,1,2,3,4,5) == gtl::symbolic::get_cell<0,0>(mat2x3).solve<int>(0,1,2,3,4,5)));
        REQUIRE((gtl::symbolic::get_cell<0,1>(mat3x2).solve<int>(0,1,2,3,4,5) == gtl::symbolic::get_cell<1,0>(mat2x3).solve<int>(0,1,2,3,4,5)));
        REQUIRE((gtl::symbolic::get_cell<1,0>(mat3x2).solve<int>(0,1,2,3,4,5) == gtl::symbolic::get_cell<0,1>(mat2x3).solve<int>(0,1,2,3,4,5)));
        REQUIRE((gtl::symbolic::get_cell<1,1>(mat3x2).solve<int>(0,1,2,3,4,5) == gtl::symbolic::get_cell<1,1>(mat2x3).solve<int>(0,1,2,3,4,5)));
        REQUIRE((gtl::symbolic::get_cell<2,0>(mat3x2).solve<int>(0,1,2,3,4,5) == gtl::symbolic::get_cell<0,2>(mat2x3).solve<int>(0,1,2,3,4,5)));
        REQUIRE((gtl::symbolic::get_cell<2,1>(mat3x2).solve<int>(0,1,2,3,4,5) == gtl::symbolic::get_cell<1,2>(mat2x3).solve<int>(0,1,2,3,4,5)));
    }

    {
        auto mat1x3 = gtl::symbolic::matrix<1,3,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>>{};
        auto mat3x1 = gtl::symbolic::transpose(mat1x3);

        REQUIRE((gtl::symbolic::get_cell<0,0>(mat1x3).solve<int>(0,1,2) == gtl::symbolic::get_cell<0,0>(mat3x1).solve<int>(0,1,2)));
        REQUIRE((gtl::symbolic::get_cell<0,1>(mat1x3).solve<int>(0,1,2) == gtl::symbolic::get_cell<1,0>(mat3x1).solve<int>(0,1,2)));
        REQUIRE((gtl::symbolic::get_cell<0,2>(mat1x3).solve<int>(0,1,2) == gtl::symbolic::get_cell<2,0>(mat3x1).solve<int>(0,1,2)));
    }

    {
        auto mat2x2a = gtl::symbolic::matrix<2,2,gtl::symbolic::parameter<0>,gtl::symbolic::parameter<1>,gtl::symbolic::parameter<2>,gtl::symbolic::parameter<3>>{};
        auto mat2x2b = gtl::symbolic::transpose(mat2x2a);

        REQUIRE((gtl::symbolic::get_cell<0,0>(mat2x2a).solve<int>(0,1,2,3) == gtl::symbolic::get_cell<0,0>(mat2x2b).solve<int>(0,1,2,3)));
        REQUIRE((gtl::symbolic::get_cell<0,1>(mat2x2a).solve<int>(0,1,2,3) == gtl::symbolic::get_cell<1,0>(mat2x2b).solve<int>(0,1,2,3)));
        REQUIRE((gtl::symbolic::get_cell<1,0>(mat2x2a).solve<int>(0,1,2,3) == gtl::symbolic::get_cell<0,1>(mat2x2b).solve<int>(0,1,2,3)));
        REQUIRE((gtl::symbolic::get_cell<1,1>(mat2x2a).solve<int>(0,1,2,3) == gtl::symbolic::get_cell<1,1>(mat2x2b).solve<int>(0,1,2,3)));
    }
}
