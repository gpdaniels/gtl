/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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
    static const inline testbench::test_node TEST_EXPAND_NODE(TEST_FILE, TEST_GROUP, TEST_NAME)     \
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
