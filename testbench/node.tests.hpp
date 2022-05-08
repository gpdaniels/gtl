/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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
