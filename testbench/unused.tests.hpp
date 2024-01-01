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
#ifndef GTL_UNUSED_TESTS_HPP
#define GTL_UNUSED_TESTS_HPP

namespace testbench {

}

/// @brief  A helper macro to prevent compiler warnings about unused variables.
#define UNUSED(VARIABLE) (static_cast<void>(VARIABLE))

#endif // GTL_UNUSED_TESTS_HPP
