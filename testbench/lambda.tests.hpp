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
        /// @brief  A template to remove references from types.
        template<typename type>
        struct remove_reference final {
            using type_unreferenced = type;
        };

        /// @brief  A template to remove references from types.
        template<typename type>
        struct remove_reference<type&> final {
            using type_unreferenced = type;
        };

        /// @brief  A template to remove references from types.
        template<typename type>
        struct remove_reference<type&&> final {
            using type_unreferenced = type;
        };

        /// @brief  A template to check if two types are the same.
        template<typename lhs_type, typename rhs_type>
        struct is_same_type final {
            constexpr static const bool value = false;
        };

        /// @brief  A template to check if two types are the same.
        template<typename type>
        struct is_same_type<type, type> final {
            constexpr static const bool value = true;
        };

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

        constexpr lambda()
            : function(nullptr)
            , executor(nullptr)
            , destructor(nullptr) {
        }

    private:
        constexpr static void swap(lambda& lhs, lambda& rhs) {
            void* function = lhs.function;
            lhs.function = rhs.function;
            rhs.function = function;

            return_type (*executor)(const void*, argument_types...) = lhs.executor;
            lhs.executor = rhs.executor;
            rhs.executor = executor;

            void (*destructor)(void*&) = lhs.destructor;
            lhs.destructor = rhs.destructor;
            rhs.destructor = destructor;
        }

    public:
        template <typename function_type>
        constexpr lambda(const function_type& raw_function)
            : lambda() {
            // First get real function type.
            using real_function_type = decltype(raw_function);
            using pure_function_type = typename remove_reference<real_function_type>::type_unreferenced;
            // If input is already a lambda type, cast can call the appropriate constructor.
            constexpr bool is_lambda = is_same_type<lambda, pure_function_type>::value;
            if constexpr (is_lambda) {
                constexpr bool is_move = is_same_type<real_function_type, pure_function_type&&>::value;
                if constexpr (is_move) {
                    lambda other(static_cast<lambda&&>(raw_function));
                    lambda::swap(*this, other);
                }
                else {
                    lambda other(static_cast<const lambda&>(raw_function));
                    lambda::swap(*this, other);
                }
            }
            else {
                this->executor = [](const void* function_pointer, argument_types... arguments) -> return_type {
                    return static_cast<const pure_function_type*>(function_pointer)->operator()(arguments...);
                };
                this->destructor = [](void*& function_pointer) -> void {
                    delete static_cast<pure_function_type*>(function_pointer);
                    function_pointer = nullptr;
                };
            }
        }

    private:
        lambda(const lambda& other) = delete;
        lambda& operator=(const lambda& other) = delete;

    public:
        constexpr return_type operator()(argument_types... arguments) const {
            return this->executor(this->function, arguments...);
        }
    };
}

#endif // GTL_LAMBDA_TESTS_HPP
