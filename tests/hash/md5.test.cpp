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

#include <hash/md5>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(md5, traits, standard) {
    REQUIRE((std::is_pod<gtl::md5>::value == false));

    REQUIRE((std::is_trivial<gtl::md5>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::md5>::value == true));

    REQUIRE((std::is_standard_layout<gtl::md5>::value == true));
}

TEST(md5, constructor, empty) {
    gtl::md5 md5;
    testbench::do_not_optimise_away(md5);
}

TEST(md5, function, reset) {
    gtl::md5 md5;
    md5.reset();
}

TEST(md5, function, consume) {
    gtl::md5 md5;
    md5.consume("", 0);
    md5.consume("123456781234567812345678123456781234567812345678123456781234567", 63);
    md5.consume("1234567812345678123456781234567812345678123456781234567812345678", 64);
    md5.consume("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(md5, function, finalise) {
    gtl::md5 md5;
    md5.finalise();
}

TEST(md5, function, get_hash) {
    gtl::md5 md5;
    md5.finalise();
    md5.get_hash();
}

TEST(md5, function, hash_buffer) {
    gtl::md5::hash_buffer("", 0);
    gtl::md5::hash_buffer("123456781234567812345678123456781234567812345678123456781234567", 63);
    gtl::md5::hash_buffer("1234567812345678123456781234567812345678123456781234567812345678", 64);
    gtl::md5::hash_buffer("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(md5, evaluate, hash_as_integer) {
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

    constexpr static unsigned int result[data_count][gtl::md5::hash_size / sizeof(unsigned int)] = {
        {0xD41D8CD9, 0x8F00B204, 0xE9800998, 0xECF8427E},
        {0x0CC175B9, 0xC0F1B6A8, 0x31C399E2, 0x69772661},
        {0x90015098, 0x3CD24FB0, 0xD6963F7D, 0x28E17F72},
        {0xF96B697D, 0x7CB7938D, 0x525A2F31, 0xAAF161D0},
        {0xC3FCD3D7, 0x6192E400, 0x7DFB496C, 0xCA67E13B},
        {0xD174AB98, 0xD277D9F5, 0xA5611C2C, 0x9F419D9F},
        {0x57EDF4A2, 0x2BE3C955, 0xAC49DA2E, 0x2107B67A}
    };

    gtl::md5 md5;
    for (unsigned int i = 0; i < data_count; ++i) {
        md5.reset();
        md5.consume(&data[i][0], testbench::string_length(&data[i][0]));
        md5.finalise();

        gtl::md5::hash_type hash = md5.get_hash();

        char hash_string[32 + 4 + 32 + 1] = {};
        snprintf(hash_string, 32 + 4 + 32 + 1, "%08X%08X%08X%08X == %08X%08X%08X%08X", hash.data[0], hash.data[1], hash.data[2], hash.data[3], result[i][0], result[i][1], result[i][2], result[i][3]);
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::md5::hash_size) == true);
    }
}

TEST(md5, evaluate, hash_as_string) {
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
        "D41D8CD98F00B204E9800998ECF8427E",
        "0CC175B9C0F1B6A831C399E269772661",
        "900150983CD24FB0D6963F7D28E17F72",
        "F96B697D7CB7938D525A2F31AAF161D0",
        "C3FCD3D76192E4007DFB496CCA67E13B",
        "D174AB98D277D9F5A5611C2C9F419D9F",
        "57EDF4A22BE3C955AC49DA2E2107B67A"
    };

    gtl::md5 md5;
    for (unsigned int i = 0; i < data_count; ++i) {
        md5.reset();
        md5.consume(&data[i][0], testbench::string_length(&data[i][0]));
        md5.finalise();

        gtl::md5::hash_type hash = md5.get_hash();

        PRINT("%s == %s\n", gtl::md5::hash_to_string(hash).hash, result[i]);
        REQUIRE(testbench::is_string_same(gtl::md5::hash_to_string(hash).hash, result[i]) == true);
    }
}

TEST(md5, evaluate, partial_insert) {
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

    constexpr static unsigned int result[data_count][gtl::md5::hash_size / sizeof(unsigned int)] = {
        {0xD41D8CD9, 0x8F00B204, 0xE9800998, 0xECF8427E},
        {0x0CC175B9, 0xC0F1B6A8, 0x31C399E2, 0x69772661},
        {0x90015098, 0x3CD24FB0, 0xD6963F7D, 0x28E17F72},
        {0xF96B697D, 0x7CB7938D, 0x525A2F31, 0xAAF161D0},
        {0xC3FCD3D7, 0x6192E400, 0x7DFB496C, 0xCA67E13B},
        {0xD174AB98, 0xD277D9F5, 0xA5611C2C, 0x9F419D9F},
        {0x57EDF4A2, 0x2BE3C955, 0xAC49DA2E, 0x2107B67A}
    };

    gtl::md5 md5;
    for (unsigned int i = 0; i < data_count; ++i) {
        md5.reset();
        md5.consume(&data1[i][0], testbench::string_length(&data1[i][0]));
        md5.consume(&data2[i][0], testbench::string_length(&data2[i][0]));
        md5.finalise();

        gtl::md5::hash_type hash = md5.get_hash();

        char hash_string[32 + 4 + 32 + 1] = {};
        snprintf(hash_string, 32 + 4 + 32 + 1, "%08X%08X%08X%08X == %08X%08X%08X%08X", hash.data[0], hash.data[1], hash.data[2], hash.data[3], result[i][0], result[i][1], result[i][2], result[i][3]);
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::md5::hash_size) == true);
    }
}
