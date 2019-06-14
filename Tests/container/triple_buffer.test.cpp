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
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <container/triple_buffer>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            REQUIRE(sizeof(gtl::triple_buffer<type>) >= (sizeof(std::atomic<unsigned char>) + sizeof(type) * 3), "sizeof(gtl::triple_buffer<type>) = %ld, expected >= %ld", sizeof(gtl::triple_buffer<type>), (sizeof(std::atomic<unsigned char>) + sizeof(type) * 3));

            REQUIRE(std::is_pod<gtl::triple_buffer<type>>::value == false, "Expected std::is_pod to be false.");

            REQUIRE(std::is_trivial<gtl::triple_buffer<type>>::value == false, "Expected std::is_trivial to be false.");

            #if defined(__APPLE__)
                REQUIRE(std::is_trivially_copyable<gtl::triple_buffer<type>>::value == true, "Expected std::is_trivially_copyable to be true.");
            #elif defined(__linux__)
                REQUIRE(std::is_trivially_copyable<gtl::triple_buffer<type>>::value == true, "Expected std::is_trivially_copyable to be true.");
            #elif defined(_WIN32)
                REQUIRE(std::is_trivially_copyable<gtl::triple_buffer<type>>::value == false, "Expected std::is_trivially_copyable to be true.");
            #endif

            REQUIRE(std::is_standard_layout<gtl::triple_buffer<type>>::value == true, "Expected std::is_standard_layout to be true.");
        }
    );
}

TEST(constructor, empty) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            do_not_optimise_away(triple_buffer);
        }
    );
}

TEST(function, update_read) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            REQUIRE(triple_buffer.update_read() == false);
            REQUIRE(triple_buffer.update_read() == false);
            REQUIRE(triple_buffer.update_read() == false);
        }
    );
}

TEST(function, get_read) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            type* read_buffer = &triple_buffer.get_read();
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
        }
    );
}

TEST(function, get_write) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            type* write_buffer = &triple_buffer.get_write();
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
        }
    );
}

TEST(function, update_write) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            triple_buffer.update_write();
            triple_buffer.update_write();
            triple_buffer.update_write();
        }
    );
}

TEST(evaluation, buffer_progression) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;
            type* read_buffer = &triple_buffer.get_read();
            type* write_buffer = &triple_buffer.get_write();

            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);

            REQUIRE(triple_buffer.update_read() == false);

            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);

            triple_buffer.update_write();

            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_write()) != write_buffer);
            REQUIRE(&(triple_buffer.get_write()) != write_buffer);

            write_buffer = &triple_buffer.get_write();
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);

            REQUIRE(triple_buffer.update_read() == true);

            REQUIRE(&(triple_buffer.get_read()) != read_buffer);
            REQUIRE(&(triple_buffer.get_read()) != read_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);

            read_buffer = &triple_buffer.get_read();
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
        }
    );
}

TEST(evaluation, buffer_values) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            triple_buffer.get_write() = {};
            REQUIRE(triple_buffer.update_read() == false);
            triple_buffer.update_write();
            REQUIRE(triple_buffer.update_read() == true);
            REQUIRE(comparison::is_equal(triple_buffer.get_read(), type{}));

            triple_buffer.get_write() = test_data<type>::data[1];
            REQUIRE(comparison::is_equal(triple_buffer.get_read(), type{}));
            REQUIRE(triple_buffer.update_read() == false);
            REQUIRE(comparison::is_equal(triple_buffer.get_read(), type{}));
            triple_buffer.update_write();
            REQUIRE(comparison::is_equal(triple_buffer.get_read(), type{}));
            REQUIRE(triple_buffer.update_read() == true);
            REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[1]));

            for (unsigned int i = 0; i < 100; ++i) {
                triple_buffer.get_write() = test_data<type>::data[0];
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[1]));
                REQUIRE(triple_buffer.update_read() == false);
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[1]));
                triple_buffer.update_write();
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[1]));
                REQUIRE(triple_buffer.update_read() == true);
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[0]));

                triple_buffer.get_write() = test_data<type>::data[1];
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[0]));
                REQUIRE(triple_buffer.update_read() == false);
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[0]));
                triple_buffer.update_write();
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[0]));
                REQUIRE(triple_buffer.update_read() == true);
                REQUIRE(comparison::is_equal(triple_buffer.get_read(), test_data<type>::data[1]));
            }
        }
    );
}
#include <mutex>
TEST(evaluation, threads) {
    constexpr static const unsigned int test_size = 100000;

    gtl::triple_buffer<unsigned int> triple_buffer;

    std::thread writer = std::thread([&](){
        for (unsigned int i = 1; i < test_size + 1; ++i) {
            triple_buffer.get_write() = i;
            triple_buffer.update_write();
        }
    });

    std::thread reader = std::thread([&](){
        for (unsigned int i = 0; i < test_size;) {
            while (!triple_buffer.update_read());
            REQUIRE(triple_buffer.get_read() > i, "Incorrect value in triple buffer. Expected: %u > %u", triple_buffer.get_read(), i);
            i = triple_buffer.get_read();
        }
    });

    while (writer.joinable() || reader.joinable()) {
        if (writer.joinable()) writer.join();
        if (reader.joinable()) reader.join();
    }
}
