/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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
#ifndef GTL_LAMBDA_TESTS_HPP
#define GTL_LAMBDA_TESTS_HPP

namespace testbench {
    template <typename function_type>
    class lambda;

    template <typename return_type, typename... argument_types>
    class lambda<return_type(argument_types...)> final {
    private:
        void* function;
        return_type (*executor)(const void*, argument_types...);
        void (*destructor)(void*&);

    public:
        ~lambda() {
            if (this->destructor) {
                this->destructor(this->function);
            }
        }

    public:
        template <typename function_type>
        constexpr lambda(const function_type& raw_function)
            : function(new function_type(*static_cast<function_type*>(const_cast<void*>(&raw_function))))
            , executor([](const void* function_pointer, argument_types... arguments) -> return_type {
                return static_cast<const function_type*>(function_pointer)->operator()(arguments...);
            })
            , destructor([](void*& function_pointer) -> void {
                delete static_cast<function_type*>(function_pointer);
                function_pointer = nullptr;
            }) {
        }

    private:
        lambda(const lambda& other) = delete;
        lambda(lambda&& other) = delete;
        lambda& operator=(const lambda& other) = delete;
        lambda& operator=(lambda&& other) = delete;

    public:
        constexpr return_type operator()(argument_types... arguments) const {
            return this->executor(this->function, arguments...);
        }
    };
}

#endif // GTL_LAMBDA_TESTS_HPP
