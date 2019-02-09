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
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <triple_buffer>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    test_template<test_types>(
       [](auto test_type)->void {
            using type = typename decltype(test_type)::type;

            REQUIRE(sizeof(gtl::triple_buffer<type>) >= 16, "sizeof(gtl::triple_buffer<type>) = %ld, expected >= %lld", sizeof(gtl::triple_buffer<type>), 16ull);

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
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    do_not_optimise_away(triple_buffer);
}

TEST(function, update_read) {
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    REQUIRE(triple_buffer.update_read() == false);
    REQUIRE(triple_buffer.update_read() == false);
    REQUIRE(triple_buffer.update_read() == false);
}

TEST(function, get_read) {
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
}

TEST(function, get_write) {
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
}

TEST(function, update_write) {
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    triple_buffer.update_write();
    triple_buffer.update_write();
    triple_buffer.update_write();
}

TEST(evaluation, buffer_progression) {
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);

    REQUIRE(triple_buffer.update_read() == false);

    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);

    triple_buffer.update_write();

    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);

    REQUIRE(triple_buffer.update_read() == true);

    REQUIRE(&(triple_buffer.get_read()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
}

TEST(evaluation, buffer_values) {
    int buffers[3] = { 0, 0, 0 };
    gtl::triple_buffer<int> triple_buffer(buffers[0], buffers[1], buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);

    REQUIRE(triple_buffer.get_read() == 0);
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 0);
    REQUIRE(buffers[2] == 0);
    triple_buffer.get_write() = 1;
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 0);
    REQUIRE(buffers[2] == 1);
    REQUIRE(triple_buffer.get_read() == 0);

    REQUIRE(triple_buffer.update_read() == false);

    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[2]);

    REQUIRE(triple_buffer.get_read() == 0);
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 0);
    REQUIRE(buffers[2] == 1);
    triple_buffer.get_write() = 2;
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 0);
    REQUIRE(buffers[2] == 2);
    REQUIRE(triple_buffer.get_read() == 0);

    triple_buffer.update_write();

    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[0]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);

    REQUIRE(triple_buffer.get_read() == 0);
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 0);
    REQUIRE(buffers[2] == 2);
    triple_buffer.get_write() = 3;
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 3);
    REQUIRE(buffers[2] == 2);
    REQUIRE(triple_buffer.get_read() == 0);

    REQUIRE(triple_buffer.update_read() == true);

    REQUIRE(&(triple_buffer.get_read()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_read()) == &buffers[2]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);
    REQUIRE(&(triple_buffer.get_write()) == &buffers[1]);

    REQUIRE(triple_buffer.get_read() == 2);
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 3);
    REQUIRE(buffers[2] == 2);
    triple_buffer.get_write() =4;
    REQUIRE(buffers[0] == 0);
    REQUIRE(buffers[1] == 4);
    REQUIRE(buffers[2] == 2);
    REQUIRE(triple_buffer.get_read() == 2);
}
