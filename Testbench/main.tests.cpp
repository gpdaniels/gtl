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

#include "main.tests.hpp"
#include "macro.tests.hpp"

TEST(SUB_TEST_GROUP, SUB_TEST_NAME);

int main(int argument_count, char* arguments[]) {
    UNUSED(argument_count);
    UNUSED(arguments);
    PRINT("Starting test...");
    TEST_CALL(SUB_TEST_GROUP, SUB_TEST_NAME);
    PRINT("Finish test, '%lld' assertions, detected '%lld' errors.", REQUIRE_COUNT, REQUIRE_FAILURE_COUNT);
    return static_cast<int>(REQUIRE_FAILURE_COUNT);
}
