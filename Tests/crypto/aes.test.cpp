/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
*/

#include <main.tests.hpp>
#include <benchmark.tests.hpp>
#include <comparison.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <crypto/aes>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

static std::string hex_2_string(const unsigned char* hex, const unsigned int length) {
    std::string result;
    for (unsigned int i = 0; i < length; ++i) {
        result.push_back("0123456789ABCDEF"[hex[i] / 16]);
        result.push_back("0123456789ABCDEF"[hex[i] % 16]);
    }
    return result;
}

using aes_types = testbench::value_collection<128, 192, 256>;
using aes_modes = testbench::enum_collection<gtl::aes_mode, gtl::aes_mode::cbc, gtl::aes_mode::cfb, gtl::aes_mode::ecb>;

TEST(aes, traits, standard) {
    testbench::test_template<aes_types, aes_modes>(
        [](auto test_value, auto test_mode)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::aes_mode mode = decltype(test_mode)::value;

            REQUIRE((std::is_pod<gtl::aes<value, mode>>::value == true));

            REQUIRE((std::is_trivial<gtl::aes<value, mode>>::value == true));

            REQUIRE((std::is_trivially_copyable<gtl::aes<value, mode>>::value == true));

            REQUIRE((std::is_standard_layout<gtl::aes<value, mode>>::value == true));
        }
    );
}

TEST(aes, constructor, empty) {
    testbench::test_template<aes_types, aes_modes>(
        [](auto test_value, auto test_mode)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::aes_mode mode = decltype(test_mode)::value;

            gtl::aes<value, mode> aes;
            testbench::do_not_optimise_away(aes);
        }
    );
}

