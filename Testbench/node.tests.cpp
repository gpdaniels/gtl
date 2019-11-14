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

#include "node.tests.hpp"

namespace testbench {
    test_node::test_node(const char* const test_file, const char* const test_group, const char* const test_name, function_type test_function)
        : file(test_file)
        , group(test_group)
        , name(test_name)
        , function(test_function)
        , next(test_node::root) {
        test_node::root = this;
    }

    test_node* test_node::get_root() {
        return test_node::root;
    }

    const char* test_node::get_file() const {
        return this->file;
    }

    const char* test_node::get_group() const {
        return this->group;
    }

    const char* test_node::get_name() const {
        return this->name;
    }

    test_node::function_type test_node::get_function() const {
        return this->function;
    }

    const test_node* test_node::get_next() const {
        return this->next;
    }
}
