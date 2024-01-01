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

    bool is_memory_same(const void* lhs, const void* rhs, unsigned long long int length) {
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