TEST(aes, evaluate, key_length_128) {
    gtl::aes<128, gtl::aes_mode::ecb> aes;
    constexpr static const unsigned int length = 16;
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A };

    unsigned char output[length];
    aes.encrypt(plain, length, key, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, evaluate, key_length_192) {
    gtl::aes<192, gtl::aes_mode::ecb> aes;
    constexpr static const unsigned int length = 16;
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
    const unsigned char encrypted[length] = { 0xDD, 0xA9, 0x7C, 0xA4, 0x86, 0x4C, 0xDF, 0xE0, 0x6E, 0xAF, 0x70, 0xA0, 0xEC, 0x0D, 0x71, 0x91 };

    unsigned char output[length];
    aes.encrypt(plain, length, key, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, evaluate, key_length_256) {
    gtl::aes<256, gtl::aes_mode::ecb> aes;
    constexpr static const unsigned int length = 16;
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
    const unsigned char encrypted[length] = { 0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45, 0xBF, 0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89 };

    unsigned char output[length];
    aes.encrypt(plain, length, key, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, encrypt_decrypt, one_block) {
    testbench::test_template<aes_types, aes_modes>(
        [](auto test_value, auto test_mode)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::aes_mode mode = decltype(test_mode)::value;

            gtl::aes<value, mode> aes;
            constexpr static const unsigned int length = 16;
            unsigned char plain[length] =   { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
            unsigned char key[256 / 8] =    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

            unsigned char output[length];
            aes.encrypt(plain, length, key, output);
            unsigned char input[length];
            aes.decrypt(output, length, key, input);

            REQUIRE(testbench::is_memory_same(plain, input, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(input, length).c_str());
        }
    );
}

TEST(aes, encrypt_decrypt, one_block_padded) {
    testbench::test_template<aes_types, aes_modes>(
        [](auto test_value, auto test_mode)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::aes_mode mode = decltype(test_mode)::value;

            gtl::aes<value, mode> aes;
            constexpr static const unsigned int length = 16;
            unsigned char plain[length] =   { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x00 }; // End byte padded with a zero.
            unsigned char key[256 / 8] =    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

            unsigned char output[length];
            aes.encrypt(plain, length, key, output);
            unsigned char input[length];
            aes.decrypt(output, length, key, input);

            REQUIRE(testbench::is_memory_same(plain, input, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(input, length).c_str());
        }
    );
}

TEST(aes, encrypt_decrypt, two_blocks) {
    testbench::test_template<aes_types, aes_modes>(
        [](auto test_value, auto test_mode)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::aes_mode mode = decltype(test_mode)::value;

            gtl::aes<value, mode> aes;
            constexpr static const unsigned int length = 32;
            unsigned char plain[length] =   { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
            unsigned char key[256 / 8] =    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

            unsigned char output[length];
            aes.encrypt(plain, length, key, output);
            unsigned char input[length];
            aes.decrypt(output, length, key, input);

            REQUIRE(testbench::is_memory_same(plain, input, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(input, length).c_str());
        }
    );
}

TEST(aes, encrypt_decrypt, two_blocks_padded) {
    testbench::test_template<aes_types, aes_modes>(
        [](auto test_value, auto test_mode)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const gtl::aes_mode mode = decltype(test_mode)::value;

            gtl::aes<value, mode> aes;
            constexpr static const unsigned int length = 32;
            unsigned char plain[length] =   { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            unsigned char key[256 / 8] =    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

            unsigned char output[length];
            aes.encrypt(plain, length, key, output);
            unsigned char input[length];
            aes.decrypt(output, length, key, input);

            REQUIRE(testbench::is_memory_same(plain, input, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(input, length).c_str());
        }
    );
}

TEST(aes, verify_encrypt, ecb) {
    gtl::aes<128, gtl::aes_mode::ecb> aes;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A, 0x07, 0xFE, 0xEF, 0x74, 0xE1, 0xD5, 0x03, 0x6E, 0x90, 0x0E, 0xEE, 0x11, 0x8E, 0x94, 0x92, 0x93 };

    unsigned char output[length];
    aes.encrypt(plain, length, key, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, verify_encrypt, cbc) {
    gtl::aes<128, gtl::aes_mode::cbc> aes;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[length] =        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x1B, 0x87, 0x23, 0x78, 0x79, 0x5F, 0x4F, 0xFD, 0x77, 0x28, 0x55, 0xFC, 0x87, 0xCA, 0x96, 0x4D, 0x4C, 0x5B, 0xCA, 0x1C, 0x48, 0xCD, 0x88, 0x00, 0x3A, 0x10, 0x52, 0x11, 0x88, 0x12, 0x5E, 0x00 };

    unsigned char output[length];
    aes.encrypt(plain, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, verify_encrypt, cfb) {
    gtl::aes<128, gtl::aes_mode::cfb> aes;
    constexpr static const unsigned int length = 32;
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char iv[length] =        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xE3, 0x5A, 0x40, 0xAB, 0x30, 0x29, 0xF3, 0x0C, 0xE1, 0xDB, 0x30, 0x6C, 0xA1, 0x05, 0xCB, 0xA9 };

    unsigned char output[length];
    aes.encrypt(plain, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, verify_decrypt, ecb) {
    gtl::aes<128, gtl::aes_mode::ecb> aes;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A, 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    aes.decrypt(encrypted, length, key, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}


TEST(aes, verify_decrypt, cbc) {
    gtl::aes<128, gtl::aes_mode::cbc> aes;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x1B, 0x87, 0x23, 0x78, 0x79, 0x5F, 0x4F, 0xFD, 0x77, 0x28, 0x55, 0xFC, 0x87, 0xCA, 0x96, 0x4D,  0x4C, 0x5B, 0xCA, 0x1C, 0x48, 0xCD, 0x88, 0x00, 0x3A, 0x10, 0x52, 0x11, 0x88, 0x12, 0x5E, 0x00 };
    const unsigned char iv[length] =        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    aes.decrypt(encrypted, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, verify_decrypt, cfb) {
    gtl::aes<128, gtl::aes_mode::cfb> aes;
    constexpr static const unsigned int length = 32;
    const unsigned char encrypted[length] = { 0x3C, 0x55, 0x3D, 0x01, 0x8A, 0x52, 0xE4, 0x54, 0xEC, 0x4E, 0x08, 0x22, 0xC2, 0x8D, 0x55, 0xEC, 0xE3, 0x5A, 0x40, 0xAB, 0x30, 0x29, 0xF3, 0x0C, 0xE1, 0xDB, 0x30, 0x6C, 0xA1, 0x05, 0xCB, 0xA9 };
    const unsigned char iv[length] =        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const unsigned char key[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char plain[length] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    unsigned char output[length];
    aes.decrypt(encrypted, length, key, iv, output);

    REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());
}
