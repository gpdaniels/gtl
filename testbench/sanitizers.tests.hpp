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
#ifndef GTL_SANITIZERS_TESTS_HPP
#define GTL_SANITIZERS_TESTS_HPP

namespace testbench {

}

#if !defined(GTL_HAS_SANITIZER_ADDRESS)
#   if defined(__has_feature)
#      if __has_feature(address_sanitizer)
#          define GTL_HAS_SANITIZER_ADDRESS 1
#      else
#          define GTL_HAS_SANITIZER_ADDRESS 0
#      endif
#   else
#      define GTL_HAS_SANITIZER_ADDRESS 0
#   endif
#endif

#if !defined(GTL_HAS_SANITIZER_MEMORY)
#   if defined(__has_feature)
#      if __has_feature(memory_sanitizer)
#          define GTL_HAS_SANITIZER_MEMORY 1
#      else
#          define GTL_HAS_SANITIZER_MEMORY 0
#      endif
#   else
#      define GTL_HAS_SANITIZER_MEMORY 0
#   endif
#endif

#if !defined(GTL_HAS_SANITIZER_THREAD)
#   if defined(__has_feature)
#       if __has_feature(thread_sanitizer)
#           define GTL_HAS_SANITIZER_THREAD 1
#       else
#           define GTL_HAS_SANITIZER_THREAD 0
#       endif
#   else
#      define GTL_HAS_SANITIZER_THREAD 0
#   endif
#endif

#if !defined(GTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR)
#   if defined(__has_feature)
#       if __has_feature(undefined_behavior_sanitizer)
#           define GTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR 1
#       else
#           define GTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR 0
#       endif
#   else
#      define GTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR 0
#   endif
#endif

#endif // GTL_SANITIZERS_TESTS_HPP
