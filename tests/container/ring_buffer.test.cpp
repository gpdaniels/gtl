/*
Copyright (C) 2018-2024 Geoffrey Daniels. https://gpdaniels.com/

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

#include <testbench/main.tests.hpp>

#include <testbench/comparison.tests.hpp>
#include <testbench/data.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <container/ring_buffer>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <array>
#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(ring_buffer, traits, standard) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type) -> void {
            using type = typename decltype(test_type)::type;

            REQUIRE((std::is_pod<gtl::ring_buffer<type>>::value == false), "Expected std::is_pod to be false.");

            REQUIRE((std::is_trivial<gtl::ring_buffer<type>>::value == false), "Expected std::is_trivial to be false.");

            REQUIRE((std::is_trivially_copyable<gtl::ring_buffer<type>>::value == false), "Expected std::is_trivially_copyable to be false.");

            REQUIRE((std::is_standard_layout<gtl::ring_buffer<type>>::value == true), "Expected std::is_standard_layout to be true.");
        });
}

TEST(ring_buffer, constructor, empty) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type) -> void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type> ring_buffer(value);
            testbench::do_not_optimise_away(ring_buffer);
        });
}

TEST(ring_buffer, function, empty_push_pop) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type) -> void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type> ring_buffer(value);
            REQUIRE(ring_buffer.empty());
            for (const type& data_value : testbench::test_data<type>()) {
                gtl::ring_buffer<type> ring_buffer_nd2(value);
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
        });
}

TEST(ring_buffer, function, full_push_pop) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type) -> void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type> ring_buffer(value);
            REQUIRE(!ring_buffer.full());
            for (const type& data_value : testbench::test_data<type>()) {
                gtl::ring_buffer<type> ring_buffer_nd2(value);
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
        });
}

TEST(ring_buffer, function, size_push_pop) {
    testbench::test_template<testbench::test_types, testbench::value_collection<1, 10, 100>>(
        [](auto test_type, auto value_type) -> void {
            using type = typename decltype(test_type)::type;
            using type_value = decltype(value_type);
            constexpr static const unsigned long long value = type_value::value;
            gtl::ring_buffer<type> ring_buffer(value);
            REQUIRE(ring_buffer.size() == 0);
            for (const type& data_value : testbench::test_data<type>()) {
                gtl::ring_buffer<type> ring_buffer_nd2(value);
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
        });
}

TEST(ring_buffer, evaluation, threads_x2) {
    constexpr static const unsigned int buffer_size = 5;
    constexpr static const unsigned int test_size = 1000;

    gtl::ring_buffer<unsigned int> ring_buffer(buffer_size);
    std::array<bool, test_size> have_pushed = {};
    std::array<unsigned int, test_size> have_popped = {};

    std::thread pusher = std::thread([&]() {
        for (unsigned int i = 0; i < test_size; ++i) {
            while (!ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread popper = std::thread([&]() {
        for (unsigned int i = 0; i < test_size; ++i) {
            unsigned int value;
            while (!ring_buffer.try_pop(value)) {
                std::this_thread::yield();
            }
            have_popped[i] = value;
        }
    });

    while (pusher.joinable() || popper.joinable()) {
        if (pusher.joinable())
            pusher.join();
        if (popper.joinable())
            popper.join();
    }

    for (unsigned int i = 0; i < test_size; ++i) {
        REQUIRE(have_pushed[i]);
        REQUIRE(have_popped[i] == i, "Expected '%d' but got '%d'.", i, have_popped[i]);
    }
}

TEST(ring_buffer, evaluation, threads_x4) {
    constexpr static const unsigned int buffer_size = 5;
    constexpr static const unsigned int test_size1 = 1000;
    constexpr static const unsigned int test_size2 = 1000;

    gtl::ring_buffer<unsigned int> ring_buffer(buffer_size);
    std::array<bool, test_size1 + test_size2> have_pushed = {};
    std::array<bool, test_size1 + test_size2> have_popped = {};

    std::thread pusher1 = std::thread([&]() {
        for (unsigned int i = 0; i < test_size1; ++i) {
            while (!ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread pusher2 = std::thread([&]() {
        for (unsigned int i = test_size1; i < test_size1 + test_size2; ++i) {
            while (!ring_buffer.try_push(i)) {
                std::this_thread::yield();
            }
            have_pushed[i] = true;
        }
    });

    std::thread popper1 = std::thread([&]() {
        for (unsigned int i = 0; i < test_size1; ++i) {
            unsigned int value;
            while (!ring_buffer.try_pop(value)) {
                std::this_thread::yield();
            }
            have_popped[value] = true;
        }
    });

    std::thread popper2 = std::thread([&]() {
        for (unsigned int i = test_size1; i < test_size1 + test_size2; ++i) {
            unsigned int value;
            while (!ring_buffer.try_pop(value)) {
                std::this_thread::yield();
            }
            have_popped[value] = true;
        }
    });

    while (pusher1.joinable() || pusher2.joinable() || popper1.joinable() || popper2.joinable()) {
        if (pusher1.joinable())
            pusher1.join();
        if (pusher2.joinable())
            pusher2.join();
        if (popper1.joinable())
            popper1.join();
        if (popper2.joinable())
            popper2.join();
    }

    for (unsigned int i = 0; i < test_size1 + test_size2; ++i) {
        REQUIRE(have_pushed[i], "Value '%d' was not pushed.", i);
        REQUIRE(have_popped[i], "Value '%d' was not popped.", i);
    }
}
