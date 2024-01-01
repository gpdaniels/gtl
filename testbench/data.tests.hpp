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

#pragma once
#ifndef GTL_DATA_TESTS_HPP
#define GTL_DATA_TESTS_HPP

namespace testbench {
    // Useful test data.
    template <typename template_type>
    struct test_data { };

    // Macro for generating test data structures.
    #define TEST_DATA_STRUCT(TYPE)          \
        template <>                         \
        struct test_data<TYPE> {            \
            static const char* name;        \
            static const TYPE data[];       \
            static const TYPE* begin();     \
            static const TYPE* end();       \
        }

    // Boolean.
    TEST_DATA_STRUCT(bool);

    // Character
    TEST_DATA_STRUCT(unsigned char);
    TEST_DATA_STRUCT(signed char);
    TEST_DATA_STRUCT(char);

    // Extended characters.
    TEST_DATA_STRUCT(wchar_t);
    TEST_DATA_STRUCT(char16_t);
    TEST_DATA_STRUCT(char32_t);

    // Fixed point.
    TEST_DATA_STRUCT(unsigned short int);
    TEST_DATA_STRUCT(signed short int);
    TEST_DATA_STRUCT(unsigned int);
    TEST_DATA_STRUCT(signed int);
    TEST_DATA_STRUCT(unsigned long int);
    TEST_DATA_STRUCT(signed long int);
    TEST_DATA_STRUCT(unsigned long long int);
    TEST_DATA_STRUCT(signed long long int);

    // Floating point.
    TEST_DATA_STRUCT(float);
    TEST_DATA_STRUCT(double);
    TEST_DATA_STRUCT(long double);

    // Pointer type.
    TEST_DATA_STRUCT(decltype(nullptr));

    #undef TEST_DATA_STRUCT
}

#endif // GTL_DATA_TESTS_HPP
