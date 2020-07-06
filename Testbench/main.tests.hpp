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
#ifndef GTL_MAIN_TESTS_HPP
#define GTL_MAIN_TESTS_HPP

// These macros are defined in the main tests header as they are the minumum required to run tests.
// They require the test_node type.

#include "node.tests.hpp"

// Macro to expand a test name.
#define TEST_EXPAND_TEST(TEST_FILE, TEST_GROUP, TEST_NAME) test_##TEST_FILE##_##TEST_GROUP##_##TEST_NAME
#define TEST_EXPAND_NODE(TEST_FILE, TEST_GROUP, TEST_NAME) node_##TEST_FILE##_##TEST_GROUP##_##TEST_NAME

// Macro to declare a test.
#define TEST(TEST_FILE, TEST_GROUP, TEST_NAME)                                                      \
    void TEST_EXPAND_TEST(TEST_FILE, TEST_GROUP, TEST_NAME)();                                      \
    static inline testbench::test_node TEST_EXPAND_NODE(TEST_FILE, TEST_GROUP, TEST_NAME)           \
        (#TEST_FILE, #TEST_GROUP, #TEST_NAME, TEST_EXPAND_TEST(TEST_FILE, TEST_GROUP, TEST_NAME));  \
    void TEST_EXPAND_TEST(TEST_FILE, TEST_GROUP, TEST_NAME)()

// This test macro can be disabled in a test file using the following:
// #pragma push_macro("TEST")
// #undef TEST
// #define TEST(TEST_FILE, TEST_GROUP, TEST_NAME) void TEST_EXPAND_TEST(TEST_FILE, TEST_GROUP, TEST_NAME)()
// ...
// #pragma pop_macro("TEST")

// Declare the main function.
int main(int argument_count, char* arguments[]);

#endif // GTL_MAIN_TESTS_HPP
