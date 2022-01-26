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

#include <main.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <io/file>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

using access_types = testbench::enum_collection<
    gtl::file::access_type,
    gtl::file::access_type::read_only,
    gtl::file::access_type::write_only,
    gtl::file::access_type::read_and_write
>;

using creation_types = testbench::enum_collection<
    gtl::file::creation_type,
    gtl::file::creation_type::open_only,
    gtl::file::creation_type::create_only,
    gtl::file::creation_type::create_or_open
>;

using cursor_types = testbench::enum_collection<
    gtl::file::cursor_type,
    gtl::file::cursor_type::start_of_file,
    gtl::file::cursor_type::start_of_truncated,
    gtl::file::cursor_type::end_of_file,
    gtl::file::cursor_type::end_of_truncated
>;

TEST(file, traits, standard) {
    REQUIRE((std::is_pod<gtl::file>::value == false));

    REQUIRE((std::is_trivial<gtl::file>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::file>::value == false));

    REQUIRE((std::is_standard_layout<gtl::file>::value == true));
}

TEST(file, constructor, empty) {
    gtl::file file;
    testbench::do_not_optimise_away(file);
}

TEST(file, constructor, parameterised) {

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    // Ensure every combination works.
    testbench::test_template<access_types, creation_types, cursor_types>(
        [&temp_filename](auto test_access, auto test_creation, auto test_cursor)->void {
            constexpr static const gtl::file::access_type access = decltype(test_access)::value;
            constexpr static const gtl::file::creation_type creation = decltype(test_creation)::value;
            constexpr static const gtl::file::cursor_type cursor = decltype(test_cursor)::value;
            gtl::file file(temp_filename, access, creation, cursor);
        }
    );
}

TEST(file, function, is_open) {
    gtl::file file;
    REQUIRE(file.is_open() == false);

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only));
    REQUIRE(file.is_open() == true);
}

TEST(file, function, is_eof) {
    gtl::file file;
    bool eof = false;
    REQUIRE(file.is_eof(eof) == false);
    REQUIRE(eof == true);

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));
    eof = false;
    REQUIRE(file.is_eof(eof) == true);
    REQUIRE(eof == true);

    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("a", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("b", length));
        REQUIRE(length == 1);
    }

    REQUIRE(file.set_cursor_position(0));

    eof = false;
    REQUIRE(file.is_eof(eof) == true);
    REQUIRE(eof == false);


    REQUIRE(file.set_cursor_position(0, gtl::file::position_type::end));

    eof = false;
    REQUIRE(file.is_eof(eof) == true);
    REQUIRE(eof == true);
}

TEST(file, function, open) {

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    // Ensure every combination works.
    testbench::test_template<access_types, creation_types, cursor_types>(
        [&temp_filename](auto test_access, auto test_creation, auto test_cursor)->void {
            constexpr static const gtl::file::access_type access = decltype(test_access)::value;
            constexpr static const gtl::file::creation_type creation = decltype(test_creation)::value;
            constexpr static const gtl::file::cursor_type cursor = decltype(test_cursor)::value;
            gtl::file file;
            file.open(temp_filename, access, creation, cursor);
        }
    );

    {
        gtl::file file;

        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only));
        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));
        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file));
        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated));
        REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename, gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file));
        REQUIRE(file.open(temp_filename, gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename, gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated));
        REQUIRE(file.open(temp_filename, gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated) == false);
    }
}

TEST(file, function, close) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only));
    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only) == false);

    REQUIRE(file.close());
    REQUIRE(file.close() == false);

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only));

    REQUIRE(file.close());
    REQUIRE(file.close() == false);
}


TEST(file, function, get_handle) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_only));

    REQUIRE(file.get_handle() >= 0);
}

TEST(file, function, get_size) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));

    {
        gtl::file::size_type size = 0;
        REQUIRE(file.get_size(size));
        REQUIRE(size == 0);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("a", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type size = 0;
        REQUIRE(file.get_size(size));
        REQUIRE(size == 1);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("b", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type size = 0;
        REQUIRE(file.get_size(size));
        REQUIRE(size == 2);
    }
}


TEST(file, function, get_cursor_position) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));

    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 0);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("a", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 1);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("b", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 2);
    }
    {
        REQUIRE(file.set_cursor_position(1));
    }
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 1);
    }
}

TEST(file, function, set_cursor_position) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));

    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("a", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("b", length));
        REQUIRE(length == 1);
    }
    REQUIRE(file.set_cursor_position(0));
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 0);
    }

    REQUIRE(file.set_cursor_position(2));
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 2);
    }

    REQUIRE(file.set_cursor_position(1));
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 1);
    }

    REQUIRE(file.set_cursor_position(1, gtl::file::position_type::current));
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 2);
    }

    REQUIRE(file.set_cursor_position(-1, gtl::file::position_type::current));
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 1);
    }

    REQUIRE(file.set_cursor_position(-2, gtl::file::position_type::end));
    {
        gtl::file::size_type position = 0;
        REQUIRE(file.get_cursor_position(position));
        REQUIRE(position == 0);
    }
}

TEST(file, function, read) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));

    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("a", length));
        REQUIRE(length == 1);
    }
    {
        gtl::file::size_type length = 1;
        REQUIRE(file.write("b", length));
        REQUIRE(length == 1);
    }

    REQUIRE(file.set_cursor_position(0));

    {
        char buffer[2] = {};
        gtl::file::size_type length = 2;
        REQUIRE(file.read(buffer, length));
        REQUIRE(length == 2);
        REQUIRE(buffer[0] == 'a');
        REQUIRE(buffer[1] == 'b');
    }
}

TEST(file, function, write) {
    gtl::file file;

    const char* temp_filename = std::tmpnam(nullptr);
    IGNORED(std::fopen(temp_filename, "wb"));

    REQUIRE(file.open(temp_filename, gtl::file::access_type::read_and_write));

    {
        char buffer[2] = {'a', 'b'};
        gtl::file::size_type length = 2;
        REQUIRE(file.write(buffer, length));
        REQUIRE(length == 2);
    }

    REQUIRE(file.set_cursor_position(0));

    {
        char character = 0;
        gtl::file::size_type length = 1;
        REQUIRE(file.read(&character, length));
        REQUIRE(length == 1);
        REQUIRE(character == 'a');
    }

    {
        char character = 0;
        gtl::file::size_type length = 1;
        REQUIRE(file.read(&character, length));
        REQUIRE(length == 1);
        REQUIRE(character == 'b');
    }
}
