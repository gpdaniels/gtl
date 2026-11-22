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

#include <crypto/hash/sha2>
#include <crypto/mac/hmac>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(hmac, traits, standard) {
    REQUIRE((std::is_pod<gtl::hmac<gtl::sha2<256>>>::value == false));

    REQUIRE((std::is_trivial<gtl::hmac<gtl::sha2<256>>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::hmac<gtl::sha2<256>>>::value == true));

    REQUIRE((std::is_standard_layout<gtl::hmac<gtl::sha2<256>>>::value == true));
}

TEST(hmac, constructor, empty) {
    gtl::hmac<gtl::sha2<256>>::key_type key = {};
    gtl::hmac<gtl::sha2<256>> hmac(key);
    testbench::do_not_optimise_away(hmac);
}

TEST(hmac, function, reset) {
    gtl::hmac<gtl::sha2<256>>::key_type key = {};
    gtl::hmac<gtl::sha2<256>> hmac(key);
    hmac.reset();
}

TEST(hmac, function, consume) {
    gtl::hmac<gtl::sha2<256>>::key_type key = {};
    gtl::hmac<gtl::sha2<256>> hmac(key);
    hmac.consume("", 0);
    hmac.consume("123456781234567", 15);
    hmac.consume("1234567812345678", 16);
    hmac.consume("12345678123456781", 17);
}

TEST(hmac, function, finalise) {
    gtl::hmac<gtl::sha2<256>>::key_type key = {};
    gtl::hmac<gtl::sha2<256>> hmac(key);
    hmac.finalise();
}

TEST(hmac, function, get_hash) {
    gtl::hmac<gtl::sha2<256>>::key_type key = {};
    gtl::hmac<gtl::sha2<256>> hmac(key);
    hmac.finalise();
    hmac.get_hash();
}

TEST(hmac, function, hash) {
    gtl::hmac<gtl::sha2<256>>::key_type key = {};
    gtl::hmac<gtl::sha2<256>>::hash("", 0, key);
    gtl::hmac<gtl::sha2<256>>::hash("123456781234567", 15, key);
    gtl::hmac<gtl::sha2<256>>::hash("1234567812345678", 16, key);
    gtl::hmac<gtl::sha2<256>>::hash("12345678123456781", 17, key);
}

TEST(hmac, evaluate, hash_as_integer_sha256) {
    constexpr static const unsigned int data_count = 2;

    constexpr static const char* data[data_count] = {
        "Hi There",
        "what do ya want for nothing?"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::hmac<gtl::sha2<256>>::block_size] = {
        { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b },
        { 'J', 'e', 'f', 'e' }
    };

    constexpr static unsigned char result[data_count][gtl::hmac<gtl::sha2<256>>::hash_size] = {
        { 0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7 },
        { 0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e, 0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7, 0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83, 0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43 }
    };

    gtl::hmac<gtl::sha2<256>>::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::hmac<gtl::sha2<256>>::block_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::hmac<gtl::sha2<256>> hmac(key);
        hmac.consume(&data[i][0], testbench::string_length(&data[i][0]));
        hmac.finalise();

        gtl::hmac<gtl::sha2<256>>::hash_type hash = hmac.get_hash();

        char hash_string[(2 * gtl::hmac<gtl::sha2<256>>::hash_size) + 1] = {};
        char result_string[(2 * gtl::hmac<gtl::sha2<256>>::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::hmac<gtl::sha2<256>>::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::hmac<gtl::sha2<256>>::hash_size) == true);
    }
}

TEST(hmac, evaluate, hash_as_string_sha256) {
    constexpr static const unsigned int data_count = 2;

    constexpr static const char* data[data_count] = {
        "Hi There",
        "what do ya want for nothing?"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::hmac<gtl::sha2<256>>::block_size] = {
        { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b },
        { 'J', 'e', 'f', 'e' }
    };

    constexpr static const char* result[data_count] = {
        "B0344C61D8DB38535CA8AFCEAF0BF12B881DC200C9833DA726E9376C2E32CFF7",
        "5BDCC146BF60754E6A042426089575C75A003F089D2739839DEC58B964EC3843"
    };

    gtl::hmac<gtl::sha2<256>>::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::hmac<gtl::sha2<256>>::block_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::hmac<gtl::sha2<256>> hmac(key);
        hmac.consume(&data[i][0], testbench::string_length(&data[i][0]));
        hmac.finalise();

        PRINT("%s == %s\n", gtl::hmac<gtl::sha2<256>>::hash_to_string(hmac.get_hash()).hash, result[i]);
        REQUIRE(testbench::is_string_same(gtl::hmac<gtl::sha2<256>>::hash_to_string(hmac.get_hash()).hash, result[i]) == true);
    }
}

