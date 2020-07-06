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
