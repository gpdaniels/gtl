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

    template <>
    void do_not_optimise_away(lambda<void()>&& function) {
        // Call function.
        function();

        // To prevent value and function being optimised away they must be used somwhere.
        // When using the value and function they must not impact the benchmark being performed.
        // Therefore, use the value and function inside a never executed if block.
        // However, compilers are smart enough that using an if(false) block is not enough.
        // An if block is required that will never execute and complex enough that the compiler cannot remove it.
        // Enter std::thread::id, the compiler cannot know that the current thread id will never match std::thread::id().
        if (check_thread_id()) {
            // Once inside the if block we must now "use" the function.
            for (unsigned long long int index = 0; index < sizeof(lambda<void()>); ++index) {
                use_character(reinterpret_cast<char*>(&function)[index]);
            }
            // To sanity check that this block of code is never reached, abort.
            testbench::abort();
        }
    }
}

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
#   pragma GCC diagnostic pop
#endif
