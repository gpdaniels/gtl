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

TEST(md5, function, hash) {
    gtl::md5::hash("", 0);
    gtl::md5::hash("123456781234567812345678123456781234567812345678123456781234567", 63);
    gtl::md5::hash("1234567812345678123456781234567812345678123456781234567812345678", 64);
    gtl::md5::hash("12345678123456781234567812345678123456781234567812345678123456781", 65);
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

    constexpr static unsigned char result[data_count][gtl::md5::hash_size] = {
        { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E },
        { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 },
        { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 },
        { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 },
        { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B },
        { 0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5, 0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F },
        { 0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55, 0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A }
    };

    gtl::md5 md5;
    for (unsigned int i = 0; i < data_count; ++i) {
        md5.reset();
        md5.consume(&data[i][0], testbench::string_length(&data[i][0]));
        md5.finalise();

        gtl::md5::hash_type hash = md5.get_hash();

        char hash_string[(2 * gtl::md5::hash_size) + 1] = {};
        char result_string[(2 * gtl::md5::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::md5::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

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

    constexpr static unsigned char result[data_count][gtl::md5::hash_size] = {
        { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E },
        { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 },
        { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 },
        { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 },
        { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B },
        { 0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5, 0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F },
        { 0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55, 0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A }
    };

    gtl::md5 md5;
    for (unsigned int i = 0; i < data_count; ++i) {
        md5.reset();
        md5.consume(&data1[i][0], testbench::string_length(&data1[i][0]));
        md5.consume(&data2[i][0], testbench::string_length(&data2[i][0]));
        md5.finalise();

        gtl::md5::hash_type hash = md5.get_hash();

        char hash_string[(2 * gtl::md5::hash_size) + 1] = {};
        char result_string[(2 * gtl::md5::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::md5::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::md5::hash_size) == true);
    }
}
