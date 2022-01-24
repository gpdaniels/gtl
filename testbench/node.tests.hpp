/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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
#ifndef GTL_NODE_TESTS_HPP
#define GTL_NODE_TESTS_HPP

namespace testbench {
    // Define the test_node type, used to create a linked list of tests at program startup.
    class test_node final {
    public:
        using function_type = void(*)();

    private:
        static inline const test_node* root = nullptr;

        const char* const file;
        const char* const group;
        const char* const name;

        const function_type function;

        const test_node* next;

    public:
        test_node(const char* const test_file, const char* const test_group, const char* const test_name, function_type test_function);

    public:
        static void reverse();

        static const test_node* get_root();

        const char* get_file() const;

        const char* get_group() const;

        const char* get_name() const;

        function_type get_function() const;

        const test_node* get_next() const;
    };
}

#endif // GTL_NODE_TESTS_HPP
