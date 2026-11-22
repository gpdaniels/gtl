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

#include <crypto/mac/poly1305>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(poly1305, traits, standard) {
    REQUIRE((std::is_pod<gtl::poly1305>::value == false));

    REQUIRE((std::is_trivial<gtl::poly1305>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::poly1305>::value == true));

    REQUIRE((std::is_standard_layout<gtl::poly1305>::value == true));
}

TEST(poly1305, constructor, empty) {
    gtl::poly1305::key_type key = {};
    gtl::poly1305 poly1305(key);
    testbench::do_not_optimise_away(poly1305);
}

TEST(poly1305, function, reset) {
    gtl::poly1305::key_type key = {};
    gtl::poly1305 poly1305(key);
    poly1305.reset();
}

TEST(poly1305, function, consume) {
    gtl::poly1305::key_type key = {};
    gtl::poly1305 poly1305(key);
    poly1305.consume("", 0);
    poly1305.consume("123456781234567", 15);
    poly1305.consume("1234567812345678", 16);
    poly1305.consume("12345678123456781", 17);
}

TEST(poly1305, function, finalise) {
    gtl::poly1305::key_type key = {};
    gtl::poly1305 poly1305(key);
    poly1305.finalise();
}

TEST(poly1305, function, get_hash) {
    gtl::poly1305::key_type key = {};
    gtl::poly1305 poly1305(key);
    poly1305.finalise();
    poly1305.get_hash();
}

TEST(poly1305, function, hash) {
    gtl::poly1305::key_type key = {};
    gtl::poly1305::hash("", 0, key);
    gtl::poly1305::hash("123456781234567", 15, key);
    gtl::poly1305::hash("1234567812345678", 16, key);
    gtl::poly1305::hash("12345678123456781", 17, key);
}

TEST(poly1305, evaluate, hash_as_integer) {
    constexpr static const unsigned int data_count = 2;

    constexpr static const char* data[data_count] = {
        "",
        "Cryptographic Forum Research Group"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::poly1305::key_size] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        { 0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33, 0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8, 0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd, 0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b }
    };

    constexpr static unsigned char result[data_count][gtl::poly1305::hash_size] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        { 0xa8, 0x06, 0x1d, 0xc1, 0x30, 0x51, 0x36, 0xc6, 0xc2, 0x2b, 0x8b, 0xaf, 0x0c, 0x01, 0x27, 0xa9 }
    };

    gtl::poly1305::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::poly1305::key_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::poly1305 poly1305(key);
        poly1305.consume(&data[i][0], testbench::string_length(&data[i][0]));
        poly1305.finalise();

        gtl::poly1305::hash_type hash = poly1305.get_hash();

        char hash_string[(2 * gtl::poly1305::hash_size) + 1] = {};
        char result_string[(2 * gtl::poly1305::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::poly1305::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::poly1305::hash_size) == true);
    }
}

TEST(poly1305, evaluate, hash_as_string) {
    constexpr static const unsigned int data_count = 2;

    constexpr static const char* data[data_count] = {
        "",
        "Cryptographic Forum Research Group"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::poly1305::key_size] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        { 0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33, 0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8, 0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd, 0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b }
    };

    constexpr static const char* result[data_count] = {
        "00000000000000000000000000000000",
        "A8061DC1305136C6C22B8BAF0C0127A9"
    };

    gtl::poly1305::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::poly1305::key_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::poly1305 poly1305(key);
        poly1305.consume(&data[i][0], testbench::string_length(&data[i][0]));
        poly1305.finalise();

        gtl::poly1305::hash_type hash = poly1305.get_hash();

        PRINT("%s == %s\n", gtl::poly1305::hash_to_string(hash).hash, result[i]);
        REQUIRE(testbench::is_string_same(gtl::poly1305::hash_to_string(hash).hash, result[i]) == true);
    }
}

TEST(poly1305, evaluate, partial_insert) {
    constexpr static const unsigned int data_count = 1;

    constexpr static const char* data1[data_count] = {
        "Cryptographic"
    };

    constexpr static const char* data2[data_count] = {
        " Forum Research Group"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::poly1305::key_size] = {
        { 0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33, 0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8, 0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd, 0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b }
    };

    constexpr static unsigned char result[data_count][gtl::poly1305::hash_size] = {
        { 0xa8, 0x06, 0x1d, 0xc1, 0x30, 0x51, 0x36, 0xc6, 0xc2, 0x2b, 0x8b, 0xaf, 0x0c, 0x01, 0x27, 0xa9 }
    };

    gtl::poly1305::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::poly1305::key_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::poly1305 poly1305(key);
        poly1305.consume(&data1[i][0], testbench::string_length(&data1[i][0]));
        poly1305.consume(&data2[i][0], testbench::string_length(&data2[i][0]));
        poly1305.finalise();

        gtl::poly1305::hash_type hash = poly1305.get_hash();

        char hash_string[(2 * gtl::poly1305::hash_size) + 1] = {};
        char result_string[(2 * gtl::poly1305::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::poly1305::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::poly1305::hash_size) == true);
    }
}
