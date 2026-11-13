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
#include <testbench/template.tests.hpp>

#include <crypto/aes>
#include <crypto/block_cipher>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <string>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

static std::string hex_2_string(const unsigned char* hex, const unsigned int length) {
    std::string result;
    for (unsigned int i = 0; i < length; ++i) {
        result.push_back("0123456789ABCDEF"[hex[i] / 16]);
        result.push_back("0123456789ABCDEF"[hex[i] % 16]);
    }
    return result;
}

using aes_key_sizes = testbench::value_collection<128, 192, 256>;
using aes_block_modes = testbench::enum_collection<
    gtl::block_cipher_mode,
    gtl::block_cipher_mode::ecb,
    gtl::block_cipher_mode::cbc,
    gtl::block_cipher_mode::cfb,
    gtl::block_cipher_mode::ofb,
    gtl::block_cipher_mode::ctr,
    gtl::block_cipher_mode::gcm,
    gtl::block_cipher_mode::ccm,
    gtl::block_cipher_mode::siv
>;

TEST(block_cipher, traits, standard) {
    testbench::test_template<aes_key_sizes, aes_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            REQUIRE((std::is_pod<gtl::block_cipher<gtl::aes<value>, mode>>::value == true));
            REQUIRE((std::is_trivial<gtl::block_cipher<gtl::aes<value>, mode>>::value == true));
            REQUIRE((std::is_trivially_copyable<gtl::block_cipher<gtl::aes<value>, mode>>::value == true));
            REQUIRE((std::is_standard_layout<gtl::block_cipher<gtl::aes<value>, mode>>::value == true));
        }
    );
}

TEST(block_cipher, constructor, empty) {
    testbench::test_template<aes_key_sizes, aes_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            gtl::block_cipher<gtl::aes<value>, mode> bc;
            testbench::do_not_optimise_away(bc);
        }
    );
}

TEST(block_cipher, encrypt_decrypt, one_block) {
    testbench::test_template<aes_key_sizes, aes_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            using bc = gtl::block_cipher<gtl::aes<value>, mode>;
            constexpr static const unsigned int length = 16;
            unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
            unsigned char key[2 * 256 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
            const unsigned char iv[bc::block_size * bc::block_size] = { 0 };

            unsigned char output[length];
            unsigned char input[length];
            if constexpr (mode == gtl::block_cipher_mode::ecb) {
                bc::encrypt(plain, length, key, output);
                bc::decrypt(output, length, key, input);
            }
            else if constexpr (mode == gtl::block_cipher_mode::cbc || mode == gtl::block_cipher_mode::cfb || mode == gtl::block_cipher_mode::ofb || mode == gtl::block_cipher_mode::ctr) {
                bc::encrypt(plain, length, key, iv, output);
                bc::decrypt(output, length, key, iv, input);
            }
            else {
                unsigned char tag[bc::block_size * bc::block_size] = { 0 };
                bc::encrypt(plain, length, key, iv, output, tag);
                bc::decrypt(output, length, key, iv, input, tag);
            }

            REQUIRE(testbench::is_memory_same(plain, input, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(input, length).c_str());
        }
    );
}

TEST(block_cipher, encrypt_decrypt, two_blocks) {
    testbench::test_template<aes_key_sizes, aes_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            using bc = gtl::block_cipher<gtl::aes<value>, mode>;
            constexpr static const unsigned int length = 32;
            unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
            unsigned char key[2 * 256 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
            const unsigned char iv[bc::block_size * bc::block_size] = { 0 };

            unsigned char output[length];
            unsigned char input[length];
            if constexpr (mode == gtl::block_cipher_mode::ecb) {
                bc::encrypt(plain, length, key, output);
                bc::decrypt(output, length, key, input);
            }
            else if constexpr (mode == gtl::block_cipher_mode::cbc || mode == gtl::block_cipher_mode::cfb || mode == gtl::block_cipher_mode::ofb || mode == gtl::block_cipher_mode::ctr) {
                bc::encrypt(plain, length, key, iv, output);
                bc::decrypt(output, length, key, iv, input);
            }
            else {
                unsigned char tag[bc::block_size * bc::block_size] = { 0 };
                bc::encrypt(plain, length, key, iv, output, tag);
                bc::decrypt(output, length, key, iv, input, tag);
            }

            REQUIRE(testbench::is_memory_same(plain, input, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(input, length).c_str());
        }
    );
}

