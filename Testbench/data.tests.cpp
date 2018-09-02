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

#include "data.tests.hpp"

#include <limits>
#include <climits>

// Boolean.
const char* test_data<bool>::name = "bool";
const bool test_data<bool>::data[] = {
    false,
    true
};
const bool* test_data<bool>::begin() { return data; }
const bool* test_data<bool>::end()   { return data + sizeof(data) / sizeof(bool); }

// Character.
const char* test_data<unsigned char>::name = "unsigned char";
const unsigned char test_data<unsigned char>::data[] = {
    0, 1, 127, 128, 255
};
const unsigned char* test_data<unsigned char>::begin() { return data; }
const unsigned char* test_data<unsigned char>::end()   { return data + sizeof(data) / sizeof(unsigned char); }

const char* test_data<signed char>::name = "signed char";
const signed char test_data<signed char>::data[] = {
    -128, -127, -1, 0, 1, 127
};
const signed char* test_data<signed char>::begin() { return data; }
const signed char* test_data<signed char>::end()   { return data + sizeof(data) / sizeof(signed char); }

const char* test_data<char>::name = "char";
const char test_data<char>::data[] = {
#if CHAR_MIN == 0
    0, 1, 127, 128, 255
#else
    -128, -127, -1, 0, 1, 127
#endif
};
const char* test_data<char>::begin() { return data; }
const char* test_data<char>::end()   { return data + sizeof(data) / sizeof(char); }

// Extended characters.
const char* test_data<wchar_t>::name = "wchar_t";
const wchar_t test_data<wchar_t>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535
};
const wchar_t* test_data<wchar_t>::begin() { return data; }
const wchar_t* test_data<wchar_t>::end()   { return data + sizeof(data) / sizeof(wchar_t); }

const char* test_data<char16_t>::name = "char16_t";
const char16_t test_data<char16_t>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535
};
const char16_t* test_data<char16_t>::begin() { return data; }
const char16_t* test_data<char16_t>::end()   { return data + sizeof(data) / sizeof(char16_t); }

const char* test_data<char32_t>::name = "char32_t";
const char32_t test_data<char32_t>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647, 2147483648, 4294967295
};
const char32_t* test_data<char32_t>::begin() { return data; }
const char32_t* test_data<char32_t>::end()   { return data + sizeof(data) / sizeof(char32_t); }

// Fixed point.
#if defined(__clang__)
    // clang.
    const char* test_data<unsigned short int>::name = "unsigned short";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<unsigned short int>::name = "short unsigned int";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<unsigned short int>::name = "unsigned short";
#else
    #error "No support for this compiler."
#endif
const unsigned short int test_data<unsigned short int>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535
};
const unsigned short int* test_data<unsigned short int>::begin() { return data; }
const unsigned short int* test_data<unsigned short int>::end()   { return data + sizeof(data) / sizeof(unsigned short int); }

#if defined(__clang__)
    // clang.
    const char* test_data<signed short int>::name = "short";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<signed short int>::name = "short int";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<signed short int>::name = "short";
#else
    #error "No support for this compiler."
#endif
const signed short int test_data<signed short int>::data[] = {
    -32768, -32767, -256, -255, -128, -127, -1, 0, 1, 127, 128, 255, 256, 32767
};
const signed short int* test_data<signed short int>::begin() { return data; }
const signed short int* test_data<signed short int>::end()   { return data + sizeof(data) / sizeof(signed short int); }

const char* test_data<unsigned int>::name = "unsigned int";
const unsigned int test_data<unsigned int>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647, 2147483648, 4294967295
};
const unsigned int* test_data<unsigned int>::begin() { return data; }
const unsigned int* test_data<unsigned int>::end()   { return data + sizeof(data) / sizeof(unsigned int); }

const char* test_data<signed int>::name = "int";
const signed int test_data<signed int>::data[] = {
    -2147483648, -2147483647, -65536, -65535, -32768, -32767, -256, -255, -128, -127, -1, 0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647
};
const signed int* test_data<signed int>::begin() { return data; }
const signed int* test_data<signed int>::end()   { return data + sizeof(data) / sizeof(signed int); }

#if defined(__clang__)
    // clang.
    const char* test_data<unsigned long int>::name = "unsigned long";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<unsigned long int>::name = "long unsigned int";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<unsigned long int>::name = "unsigned long";
#else
    #error "No support for this compiler."
#endif
const unsigned long int test_data<unsigned long int>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647, 2147483648, 4294967295
};
const unsigned long int* test_data<unsigned long int>::begin() { return data; }
const unsigned long int* test_data<unsigned long int>::end()   { return data + sizeof(data) / sizeof(unsigned long int); }