TEST(hmac, evaluate, hash_as_integer_sha384) {
    constexpr static const unsigned int data_count = 2;

    constexpr static const char* data[data_count] = {
        "Hi There",
        "what do ya want for nothing?"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::hmac<gtl::sha2<384>>::block_size] = {
        { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b },
        { 'J', 'e', 'f', 'e' }
    };

    constexpr static unsigned char result[data_count][gtl::hmac<gtl::sha2<384>>::hash_size] = {
        { 0xaf, 0xd0, 0x39, 0x44, 0xd8, 0x48, 0x95, 0x62, 0x6b, 0x08, 0x25, 0xf4, 0xab, 0x46, 0x90, 0x7f, 0x15, 0xf9, 0xda, 0xdb, 0xe4, 0x10, 0x1e, 0xc6, 0x82, 0xaa, 0x03, 0x4c, 0x7c, 0xeb, 0xc5, 0x9c, 0xfa, 0xea, 0x9e, 0xa9, 0x07, 0x6e, 0xde, 0x7f, 0x4a, 0xf1, 0x52, 0xe8, 0xb2, 0xfa, 0x9c, 0xb6 },
        { 0xaf, 0x45, 0xd2, 0xe3, 0x76, 0x48, 0x40, 0x31, 0x61, 0x7f, 0x78, 0xd2, 0xb5, 0x8a, 0x6b, 0x1b, 0x9c, 0x7e, 0xf4, 0x64, 0xf5, 0xa0, 0x1b, 0x47, 0xe4, 0x2e, 0xc3, 0x73, 0x63, 0x22, 0x44, 0x5e, 0x8e, 0x22, 0x40, 0xca, 0x5e, 0x69, 0xe2, 0xc7, 0x8b, 0x32, 0x39, 0xec, 0xfa, 0xb2, 0x16, 0x49 }
    };

    gtl::hmac<gtl::sha2<384>>::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::hmac<gtl::sha2<384>>::block_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::hmac<gtl::sha2<384>> hmac(key);
        hmac.consume(&data[i][0], testbench::string_length(&data[i][0]));
        hmac.finalise();

        gtl::hmac<gtl::sha2<384>>::hash_type hash = hmac.get_hash();

        char hash_string[(2 * gtl::hmac<gtl::sha2<384>>::hash_size) + 1] = {};
        char result_string[(2 * gtl::hmac<gtl::sha2<384>>::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::hmac<gtl::sha2<384>>::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::hmac<gtl::sha2<384>>::hash_size) == true);
    }
}