TEST(block_cipher, verify_encrypt, ecb) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ecb>;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A, 0x07, 0xFE, 0xEF, 0x74, 0xE1, 0xD5, 0x03, 0x6E, 0x90, 0x0E, 0xEE, 0x11, 0x8E, 0x94, 0x92, 0x93 };

    unsigned char output[length];
    bc::encrypt(plain, length, key, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_encrypt, cbc) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cbc>;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x1B, 0x87, 0x23, 0x78, 0x79, 0x5F, 0x4F, 0xFD, 0x77, 0x28, 0x55, 0xFC, 0x87, 0xCA, 0x96, 0x4D, 0x4C, 0x5B, 0xCA, 0x1C, 0x48, 0xCD, 0x88, 0x00, 0x3A, 0x10, 0x52, 0x11, 0x88, 0x12, 0x5E, 0x00 };

    unsigned char output[length];
    bc::encrypt(plain, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_encrypt, cfb) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cfb>;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xE3, 0x5A, 0x40, 0xAB, 0x30, 0x29, 0xF3, 0x0C, 0xE1, 0xDB, 0x30, 0x6C, 0xA1, 0x05, 0xCB, 0xA9 };

    unsigned char output[length];
    bc::encrypt(plain, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_encrypt, ofb) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ofb>;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xA3, 0x01, 0xDE, 0xC0, 0x19, 0xAE, 0xA5, 0xAA, 0x23, 0x3F, 0x27, 0x7E, 0x01, 0x50, 0x85, 0x29 };

    unsigned char output[length];
    bc::encrypt(plain, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_encrypt, ctr) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ctr>;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xC6, 0xB0, 0x19, 0x04, 0xC3, 0xDA, 0x3D, 0xF5, 0xE7, 0xD6, 0x2B, 0xD9, 0x6D, 0x15, 0x36, 0x86 };

    unsigned char output[length];
    bc::encrypt(plain, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_encrypt, gcm) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::gcm>;
    constexpr static const unsigned int plain_length = 32;
    constexpr static const unsigned int iv_length = 12;
    constexpr static const unsigned int tag_length = 16;
    constexpr static const unsigned int aad_length = 16;
    const unsigned char plain[plain_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[iv_length] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
    const unsigned char encrypted_plain[plain_length] = { 0xF1, 0xDF, 0x0F, 0xE7, 0xCF, 0x2B, 0xB7, 0xF6, 0x5D, 0x08, 0x36, 0xAB, 0xFB, 0x34, 0x76, 0xB2, 0xEB, 0x4C, 0xA8, 0x4B, 0xB7, 0xD0, 0x09, 0xB7, 0x38, 0x1C, 0xA1, 0xCE, 0x3A, 0xDA, 0x4E, 0xEE };
    const unsigned char encrypted_tag[tag_length] = { 0xAD, 0xB6, 0x51, 0x3B, 0x5C, 0x3C, 0x71, 0xDD, 0x9C, 0x8D, 0xD9, 0xDE, 0x94, 0x0E, 0xA3, 0x42 };

    unsigned char output_data[plain_length];
    unsigned char output_tag[tag_length];
    bc::encrypt(plain, plain_length, key, iv, output_data, output_tag, aad, aad_length);

    REQUIRE(testbench::is_memory_same(encrypted_plain, output_data, plain_length), "%s != %s\n", hex_2_string(encrypted_plain, plain_length).c_str(), hex_2_string(output_data, plain_length).c_str());
    REQUIRE(testbench::is_memory_same(encrypted_tag, output_tag, tag_length), "%s != %s\n", hex_2_string(encrypted_tag, tag_length).c_str(), hex_2_string(output_tag, tag_length).c_str());
}

TEST(block_cipher, verify_encrypt, ccm) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ccm>;
    constexpr static const unsigned int plain_length = 32;
    constexpr static const unsigned int iv_length = 12;
    constexpr static const unsigned int tag_length = 16;
    constexpr static const unsigned int aad_length = 16;
    const unsigned char plain[plain_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[iv_length] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
    const unsigned char encrypted_plain[plain_length] = { 0x22, 0x5B, 0xC6, 0x6D, 0x77, 0xE1, 0x62, 0x53, 0x4E, 0x21, 0x8B, 0xE9, 0x74, 0xF9, 0x44, 0x0D, 0xE8, 0x57, 0x4C, 0x47, 0x53, 0xC8, 0x32, 0x91, 0xE7, 0x7B, 0xC0, 0xF4, 0x1C, 0x6F, 0x17, 0x78 };
    const unsigned char encrypted_tag[tag_length] = { 0x9B, 0xDD, 0x03, 0x1E, 0x3F, 0xCE, 0x61, 0x1B, 0x01, 0xE4, 0x16, 0x5C, 0xD5, 0x97, 0xBA, 0xDB };

    unsigned char output_data[plain_length];
    unsigned char output_tag[tag_length];
    bc::encrypt(plain, plain_length, key, iv, output_data, output_tag, aad, aad_length);

    REQUIRE(testbench::is_memory_same(encrypted_plain, output_data, plain_length), "%s != %s\n", hex_2_string(encrypted_plain, plain_length).c_str(), hex_2_string(output_data, plain_length).c_str());
    REQUIRE(testbench::is_memory_same(encrypted_tag, output_tag, tag_length), "%s != %s\n", hex_2_string(encrypted_tag, tag_length).c_str(), hex_2_string(output_tag, tag_length).c_str());
}

TEST(block_cipher, verify_encrypt, siv) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::siv>;
    constexpr static const unsigned int plain_length = 32;
    constexpr static const unsigned int iv_length = 12;
    constexpr static const unsigned int tag_length = 16;
    constexpr static const unsigned int aad_length = 16;
    const unsigned char plain[plain_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[iv_length] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    // The mode uses two keys, the first to authenticate and the second to encrypt.
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
    const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
    const unsigned char encrypted_plain[plain_length] = { 0x93, 0xEA, 0xF1, 0xF6, 0x5E, 0x2C, 0x7A, 0xBA, 0x45, 0xAC, 0x97, 0xCF, 0xB3, 0x0A, 0x0E, 0xDB, 0x7E, 0xD7, 0x97, 0xB3, 0x60, 0x80, 0x33, 0x2F, 0x3F, 0x1D, 0x2D, 0x6D, 0xB4, 0x82, 0x8C, 0x66 };
    const unsigned char encrypted_tag[tag_length] = { 0x5B, 0x50, 0x5E, 0x8B, 0xEF, 0x17, 0xCF, 0xB2, 0x7C, 0x69, 0xB7, 0xD9, 0x95, 0x0E, 0x37, 0x21 };

    unsigned char output_data[plain_length];
    unsigned char output_tag[tag_length];
    bc::encrypt(plain, plain_length, key, iv, output_data, output_tag, aad, aad_length);

    REQUIRE(testbench::is_memory_same(encrypted_plain, output_data, plain_length), "%s != %s\n", hex_2_string(encrypted_plain, plain_length).c_str(), hex_2_string(output_data, plain_length).c_str());
    REQUIRE(testbench::is_memory_same(encrypted_tag, output_tag, tag_length), "%s != %s\n", hex_2_string(encrypted_tag, tag_length).c_str(), hex_2_string(output_tag, tag_length).c_str());
}

