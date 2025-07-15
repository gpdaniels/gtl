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

#include <hash/sha0>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(sha0, traits, standard) {
    REQUIRE((std::is_pod<gtl::sha0>::value == false));

    REQUIRE((std::is_trivial<gtl::sha0>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::sha0>::value == true));

    REQUIRE((std::is_standard_layout<gtl::sha0>::value == true));
}

TEST(sha0, constructor, empty) {
    gtl::sha0 sha0;
    testbench::do_not_optimise_away(sha0);
}

TEST(sha0, function, reset) {
    gtl::sha0 sha0;
    sha0.reset();
}

TEST(sha0, function, consume) {
    gtl::sha0 sha0;
    sha0.consume("", 0);
    sha0.consume("123456781234567812345678123456781234567812345678123456781234567", 63);
    sha0.consume("1234567812345678123456781234567812345678123456781234567812345678", 64);
    sha0.consume("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(sha0, function, finalise) {
    gtl::sha0 sha0;
    sha0.finalise();
}

TEST(sha0, function, get_hash) {
    gtl::sha0 sha0;
    sha0.finalise();
    sha0.get_hash();
}

TEST(sha0, function, hash) {
    gtl::sha0::hash("", 0);
    gtl::sha0::hash("123456781234567812345678123456781234567812345678123456781234567", 63);
    gtl::sha0::hash("1234567812345678123456781234567812345678123456781234567812345678", 64);
    gtl::sha0::hash("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(sha0, evaluate, hash_as_integer) {
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

    constexpr static unsigned char result[data_count][gtl::sha0::hash_size] = {
        { 0xF9, 0x6C, 0xEA, 0x19, 0x8A, 0xD1, 0xDD, 0x56, 0x17, 0xAC, 0x08, 0x4A, 0x3D, 0x92, 0xC6, 0x10, 0x77, 0x08, 0xC0, 0xEF },
        { 0x37, 0xF2, 0x97, 0x77, 0x2F, 0xAE, 0x4C, 0xB1, 0xBA, 0x39, 0xB6, 0xCF, 0x9C, 0xF0, 0x38, 0x11, 0x80, 0xBD, 0x62, 0xF2 },
        { 0x01, 0x64, 0xB8, 0xA9, 0x14, 0xCD, 0x2A, 0x5E, 0x74, 0xC4, 0xF7, 0xFF, 0x08, 0x2C, 0x4D, 0x97, 0xF1, 0xED, 0xF8, 0x80 },
        { 0xC1, 0xB0, 0xF2, 0x22, 0xD1, 0x50, 0xEB, 0xB9, 0xAA, 0x36, 0xA4, 0x0C, 0xAF, 0xDC, 0x8B, 0xCB, 0xED, 0x83, 0x0B, 0x14 },
        { 0xB4, 0x0C, 0xE0, 0x7A, 0x43, 0x0C, 0xFD, 0x3C, 0x03, 0x30, 0x39, 0xB9, 0xFE, 0x9A, 0xFE, 0xC9, 0x5D, 0xC1, 0xBD, 0xCD },
        { 0x79, 0xE9, 0x66, 0xF7, 0xA3, 0xA9, 0x90, 0xDF, 0x33, 0xE4, 0x0E, 0x3D, 0x7F, 0x8F, 0x18, 0xD2, 0xCA, 0xEB, 0xAD, 0xFA },
        { 0x4A, 0xA2, 0x9D, 0x14, 0xD1, 0x71, 0x52, 0x2E, 0xCE, 0x47, 0xBE, 0xE8, 0x95, 0x7E, 0x35, 0xA4, 0x1F, 0x3E, 0x9C, 0xFF }
    };

    gtl::sha0 sha0;
    for (unsigned int i = 0; i < data_count; ++i) {
        sha0.reset();
        sha0.consume(data[i], testbench::string_length(data[i]));
        sha0.finalise();
        gtl::sha0::hash_type hash = sha0.get_hash();

        char hash_string[(2 * gtl::sha0::hash_size) + 1] = {};
        char result_string[(2 * gtl::sha0::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::sha0::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha0::hash_size) == true);
    }
}

TEST(sha0, evaluate, hash_as_string) {
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
        "F96CEA198AD1DD5617AC084A3D92C6107708C0EF",
        "37F297772FAE4CB1BA39B6CF9CF0381180BD62F2",
        "0164B8A914CD2A5E74C4F7FF082C4D97F1EDF880",
        "C1B0F222D150EBB9AA36A40CAFDC8BCBED830B14",
        "B40CE07A430CFD3C033039B9FE9AFEC95DC1BDCD",
        "79E966F7A3A990DF33E40E3D7F8F18D2CAEBADFA",
        "4AA29D14D171522ECE47BEE8957E35A41F3E9CFF"
    };

    gtl::sha0 sha0;
    for (unsigned int i = 0; i < data_count; ++i) {
        sha0.reset();
        sha0.consume(data[i], testbench::string_length(data[i]));
        sha0.finalise();
        gtl::sha0::hash_type hash = sha0.get_hash();
        PRINT("%s == %s\n", gtl::sha0::hash_to_string(hash).hash, result[i]);
        REQUIRE(testbench::is_string_same(gtl::sha0::hash_to_string(hash).hash, result[i]) == true);
    }
}

TEST(sha0, evaluate, partial_insert) {
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

    constexpr static unsigned char result[data_count][gtl::sha0::hash_size] = {
        { 0xF9, 0x6C, 0xEA, 0x19, 0x8A, 0xD1, 0xDD, 0x56, 0x17, 0xAC, 0x08, 0x4A, 0x3D, 0x92, 0xC6, 0x10, 0x77, 0x08, 0xC0, 0xEF },
        { 0x37, 0xF2, 0x97, 0x77, 0x2F, 0xAE, 0x4C, 0xB1, 0xBA, 0x39, 0xB6, 0xCF, 0x9C, 0xF0, 0x38, 0x11, 0x80, 0xBD, 0x62, 0xF2 },
        { 0x01, 0x64, 0xB8, 0xA9, 0x14, 0xCD, 0x2A, 0x5E, 0x74, 0xC4, 0xF7, 0xFF, 0x08, 0x2C, 0x4D, 0x97, 0xF1, 0xED, 0xF8, 0x80 },
        { 0xC1, 0xB0, 0xF2, 0x22, 0xD1, 0x50, 0xEB, 0xB9, 0xAA, 0x36, 0xA4, 0x0C, 0xAF, 0xDC, 0x8B, 0xCB, 0xED, 0x83, 0x0B, 0x14 },
        { 0xB4, 0x0C, 0xE0, 0x7A, 0x43, 0x0C, 0xFD, 0x3C, 0x03, 0x30, 0x39, 0xB9, 0xFE, 0x9A, 0xFE, 0xC9, 0x5D, 0xC1, 0xBD, 0xCD },
        { 0x79, 0xE9, 0x66, 0xF7, 0xA3, 0xA9, 0x90, 0xDF, 0x33, 0xE4, 0x0E, 0x3D, 0x7F, 0x8F, 0x18, 0xD2, 0xCA, 0xEB, 0xAD, 0xFA },
        { 0x4A, 0xA2, 0x9D, 0x14, 0xD1, 0x71, 0x52, 0x2E, 0xCE, 0x47, 0xBE, 0xE8, 0x95, 0x7E, 0x35, 0xA4, 0x1F, 0x3E, 0x9C, 0xFF }
    };

    gtl::sha0 sha0;
    for (unsigned int i = 0; i < data_count; ++i) {
        sha0.reset();
        sha0.consume(data1[i], testbench::string_length(data1[i]));
        sha0.consume(data2[i], testbench::string_length(data2[i]));
        sha0.finalise();
        gtl::sha0::hash_type hash = sha0.get_hash();

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

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha0::hash_size) == true);
    }
}
