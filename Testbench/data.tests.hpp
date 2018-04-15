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
#ifndef DATA_TESTS_HPP
#define DATA_TESTS_HPP

// Useful test data.
template <typename template_type>
struct test_data { };

// Macro for generating test data structures.
#define TEST_DATA_STRUCT(TYPE) \
    template <> \
    struct test_data<TYPE> { \
        static const char* name; \
        static const TYPE data[]; \
        static const TYPE* begin(void); \
        static const TYPE* end(void); \
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

#endif // DATA_TESTS_HPP
