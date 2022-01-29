/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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
        constexpr operator bool() const {
            return (this->function != nullptr);
        }

    public:
        constexpr return_type operator()(argument_types... arguments) const {
            return this->executor(this->function, arguments...);
        }
    };
}

#endif // GTL_LAMBDA_TESTS_HPP
