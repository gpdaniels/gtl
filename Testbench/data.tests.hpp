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

// Boolean.
template <>
struct test_data<bool> {
    static const bool data[];
    const bool* begin(void) const;
    const bool* end(void) const;
};

// Character.
template <>
struct test_data<unsigned char> {
    static const unsigned char data[];
    const unsigned char* begin(void) const;
    const unsigned char* end(void) const;
};

template <>
struct test_data<signed char> {
    static const signed char data[];
    const signed char* begin(void) const;
    const signed char* end(void) const;
};

template <>
struct test_data<char> {
    static const char data[];
    const char* begin(void) const;
    const char* end(void) const;
};

// Extended characters.
template <>
struct test_data<wchar_t> {
    static const wchar_t data[];
    const wchar_t* begin(void) const;
    const wchar_t* end(void) const;
};

template <>
struct test_data<char16_t> {
    static const char16_t data[];
    const char16_t* begin(void) const;
    const char16_t* end(void) const;
};

template <>
struct test_data<char32_t> {
    static const char32_t data[];
    const char32_t* begin(void) const;
    const char32_t* end(void) const;
};

// Fixed point.
template <>
struct test_data<unsigned short int> {
    static const unsigned short int data[];
    const unsigned short int* begin(void) const;
    const unsigned short int* end(void) const;
};

template <>
struct test_data<signed short int> {
    static const signed short int data[];
    const signed short int* begin(void) const;
    const signed short int* end(void) const;
};

template <>
struct test_data<unsigned int> {
    static const unsigned int data[];
    const unsigned int* begin(void) const;
    const unsigned int* end(void) const;
};

template <>
struct test_data<signed int> {
    static const signed int data[];
    const signed int* begin(void) const;
    const signed int* end(void) const;
};

template <>
struct test_data<unsigned long int> {
    static const unsigned long int data[];
    const unsigned long int* begin(void) const;
    const unsigned long int* end(void) const;
};

template <>
struct test_data<signed long int> {
    static const signed long int data[];
    const signed long int* begin(void) const;
    const signed long int* end(void) const;
};

template <>
struct test_data<unsigned long long int> {
    static const unsigned long long int data[];
    const unsigned long long int* begin(void) const;
    const unsigned long long int* end(void) const;
};

template <>
struct test_data<signed long long int> {
    static const signed long long int data[];
    const signed long long int* begin(void) const;
    const signed long long int* end(void) const;
};

// Floating point.
template <>
struct test_data<float> {
    static const float data[];
    const float* begin(void) const;
    const float* end(void) const;
};

template <>
struct test_data<double> {
    static const double data[];
    const double* begin(void) const;
    const double* end(void) const;
};

template <>
struct test_data<long double> {
    static const long double data[];
    const long double* begin(void) const;
    const long double* end(void) const;
};

// Pointer type.
template <>
struct test_data<decltype(nullptr)> {
    static const decltype(nullptr) data[];
    const decltype(nullptr)* begin(void) const;
    const decltype(nullptr)* end(void) const;
};

#endif // DATA_TESTS_HPP
