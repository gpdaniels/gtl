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

#pragma once
#ifndef GTL_COMPARISON_TESTS_HPP
#define GTL_COMPARISON_TESTS_HPP

namespace testbench {
    unsigned long long int string_length(const char* string);

    bool is_memory_same(const void* lhs, const void* rhs, unsigned int length);

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
