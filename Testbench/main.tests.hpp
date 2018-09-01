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
#ifndef MAIN_TESTS_HPP
#define MAIN_TESTS_HPP

// Macros to expand, call, and uniquely name all the tests in the test file.
// These are defined in the main tests header as they are the minumum required to run tests.
#define TEST_EXPAND(GROUP_NAME, TEST_NAME)  TEST_##GROUP_NAME##_##TEST_NAME
#define TEST_CALL(GROUP_NAME, TEST_NAME)    TEST_EXPAND(GROUP_NAME, TEST_NAME)()
#define TEST(GROUP_NAME, TEST_NAME)         void TEST_EXPAND(GROUP_NAME, TEST_NAME)(void)

// Declare the main function.
int main(int argument_count, char* arguments[]);

#endif // MAIN_TESTS_HPP
