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

#include <functional>
#include <thread>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER)) && (!defined(__clang__))
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

namespace testbench {
    template <typename member_type, member_type member, typename connection_id>
    class private_connection final {
        friend member_type get_member(connection_id) {
            return member;
        }
    };

    template <typename class_type, typename return_type>
    struct private_target final {
        using member_type = return_type class_type::*;
        using connection_type = return_type&;
    };

    struct private_connection_to_M_thread final {};
    private_target<std::thread::id, std::thread::native_handle_type>::member_type get_member(private_connection_to_M_thread);
    template class private_connection<private_target<std::thread::id, std::thread::native_handle_type>::member_type, &std::thread::id::_M_thread, private_connection_to_M_thread>;

    unsigned long long int get_thread_id() {
        std::thread::id thread_id = std::this_thread::get_id();
        private_target<std::thread::id, std::thread::native_handle_type>::connection_type _M_thread = &thread_id->*get_member(private_connection_to_M_thread{});
        return static_cast<unsigned long long int>(_M_thread);
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
        static unsigned long long int thread_id = get_thread_id();
        if (thread_id == 0xFFFFFFFFFFFFFFFF) {
            // Once inside the if block we must now "use" the function.
            for (unsigned long long int index = 0; index < sizeof(lambda<void()>); ++index) {
                putchar(reinterpret_cast<char*>(&function)[index]);
            }
            // To sanity check that this block of code is never reached, abort.
            testbench::abort();
        }
    }
}

#if (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER)) && (!defined(__clang__))
#   pragma GCC diagnostic pop
#endif
