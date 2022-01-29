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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <data.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <container/static_ring_buffer>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <array>
#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(static_ring_buffer, traits, standard) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;

            REQUIRE((std::is_pod<gtl::static_ring_buffer<type, value> >::value == false), "Expected std::is_pod to be false.");

            REQUIRE((std::is_trivial<gtl::static_ring_buffer<type, value> >::value == false), "Expected std::is_trivial to be false.");

            #if defined(__clang__)
                REQUIRE((std::is_trivially_copyable<gtl::static_ring_buffer<type, value> >::value == true), "Expected std::is_trivially_copyable to be true.");
            #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
                REQUIRE((std::is_trivially_copyable<gtl::static_ring_buffer<type, value> >::value == true), "Expected std::is_trivially_copyable to be true.");
            #elif defined(_MSC_VER)
                REQUIRE((std::is_trivially_copyable<gtl::static_ring_buffer<type, value> >::value == false), "Expected std::is_trivially_copyable to be false.");
            #endif

            REQUIRE((std::is_standard_layout<gtl::static_ring_buffer<type, value> >::value == true), "Expected std::is_standard_layout to be true.");
        }
    );
}

TEST(static_ring_buffer, constructor, empty) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::static_ring_buffer<type, value> static_ring_buffer;
            testbench::do_not_optimise_away(static_ring_buffer);
        }
    );
}

TEST(static_ring_buffer, function, empty_push_pop) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::static_ring_buffer<type, value> static_ring_buffer;
            REQUIRE(static_ring_buffer.empty());
            for (const type& data_value : testbench::test_data<type>()) {
                gtl::static_ring_buffer<type, value> static_ring_buffer_nd2;
                REQUIRE(static_ring_buffer_nd2.empty());
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(static_ring_buffer_nd2.try_push(data_value));
                    REQUIRE(!static_ring_buffer_nd2.empty());
                }
                REQUIRE(static_ring_buffer_nd2.try_push(data_value));
                REQUIRE(!static_ring_buffer_nd2.empty());
                type output_value;
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(static_ring_buffer_nd2.try_pop(output_value));
                    REQUIRE(!static_ring_buffer_nd2.empty());
                }
                REQUIRE(static_ring_buffer_nd2.try_pop(output_value));
                REQUIRE(static_ring_buffer_nd2.empty());
            }
        }
    );
}

TEST(static_ring_buffer, function, full_push_pop) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::static_ring_buffer<type, value> static_ring_buffer;
            REQUIRE(!static_ring_buffer.full());
            for (const type& data_value : testbench::test_data<type>()) {
                gtl::static_ring_buffer<type, value> static_ring_buffer_nd2;
                REQUIRE(!static_ring_buffer_nd2.full());
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(static_ring_buffer_nd2.try_push(data_value));
                    REQUIRE(!static_ring_buffer_nd2.full());
                }
                REQUIRE(static_ring_buffer_nd2.try_push(data_value));
                REQUIRE(static_ring_buffer_nd2.full());
                type output_value;
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(static_ring_buffer_nd2.try_pop(output_value));
                    REQUIRE(!static_ring_buffer_nd2.full());
                }
                REQUIRE(static_ring_buffer_nd2.try_pop(output_value));
                REQUIRE(!static_ring_buffer_nd2.full());
            }
        }
    );
}

TEST(static_ring_buffer, function, size_push_pop) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::static_ring_buffer<type, value> static_ring_buffer;
            REQUIRE(static_ring_buffer.size() == 0);
            for (const type& data_value : testbench::test_data<type>()) {
                gtl::static_ring_buffer<type, value> static_ring_buffer_nd2;
                REQUIRE(static_ring_buffer_nd2.size() == 0);
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(static_ring_buffer_nd2.try_push(data_value));
                    REQUIRE(static_ring_buffer_nd2.size() == i + 1);
                }
                REQUIRE(static_ring_buffer_nd2.try_push(data_value));
                REQUIRE(static_ring_buffer_nd2.size() == value);
                type output_value;
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(static_ring_buffer_nd2.try_pop(output_value));
                    REQUIRE(static_ring_buffer_nd2.size() == value - i - 1);
                }
                REQUIRE(static_ring_buffer_nd2.try_pop(output_value));
                REQUIRE(static_ring_buffer_nd2.size() == 0);
            }
        }
    );
}

TEST(static_ring_buffer, evaluation, threads_x2) {
    constexpr static const unsigned int buffer_size = 5;
    constexpr static const unsigned int test_size = 1000;

    gtl::static_ring_buffer<unsigned int, buffer_size> static_ring_buffer;
    std::array<bool, test_size> have_pushed = {};
    std::array<unsigned int, test_size> have_popped = {};

    std::thread pusher = std::thread([&](){
        for (unsigned int i = 0; i < test_size; ++i) {
            while (!static_ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread popper = std::thread([&](){
        for (unsigned int i = 0; i < test_size; ++i) {
            unsigned int value;
            while (!static_ring_buffer.try_pop(value)) {
                std::this_thread::yield();
            }
            have_popped[i] = value;
        }
    });

    while (pusher.joinable() || popper.joinable()) {
        if (pusher.joinable()) pusher.join();
        if (popper.joinable()) popper.join();
    }

    for (unsigned int i = 0; i < test_size; ++i) {
        REQUIRE(have_pushed[i]);
        REQUIRE(have_popped[i] == i, "Expected '%d' but got '%d'.", i, have_popped[i]);
    }
}

TEST(static_ring_buffer, evaluation, threads_x4) {
    constexpr static const unsigned int buffer_size = 5;
    constexpr static const unsigned int test_size1 = 1000;
    constexpr static const unsigned int test_size2 = 1000;

    gtl::static_ring_buffer<unsigned int, buffer_size> static_ring_buffer;
    std::array<bool, test_size1 + test_size2> have_pushed = {};
    std::array<bool, test_size1 + test_size2> have_popped = {};

    std::thread pusher1 = std::thread([&](){
        for (unsigned int i = 0; i < test_size1; ++i) {
            while (!static_ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread pusher2 = std::thread([&](){
        for (unsigned int i = test_size1; i < test_size1 + test_size2; ++i) {
            while (!static_ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread popper1 = std::thread([&](){
        for (unsigned int i = 0; i < test_size1; ++i) {
            unsigned int value;
            while (!static_ring_buffer.try_pop(value)) {
                std::this_thread::yield();
            }
            have_popped[value] = true;
        }
    });


    std::thread popper2 = std::thread([&](){
        for (unsigned int i = test_size1; i < test_size1 + test_size2; ++i) {
            unsigned int value;
            while (!static_ring_buffer.try_pop(value)) {
                std::this_thread::yield();
            }
            have_popped[value] = true;
        }
    });

    while (pusher1.joinable() || pusher2.joinable() || popper1.joinable() || popper2.joinable() ) {
        if (pusher1.joinable()) pusher1.join();
        if (pusher2.joinable()) pusher2.join();
        if (popper1.joinable()) popper1.join();
        if (popper2.joinable()) popper2.join();
    }

    for (unsigned int i = 0; i < test_size1 + test_size2; ++i) {
        REQUIRE(have_pushed[i], "Value '%d' was not pushed.", i);
        REQUIRE(have_popped[i], "Value '%d' was not popped.", i);
    }
}
