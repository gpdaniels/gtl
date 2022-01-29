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

    void test_node::reverse() {
        const test_node* current = test_node::root;
        const test_node *prev = nullptr;
        const test_node *next = nullptr;
        while (current != nullptr) {
            next = current->next;
            const_cast<test_node*>(current)->next = prev;
            prev = current;
            current = next;
        }
        test_node::root = prev;
    }

    const test_node* test_node::get_root() {
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
