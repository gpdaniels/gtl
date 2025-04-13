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
#include <testbench/optimise.tests.hpp>
#include <testbench/process.tests.hpp>
#include <testbench/require.tests.hpp>

#include <string/static_string_encrypted>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <string>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(static_string_encrypted, evaluate, obfusocated) {
    REQUIRE(testbench::is_memory_same("STRING", GTL_STATIC_STRING_ENCRYPTED("STRING").get_decoded(), testbench::string_length("STRING")) == true);
    REQUIRE(testbench::is_memory_same("STRING", GTL_STATIC_STRING_ENCRYPTED("STRING").get_encoded(), testbench::string_length("STRING")) == false);
}

TEST(static_string_encrypted, evaluate, executable_search) {

    // Load the executable binary into memory.
    const char* executable_path = testbench::get_executable();
    REQUIRE(executable_path != nullptr);

    std::FILE* executable_handle = std::fopen(executable_path, "rb");
    REQUIRE(executable_handle != nullptr);

    REQUIRE(std::fseek(executable_handle, 0, SEEK_END) == 0);
    const long long int executable_length_and_return_code = std::ftell(executable_handle);
    REQUIRE(executable_length_and_return_code > 0);
    const unsigned long long int executable_length = static_cast<unsigned long long int>(executable_length_and_return_code);
    REQUIRE(std::fseek(executable_handle, 0, SEEK_SET) == 0);

    std::vector<char> executable_buffer(executable_length);

    REQUIRE(std::fread(executable_buffer.data(), 1, executable_length, executable_handle) == executable_length);
    std::fclose(executable_handle);

    // Define some strings.
    const char* secret_strings[] = {
        GTL_STATIC_STRING_ENCRYPTED("static_string_encrypted::evaluate::executable_search = secret").get_decoded(),
        GTL_STATIC_STRING_ENCRYPTED("static_string_encrypted::evaluate::executable_search = SECRET").get_decoded(),
        GTL_STATIC_STRING_ENCRYPTED("static_string_encrypted::evaluate::executable_search = Hello secret world").get_decoded(),
        GTL_STATIC_STRING_ENCRYPTED("static_string_encrypted::evaluate::executable_search = Some secret API function name").get_decoded()
    };
    testbench::do_not_optimise_away(secret_strings);
    testbench::do_not_optimise_away(secret_strings[0]);
    testbench::do_not_optimise_away(secret_strings[1]);
    testbench::do_not_optimise_away(secret_strings[2]);
    testbench::do_not_optimise_away(secret_strings[3]);

    const char* public_strings[] = {
        "static_string_encrypted::evaluate::executable_search = public",
        "static_string_encrypted::evaluate::executable_search = PUBLIC",
        "static_string_encrypted::evaluate::executable_search = Hello public world",
        "static_string_encrypted::evaluate::executable_search = Some public API function name"
    };
    testbench::do_not_optimise_away(public_strings);
    testbench::do_not_optimise_away(public_strings[0]);
    testbench::do_not_optimise_away(public_strings[1]);
    testbench::do_not_optimise_away(public_strings[2]);
    testbench::do_not_optimise_away(public_strings[3]);

    // Search the memory for the secret and non-secret strings.
    auto search = [](const std::string& string, const std::vector<char>& buffer) -> unsigned long long int {
        if (string.empty()) {
            return false;
        }
        unsigned long long int found = 0;
        for (unsigned long long int index_buffer = 0; index_buffer < buffer.size(); ++index_buffer) {
            bool match = true;
            for (unsigned long long int index_string = 0; index_string < string.size(); ++index_string) {
                if (buffer[index_buffer + index_string] != string[index_string]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                ++found;
            }
        }
        return found;
    };

    REQUIRE(search(secret_strings[0], executable_buffer) == 0);
    REQUIRE(search(secret_strings[1], executable_buffer) == 0);
    REQUIRE(search(secret_strings[2], executable_buffer) == 0);
    REQUIRE(search(secret_strings[3], executable_buffer) == 0);

    REQUIRE(search(public_strings[0], executable_buffer) == 1);
    REQUIRE(search(public_strings[1], executable_buffer) == 1);
    REQUIRE(search(public_strings[2], executable_buffer) == 1);
    REQUIRE(search(public_strings[3], executable_buffer) == 1);
}

TEST(static_string_encrypted, evaluate, print) {
    PRINT("Visible: '%s' vs. Hidden: '%s'\n", "Hello public world", GTL_STATIC_STRING_ENCRYPTED("Hello secret world").get_decoded());

    const char* raw_string = "raw string";
    unsigned long long int raw_string_length = testbench::string_length(raw_string);

    PRINT("Raw:     ");
    for (unsigned long long int i = 0; i < raw_string_length; ++i) {
        PRINT("%02x", raw_string[i]);
    }
    PRINT("\n");

    PRINT("Encoded: ");
    for (unsigned long long int i = 0; i < raw_string_length; ++i) {
        PRINT("%02x", GTL_STATIC_STRING_ENCRYPTED("raw string").get_encoded()[i]);
    }
    PRINT("\n");

    PRINT("Decoded: ");
    for (unsigned long long int i = 0; i < raw_string_length; ++i) {
        PRINT("%02x", GTL_STATIC_STRING_ENCRYPTED("raw string").get_decoded()[i]);
    }
    PRINT("\n");
}
