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

#pragma once
#ifndef GTL_COMPARISON_TESTS_HPP
#define GTL_COMPARISON_TESTS_HPP

namespace testbench {
    unsigned long long int string_length(const char* string);

    bool is_memory_same(const void* lhs, const void* rhs, unsigned long long int length);

    bool is_string_same(const char* lhs, const char* rhs);

    template <typename type>
    bool is_value_equal(const type& lhs, const type& rhs);

    // Macro for generating test functions.
    #define TEST_IS_VALUE_EQUAL_FUNCTION(TYPE)                  \
        template <>                                             \
        bool is_value_equal(const TYPE& lhs, const TYPE& rhs)   \

    // Boolean.
    TEST_IS_VALUE_EQUAL_FUNCTION(bool);

    // Character
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned char);
    TEST_IS_VALUE_EQUAL_FUNCTION(signed char);
    TEST_IS_VALUE_EQUAL_FUNCTION(char);

    // Extended characters.
    TEST_IS_VALUE_EQUAL_FUNCTION(wchar_t);
    TEST_IS_VALUE_EQUAL_FUNCTION(char16_t);
    TEST_IS_VALUE_EQUAL_FUNCTION(char32_t);

    // Fixed point.
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned short int);
    TEST_IS_VALUE_EQUAL_FUNCTION(signed short int);
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned int);
    TEST_IS_VALUE_EQUAL_FUNCTION(signed int);
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned long int);
    TEST_IS_VALUE_EQUAL_FUNCTION(signed long int);
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned long long int);
    TEST_IS_VALUE_EQUAL_FUNCTION(signed long long int);

    // Floating point.
    TEST_IS_VALUE_EQUAL_FUNCTION(float);
    TEST_IS_VALUE_EQUAL_FUNCTION(double);
    TEST_IS_VALUE_EQUAL_FUNCTION(long double);

    // Pointer type.
    TEST_IS_VALUE_EQUAL_FUNCTION(decltype(nullptr));

    #undef TEST_IS_VALUE_EQUAL_FUNCTION

    template <typename type>
    bool is_value_approx(const type& lhs, const type& rhs, const type& epsilon) {
        const type difference = lhs - rhs;
        return ((difference >= 0) && (difference <= epsilon)) || ((difference <= 0) && (difference >= -epsilon));
    }

}

#endif // GTL_COMPARISON_TESTS_HPP