TEST(hmac, evaluate, hash_as_integer_sha512) {
    constexpr static const unsigned int data_count = 2;

    constexpr static const char* data[data_count] = {
        "Hi There",
        "what do ya want for nothing?"
    };

    constexpr static unsigned char key_bytes[data_count][gtl::hmac<gtl::sha2<512>>::block_size] = {
        { 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b },
        { 'J', 'e', 'f', 'e' }
    };

    constexpr static unsigned char result[data_count][gtl::hmac<gtl::sha2<512>>::hash_size] = {
        { 0x87, 0xaa, 0x7c, 0xde, 0xa5, 0xef, 0x61, 0x9d, 0x4f, 0xf0, 0xb4, 0x24, 0x1a, 0x1d, 0x6c, 0xb0, 0x23, 0x79, 0xf4, 0xe2, 0xce, 0x4e, 0xc2, 0x78, 0x7a, 0xd0, 0xb3, 0x05, 0x45, 0xe1, 0x7c, 0xde, 0xda, 0xa8, 0x33, 0xb7, 0xd6, 0xb8, 0xa7, 0x02, 0x03, 0x8b, 0x27, 0x4e, 0xae, 0xa3, 0xf4, 0xe4, 0xbe, 0x9d, 0x91, 0x4e, 0xeb, 0x61, 0xf1, 0x70, 0x2e, 0x69, 0x6c, 0x20, 0x3a, 0x12, 0x68, 0x54 },
        { 0x16, 0x4b, 0x7a, 0x7b, 0xfc, 0xf8, 0x19, 0xe2, 0xe3, 0x95, 0xfb, 0xe7, 0x3b, 0x56, 0xe0, 0xa3, 0x87, 0xbd, 0x64, 0x22, 0x2e, 0x83, 0x1f, 0xd6, 0x10, 0x27, 0x0c, 0xd7, 0xea, 0x25, 0x05, 0x54, 0x97, 0x58, 0xbf, 0x75, 0xc0, 0x5a, 0x99, 0x4a, 0x6d, 0x03, 0x4f, 0x65, 0xf8, 0xf0, 0xe6, 0xfd, 0xca, 0xea, 0xb1, 0xa3, 0x4d, 0x4a, 0x6b, 0x4b, 0x63, 0x6e, 0x07, 0x0a, 0x38, 0xbc, 0xe7, 0x37 }
    };

    gtl::hmac<gtl::sha2<512>>::key_type key;
    for (unsigned int i = 0; i < data_count; ++i) {
        for (unsigned int j = 0; j < gtl::hmac<gtl::sha2<512>>::block_size; ++j) {
            key.data[j] = key_bytes[i][j];
        }
        gtl::hmac<gtl::sha2<512>> hmac(key);
        hmac.consume(&data[i][0], testbench::string_length(&data[i][0]));
        hmac.finalise();

        gtl::hmac<gtl::sha2<512>>::hash_type hash = hmac.get_hash();

        char hash_string[(2 * gtl::hmac<gtl::sha2<512>>::hash_size) + 1] = {};
        char result_string[(2 * gtl::hmac<gtl::sha2<512>>::hash_size) + 1] = {};
        for (size_t index = 0; index < gtl::hmac<gtl::sha2<512>>::hash_size; ++index) {
            snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
            snprintf(result_string + (index * 2), 3, "%02X", result[i][index]);
        }
        PRINT("%s == %s\n", hash_string, result_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::hmac<gtl::sha2<512>>::hash_size) == true);
    }
}

TEST(hmac, evaluate, partial_insert) {
    constexpr static const char* data1 = "what do ya";
    constexpr static const char* data2 = " want for nothing?";

    constexpr static unsigned char key_bytes[gtl::hmac<gtl::sha2<256>>::block_size] = {
        'J',
        'e',
        'f',
        'e'
    };

    constexpr static unsigned char result[gtl::hmac<gtl::sha2<256>>::hash_size] = {
        0x5b,
        0xdc,
        0xc1,
        0x46,
        0xbf,
        0x60,
        0x75,
        0x4e,
        0x6a,
        0x04,
        0x24,
        0x26,
        0x08,
        0x95,
        0x75,
        0xc7,
        0x5a,
        0x00,
        0x3f,
        0x08,
        0x9d,
        0x27,
        0x39,
        0x83,
        0x9d,
        0xec,
        0x58,
        0xb9,
        0x64,
        0xec,
        0x38,
        0x43
    };

    gtl::hmac<gtl::sha2<256>>::key_type key;
    for (unsigned int j = 0; j < gtl::hmac<gtl::sha2<256>>::block_size; ++j) {
        key.data[j] = key_bytes[j];
    }
    gtl::hmac<gtl::sha2<256>> hmac(key);
    hmac.consume(&data1[0], testbench::string_length(&data1[0]));
    hmac.consume(&data2[0], testbench::string_length(&data2[0]));
    hmac.finalise();

    gtl::hmac<gtl::sha2<256>>::hash_type hash = hmac.get_hash();

    char hash_string[(2 * gtl::hmac<gtl::sha2<256>>::hash_size) + 1] = {};
    char result_string[(2 * gtl::hmac<gtl::sha2<256>>::hash_size) + 1] = {};
    for (size_t index = 0; index < gtl::hmac<gtl::sha2<256>>::hash_size; ++index) {
        snprintf(hash_string + (index * 2), 3, "%02X", hash.data[index]);
        snprintf(result_string + (index * 2), 3, "%02X", result[index]);
    }
    PRINT("%s == %s\n", hash_string, result_string);

    REQUIRE(testbench::is_memory_same(hash.data, result, gtl::hmac<gtl::sha2<256>>::hash_size) == true);
}