TEST(block_cipher, verify_decrypt, ecb) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ecb>;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A, 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    bc::decrypt(encrypted, length, key, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_decrypt, cbc) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cbc>;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x1B, 0x87, 0x23, 0x78, 0x79, 0x5F, 0x4F, 0xFD, 0x77, 0x28, 0x55, 0xFC, 0x87, 0xCA, 0x96, 0x4D, 0x4C, 0x5B, 0xCA, 0x1C, 0x48, 0xCD, 0x88, 0x00, 0x3A, 0x10, 0x52, 0x11, 0x88, 0x12, 0x5E, 0x00 };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    bc::decrypt(encrypted, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_decrypt, cfb) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cfb>;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xE3, 0x5A, 0x40, 0xAB, 0x30, 0x29, 0xF3, 0x0C, 0xE1, 0xDB, 0x30, 0x6C, 0xA1, 0x05, 0xCB, 0xA9 };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    bc::decrypt(encrypted, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_decrypt, ofb) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ofb>;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xA3, 0x01, 0xDE, 0xC0, 0x19, 0xAE, 0xA5, 0xAA, 0x23, 0x3F, 0x27, 0x7E, 0x01, 0x50, 0x85, 0x29 };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    bc::decrypt(encrypted, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_decrypt, ctr) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ctr>;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xC6, 0xB0, 0x19, 0x04, 0xC3, 0xDA, 0x3D, 0xF5, 0xE7, 0xD6, 0x2B, 0xD9, 0x6D, 0x15, 0x36, 0x86 };
    const unsigned char iv[bc::block_size * bc::block_size] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    bc::decrypt(encrypted, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(block_cipher, verify_decrypt, gcm) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::gcm>;
    constexpr static const unsigned int plain_length = 32;
    constexpr static const unsigned int iv_length = 12;
    constexpr static const unsigned int tag_length = 16;
    constexpr static const unsigned int aad_length = 16;
    const unsigned char plain[plain_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[iv_length] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
    const unsigned char encrypted_plain[plain_length] = { 0xF1, 0xDF, 0x0F, 0xE7, 0xCF, 0x2B, 0xB7, 0xF6, 0x5D, 0x08, 0x36, 0xAB, 0xFB, 0x34, 0x76, 0xB2, 0xEB, 0x4C, 0xA8, 0x4B, 0xB7, 0xD0, 0x09, 0xB7, 0x38, 0x1C, 0xA1, 0xCE, 0x3A, 0xDA, 0x4E, 0xEE };
    const unsigned char encrypted_tag[tag_length] = { 0xAD, 0xB6, 0x51, 0x3B, 0x5C, 0x3C, 0x71, 0xDD, 0x9C, 0x8D, 0xD9, 0xDE, 0x94, 0x0E, 0xA3, 0x42 };

    unsigned char output[plain_length];
    bc::decrypt(encrypted_plain, plain_length, key, iv, output, encrypted_tag, aad, aad_length);

    REQUIRE(testbench::is_memory_same(plain, output, plain_length), "%s != %s\n", hex_2_string(plain, plain_length).c_str(), hex_2_string(output, plain_length).c_str());
}

TEST(block_cipher, verify_decrypt, ccm) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ccm>;
    constexpr static const unsigned int plain_length = 32;
    constexpr static const unsigned int iv_length = 12;
    constexpr static const unsigned int tag_length = 16;
    constexpr static const unsigned int aad_length = 16;
    const unsigned char plain[plain_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[iv_length] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
    const unsigned char encrypted_plain[plain_length] = { 0x22, 0x5B, 0xC6, 0x6D, 0x77, 0xE1, 0x62, 0x53, 0x4E, 0x21, 0x8B, 0xE9, 0x74, 0xF9, 0x44, 0x0D, 0xE8, 0x57, 0x4C, 0x47, 0x53, 0xC8, 0x32, 0x91, 0xE7, 0x7B, 0xC0, 0xF4, 0x1C, 0x6F, 0x17, 0x78 };
    const unsigned char encrypted_tag[tag_length] = { 0x9B, 0xDD, 0x03, 0x1E, 0x3F, 0xCE, 0x61, 0x1B, 0x01, 0xE4, 0x16, 0x5C, 0xD5, 0x97, 0xBA, 0xDB };

    unsigned char output[plain_length];
    bc::decrypt(encrypted_plain, plain_length, key, iv, output, encrypted_tag, aad, aad_length);

    REQUIRE(testbench::is_memory_same(plain, output, plain_length), "%s != %s\n", hex_2_string(plain, plain_length).c_str(), hex_2_string(output, plain_length).c_str());
}

TEST(block_cipher, verify_decrypt, siv) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::siv>;
    constexpr static const unsigned int plain_length = 32;
    constexpr static const unsigned int iv_length = 12;
    constexpr static const unsigned int tag_length = 16;
    constexpr static const unsigned int aad_length = 16;
    const unsigned char plain[plain_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[iv_length] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    // The mode uses two keys, the first to authenticate and the second to encrypt.
    const unsigned char key[bc::key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
    const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
    const unsigned char encrypted_plain[plain_length] = { 0x93, 0xEA, 0xF1, 0xF6, 0x5E, 0x2C, 0x7A, 0xBA, 0x45, 0xAC, 0x97, 0xCF, 0xB3, 0x0A, 0x0E, 0xDB, 0x7E, 0xD7, 0x97, 0xB3, 0x60, 0x80, 0x33, 0x2F, 0x3F, 0x1D, 0x2D, 0x6D, 0xB4, 0x82, 0x8C, 0x66 };
    const unsigned char encrypted_tag[tag_length] = { 0x5B, 0x50, 0x5E, 0x8B, 0xEF, 0x17, 0xCF, 0xB2, 0x7C, 0x69, 0xB7, 0xD9, 0x95, 0x0E, 0x37, 0x21 };

    unsigned char output[plain_length];
    bc::decrypt(encrypted_plain, plain_length, key, iv, output, encrypted_tag, aad, aad_length);

    REQUIRE(testbench::is_memory_same(plain, output, plain_length), "%s != %s\n", hex_2_string(plain, plain_length).c_str(), hex_2_string(output, plain_length).c_str());
}

using aes_authenticated_block_modes = testbench::enum_collection<
    gtl::block_cipher_mode,
    gtl::block_cipher_mode::gcm,
    gtl::block_cipher_mode::ccm,
    gtl::block_cipher_mode::siv
>;

TEST(block_cipher, authenticate, without_aad) {
    testbench::test_template<aes_key_sizes, aes_authenticated_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            using bc = gtl::block_cipher<gtl::aes<value>, mode>;
            constexpr static const unsigned int length = 32;
            constexpr static const unsigned int tag_length = 16;
            const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
            const unsigned char key[2 * 256 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
            const unsigned char iv[bc::block_size * bc::block_size] = { 0 };
            const unsigned char zeros[length] = { 0 };

            unsigned char encrypted[length];
            unsigned char tag[tag_length] = { 0 };
            bc::encrypt(plain, length, key, iv, encrypted, tag);

            // The untouched data must decrypt and authenticate.
            unsigned char output[length];
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag) == true);
            REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());

            // A missing tag must be rejected.
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, nullptr) == false);
            REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());

            // An incorrect tag must be rejected.
            unsigned char tag_zero[tag_length] = { 0 };
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag_zero) == false);
            REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());

            for (unsigned int i = 0; i < tag_length; ++i) {
                unsigned char tag_tampered[tag_length];
                for (unsigned int j = 0; j < tag_length; ++j) {
                    tag_tampered[j] = tag[j];
                }
                tag_tampered[i] ^= 0x01;
                REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag_tampered) == false);
                REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());
            }

            // Tampered data must be rejected.
            for (unsigned int i = 0; i < length; ++i) {
                unsigned char encrypted_tampered[length];
                for (unsigned int j = 0; j < length; ++j) {
                    encrypted_tampered[j] = encrypted[j];
                }
                encrypted_tampered[i] ^= 0x80;
                REQUIRE(bc::decrypt(encrypted_tampered, length, key, iv, output, tag) == false);
                REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());
            }

            // Unexpected additional authenticated data must be rejected.
            const unsigned char aad[4] = { 0x01, 0x02, 0x03, 0x04 };
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag, aad, 4) == false);
            REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());
        }
    );
}

