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
#include <testbench/require.tests.hpp>

#include <hash/sha1>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
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

TEST(sha1, function, hash) {
    gtl::sha1::hash("", 0);
    gtl::sha1::hash("123456781234567812345678123456781234567812345678123456781234567", 63);
    gtl::sha1::hash("1234567812345678123456781234567812345678123456781234567812345678", 64);
    gtl::sha1::hash("12345678123456781234567812345678123456781234567812345678123456781", 65);
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

    constexpr static unsigned char result[data_count][gtl::sha1::hash_size] = {
        { 0xDA, 0x39, 0xA3, 0xEE, 0x5E, 0x6B, 0x4B, 0x0D, 0x32, 0x55, 0xBF, 0xEF, 0x95, 0x60, 0x18, 0x90, 0xAF, 0xD8, 0x07, 0x09 },
        { 0x86, 0xF7, 0xE4, 0x37, 0xFA, 0xA5, 0xA7, 0xFC, 0xE1, 0x5D, 0x1D, 0xDC, 0xB9, 0xEA, 0xEA, 0xEA, 0x37, 0x76, 0x67, 0xB8 },
        { 0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D },
        { 0xC1, 0x22, 0x52, 0xCE, 0xDA, 0x8B, 0xE8, 0x99, 0x4D, 0x5F, 0xA0, 0x29, 0x0A, 0x47, 0x23, 0x1C, 0x1D, 0x16, 0xAA, 0xE3 },
        { 0x32, 0xD1, 0x0C, 0x7B, 0x8C, 0xF9, 0x65, 0x70, 0xCA, 0x04, 0xCE, 0x37, 0xF2, 0xA1, 0x9D, 0x84, 0x24, 0x0D, 0x3A, 0x89 },
        { 0x76, 0x1C, 0x45, 0x7B, 0xF7, 0x3B, 0x14, 0xD2, 0x7E, 0x9E, 0x92, 0x65, 0xC4, 0x6F, 0x4B, 0x4D, 0xDA, 0x11, 0xF9, 0x40 },
        { 0x50, 0xAB, 0xF5, 0x70, 0x6A, 0x15, 0x09, 0x90, 0xA0, 0x8B, 0x2C, 0x5E, 0xA4, 0x0F, 0xA0, 0xE5, 0x85, 0x55, 0x47, 0x32 }
    };

    gtl::sha1 sha1;

    for (unsigned int i = 0; i < data_count; ++i) {
        sha1.reset();
        sha1.consume(data[i], testbench::string_length(data[i]));
        sha1.finalise();
        gtl::sha1::hash_type hash = sha1.get_hash();

        char hash_string[(2 * gtl::sha1::hash_size) + 1] = {};
        char result_string[(2 * gtl::sha1::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::sha1::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

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

    constexpr static unsigned char result[data_count][gtl::sha1::hash_size] = {
        { 0xDA, 0x39, 0xA3, 0xEE, 0x5E, 0x6B, 0x4B, 0x0D, 0x32, 0x55, 0xBF, 0xEF, 0x95, 0x60, 0x18, 0x90, 0xAF, 0xD8, 0x07, 0x09 },
        { 0x86, 0xF7, 0xE4, 0x37, 0xFA, 0xA5, 0xA7, 0xFC, 0xE1, 0x5D, 0x1D, 0xDC, 0xB9, 0xEA, 0xEA, 0xEA, 0x37, 0x76, 0x67, 0xB8 },
        { 0xA9, 0x99, 0x3E, 0x36, 0x47, 0x06, 0x81, 0x6A, 0xBA, 0x3E, 0x25, 0x71, 0x78, 0x50, 0xC2, 0x6C, 0x9C, 0xD0, 0xD8, 0x9D },
        { 0xC1, 0x22, 0x52, 0xCE, 0xDA, 0x8B, 0xE8, 0x99, 0x4D, 0x5F, 0xA0, 0x29, 0x0A, 0x47, 0x23, 0x1C, 0x1D, 0x16, 0xAA, 0xE3 },
        { 0x32, 0xD1, 0x0C, 0x7B, 0x8C, 0xF9, 0x65, 0x70, 0xCA, 0x04, 0xCE, 0x37, 0xF2, 0xA1, 0x9D, 0x84, 0x24, 0x0D, 0x3A, 0x89 },
        { 0x76, 0x1C, 0x45, 0x7B, 0xF7, 0x3B, 0x14, 0xD2, 0x7E, 0x9E, 0x92, 0x65, 0xC4, 0x6F, 0x4B, 0x4D, 0xDA, 0x11, 0xF9, 0x40 },
        { 0x50, 0xAB, 0xF5, 0x70, 0x6A, 0x15, 0x09, 0x90, 0xA0, 0x8B, 0x2C, 0x5E, 0xA4, 0x0F, 0xA0, 0xE5, 0x85, 0x55, 0x47, 0x32 }
    };

    gtl::sha1 sha1;
    for (unsigned int i = 0; i < data_count; ++i) {
        sha1.reset();
        sha1.consume(data1[i], testbench::string_length(data1[i]));
        sha1.consume(data2[i], testbench::string_length(data2[i]));
        sha1.finalise();
        gtl::sha1::hash_type hash = sha1.get_hash();

        char hash_string[40 + 4 + 40 + 1] = {};
        snprintf(
            hash_string,
            40 + 4 + 40 + 1,
            "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X == %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            hash.data[0],
            hash.data[1],
            hash.data[2],
            hash.data[3],
            hash.data[4],
            hash.data[5],
            hash.data[6],
            hash.data[7],
            hash.data[8],
            hash.data[9],
            hash.data[10],
            hash.data[11],
            hash.data[12],
            hash.data[13],
            hash.data[14],
            hash.data[15],
            hash.data[16],
            hash.data[17],
            result[i][0],
            result[i][1],
            result[i][2],
            result[i][3],
            result[i][4],
            result[i][5],
            result[i][6],
            result[i][7],
            result[i][8],
            result[i][9],
            result[i][10],
            result[i][11],
            result[i][12],
            result[i][13],
            result[i][14],
            result[i][15],
            result[i][16],
            result[i][17]
        );
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha1::hash_size) == true);
    }
}
