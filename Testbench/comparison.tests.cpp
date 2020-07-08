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

#include "comparison.tests.hpp"

namespace testbench {
    unsigned long long int string_length(const char* string) {
        if (!string) {
            return 0;
        }
        unsigned long long int length = 0;
        while (*string++) {
            ++length;
        }
        return length;
    }

    bool is_memory_same(const void* lhs, const void* rhs, unsigned int length) {
        if ((length > 0) && ((!lhs) || (!rhs))) {
            return false;
        }
        for (unsigned int i = 0; i < length; ++i) {
            if (reinterpret_cast<const char*>(lhs)[i] != reinterpret_cast<const char*>(rhs)[i]) {
                return false;
            }
        }
        return true;
    }

    bool is_string_same(const char* lhs, const char* rhs) {
        if ((!lhs) || (!rhs)) {
            return false;
        }
        while (*lhs && (*lhs == *rhs)) {
            ++lhs;
            ++rhs;
        }
        return *lhs == *rhs;
    }

    // Macro for generating test data structures.
    #define TEST_IS_VALUE_EQUAL_FUNCTION(TYPE)                  \
        template <>                                             \
        bool is_value_equal(const TYPE& lhs, const TYPE& rhs) { \
            return lhs == rhs;                                  \
        }                                                       \

    // Boolean.
    TEST_IS_VALUE_EQUAL_FUNCTION(bool)

    // Character
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned char)
    TEST_IS_VALUE_EQUAL_FUNCTION(signed char)
    TEST_IS_VALUE_EQUAL_FUNCTION(char)

    // Extended characters.
    TEST_IS_VALUE_EQUAL_FUNCTION(wchar_t)
    TEST_IS_VALUE_EQUAL_FUNCTION(char16_t)
    TEST_IS_VALUE_EQUAL_FUNCTION(char32_t)

    // Fixed point.
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned short int)
    TEST_IS_VALUE_EQUAL_FUNCTION(signed short int)
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned int)
    TEST_IS_VALUE_EQUAL_FUNCTION(signed int)
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned long int)
    TEST_IS_VALUE_EQUAL_FUNCTION(signed long int)
    TEST_IS_VALUE_EQUAL_FUNCTION(unsigned long long int)
    TEST_IS_VALUE_EQUAL_FUNCTION(signed long long int)

    #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wfloat-equal"
    #endif

    // Floating point.
    TEST_IS_VALUE_EQUAL_FUNCTION(float)
    TEST_IS_VALUE_EQUAL_FUNCTION(double)
    TEST_IS_VALUE_EQUAL_FUNCTION(long double)

    #if defined(__clang__)
        #pragma clang diagnostic pop
    #endif

    // Pointer type.
    TEST_IS_VALUE_EQUAL_FUNCTION(decltype(nullptr))

    #undef TEST_IS_VALUE_EQUAL_FUNCTION
}
