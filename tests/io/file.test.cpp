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

#include <testbench/ignored.tests.hpp>
#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <io/file>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <string>
#include <type_traits>
#include <utility>

#if defined(_MSC_VER)
#pragma warning(pop)
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
    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    // Ensure every combination works.
    testbench::test_template<access_types, creation_types, cursor_types>(
        [&temp_filename](auto test_access, auto test_creation, auto test_cursor) -> void {
            constexpr static const gtl::file::access_type access = decltype(test_access)::value;
            constexpr static const gtl::file::creation_type creation = decltype(test_creation)::value;
            constexpr static const gtl::file::cursor_type cursor = decltype(test_cursor)::value;
            gtl::file file(temp_filename.c_str(), access, creation, cursor);
        }
    );

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, is_open) {
    gtl::file file;
    REQUIRE(file.is_open() == false);

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only));
    REQUIRE(file.is_open() == true);

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, is_eof) {
    gtl::file file;
    bool eof = false;
    REQUIRE(file.is_eof(eof) == false);
    REQUIRE(eof == true);

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));
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

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, open) {
    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    // Ensure every combination doesn't crash.
    testbench::test_template<access_types, creation_types, cursor_types>(
        [&temp_filename](auto test_access, auto test_creation, auto test_cursor) -> void {
            constexpr static const gtl::file::access_type access = decltype(test_access)::value;
            constexpr static const gtl::file::creation_type creation = decltype(test_creation)::value;
            constexpr static const gtl::file::cursor_type cursor = decltype(test_cursor)::value;
            gtl::file file;
            file.open(temp_filename.c_str(), access, creation, cursor);
        }
    );

    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file) == false);
    }
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated) == false);
    }

    IGNORED(std::remove(temp_filename.c_str()));

    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only) == false);
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only, gtl::file::creation_type::create_only));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only, gtl::file::creation_type::create_only) == false);
    }
    IGNORED(std::remove(temp_filename.c_str()));
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write) == false);
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::create_only));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::create_only) == false);
    }
    IGNORED(std::remove(temp_filename.c_str()));
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file) == false);
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::create_only, gtl::file::cursor_type::end_of_file));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::create_only, gtl::file::cursor_type::end_of_file) == false);
    }
    IGNORED(std::remove(temp_filename.c_str()));
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated) == false);
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::create_only, gtl::file::cursor_type::start_of_truncated));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write, gtl::file::creation_type::create_only, gtl::file::cursor_type::start_of_truncated) == false);
    }
    IGNORED(std::remove(temp_filename.c_str()));
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::end_of_file) == false);
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::create_only, gtl::file::cursor_type::end_of_file));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::create_only, gtl::file::cursor_type::end_of_file) == false);
    }
    IGNORED(std::remove(temp_filename.c_str()));
    {
        gtl::file file;

        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::open_only, gtl::file::cursor_type::start_of_truncated) == false);
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::create_only, gtl::file::cursor_type::start_of_truncated));
        REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::write_only, gtl::file::creation_type::create_only, gtl::file::cursor_type::start_of_truncated) == false);
    }

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, close) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only));
    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only) == false);

    REQUIRE(file.close());
    REQUIRE(file.close() == false);

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only));

    REQUIRE(file.close());
    REQUIRE(file.close() == false);

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, get_handle) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_only));

    REQUIRE(file.get_handle() >= 0);

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, get_size) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));

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

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, get_cursor_position) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));

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

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, set_cursor_position) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));

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

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, read) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));

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

    IGNORED(std::remove(temp_filename.c_str()));
}

TEST(file, function, write) {
    gtl::file file;

    const std::string temp_filename = std::to_string(std::hash<std::string>{}(std::string(__FUNCTION__)));
    PRINT("Temp filename for '%s' is: %s\n", __FUNCTION__, temp_filename.c_str());
    IGNORED(std::remove(temp_filename.c_str()));
    IGNORED(std::fclose(std::fopen(temp_filename.c_str(), "wb")));

    REQUIRE(file.open(temp_filename.c_str(), gtl::file::access_type::read_and_write));

    {
        char buffer[2] = { 'a', 'b' };
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

    IGNORED(std::remove(temp_filename.c_str()));
}