TEST(block_cipher, authenticate, with_aad) {
    testbench::test_template<aes_key_sizes, aes_authenticated_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            using bc = gtl::block_cipher<gtl::aes<value>, mode>;
            constexpr static const unsigned int length = 32;
            constexpr static const unsigned int tag_length = 16;
            constexpr static const unsigned int aad_length = 20;
            const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
            const unsigned char key[2 * 256 / 8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
            const unsigned char iv[bc::block_size * bc::block_size] = { 0 };
            const unsigned char aad[aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00, 0x0F, 0x0E, 0x0D, 0x0C };
            const unsigned char zeros[length] = { 0 };

            unsigned char encrypted[length];
            unsigned char tag[tag_length] = { 0 };
            bc::encrypt(plain, length, key, iv, encrypted, tag, aad, aad_length);

            // The untouched data must decrypt and authenticate.
            unsigned char output[length];
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag, aad, aad_length) == true);
            REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());

            // A missing tag must be rejected.
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, nullptr, aad, aad_length) == false);
            REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());

            // Missing additional authenticated data must be rejected.
            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag) == false);
            REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());

            REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag, aad, 0) == false);
            REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());

            // Tampered additional authenticated data must be rejected.
            for (unsigned int i = 0; i < aad_length; ++i) {
                unsigned char aad_tampered[aad_length];
                for (unsigned int j = 0; j < aad_length; ++j) {
                    aad_tampered[j] = aad[j];
                }
                aad_tampered[i] ^= 0x01;
                REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag, aad_tampered, aad_length) == false);
                REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());
            }

            // Tampered data must be rejected.
            for (unsigned int i = 0; i < length; ++i) {
                unsigned char encrypted_tampered[length];
                for (unsigned int j = 0; j < length; ++j) {
                    encrypted_tampered[j] = encrypted[j];
                }
                encrypted_tampered[i] ^= 0x80;
                REQUIRE(bc::decrypt(encrypted_tampered, length, key, iv, output, tag, aad, aad_length) == false);
                REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());
            }

            // An incorrect tag must be rejected.
            for (unsigned int i = 0; i < tag_length; ++i) {
                unsigned char tag_tampered[tag_length];
                for (unsigned int j = 0; j < tag_length; ++j) {
                    tag_tampered[j] = tag[j];
                }
                tag_tampered[i] ^= 0x01;
                REQUIRE(bc::decrypt(encrypted, length, key, iv, output, tag_tampered, aad, aad_length) == false);
                REQUIRE(testbench::is_memory_same(zeros, output, length), "%s != %s\n", hex_2_string(zeros, length).c_str(), hex_2_string(output, length).c_str());
            }
        }
    );
}

template <unsigned long long int value, gtl::block_cipher_mode mode>
static bool encrypt_any_mode(const unsigned char* data, const unsigned int length, const unsigned char* key, const unsigned char* iv, unsigned char* output, unsigned char* tag) {
    using bc = gtl::block_cipher<gtl::aes<value>, mode>;
    if constexpr (mode == gtl::block_cipher_mode::ecb) {
        static_cast<void>(iv);
        static_cast<void>(tag);
        return bc::encrypt(data, length, key, output);
    }
    else if constexpr (mode == gtl::block_cipher_mode::cbc) {
        static_cast<void>(tag);
        return bc::encrypt(data, length, key, iv, output);
    }
    else if constexpr (mode == gtl::block_cipher_mode::cfb || mode == gtl::block_cipher_mode::ofb || mode == gtl::block_cipher_mode::ctr) {
        static_cast<void>(tag);
        bc::encrypt(data, length, key, iv, output);
        return true;
    }
    else if constexpr (mode == gtl::block_cipher_mode::gcm) {
        bc::encrypt(data, length, key, iv, output, tag);
        return true;
    }
    else {
        return bc::encrypt(data, length, key, iv, output, tag);
    }
}

template <unsigned long long int value, gtl::block_cipher_mode mode>
static bool decrypt_any_mode(const unsigned char* data, const unsigned int length, const unsigned char* key, const unsigned char* iv, unsigned char* output, const unsigned char* tag) {
    using bc = gtl::block_cipher<gtl::aes<value>, mode>;
    if constexpr (mode == gtl::block_cipher_mode::ecb) {
        static_cast<void>(iv);
        static_cast<void>(tag);
        return bc::decrypt(data, length, key, output);
    }
    else if constexpr (mode == gtl::block_cipher_mode::cbc) {
        static_cast<void>(tag);
        return bc::decrypt(data, length, key, iv, output);
    }
    else if constexpr (mode == gtl::block_cipher_mode::cfb || mode == gtl::block_cipher_mode::ofb || mode == gtl::block_cipher_mode::ctr) {
        static_cast<void>(tag);
        bc::decrypt(data, length, key, iv, output);
        return true;
    }
    else {
        return bc::decrypt(data, length, key, iv, output, tag);
    }
}

