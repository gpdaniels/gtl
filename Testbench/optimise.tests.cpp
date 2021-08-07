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

#include "optimise.tests.hpp"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <functional>
#include <thread>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

namespace testbench {
    bool check_thread_id() {
        static std::thread::id thread_id = std::this_thread::get_id();
        return thread_id == std::thread::id();
    }

    void use_character(char character) {
        putchar(character);
    }
}

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#   pragma GCC diagnostic pop
#endif
