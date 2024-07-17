/*
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

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

    public:
        static inline test_node* root = nullptr;

    public:
        const char* file;
        const char* group;
        const char* name;
        function_type function;
        test_node* next;
    };
}

#endif // GTL_NODE_TESTS_HPP