TEST(block_cipher, parameter, sizes) {
    testbench::test_template<aes_key_sizes, aes_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            using bc = gtl::block_cipher<gtl::aes<value>, mode>;
            REQUIRE(bc::block_size == 4);
            REQUIRE(bc::block_size_bytes == 16);

            if constexpr (mode == gtl::block_cipher_mode::siv) {
                // Two keys are used, the first to authenticate and the second to encrypt.
                REQUIRE(bc::key_size == 2 * value / 8);
            }
            else {
                REQUIRE(bc::key_size == value / 8);
            }

            if constexpr (mode == gtl::block_cipher_mode::cbc || mode == gtl::block_cipher_mode::cfb || mode == gtl::block_cipher_mode::ofb || mode == gtl::block_cipher_mode::ctr) {
                REQUIRE(bc::iv_size == 16);
            }
            else if constexpr (mode == gtl::block_cipher_mode::gcm || mode == gtl::block_cipher_mode::ccm || mode == gtl::block_cipher_mode::siv) {
                REQUIRE(bc::iv_size == 12);
                REQUIRE(bc::tag_size == 16);
            }

            if constexpr (mode == gtl::block_cipher_mode::ccm) {
                REQUIRE(bc::length_max == 0xFFFFFF);
            }
        }
    );
}

// The data shared by the tests of lengths that are not a multiple of the block size.
// The expected values of these tests were computed independently of this library.
constexpr static const unsigned int partial_length = 33;
constexpr static const unsigned int partial_aad_length = 20;
static const unsigned char partial_plain[partial_length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x21, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98, 0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x0F, 0x20 };
static const unsigned char partial_key[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
static const unsigned char partial_iv_16[16] = { 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };
static const unsigned char partial_iv_12[12] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB };
static const unsigned char partial_aad[partial_aad_length] = { 0xF0, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80, 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00, 0xF0, 0xE0, 0xD0, 0xC0 };

