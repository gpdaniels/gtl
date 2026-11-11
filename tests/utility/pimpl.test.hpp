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
#ifndef GTL_UTILITY_PIMPL_TEST_HPP
#define GTL_UTILITY_PIMPL_TEST_HPP

#include <utility/pimpl>

class test_pimpl
    : private gtl::pimpl<test_pimpl, 4, alignof(int)> {
public:
    ~test_pimpl();
    test_pimpl(int value);
    int return_value() const;
};

class test_pimpl_tracked
    : private gtl::pimpl<test_pimpl_tracked, sizeof(int*), alignof(int*)> {
public:
    ~test_pimpl_tracked();
    test_pimpl_tracked(int value);
    test_pimpl_tracked(const test_pimpl_tracked& other);
    test_pimpl_tracked(test_pimpl_tracked&& other);
    test_pimpl_tracked& operator=(const test_pimpl_tracked& other);
    test_pimpl_tracked& operator=(test_pimpl_tracked&& other);
    int return_value() const;
};

#endif // GTL_UTILITY_PIMPL_TEST_HPP
