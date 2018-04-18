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
#ifndef TEMPLATE_TESTS_HPP
#define TEMPLATE_TESTS_HPP

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

// Define a template struct to hold a collection of types.
template <typename... types>
struct type_collection { };

// Define a template struct to hold a collection of values.
template <unsigned long long... values>
struct value_collection { };

// Define a template struct to hold a function to call for each collection of types.
template <typename function_type, typename type>
struct type_function_container {
    function_type function;
    type_function_container(function_type function)
        : function(function) {
    }
    template <typename... argument_types>
    void operator()(argument_types... arguments) {
        function(type_container<type>{}, arguments...);
    }
};

// Define a template struct to hold a function to call for each collection of values.
template <typename function_type, unsigned long long value>
struct value_function_container {
    function_type function;
    value_function_container(function_type function)
        : function(function) {
    }
    template <typename... argument_types>
    void operator()(argument_types... arguments) {
        function(value_container<value>{}, arguments...);
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
void apply_collection(function_type function, value_collection<first_value, remaining_values...>, remaining_collections... collections) {
    apply_container(function, value_container<first_value>{}, collections...);
    apply_collection(function, value_collection<remaining_values...>{}, collections...);
}

// Template function to end the apply_collection template recusion for type apply_collection.
template <typename... remaining_collections, typename function_type>
void apply_collection(function_type, type_collection<>, remaining_collections...) {}

// Template function to end the apply_collection template recusion for value apply_collection.
template <typename... remaining_collections, typename function_type>
void apply_collection(function_type, value_collection<>, remaining_collections...) {}

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

#endif // TEMPLATE_TESTS_HPP