TEST(block_cipher, verify_partial_block, cfb_ofb_ctr) {
    const unsigned char encrypted_cfb[partial_length] = { 0x66, 0xB6, 0xE5, 0xDB, 0x70, 0x07, 0x57, 0x3F, 0x1F, 0xC8, 0x74, 0xBC, 0xFF, 0xCB, 0x43, 0x52, 0xDE, 0x0E, 0x73, 0x08, 0x9A, 0x33, 0xD6, 0x41, 0x5F, 0xDA, 0xC3, 0xFE, 0x93, 0xD7, 0xD1, 0x6E, 0xB5 };
    const unsigned char encrypted_ofb[partial_length] = { 0x66, 0xB6, 0xE5, 0xDB, 0x70, 0x07, 0x57, 0x3F, 0x1F, 0xC8, 0x74, 0xBC, 0xFF, 0xCB, 0x43, 0x52, 0x7E, 0x40, 0xAB, 0xF9, 0x02, 0xB0, 0xFA, 0xD5, 0x93, 0xC7, 0xDF, 0xDD, 0x2D, 0x27, 0x7F, 0xA5, 0xFF };
    const unsigned char encrypted_ctr[partial_length] = { 0x66, 0xB6, 0xE5, 0xDB, 0x70, 0x07, 0x57, 0x3F, 0x1F, 0xC8, 0x74, 0xBC, 0xFF, 0xCB, 0x43, 0x52, 0xA2, 0xA0, 0xE5, 0x43, 0xE3, 0xFB, 0x4A, 0x2A, 0x3C, 0x04, 0xC9, 0x70, 0xB2, 0x71, 0xE1, 0xE5, 0xF2 };

    unsigned char output[partial_length] = {};

    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cfb>::encrypt(partial_plain, partial_length, partial_key, partial_iv_16, output);
    REQUIRE(testbench::is_memory_same(encrypted_cfb, output, partial_length), "%s != %s\n", hex_2_string(encrypted_cfb, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cfb>::decrypt(encrypted_cfb, partial_length, partial_key, partial_iv_16, output);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ofb>::encrypt(partial_plain, partial_length, partial_key, partial_iv_16, output);
    REQUIRE(testbench::is_memory_same(encrypted_ofb, output, partial_length), "%s != %s\n", hex_2_string(encrypted_ofb, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ofb>::decrypt(encrypted_ofb, partial_length, partial_key, partial_iv_16, output);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ctr>::encrypt(partial_plain, partial_length, partial_key, partial_iv_16, output);
    REQUIRE(testbench::is_memory_same(encrypted_ctr, output, partial_length), "%s != %s\n", hex_2_string(encrypted_ctr, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ctr>::decrypt(encrypted_ctr, partial_length, partial_key, partial_iv_16, output);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
}

TEST(block_cipher, verify_partial_block, gcm) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::gcm>;
    const unsigned char encrypted[partial_length] = { 0xAA, 0x97, 0x1A, 0x88, 0x3A, 0xDC, 0x55, 0x7D, 0x02, 0xE1, 0x1F, 0xBB, 0x8A, 0xCF, 0x5E, 0x9F, 0x43, 0xCF, 0x11, 0x4B, 0xD7, 0x72, 0x4B, 0x28, 0xBD, 0xB9, 0xCB, 0x50, 0x9E, 0x60, 0x71, 0xA7, 0xBA };
    const unsigned char tag_without_aad[bc::tag_size] = { 0x51, 0xA9, 0x97, 0xD4, 0xCE, 0xD9, 0xBB, 0xA4, 0xB5, 0x42, 0x3F, 0xC6, 0x4E, 0xFC, 0xEA, 0x64 };
    const unsigned char tag_with_aad[bc::tag_size] = { 0xF9, 0x01, 0xD2, 0x60, 0x6A, 0xA1, 0x0E, 0x2D, 0x03, 0x15, 0x0F, 0x64, 0xEF, 0x19, 0x4A, 0x85 };

    unsigned char output[partial_length] = {};
    unsigned char tag[bc::tag_size] = {};

    bc::encrypt(partial_plain, partial_length, partial_key, partial_iv_12, output, tag);
    REQUIRE(testbench::is_memory_same(encrypted, output, partial_length), "%s != %s\n", hex_2_string(encrypted, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_without_aad, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_without_aad, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted, partial_length, partial_key, partial_iv_12, output, tag_without_aad) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    bc::encrypt(partial_plain, partial_length, partial_key, partial_iv_12, output, tag, partial_aad, partial_aad_length);
    REQUIRE(testbench::is_memory_same(encrypted, output, partial_length), "%s != %s\n", hex_2_string(encrypted, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_with_aad, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_with_aad, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted, partial_length, partial_key, partial_iv_12, output, tag_with_aad, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
}

TEST(block_cipher, verify_partial_block, ccm) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::ccm>;
    const unsigned char encrypted[partial_length] = { 0xAE, 0xA2, 0x4A, 0xE2, 0xE1, 0x6A, 0xAC, 0xF6, 0x5A, 0x5F, 0xAA, 0x46, 0x56, 0xED, 0x37, 0x1C, 0x5B, 0xD6, 0x1D, 0x8A, 0xE5, 0xEE, 0xAC, 0x87, 0x17, 0x7E, 0x8A, 0xC7, 0x79, 0xC1, 0x16, 0xCA, 0x0C };
    const unsigned char tag_without_aad[bc::tag_size] = { 0x21, 0x0A, 0x8C, 0x93, 0x1C, 0x6B, 0xF0, 0x13, 0x83, 0x39, 0x98, 0xDC, 0x6D, 0x99, 0x5E, 0xB2 };
    const unsigned char tag_with_aad[bc::tag_size] = { 0xB5, 0xB2, 0x36, 0x22, 0xEC, 0xCB, 0x62, 0x2A, 0xB2, 0x42, 0xD3, 0x39, 0x34, 0xC6, 0xDA, 0x72 };

    unsigned char output[partial_length] = {};
    unsigned char tag[bc::tag_size] = {};

    REQUIRE(bc::encrypt(partial_plain, partial_length, partial_key, partial_iv_12, output, tag) == true);
    REQUIRE(testbench::is_memory_same(encrypted, output, partial_length), "%s != %s\n", hex_2_string(encrypted, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_without_aad, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_without_aad, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted, partial_length, partial_key, partial_iv_12, output, tag_without_aad) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    REQUIRE(bc::encrypt(partial_plain, partial_length, partial_key, partial_iv_12, output, tag, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(encrypted, output, partial_length), "%s != %s\n", hex_2_string(encrypted, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_with_aad, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_with_aad, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted, partial_length, partial_key, partial_iv_12, output, tag_with_aad, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
}

TEST(block_cipher, verify_partial_block, siv) {
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::siv>;

    // With additional authenticated data and an initialisation vector.
    const unsigned char encrypted_aad_iv[partial_length] = { 0xF8, 0x1B, 0xBF, 0xED, 0x4D, 0x4B, 0x9B, 0x2A, 0xD2, 0xEA, 0x72, 0x18, 0x7B, 0x43, 0x1B, 0x56, 0x62, 0xC2, 0x0A, 0xF9, 0x6D, 0xEB, 0x80, 0x69, 0x88, 0x0C, 0x9A, 0x72, 0x3B, 0x34, 0xCB, 0x3A, 0x08 };
    const unsigned char tag_aad_iv[bc::tag_size] = { 0x05, 0xE7, 0xA6, 0x61, 0xA5, 0x0A, 0x51, 0x99, 0xA8, 0xAE, 0xF6, 0xDC, 0xD6, 0x59, 0x3C, 0xAB };
    // With only an initialisation vector.
    const unsigned char encrypted_iv[partial_length] = { 0xFE, 0x66, 0xDD, 0xBA, 0xD3, 0x86, 0xE0, 0x34, 0xF1, 0x30, 0xD7, 0x26, 0xAA, 0x25, 0x99, 0x41, 0xAE, 0xE9, 0xA5, 0x67, 0x5A, 0x2E, 0xF8, 0xD1, 0x50, 0xDA, 0x17, 0x95, 0x45, 0xA2, 0x5F, 0xE4, 0x8C };
    const unsigned char tag_iv[bc::tag_size] = { 0xFB, 0x57, 0xE1, 0x51, 0xCF, 0x84, 0x00, 0xE4, 0x52, 0x16, 0x69, 0xEF, 0x0A, 0x33, 0xD5, 0xF1 };
    // With neither.
    const unsigned char encrypted_none[partial_length] = { 0x25, 0xF9, 0x65, 0xF0, 0xE9, 0x20, 0x99, 0xE9, 0x94, 0x54, 0x7D, 0xF3, 0x9D, 0x10, 0x8C, 0x38, 0x5B, 0x60, 0xF4, 0x80, 0x9A, 0xD9, 0xB8, 0xCA, 0xE8, 0x82, 0xB0, 0xE2, 0x22, 0xF6, 0x4F, 0x4E, 0x88 };
    const unsigned char tag_none[bc::tag_size] = { 0x8A, 0x54, 0x4A, 0xFF, 0xAE, 0x87, 0x88, 0x67, 0x4F, 0xCF, 0x57, 0x13, 0x56, 0x9E, 0x47, 0x49 };
    // With both and data that is shorter than a block.
    constexpr static const unsigned int short_length = 5;
    const unsigned char encrypted_short[short_length] = { 0xAE, 0x1B, 0x88, 0x7C, 0x10 };
    const unsigned char tag_short[bc::tag_size] = { 0x3B, 0x7D, 0x6F, 0x37, 0x0B, 0xC9, 0xDD, 0x7E, 0x08, 0x97, 0x7A, 0x81, 0x01, 0x2A, 0xD6, 0xCB };

    unsigned char output[partial_length] = {};
    unsigned char tag[bc::tag_size] = {};

    REQUIRE(bc::encrypt(partial_plain, partial_length, partial_key, partial_iv_12, output, tag, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(encrypted_aad_iv, output, partial_length), "%s != %s\n", hex_2_string(encrypted_aad_iv, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_aad_iv, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_aad_iv, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted_aad_iv, partial_length, partial_key, partial_iv_12, output, tag_aad_iv, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    REQUIRE(bc::encrypt(partial_plain, partial_length, partial_key, partial_iv_12, output, tag) == true);
    REQUIRE(testbench::is_memory_same(encrypted_iv, output, partial_length), "%s != %s\n", hex_2_string(encrypted_iv, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_iv, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_iv, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted_iv, partial_length, partial_key, partial_iv_12, output, tag_iv) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    REQUIRE(bc::encrypt(partial_plain, partial_length, partial_key, nullptr, output, tag) == true);
    REQUIRE(testbench::is_memory_same(encrypted_none, output, partial_length), "%s != %s\n", hex_2_string(encrypted_none, partial_length).c_str(), hex_2_string(output, partial_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_none, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_none, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted_none, partial_length, partial_key, nullptr, output, tag_none) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, partial_length), "%s != %s\n", hex_2_string(partial_plain, partial_length).c_str(), hex_2_string(output, partial_length).c_str());

    REQUIRE(bc::encrypt(partial_plain, short_length, partial_key, partial_iv_12, output, tag, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(encrypted_short, output, short_length), "%s != %s\n", hex_2_string(encrypted_short, short_length).c_str(), hex_2_string(output, short_length).c_str());
    REQUIRE(testbench::is_memory_same(tag_short, tag, bc::tag_size), "%s != %s\n", hex_2_string(tag_short, bc::tag_size).c_str(), hex_2_string(tag, bc::tag_size).c_str());
    REQUIRE(bc::decrypt(encrypted_short, short_length, partial_key, partial_iv_12, output, tag_short, partial_aad, partial_aad_length) == true);
    REQUIRE(testbench::is_memory_same(partial_plain, output, short_length), "%s != %s\n", hex_2_string(partial_plain, short_length).c_str(), hex_2_string(output, short_length).c_str());

    // The initialisation vector is authenticated, so decrypting without it or with a different one must fail.
    REQUIRE(bc::decrypt(encrypted_aad_iv, partial_length, partial_key, nullptr, output, tag_aad_iv, partial_aad, partial_aad_length) == false);
    REQUIRE(bc::decrypt(encrypted_none, partial_length, partial_key, partial_iv_12, output, tag_none) == false);
}

TEST(block_cipher, verify_rfc_5297, siv) {
    // The deterministic authenticated encryption example of RFC 5297 appendix A.1, which has no initialisation vector.
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::siv>;
    constexpr static const unsigned int plain_length = 14;
    constexpr static const unsigned int aad_length = 24;
    const unsigned char key[bc::key_size] = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3, 0xF2, 0xF1, 0xF0, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };
    const unsigned char aad[aad_length] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27 };
    const unsigned char plain[plain_length] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };
    const unsigned char encrypted_plain[plain_length] = { 0x40, 0xC0, 0x2B, 0x96, 0x90, 0xC4, 0xDC, 0x04, 0xDA, 0xEF, 0x7F, 0x6A, 0xFE, 0x5C };
    const unsigned char encrypted_tag[bc::tag_size] = { 0x85, 0x63, 0x2D, 0x07, 0xC6, 0xE8, 0xF3, 0x7F, 0x95, 0x0A, 0xCD, 0x32, 0x0A, 0x2E, 0xCC, 0x93 };

    unsigned char output_data[plain_length] = {};
    unsigned char output_tag[bc::tag_size] = {};
    REQUIRE(bc::encrypt(plain, plain_length, key, nullptr, output_data, output_tag, aad, aad_length) == true);
    REQUIRE(testbench::is_memory_same(encrypted_plain, output_data, plain_length), "%s != %s\n", hex_2_string(encrypted_plain, plain_length).c_str(), hex_2_string(output_data, plain_length).c_str());
    REQUIRE(testbench::is_memory_same(encrypted_tag, output_tag, bc::tag_size), "%s != %s\n", hex_2_string(encrypted_tag, bc::tag_size).c_str(), hex_2_string(output_tag, bc::tag_size).c_str());

    unsigned char output[plain_length] = {};
    REQUIRE(bc::decrypt(encrypted_plain, plain_length, key, nullptr, output, encrypted_tag, aad, aad_length) == true);
    REQUIRE(testbench::is_memory_same(plain, output, plain_length), "%s != %s\n", hex_2_string(plain, plain_length).c_str(), hex_2_string(output, plain_length).c_str());
}

TEST(block_cipher, encrypt_decrypt, any_length_and_in_place) {
    testbench::test_template<aes_key_sizes, aes_block_modes>(
        [](auto test_value, auto test_mode) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::block_cipher_mode mode = decltype(test_mode)::value;

            constexpr static const unsigned int length_maximum = 52;
            constexpr static const bool whole_blocks_only = (mode == gtl::block_cipher_mode::ecb) || (mode == gtl::block_cipher_mode::cbc);
            unsigned char key[2 * 256 / 8];
            for (unsigned int i = 0; i < 2 * 256 / 8; ++i) {
                key[i] = static_cast<unsigned char>(i * 7 + 1);
            }
            const unsigned char iv[16] = { 0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A, 0x69, 0x78, 0x87, 0x96, 0xA5, 0xB4, 0xC3, 0xD2, 0xE1, 0xF0 };
            unsigned char plain[length_maximum];
            for (unsigned int i = 0; i < length_maximum; ++i) {
                plain[i] = static_cast<unsigned char>(i * 13 + 5);
            }

            for (unsigned int length = 0; length <= length_maximum; ++length) {
                // One extra byte either side to detect writes outside of the output.
                unsigned char encrypted[1 + length_maximum + 1];
                unsigned char decrypted[1 + length_maximum + 1];
                for (unsigned int i = 0; i < 1 + length_maximum + 1; ++i) {
                    encrypted[i] = 0xA5;
                    decrypted[i] = 0xA5;
                }
                unsigned char tag[16] = {};

                const bool valid = (!whole_blocks_only) || ((length % 16) == 0);
                REQUIRE((encrypt_any_mode<value, mode>(plain, length, key, iv, &encrypted[1], tag)) == valid, "Unexpected encrypt result for length %u.", length);
                REQUIRE(encrypted[0] == 0xA5, "Length %u wrote before the output.", length);
                for (unsigned int i = (valid ? length : 0); i < length_maximum + 1; ++i) {
                    REQUIRE(encrypted[1 + i] == 0xA5, "Length %u wrote outside of the output at %u.", length, i);
                }
                if (!valid) {
                    // An invalid length must also be rejected when decrypting, without writing anything.
                    REQUIRE((decrypt_any_mode<value, mode>(plain, length, key, iv, &decrypted[1], tag)) == false, "Unexpected decrypt result for length %u.", length);
                    for (unsigned int i = 0; i < 1 + length_maximum + 1; ++i) {
                        REQUIRE(decrypted[i] == 0xA5, "Length %u wrote to the output at %u.", length, i);
                    }
                    continue;
                }

                REQUIRE((decrypt_any_mode<value, mode>(&encrypted[1], length, key, iv, &decrypted[1], tag)) == true, "Unexpected decrypt result for length %u.", length);
                REQUIRE(testbench::is_memory_same(plain, &decrypted[1], length), "Length %u: %s != %s\n", length, hex_2_string(plain, length).c_str(), hex_2_string(&decrypted[1], length).c_str());
                REQUIRE(decrypted[0] == 0xA5, "Length %u wrote before the output.", length);
                for (unsigned int i = length; i < length_maximum + 1; ++i) {
                    REQUIRE(decrypted[1 + i] == 0xA5, "Length %u wrote outside of the output at %u.", length, i);
                }

                // Encrypting in place must give the same output and tag.
                unsigned char in_place[length_maximum + 1];
                for (unsigned int i = 0; i < length_maximum; ++i) {
                    in_place[i] = plain[i];
                }
                unsigned char tag_in_place[16] = {};
                REQUIRE((encrypt_any_mode<value, mode>(in_place, length, key, iv, in_place, tag_in_place)) == true);
                REQUIRE(testbench::is_memory_same(&encrypted[1], in_place, length), "Length %u: %s != %s\n", length, hex_2_string(&encrypted[1], length).c_str(), hex_2_string(in_place, length).c_str());
                REQUIRE(testbench::is_memory_same(tag, tag_in_place, 16), "Length %u: %s != %s\n", length, hex_2_string(tag, 16).c_str(), hex_2_string(tag_in_place, 16).c_str());

                // Decrypting in place must give the original data.
                REQUIRE((decrypt_any_mode<value, mode>(in_place, length, key, iv, in_place, tag)) == true);
                REQUIRE(testbench::is_memory_same(plain, in_place, length), "Length %u: %s != %s\n", length, hex_2_string(plain, length).c_str(), hex_2_string(in_place, length).c_str());
            }
        }
    );
}

TEST(block_cipher, verify_decrypt, in_place_cbc_cfb) {
    // Decrypting in place with the modes that chain on the encrypted data, checked against known values.
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x21, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87, 0x98, 0xA9, 0xBA, 0xCB, 0xDC, 0xED, 0xFE, 0x0F };
    const unsigned char encrypted_cfb[length] = { 0x66, 0xB6, 0xE5, 0xDB, 0x70, 0x07, 0x57, 0x3F, 0x1F, 0xC8, 0x74, 0xBC, 0xFF, 0xCB, 0x43, 0x52, 0xDE, 0x0E, 0x73, 0x08, 0x9A, 0x33, 0xD6, 0x41, 0x5F, 0xDA, 0xC3, 0xFE, 0x93, 0xD7, 0xD1, 0x6E };

    unsigned char buffer[length];
    for (unsigned int i = 0; i < length; ++i) {
        buffer[i] = encrypted_cfb[i];
    }
    gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cfb>::decrypt(buffer, length, partial_key, partial_iv_16, buffer);
    REQUIRE(testbench::is_memory_same(plain, buffer, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(buffer, length).c_str());

    // Encrypt to a separate buffer, then decrypt that buffer in place.
    unsigned char encrypted_cbc[length] = {};
    REQUIRE((gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cbc>::encrypt(plain, length, partial_key, partial_iv_16, encrypted_cbc)) == true);
    REQUIRE((gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::cbc>::decrypt(encrypted_cbc, length, partial_key, partial_iv_16, encrypted_cbc)) == true);
    REQUIRE(testbench::is_memory_same(plain, encrypted_cbc, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(encrypted_cbc, length).c_str());
}

TEST(block_cipher, parameter, ccm_length_too_long) {
    testbench::test_template<aes_key_sizes>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            using bc = gtl::block_cipher<gtl::aes<value>, gtl::block_cipher_mode::ccm>;
            const unsigned char key[256 / 8] = {};
            const unsigned char iv[bc::iv_size] = {};
            const unsigned char untouched[16] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };

            // The length is rejected before any data is read or written, so the buffers do not need to be that long.
            const unsigned char data[16] = {};
            unsigned char output[16] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };
            unsigned char tag[bc::tag_size] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };

            REQUIRE(bc::encrypt(data, bc::length_max + 1, key, iv, output, tag) == false);
            REQUIRE(testbench::is_memory_same(untouched, output, 16));
            REQUIRE(testbench::is_memory_same(untouched, tag, bc::tag_size));

            REQUIRE(bc::decrypt(data, bc::length_max + 1, key, iv, output, tag) == false);
            REQUIRE(testbench::is_memory_same(untouched, output, 16));

            REQUIRE(bc::encrypt(data, 0xFFFFFFFF, key, iv, output, tag) == false);
            REQUIRE(bc::decrypt(data, 0xFFFFFFFF, key, iv, output, tag) == false);
            REQUIRE(testbench::is_memory_same(untouched, output, 16));
            REQUIRE(testbench::is_memory_same(untouched, tag, bc::tag_size));
        }
    );
}

TEST(block_cipher, parameter, siv_tag_required) {
    // The tag is the initial counter value, so encrypting without receiving it is rejected and nothing is written.
    using bc = gtl::block_cipher<gtl::aes<128>, gtl::block_cipher_mode::siv>;
    const unsigned char untouched[16] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };
    unsigned char output[16] = { 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5 };

    REQUIRE(bc::encrypt(partial_plain, 16, partial_key, partial_iv_12, output, nullptr) == false);
    REQUIRE(testbench::is_memory_same(untouched, output, 16));
}
