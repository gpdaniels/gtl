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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <data.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <execution/triple_buffer>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <thread>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(triple_buffer, traits, standard) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

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

TEST(triple_buffer, constructor, empty) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            testbench::do_not_optimise_away(triple_buffer);
        }
    );
}

TEST(triple_buffer, function, update_read) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            REQUIRE(triple_buffer.update_read() == false);
            REQUIRE(triple_buffer.update_read() == false);
            REQUIRE(triple_buffer.update_read() == false);
        }
    );
}

TEST(triple_buffer, function, get_read) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            type* read_buffer = &triple_buffer.get_read();
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
            REQUIRE(&(triple_buffer.get_read()) == read_buffer);
        }
    );
}

TEST(triple_buffer, function, get_write) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            type* write_buffer = &triple_buffer.get_write();
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
            REQUIRE(&(triple_buffer.get_write()) == write_buffer);
        }
    );
}

TEST(triple_buffer, function, update_write) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            triple_buffer.update_write();
            triple_buffer.update_write();
            triple_buffer.update_write();
        }
    );
}

TEST(triple_buffer, evaluation, buffer_progression) {
    testbench::test_template<testbench::test_types>(
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

TEST(triple_buffer, evaluation, buffer_values) {
    testbench::test_template<testbench::test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            gtl::triple_buffer<type> triple_buffer;

            triple_buffer.get_write() = {};
            REQUIRE(triple_buffer.update_read() == false);
            triple_buffer.update_write();
            REQUIRE(triple_buffer.update_read() == true);
            REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), type{}));

            triple_buffer.get_write() = testbench::test_data<type>::data[1];
            REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), type{}));
            REQUIRE(triple_buffer.update_read() == false);
            REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), type{}));
            triple_buffer.update_write();
            REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), type{}));
            REQUIRE(triple_buffer.update_read() == true);
            REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[1]));

            for (unsigned int i = 0; i < 100; ++i) {
                triple_buffer.get_write() = testbench::test_data<type>::data[0];
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[1]));
                REQUIRE(triple_buffer.update_read() == false);
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[1]));
                triple_buffer.update_write();
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[1]));
                REQUIRE(triple_buffer.update_read() == true);
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[0]));

                triple_buffer.get_write() = testbench::test_data<type>::data[1];
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[0]));
                REQUIRE(triple_buffer.update_read() == false);
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[0]));
                triple_buffer.update_write();
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[0]));
                REQUIRE(triple_buffer.update_read() == true);
                REQUIRE(testbench::is_value_equal(triple_buffer.get_read(), testbench::test_data<type>::data[1]));
            }
        }
    );
}
#include <mutex>
TEST(triple_buffer, evaluation, threads) {
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
