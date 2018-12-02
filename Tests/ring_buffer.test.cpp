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
#include <benchmark.tests.hpp>
#include <comparison.tests.hpp>
#include <data.tests.hpp>
#include <macro.tests.hpp>
#include <template.tests.hpp>

#include <ring_buffer>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <array>
#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    test_template<test_types, value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            constexpr static const unsigned long long expected_size = sizeof(type) * value + sizeof(std::atomic<unsigned long long int>) * 2;
            REQUIRE(sizeof(gtl::ring_buffer<type, value>) >= expected_size, "sizeof(gtl::ring_buffer<type, value>) = %ld, expected >= %lld", sizeof(gtl::ring_buffer<type, value>), expected_size);
            REQUIRE((std::is_pod<gtl::ring_buffer<type, value> >::value == false), "Expected std::is_pod to be false.");
            REQUIRE((std::is_trivial<gtl::ring_buffer<type, value> >::value == false), "Expected std::is_trivial to be false.");

            #if defined(__clang__)
                REQUIRE((std::is_trivially_copyable<gtl::ring_buffer<type, value> >::value == true), "Expected std::is_trivially_copyable to be true.");
            #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER))
                REQUIRE((std::is_trivially_copyable<gtl::ring_buffer<type, value> >::value == true), "Expected std::is_trivially_copyable to be true.");
            #elif defined(_MSC_VER)
                REQUIRE((std::is_trivially_copyable<gtl::ring_buffer<type, value> >::value == false), "Expected std::is_trivially_copyable to be false.");
            #endif

            REQUIRE((std::is_standard_layout<gtl::ring_buffer<type, value> >::value == true), "Expected std::is_standard_layout to be true.");
        }
    );
}

TEST(constructor, empty) {
    test_template<test_types, value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type, value> ring_buffer;
            do_not_optimise_away(ring_buffer);
        }
    );
}

TEST(function, empty_push_pop) {
    test_template<test_types, value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type, value> ring_buffer;
            REQUIRE(ring_buffer.empty());
            for (const type& data_value : test_data<type>()) {
                gtl::ring_buffer<type, value> ring_buffer_nd2;
                REQUIRE(ring_buffer_nd2.empty());
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(ring_buffer_nd2.try_push(data_value));
                    REQUIRE(!ring_buffer_nd2.empty());
                }
                REQUIRE(ring_buffer_nd2.try_push(data_value));
                REQUIRE(!ring_buffer_nd2.empty());
                type output_value;
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(ring_buffer_nd2.try_pop(output_value));
                    REQUIRE(!ring_buffer_nd2.empty());
                }
                REQUIRE(ring_buffer_nd2.try_pop(output_value));
                REQUIRE(ring_buffer_nd2.empty());
            }
        }
    );
}

TEST(function, full_push_pop) {
    test_template<test_types, value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type, value> ring_buffer;
            REQUIRE(!ring_buffer.full());
            for (const type& data_value : test_data<type>()) {
                gtl::ring_buffer<type, value> ring_buffer_nd2;
                REQUIRE(!ring_buffer_nd2.full());
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(ring_buffer_nd2.try_push(data_value));
                    REQUIRE(!ring_buffer_nd2.full());
                }
                REQUIRE(ring_buffer_nd2.try_push(data_value));
                REQUIRE(ring_buffer_nd2.full());
                type output_value;
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(ring_buffer_nd2.try_pop(output_value));
                    REQUIRE(!ring_buffer_nd2.full());
                }
                REQUIRE(ring_buffer_nd2.try_pop(output_value));
                REQUIRE(!ring_buffer_nd2.full());
            }
        }
    );
}

TEST(function, size_push_pop) {
    test_template<test_types, value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type)->void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type, value> ring_buffer;
            REQUIRE(ring_buffer.size() == 0);
            for (const type& data_value : test_data<type>()) {
                gtl::ring_buffer<type, value> ring_buffer_nd2;
                REQUIRE(ring_buffer_nd2.size() == 0);
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(ring_buffer_nd2.try_push(data_value));
                    REQUIRE(ring_buffer_nd2.size() == i + 1);
                }
                REQUIRE(ring_buffer_nd2.try_push(data_value));
                REQUIRE(ring_buffer_nd2.size() == value);
                type output_value;
                for (unsigned long long i = 0; i < value - 1; ++i) {
                    REQUIRE(ring_buffer_nd2.try_pop(output_value));
                    REQUIRE(ring_buffer_nd2.size() == value - i - 1);
                }
                REQUIRE(ring_buffer_nd2.try_pop(output_value));
                REQUIRE(ring_buffer_nd2.size() == 0);
            }
        }
    );
}

TEST(evaluation, threads) {
    constexpr static const unsigned int buffer_size = 5;
    constexpr static const unsigned int test_size = 1000;

    gtl::ring_buffer<unsigned int, buffer_size> ring_buffer;
    std::array<bool, test_size> have_pushed;
    std::array<unsigned int, test_size> have_popped;

    std::thread pusher = std::thread([&](){
        for (unsigned int i = 0; i < test_size; ++i) {
            while (!ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread popper = std::thread([&](){
        for (unsigned int i = 0; i < test_size; ++i) {
            unsigned int value;
            while (!ring_buffer.try_pop(value)) {
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
