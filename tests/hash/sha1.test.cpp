/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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
#include <require.tests.hpp>

#include <hash/sha1>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(sha1, traits, standard) {
    REQUIRE((std::is_pod<gtl::sha1>::value == false));

    REQUIRE((std::is_trivial<gtl::sha1>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::sha1>::value == true));

    REQUIRE((std::is_standard_layout<gtl::sha1>::value == true));
}

TEST(sha1, constructor, empty) {
    gtl::sha1 sha1;
    testbench::do_not_optimise_away(sha1);
}

TEST(sha1, function, reset) {
    gtl::sha1 sha1;
    sha1.reset();
}

TEST(sha1, function, consume) {
    gtl::sha1 sha1;
    sha1.consume("", 0);
    sha1.consume("123456781234567812345678123456781234567812345678123456781234567", 63);
    sha1.consume("1234567812345678123456781234567812345678123456781234567812345678", 64);
    sha1.consume("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(sha1, function, finalise) {
    gtl::sha1 sha1;
    sha1.finalise();
}

TEST(sha1, function, get_hash) {
    gtl::sha1 sha1;
    sha1.finalise();
    sha1.get_hash();
}

TEST(sha1, function, hash_buffer) {
    gtl::sha1::hash_buffer("", 0);
    gtl::sha1::hash_buffer("123456781234567812345678123456781234567812345678123456781234567", 63);
    gtl::sha1::hash_buffer("1234567812345678123456781234567812345678123456781234567812345678", 64);
    gtl::sha1::hash_buffer("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(sha1, evaluate, hash_as_integer) {

    constexpr static const unsigned int data_count = 7;

    constexpr static const char* data[data_count] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
    };

    constexpr static unsigned int result[data_count][gtl::sha1::hash_size / sizeof(unsigned int)] = {
        { 0xDA39A3EE, 0x5E6B4B0D, 0x3255BFEF, 0x95601890, 0xAFD80709 },
        { 0x86F7E437, 0xFAA5A7FC, 0xE15D1DDC, 0xB9EAEAEA, 0x377667B8 },
        { 0xA9993E36, 0x4706816A, 0xBA3E2571, 0x7850C26C, 0x9CD0D89D },
        { 0xC12252CE, 0xDA8BE899, 0x4D5FA029, 0x0A47231C, 0x1D16AAE3 },
        { 0x32D10C7B, 0x8CF96570, 0xCA04CE37, 0xF2A19D84, 0x240D3A89 },
        { 0x761C457B, 0xF73B14D2, 0x7E9E9265, 0xC46F4B4D, 0xDA11F940 },
        { 0x50ABF570, 0x6A150990, 0xA08B2C5E, 0xA40FA0E5, 0x85554732 }
    };

    gtl::sha1 sha1;

    for (unsigned int i = 0; i < data_count; ++i) {
        sha1.reset();
        sha1.consume(data[i], testbench::string_length(data[i]));
        sha1.finalise();
        gtl::sha1::hash_type hash = sha1.get_hash();

        char hash_string[40 + 4 + 40 + 1] = {};
        snprintf(hash_string, 40 + 4 + 40 + 1, "%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X", hash.data[0], hash.data[1], hash.data[2], hash.data[3], hash.data[4], result[i][0], result[i][1], result[i][2], result[i][3], result[i][4]);
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha1::hash_size) == true);
    }
}

TEST(sha1, evaluate, hash_as_string) {

    constexpr static const unsigned int data_count = 7;

    constexpr static const char* data[data_count] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
    };

    constexpr static const char* result[data_count] = {
        "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709",
        "86F7E437FAA5A7FCE15D1DDCB9EAEAEA377667B8",
        "A9993E364706816ABA3E25717850C26C9CD0D89D",
        "C12252CEDA8BE8994D5FA0290A47231C1D16AAE3",
        "32D10C7B8CF96570CA04CE37F2A19D84240D3A89",
        "761C457BF73B14D27E9E9265C46F4B4DDA11F940",
        "50ABF5706A150990A08B2C5EA40FA0E585554732"
    };

    gtl::sha1 sha1;
    for (unsigned int i = 0; i < data_count; ++i) {
        sha1.reset();
        sha1.consume(data[i], testbench::string_length(data[i]));
        sha1.finalise();
        gtl::sha1::hash_type hash = sha1.get_hash();

        PRINT("%s == %s\n", gtl::sha1::hash_to_string(hash).hash, result[i]);
        REQUIRE(testbench::is_string_same(gtl::sha1::hash_to_string(hash).hash, result[i]) == true);
    }
}


TEST(sha1, evaluate, partial_insert) {

    constexpr static const unsigned int data_count = 7;

    constexpr static const char* data1[data_count] = {
        "",
        "a",
        "",
        "message",
        "abcdefghijklm",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "123456789012345678901234567890123456789012345678901234567890"
    };

    constexpr static const char* data2[data_count] = {
        "",
        "",
        "abc",
        " digest",
        "nopqrstuvwxyz",
        "",
        "12345678901234567890"
    };

    constexpr static unsigned int result[data_count][gtl::sha1::hash_size / sizeof(unsigned int)] = {
        { 0xDA39A3EE, 0x5E6B4B0D, 0x3255BFEF, 0x95601890, 0xAFD80709 },
        { 0x86F7E437, 0xFAA5A7FC, 0xE15D1DDC, 0xB9EAEAEA, 0x377667B8 },
        { 0xA9993E36, 0x4706816A, 0xBA3E2571, 0x7850C26C, 0x9CD0D89D },
        { 0xC12252CE, 0xDA8BE899, 0x4D5FA029, 0x0A47231C, 0x1D16AAE3 },
        { 0x32D10C7B, 0x8CF96570, 0xCA04CE37, 0xF2A19D84, 0x240D3A89 },
        { 0x761C457B, 0xF73B14D2, 0x7E9E9265, 0xC46F4B4D, 0xDA11F940 },
        { 0x50ABF570, 0x6A150990, 0xA08B2C5E, 0xA40FA0E5, 0x85554732 }
    };

    gtl::sha1 sha1;
    for (unsigned int i = 0; i < data_count; ++i) {
        sha1.reset();
        sha1.consume(data1[i], testbench::string_length(data1[i]));
        sha1.consume(data2[i], testbench::string_length(data2[i]));
        sha1.finalise();
        gtl::sha1::hash_type hash = sha1.get_hash();

        char hash_string[40 + 4 + 40 + 1] = {};
        snprintf(hash_string, 40 + 4 + 40 + 1, "%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X", hash.data[0], hash.data[1], hash.data[2], hash.data[3], hash.data[4], result[i][0], result[i][1], result[i][2], result[i][3], result[i][4]);
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha1::hash_size) == true);
    }
}