#if defined(__clang__)
    // clang.
    const char* test_data<signed long int>::name = "long";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<signed long int>::name = "long int";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<signed long int>::name = "long";
#else
    #error "No support for this compiler."
#endif
const signed long int test_data<signed long int>::data[] = {
    -2147483648, -2147483647, -65536, -65535, -32768, -32767, -256, -255, -128, -127, -1, 0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647
};
const signed long int* test_data<signed long int>::begin() { return data; }
const signed long int* test_data<signed long int>::end()   { return data + sizeof(data) / sizeof(signed long int); }

#if defined(__clang__)
    // clang.
    const char* test_data<unsigned long long int>::name = "unsigned long long";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<unsigned long long int>::name = "long long unsigned int";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<unsigned long long int>::name = "unsigned __int64";
#else
    #error "No support for this compiler."
#endif
const unsigned long long int test_data<unsigned long long int>::data[] = {
    0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647ull, 2147483648ull, 4294967295ull, 4294967296ull, 18446744073709551615ull
};
const unsigned long long int* test_data<unsigned long long int>::begin() { return data; }
const unsigned long long int* test_data<unsigned long long int>::end()   { return data + sizeof(data) / sizeof(unsigned long long int); }

#if defined(__clang__)
    // clang.
    const char* test_data<signed long long int>::name = "long long";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<signed long long int>::name = "long long int";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<signed long long int>::name = "__int64";
#else
    #error "No support for this compiler."
#endif
const signed long long int test_data<signed long long int>::data[] = {
    -9223372036854775807ll - 1, -9223372036854775807ll, -4294967296, -4294967295, -2147483648, -2147483647, -65536, -65535, -32768, -32767, -256, -255, -128, -127, -1, 0, 1, 127, 128, 255, 256, 32767, 32768, 65535, 65536, 2147483647, 2147483648, 4294967295, 4294967296, 9223372036854775807ll
};
const signed long long int* test_data<signed long long int>::begin() { return data; }
const signed long long int* test_data<signed long long int>::end()   { return data + sizeof(data) / sizeof(signed long long int); }

// Floating point.
const char* test_data<float>::name = "float";
const float test_data<float>::data[] = {
    -std::numeric_limits<float>::infinity(),
    std::numeric_limits<float>::lowest(),
    -1.0f, -0.0f,
    0.0f, 1.0f,
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::infinity()
};
const float* test_data<float>::begin() { return data; }
const float* test_data<float>::end()   { return data + sizeof(data) / sizeof(float); }

const char* test_data<double>::name = "double";
const double test_data<double>::data[] = {
    -std::numeric_limits<double>::infinity(),
    std::numeric_limits<double>::lowest(),
    -1.0, -0.0,
    0.0, 1.0,
    std::numeric_limits<double>::min(),
    std::numeric_limits<double>::max(),
    std::numeric_limits<double>::infinity()
};
const double* test_data<double>::begin() { return data; }
const double* test_data<double>::end()   { return data + sizeof(data) / sizeof(double); }

const char* test_data<long double>::name = "long double";
const long double test_data<long double>::data[] = {
    -std::numeric_limits<long double>::infinity(),
    std::numeric_limits<long double>::lowest(),
    static_cast<long double>(-1.0), static_cast<long double>(-0.0),
    static_cast<long double>(0.0), static_cast<long double>(1.0),
    std::numeric_limits<long double>::min(),
    std::numeric_limits<long double>::max(),
    std::numeric_limits<long double>::infinity()
};
const long double* test_data<long double>::begin() { return data; }
const long double* test_data<long double>::end()   { return data + sizeof(data) / sizeof(long double); }

// Pointer type.
#if defined(__clang__)
    // clang.
    const char* test_data<decltype(nullptr)>::name = "nullptr_t";
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
    // gcc.
    const char* test_data<decltype(nullptr)>::name = "std::nullptr_t";
#elif defined(_MSC_VER)
    // msvc.
    const char* test_data<decltype(nullptr)>::name = "std::nullptr_t";
#else
    #error "No support for this compiler."
#endif
const decltype(nullptr) test_data<decltype(nullptr)>::data[] = {
    nullptr
};
const decltype(nullptr)* test_data<decltype(nullptr)>::begin() { return data; }
const decltype(nullptr)* test_data<decltype(nullptr)>::end()   { return data + sizeof(data) / sizeof(decltype(nullptr)); }
