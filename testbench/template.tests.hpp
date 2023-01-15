/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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
#ifndef GTL_TEMPLATE_TESTS_HPP
#define GTL_TEMPLATE_TESTS_HPP

namespace testbench {
    // For testing templates a method of storing types and looping over them is required.
    // Below is an implementation of a foreach type loop.

    // Define a template struct that holds a type, enables easier use of voids.
    template <typename contained_type>
    struct type_container {
        using type = contained_type;
    };

    // Define a template struct that holds a value.
    template <unsigned long long contained_value>
    struct value_container {
        constexpr static const unsigned long long value = contained_value;
    };

    // Define a template struct that holds an enum value.
    template <typename contained_enum_type, contained_enum_type contained_enum_value>
    struct enum_container {
        using type = contained_enum_type;
        constexpr static const contained_enum_type value = contained_enum_value;
    };

    // Define a template struct to hold a collection of types.
    template <typename... types>
    struct type_collection { };

    // Define a template struct to hold a collection of values.
    template <unsigned long long... values>
    struct value_collection { };

    // Define a template struct to hold a collection of enum values.
    template <typename contained_enum_type, contained_enum_type... contained_enum_value>
    struct enum_collection { };

    // Define a template struct to hold a function to call for each collection of types.
    template <typename function_type, typename type>
    struct type_function_container {
        function_type type_function;
        type_function_container(function_type function)
            : type_function(function) {
        }
        type_function_container(const type_function_container&) = default;
        type_function_container(type_function_container&&) = default;
        type_function_container& operator=(const type_function_container&) = delete;
        type_function_container& operator=(type_function_container&&) = delete;
        template <typename... argument_types>
        void operator()(argument_types... arguments) {
            type_function(type_container<type>{}, arguments...);
        }
    };

    // Define a template struct to hold a function to call for each collection of values.
    template <typename function_type, unsigned long long value>
    struct value_function_container {
        function_type value_function;
        value_function_container(function_type function)
            : value_function(function) {
        }
        value_function_container(const value_function_container&) = default;
        value_function_container(value_function_container&&) = default;
        value_function_container& operator=(const value_function_container&) = delete;
        value_function_container& operator=(value_function_container&&) = delete;
        template <typename... argument_types>
        void operator()(argument_types... arguments) {
            value_function(value_container<value>{}, arguments...);
        }
    };

    // Define a template struct to hold a function to call for each collection of enum values.
    template <typename function_type, typename enum_type, enum_type value>
    struct enum_function_container {
        function_type enum_function;
        enum_function_container(function_type function)
            : enum_function(function) {
        }
        enum_function_container(const enum_function_container&) = default;
        enum_function_container(enum_function_container&&) = default;
        enum_function_container& operator=(const enum_function_container&) = delete;
        enum_function_container& operator=(enum_function_container&&) = delete;
        template <typename... argument_types>
        void operator()(argument_types... arguments) {
            enum_function(enum_container<enum_type, value>{}, arguments...);
        }
    };

    // Apply a type collection to the function.
    template <typename first_type, typename... remaining_types, typename... remaining_collections, typename function_type>
    void apply_collection(function_type function, type_collection<first_type, remaining_types...>, remaining_collections... collections) {
        apply_container(function, type_container<first_type>{}, collections...);
        apply_collection(function, type_collection<remaining_types...>{}, collections...);
    }

    // Apply a value collection to the function.
    template <unsigned long long first_value, unsigned long long... remaining_values, typename... remaining_collections, typename function_type>
    void apply_collection(function_type function, testbench::value_collection<first_value, remaining_values...>, remaining_collections... collections) {
        apply_container(function, value_container<first_value>{}, collections...);
        apply_collection(function, testbench::value_collection<remaining_values...>{}, collections...);
    }

    // Apply an enum value collection to the function.
    template <typename enum_type, enum_type first_value, enum_type... remaining_values, typename... remaining_collections, typename function_type>
    void apply_collection(function_type function, testbench::enum_collection<enum_type, first_value, remaining_values...>, remaining_collections... collections) {
        apply_container(function, enum_container<enum_type, first_value>{}, collections...);
        apply_collection(function, testbench::enum_collection<enum_type, remaining_values...>{}, collections...);
    }

    // Template function to end the apply_collection template recusion for type apply_collection.
    template <typename... remaining_collections, typename function_type>
    void apply_collection(function_type, type_collection<>, remaining_collections...) {}

    // Template function to end the apply_collection template recusion for value apply_collection.
    template <typename... remaining_collections, typename function_type>
    void apply_collection(function_type, testbench::value_collection<>, remaining_collections...) {}

    // Template function to end the apply_collection template recusion for enum value apply_collection.
    template <typename enum_type, typename... remaining_collections, typename function_type>
    void apply_collection(function_type, testbench::enum_collection<enum_type>, remaining_collections...) {}

    // Apply a type followed by other collections to the function.
    template <typename type, typename first_collection, typename... remaining_collections, typename function_type>
    void apply_container(function_type function, type_container<type>, first_collection collection, remaining_collections... collections) {
        type_function_container<function_type, type> forwarder(function);
        apply_collection(forwarder, collection, collections...);
    }

    // Apply a value followed by other collections to the function.
    template <unsigned long long value, typename first_collection, typename... remaining_collections, typename function_type>
    void apply_container(function_type function, value_container<value>, first_collection collection, remaining_collections... collections) {
        value_function_container<function_type, value> forwarder(function);
        apply_collection(forwarder, collection, collections...);
    }

    // Apply an enum value followed by other collections to the function.
    template <typename enum_type, enum_type value, typename first_collection, typename... remaining_collections, typename function_type>
    void apply_container(function_type function, enum_container<enum_type, value>, first_collection collection, remaining_collections... collections) {
        enum_function_container<function_type, enum_type, value> forwarder(function);
        apply_collection(forwarder, collection, collections...);
    }

    // Call the function with a type.
    template <typename contained_type, typename function_type>
    void apply_container(function_type function, type_container<contained_type> type) {
        function(type);
    }

    // Call the function with a value.
    template <unsigned long long contained_value, typename function_type>
    void apply_container(function_type function, value_container<contained_value> value) {
        function(value);
    }

    // Call the function with an enum value.
    template <typename enum_type, enum_type contained_value, typename function_type>
    void apply_container(function_type function, enum_container<enum_type, contained_value> value) {
        function(value);
    }

    // Helper function to setup tests.
    template <typename first_collection, typename... remaining_collections, typename function_type>
    void test_template(function_type function) {
        apply_collection(function, first_collection{}, remaining_collections{}...);
    }

    // Builtin types.
    using test_types = type_collection<
        bool,
        unsigned char, char, signed char, wchar_t, char16_t, char32_t,
        unsigned short int, signed short int, unsigned int, signed int, unsigned long int, signed long int, unsigned long long int, signed long long int,
        float, double, long double,
        decltype(nullptr)
    >;
}

#endif // GTL_TEMPLATE_TESTS_HPP
