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

#include <crypto/asymmetric/rsa>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <functional>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(rsa, traits, standard) {
    REQUIRE((std::is_pod<gtl::rsa>::value == true));

    REQUIRE((std::is_trivial<gtl::rsa>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::rsa>::value == true));

    REQUIRE((std::is_standard_layout<gtl::rsa>::value == true));
}

TEST(rsa, constructor, empty) {
    gtl::rsa rsa;
    testbench::do_not_optimise_away(rsa);
}

TEST(rsa, evaluate, encrypt_and_decrypt_transform_128) {
    const unsigned char key_public[128] = {
        0xD4,
        0x34,
        0xAB,
        0x9A,
        0x10,
        0xEC,
        0x30,
        0xCE,
        0x2E,
        0x41,
        0xC8,
        0x3B,
        0x70,
        0x4E,
        0xD7,
        0x60,
        0x2C,
        0x2E,
        0x4F,
        0xFF,
        0x2D,
        0xA2,
        0x5B,
        0x24,
        0x1D,
        0x80,
        0x0C,
        0x87,
        0x85,
        0xF4,
        0x2D,
        0x98,
        0xE3,
        0x89,
        0xE9,
        0xB9,
        0xA5,
        0xDC,
        0x46,
        0x05,
        0xFB,
        0xAF,
        0x61,
        0x6E,
        0x4D,
        0x31,
        0x6A,
        0x30,
        0x30,
        0xF4,
        0x6A,
        0x66,
        0x08,
        0xB2,
        0x34,
        0x05,
        0x38,
        0x9B,
        0x87,
        0xC1,
        0xF1,
        0xBE,
        0x73,
        0x41,
        0xF9,
        0x9D,
        0x97,
        0xFD,
        0x89,
        0x0E,
        0x08,
        0xE5,
        0xC7,
        0x80,
        0x50,
        0x44,
        0xB9,
        0x2B,
        0xD1,
        0xB7,
        0xE3,
        0x54,
        0x1E,
        0xE6,
        0xB5,
        0x4A,
        0xD5,
        0xB9,
        0x45,
        0x4D,
        0xB4,
        0x89,
        0x27,
        0xC0,
        0xAB,
        0xFF,
        0xC2,
        0xB4,
        0xC7,
        0x6B,
        0xC1,
        0x62,
        0x96,
        0x76,
        0xAE,
        0x26,
        0xCB,
        0x65,
        0x2A,
        0x72,
        0xF8,
        0xD7,
        0xBB,
        0x7C,
        0xF9,
        0x68,
        0xAE,
        0x8A,
        0x9B,
        0xDF,
        0xE1,
        0x4E,
        0x61,
        0x5C,
        0x00,
        0xFF,
        0xFD,
        0xAB,
    };
    const unsigned char key_private[128] = {
        0xCD,
        0x8E,
        0xE7,
        0xDB,
        0xAA,
        0x20,
        0xE1,
        0x76,
        0x18,
        0x20,
        0xE8,
        0xD1,
        0x8B,
        0x0D,
        0x14,
        0x5C,
        0xB8,
        0xEF,
        0x73,
        0xD6,
        0x80,
        0x62,
        0x11,
        0x5F,
        0x7E,
        0xA4,
        0x17,
        0x7F,
        0x00,
        0x27,
        0x74,
        0x33,
        0xF1,
        0xD6,
        0x1A,
        0x6E,
        0x17,
        0x58,
        0xB5,
        0x0B,
        0xFE,
        0xAD,
        0xF8,
        0xF7,
        0x5C,
        0x17,
        0x4E,
        0x01,
        0xB8,
        0xAA,
        0x4A,
        0x96,
        0xE4,
        0x3D,
        0x9B,
        0x5E,
        0xA3,
        0x63,
        0x25,
        0xB2,
        0x9D,
        0x76,
        0x1B,
        0x36,
        0x3C,
        0xFB,
        0x1E,
        0x5A,
        0xA2,
        0xFB,
        0xD4,
        0xC7,
        0x37,
        0xEC,
        0x51,
        0x5D,
        0xD5,
        0x51,
        0x29,
        0xB2,
        0xA2,
        0xEF,
        0xF6,
        0x7B,
        0x4D,
        0x0F,
        0x7A,
        0x4A,
        0x9A,
        0x2E,
        0xD5,
        0x89,
        0xDB,
        0x84,
        0x69,
        0xF5,
        0x35,
        0xD8,
        0xF8,
        0xEC,
        0xB5,
        0x1A,
        0x3C,
        0xE1,
        0x68,
        0x1C,
        0xBD,
        0x53,
        0x5D,
        0x32,
        0x58,
        0x89,
        0x15,
        0x7C,
        0x08,
        0x2F,
        0x2E,
        0x9F,
        0x1E,
        0xE3,
        0x28,
        0xC9,
        0x3F,
        0x5C,
        0xC5,
        0xDF,
        0x91,
        0x21,
    };
    const unsigned char key_exponent[128] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x01,
        0x00,
        0x01
    };
    const unsigned char known_decrypted[128] = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B,
        0x0C,
        0x0D,
        0x0E,
        0x0F,
        0x10,
        0x11,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16,
        0x17,
        0x18,
        0x19,
        0x1A,
        0x1B,
        0x1C,
        0x1D,
        0x1E,
        0x1F,
        0x20,
        0x21,
        0x22,
        0x23,
        0x24,
        0x25,
        0x26,
        0x27,
        0x28,
        0x29,
        0x2A,
        0x2B,
        0x2C,
        0x2D,
        0x2E,
        0x2F,
        0x30,
        0x31,
        0x32,
        0x33,
        0x34,
        0x35,
        0x36,
        0x37,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x40,
        0x41,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x49,
        0x4A,
        0x4B,
        0x4C,
        0x4D,
        0x4E,
        0x4F,
        0x50,
        0x51,
        0x52,
        0x53,
        0x54,
        0x55,
        0x56,
        0x57,
        0x58,
        0x59,
        0x5A,
        0x5B,
        0x5C,
        0x5D,
        0x5E,
        0x5F,
        0x60,
        0x61,
        0x62,
        0x63,
        0x64,
        0x65,
        0x66,
        0x67,
        0x68,
        0x69,
        0x6A,
        0x6B,
        0x6C,
        0x6D,
        0x6E,
        0x6F,
        0x70,
        0x71,
        0x72,
        0x73,
        0x74,
        0x75,
        0x76,
        0x77,
        0x78,
        0x79,
        0x7A,
        0x7B,
        0x7C,
        0x7D,
        0x7E,
        0x7F,
        0x80,
    };
    const unsigned char known_encrypted[128] = {
        0xCF,
        0x48,
        0x81,
        0xA3,
        0xC3,
        0x0E,
        0xB5,
        0x28,
        0x41,
        0x0B,
        0x81,
        0x7E,
        0x8F,
        0x41,
        0x69,
        0xF6,
        0x52,
        0x50,
        0x33,
        0x54,
        0xB7,
        0x59,
        0x7A,
        0x03,
        0x2C,
        0x6A,
        0x65,
        0x70,
        0x38,
        0xA8,
        0x1F,
        0xCA,
        0x24,
        0xF5,
        0x93,
        0xAE,
        0xF0,
        0xF5,
        0xCE,
        0x53,
        0x47,
        0x7F,
        0xAC,
        0xC0,
        0x93,
        0x1F,
        0x6B,
        0x83,
        0xB7,
        0x9C,
        0x61,
        0xD6,
        0x48,
        0x33,
        0x71,
        0x08,
        0x6C,
        0x0D,
        0x54,
        0x19,
        0xA4,
        0x5E,
        0xCE,
        0x68,
        0x09,
        0xAA,
        0xD0,
        0xE0,
        0xA5,
        0xF2,
        0x9E,
        0xDB,
        0x10,
        0xAA,
        0xE7,
        0x99,
        0xF1,
        0x0B,
        0xA4,
        0x9D,
        0x0F,
        0x1C,
        0xAF,
        0x4A,
        0x0A,
        0x7A,
        0xCD,
        0x75,
        0x4C,
        0x25,
        0x85,
        0x3A,
        0xC2,
        0x4F,
        0x45,
        0xBD,
        0xFE,
        0xBB,
        0x5B,
        0x01,
        0x97,
        0x10,
        0xF4,
        0x4A,
        0xF0,
        0xBD,
        0x6A,
        0x2A,
        0x3A,
        0xFA,
        0x30,
        0xC9,
        0xE1,
        0xB0,
        0xA6,
        0x83,
        0x97,
        0x02,
        0xF7,
        0x88,
        0xF3,
        0x0C,
        0x16,
        0x80,
        0x87,
        0xFF,
        0x8A,
        0x57
    };

    unsigned char test_encrypted[128] = {};
    gtl::rsa::transform_block(known_decrypted, 128, key_exponent, key_public, test_encrypted);

    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted, 128));

    unsigned char test_decrypted[128] = {};
    gtl::rsa::transform_block(known_encrypted, 128, key_private, key_public, test_decrypted);

    REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted, 128));
}

TEST(rsa, evaluate, encrypt_and_decrypt_128) {
    const unsigned char key_public[128] = {
        0xD4,
        0x34,
        0xAB,
        0x9A,
        0x10,
        0xEC,
        0x30,
        0xCE,
        0x2E,
        0x41,
        0xC8,
        0x3B,
        0x70,
        0x4E,
        0xD7,
        0x60,
        0x2C,
        0x2E,
        0x4F,
        0xFF,
        0x2D,
        0xA2,
        0x5B,
        0x24,
        0x1D,
        0x80,
        0x0C,
        0x87,
        0x85,
        0xF4,
        0x2D,
        0x98,
        0xE3,
        0x89,
        0xE9,
        0xB9,
        0xA5,
        0xDC,
        0x46,
        0x05,
        0xFB,
        0xAF,
        0x61,
        0x6E,
        0x4D,
        0x31,
        0x6A,
        0x30,
        0x30,
        0xF4,
        0x6A,
        0x66,
        0x08,
        0xB2,
        0x34,
        0x05,
        0x38,
        0x9B,
        0x87,
        0xC1,
        0xF1,
        0xBE,
        0x73,
        0x41,
        0xF9,
        0x9D,
        0x97,
        0xFD,
        0x89,
        0x0E,
        0x08,
        0xE5,
        0xC7,
        0x80,
        0x50,
        0x44,
        0xB9,
        0x2B,
        0xD1,
        0xB7,
        0xE3,
        0x54,
        0x1E,
        0xE6,
        0xB5,
        0x4A,
        0xD5,
        0xB9,
        0x45,
        0x4D,
        0xB4,
        0x89,
        0x27,
        0xC0,
        0xAB,
        0xFF,
        0xC2,
        0xB4,
        0xC7,
        0x6B,
        0xC1,
        0x62,
        0x96,
        0x76,
        0xAE,
        0x26,
        0xCB,
        0x65,
        0x2A,
        0x72,
        0xF8,
        0xD7,
        0xBB,
        0x7C,
        0xF9,
        0x68,
        0xAE,
        0x8A,
        0x9B,
        0xDF,
        0xE1,
        0x4E,
        0x61,
        0x5C,
        0x00,
        0xFF,
        0xFD,
        0xAB,
    };
    const unsigned char key_private[128] = {
        0xCD,
        0x8E,
        0xE7,
        0xDB,
        0xAA,
        0x20,
        0xE1,
        0x76,
        0x18,
        0x20,
        0xE8,
        0xD1,
        0x8B,
        0x0D,
        0x14,
        0x5C,
        0xB8,
        0xEF,
        0x73,
        0xD6,
        0x80,
        0x62,
        0x11,
        0x5F,
        0x7E,
        0xA4,
        0x17,
        0x7F,
        0x00,
        0x27,
        0x74,
        0x33,
        0xF1,
        0xD6,
        0x1A,
        0x6E,
        0x17,
        0x58,
        0xB5,
        0x0B,
        0xFE,
        0xAD,
        0xF8,
        0xF7,
        0x5C,
        0x17,
        0x4E,
        0x01,
        0xB8,
        0xAA,
        0x4A,
        0x96,
        0xE4,
        0x3D,
        0x9B,
        0x5E,
        0xA3,
        0x63,
        0x25,
        0xB2,
        0x9D,
        0x76,
        0x1B,
        0x36,
        0x3C,
        0xFB,
        0x1E,
        0x5A,
        0xA2,
        0xFB,
        0xD4,
        0xC7,
        0x37,
        0xEC,
        0x51,
        0x5D,
        0xD5,
        0x51,
        0x29,
        0xB2,
        0xA2,
        0xEF,
        0xF6,
        0x7B,
        0x4D,
        0x0F,
        0x7A,
        0x4A,
        0x9A,
        0x2E,
        0xD5,
        0x89,
        0xDB,
        0x84,
        0x69,
        0xF5,
        0x35,
        0xD8,
        0xF8,
        0xEC,
        0xB5,
        0x1A,
        0x3C,
        0xE1,
        0x68,
        0x1C,
        0xBD,
        0x53,
        0x5D,
        0x32,
        0x58,
        0x89,
        0x15,
        0x7C,
        0x08,
        0x2F,
        0x2E,
        0x9F,
        0x1E,
        0xE3,
        0x28,
        0xC9,
        0x3F,
        0x5C,
        0xC5,
        0xDF,
        0x91,
        0x21,
    };
    const unsigned char known_decrypted[128] = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B,
        0x0C,
        0x0D,
        0x0E,
        0x0F,
        0x10,
        0x11,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16,
        0x17,
        0x18,
        0x19,
        0x1A,
        0x1B,
        0x1C,
        0x1D,
        0x1E,
        0x1F,
        0x20,
        0x21,
        0x22,
        0x23,
        0x24,
        0x25,
        0x26,
        0x27,
        0x28,
        0x29,
        0x2A,
        0x2B,
        0x2C,
        0x2D,
        0x2E,
        0x2F,
        0x30,
        0x31,
        0x32,
        0x33,
        0x34,
        0x35,
        0x36,
        0x37,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x40,
        0x41,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x49,
        0x4A,
        0x4B,
        0x4C,
        0x4D,
        0x4E,
        0x4F,
        0x50,
        0x51,
        0x52,
        0x53,
        0x54,
        0x55,
        0x56,
        0x57,
        0x58,
        0x59,
        0x5A,
        0x5B,
        0x5C,
        0x5D,
        0x5E,
        0x5F,
        0x60,
        0x61,
        0x62,
        0x63,
        0x64,
        0x65,
        0x66,
        0x67,
        0x68,
        0x69,
        0x6A,
        0x6B,
        0x6C,
        0x6D,
        0x6E,
        0x6F,
        0x70,
        0x71,
        0x72,
        0x73,
        0x74,
        0x75,
        0x76,
        0x77,
        0x78,
        0x79,
        0x7A,
        0x7B,
        0x7C,
        0x7D,
        0x7E,
        0x7F,
        0x80,
    };
    const unsigned char known_encrypted[128] = {
        0xCF,
        0x48,
        0x81,
        0xA3,
        0xC3,
        0x0E,
        0xB5,
        0x28,
        0x41,
        0x0B,
        0x81,
        0x7E,
        0x8F,
        0x41,
        0x69,
        0xF6,
        0x52,
        0x50,
        0x33,
        0x54,
        0xB7,
        0x59,
        0x7A,
        0x03,
        0x2C,
        0x6A,
        0x65,
        0x70,
        0x38,
        0xA8,
        0x1F,
        0xCA,
        0x24,
        0xF5,
        0x93,
        0xAE,
        0xF0,
        0xF5,
        0xCE,
        0x53,
        0x47,
        0x7F,
        0xAC,
        0xC0,
        0x93,
        0x1F,
        0x6B,
        0x83,
        0xB7,
        0x9C,
        0x61,
        0xD6,
        0x48,
        0x33,
        0x71,
        0x08,
        0x6C,
        0x0D,
        0x54,
        0x19,
        0xA4,
        0x5E,
        0xCE,
        0x68,
        0x09,
        0xAA,
        0xD0,
        0xE0,
        0xA5,
        0xF2,
        0x9E,
        0xDB,
        0x10,
        0xAA,
        0xE7,
        0x99,
        0xF1,
        0x0B,
        0xA4,
        0x9D,
        0x0F,
        0x1C,
        0xAF,
        0x4A,
        0x0A,
        0x7A,
        0xCD,
        0x75,
        0x4C,
        0x25,
        0x85,
        0x3A,
        0xC2,
        0x4F,
        0x45,
        0xBD,
        0xFE,
        0xBB,
        0x5B,
        0x01,
        0x97,
        0x10,
        0xF4,
        0x4A,
        0xF0,
        0xBD,
        0x6A,
        0x2A,
        0x3A,
        0xFA,
        0x30,
        0xC9,
        0xE1,
        0xB0,
        0xA6,
        0x83,
        0x97,
        0x02,
        0xF7,
        0x88,
        0xF3,
        0x0C,
        0x16,
        0x80,
        0x87,
        0xFF,
        0x8A,
        0x57
    };

    gtl::rsa::key_type keys{
        gtl::rsa::public_key_type{
            gtl::big_unsigned(key_public, 128),
            gtl::big_unsigned(65537) },
        gtl::rsa::private_key_type{
            gtl::big_unsigned(key_public, 128),
            gtl::big_unsigned(),
            gtl::big_unsigned(key_private, 128),
            { gtl::big_unsigned(), gtl::big_unsigned() },
            { gtl::big_unsigned(), gtl::big_unsigned() },
            gtl::big_unsigned() }
    };

    unsigned char test_encrypted[128] = {};
    gtl::rsa::encrypt(known_decrypted, 128, keys.public_key, test_encrypted);

    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted, 128));

    unsigned char test_decrypted[128] = {};
    gtl::rsa::decrypt(known_encrypted, 128, keys.private_key, test_decrypted);

    REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted, 128));
}

TEST(rsa, evaluate, generate_keys_encrypt_and_decrypt_32) {
    class random_pcg final {
    private:
        unsigned long long int state;
        unsigned long long int increment;

    public:
        random_pcg()
            : state(0x853C49E6748FEA9Bull)
            , increment(0xDA3E39CB94B95BDBull) {
        }

        unsigned int get_random_raw() {
            // Save current state for output calculation.
            unsigned long long int state_previous = this->state;
            // Advance internal state.
            this->state = state_previous * 0x5851F42D4C957F2Dull + this->increment;
            // Calculate output function.
            unsigned int state_shift_xor_shift = static_cast<unsigned int>(((state_previous >> 18u) ^ state_previous) >> 27u);
            int rotation = state_previous >> 59u;
            return (state_shift_xor_shift >> rotation) | (state_shift_xor_shift << ((-rotation) & 31));
        }
    };

    const unsigned char known_decrypted[32] = {
        0x20,
        0x1F,
        0x1E,
        0x1D,
        0x1C,
        0x1B,
        0x1A,
        0x19,
        0x18,
        0x17,
        0x16,
        0x15,
        0x14,
        0x13,
        0x12,
        0x11,
        0x10,
        0x0F,
        0x0E,
        0x0D,
        0x0C,
        0x0B,
        0x0A,
        0x09,
        0x08,
        0x07,
        0x06,
        0x05,
        0x04,
        0x03,
        0x02,
        0x01,
    };

    gtl::rsa::key_type keys = gtl::rsa::generate_key_pair([]() {
        static random_pcg rng;
        return rng.get_random_raw();
    },
                                                          32);

    unsigned char test_encrypted[32] = {};
    gtl::rsa::encrypt(known_decrypted, 32, keys.public_key, test_encrypted);

    unsigned char test_decrypted[32] = {};
    gtl::rsa::decrypt(test_encrypted, 32, keys.private_key, test_decrypted);

    REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted, 32));
}

TEST(rsa, evaluate, leading_zero_bytes_exhaustive) {
    // The classic small key: modulus 3233 = 61 * 53, public exponent 17, private exponent 2753.
    const unsigned char key_public[2] = { 0x0C, 0xA1 };
    const unsigned char key_private[2] = { 0x0A, 0xC1 };
    const unsigned char key_exponent[2] = { 0x00, 0x11 };

    gtl::rsa::key_type keys{
        gtl::rsa::public_key_type{
            gtl::big_unsigned(key_public, 2),
            gtl::big_unsigned(key_exponent, 2) },
        gtl::rsa::private_key_type{
            gtl::big_unsigned(key_public, 2),
            gtl::big_unsigned(key_exponent, 2),
            gtl::big_unsigned(key_private, 2),
            { gtl::big_unsigned(), gtl::big_unsigned() },
            { gtl::big_unsigned(), gtl::big_unsigned() },
            gtl::big_unsigned() }
    };

    // A known value: 65 encrypts to 2790.
    {
        const unsigned char known_decrypted[2] = { 0x00, 0x41 };
        const unsigned char known_encrypted[2] = { 0x0A, 0xE6 };

        unsigned char test_encrypted[2] = { 0xFF, 0xFF };
        gtl::rsa::encrypt(known_decrypted, 2, keys.public_key, test_encrypted);
        REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted, 2));

        unsigned char test_decrypted[2] = { 0xFF, 0xFF };
        gtl::rsa::decrypt(known_encrypted, 2, keys.private_key, test_decrypted);
        REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted, 2));
    }

    // Every message below the modulus must round trip, including those where the message or the encrypted message have a leading zero byte.
    unsigned int leading_zero_decrypted = 0;
    unsigned int leading_zero_encrypted = 0;
    for (unsigned int message = 0; message < 3233; ++message) {
        const unsigned char known_decrypted[2] = { static_cast<unsigned char>(message >> 8), static_cast<unsigned char>(message & 0xFF) };

        unsigned char test_encrypted[2] = { 0xFF, 0xFF };
        gtl::rsa::encrypt(known_decrypted, 2, keys.public_key, test_encrypted);

        unsigned char test_decrypted[2] = { 0xFF, 0xFF };
        gtl::rsa::decrypt(test_encrypted, 2, keys.private_key, test_decrypted);

        REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted, 2), "Message %u decrypted to 0x%02X%02X.", message, test_decrypted[0], test_decrypted[1]);

        unsigned char test_transform_encrypted[2] = { 0xFF, 0xFF };
        gtl::rsa::transform_block(known_decrypted, 2, key_exponent, key_public, test_transform_encrypted);

        REQUIRE(testbench::is_memory_same(test_transform_encrypted, test_encrypted, 2), "Message %u transformed to 0x%02X%02X.", message, test_transform_encrypted[0], test_transform_encrypted[1]);

        unsigned char test_transform_decrypted[2] = { 0xFF, 0xFF };
        gtl::rsa::transform_block(test_transform_encrypted, 2, key_private, key_public, test_transform_decrypted);

        REQUIRE(testbench::is_memory_same(test_transform_decrypted, known_decrypted, 2), "Message %u transformed back to 0x%02X%02X.", message, test_transform_decrypted[0], test_transform_decrypted[1]);

        leading_zero_decrypted += (known_decrypted[0] == 0x00);
        leading_zero_encrypted += (test_encrypted[0] == 0x00);
    }

    // Ensure the loop above covered both leading zero cases.
    REQUIRE(leading_zero_decrypted == 256, "Expected 256 messages with a leading zero byte, found %u.", leading_zero_decrypted);
    REQUIRE(leading_zero_encrypted == 256, "Expected 256 encrypted messages with a leading zero byte, found %u.", leading_zero_encrypted);
}

TEST(rsa, evaluate, leading_zero_bytes_64) {
    // Modulus 0xFFFFFFEA00000055 = 4294967291 * 4294967279, public exponent 65537, private exponent 0x81817E725D5DA2D9.
    const unsigned char key_public[8] = { 0xFF, 0xFF, 0xFF, 0xEA, 0x00, 0x00, 0x00, 0x55 };
    const unsigned char key_private[8] = { 0x81, 0x81, 0x7E, 0x72, 0x5D, 0x5D, 0xA2, 0xD9 };
    const unsigned char key_exponent[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01 };

    constexpr static const unsigned int data_count = 5;

    // Values computed independently of this library.
    const unsigned char known_decrypted[data_count][8] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
        { 0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE },
        { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCE, 0x5D },
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0xAC, 0xD4, 0x3A }
    };
    const unsigned char known_encrypted[data_count][8] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },
        { 0x05, 0x7D, 0x42, 0x4E, 0x96, 0xB6, 0x16, 0x0C },
        { 0x00, 0xB6, 0x85, 0xB1, 0xCF, 0xEF, 0xFB, 0x38 },
        { 0x00, 0x00, 0x82, 0xEA, 0xA7, 0x1E, 0x9D, 0x93 }
    };

    gtl::rsa::key_type keys{
        gtl::rsa::public_key_type{
            gtl::big_unsigned(key_public, 8),
            gtl::big_unsigned(key_exponent, 8) },
        gtl::rsa::private_key_type{
            gtl::big_unsigned(key_public, 8),
            gtl::big_unsigned(key_exponent, 8),
            gtl::big_unsigned(key_private, 8),
            { gtl::big_unsigned(), gtl::big_unsigned() },
            { gtl::big_unsigned(), gtl::big_unsigned() },
            gtl::big_unsigned() }
    };

    for (unsigned int i = 0; i < data_count; ++i) {
        // The output buffers are filled so that unwritten bytes are detected.
        unsigned char test_encrypted[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        gtl::rsa::encrypt(known_decrypted[i], 8, keys.public_key, test_encrypted);
        REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted[i], 8), "Encrypt failed for message %u.", i);

        unsigned char test_decrypted[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        gtl::rsa::decrypt(known_encrypted[i], 8, keys.private_key, test_decrypted);
        REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted[i], 8), "Decrypt failed for message %u.", i);

        unsigned char test_transform_encrypted[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        gtl::rsa::transform_block(known_decrypted[i], 8, key_exponent, key_public, test_transform_encrypted);
        REQUIRE(testbench::is_memory_same(test_transform_encrypted, known_encrypted[i], 8), "Transform to encrypt failed for message %u.", i);

        unsigned char test_transform_decrypted[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        gtl::rsa::transform_block(known_encrypted[i], 8, key_private, key_public, test_transform_decrypted);
        REQUIRE(testbench::is_memory_same(test_transform_decrypted, known_decrypted[i], 8), "Transform to decrypt failed for message %u.", i);
    }
}

namespace {
    /// @brief  Convert a string of hexadecimal characters into bytes.
    std::vector<unsigned char> hex_2_bytes(const char* hex) {
        const auto nibble = [](const char character) -> unsigned char {
            return static_cast<unsigned char>((character >= 'A') ? (character - 'A' + 10) : (character - '0'));
        };
        std::vector<unsigned char> bytes;
        for (unsigned int i = 0; (hex[i] != 0) && (hex[i + 1] != 0); i += 2) {
            bytes.push_back(static_cast<unsigned char>((nibble(hex[i]) << 4) | nibble(hex[i + 1])));
        }
        return bytes;
    }

    gtl::big_unsigned hex_2_big_unsigned(const char* hex) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        return gtl::big_unsigned(bytes.data(), bytes.size());
    }

    // A 1024 bit key that exists only for these tests, generated independently of this library.
    // All of the expected values that use this key were also computed independently of this library.
    constexpr const unsigned int known_length = 128;
    constexpr const char* known_modulus = "C94E2F3F4155BB327E4C3A14EED197EDB7D034D8950693DEF85A82764D718D56BECE43DA68294BE4DB0CECE54C31A3CCCFF43D2FFB282BCA5AAEA444124FFD0AF370C3CE96C037F89E316E3A7C2803785E4BDA6E15F8FF8EBD29711764C9BA79D4B1C5B4832D1E8ABF2C6717B7EAB5E72361E6B7338F87315D48D4FEA8748099";
    constexpr const char* known_private_exponent = "87DAE90DF284EE3A0C7C5B32675E31FF6F6EE74B477FD1E9C78825C5BB4C280BE3E786696EA47820B2288DF3CDB12E8F79C4A64AAC6096F510B00E12169DBEF222E88AF14E419795CAEC69AE6934ED82ACFA5ADA8E8D5A469AD766E3712F385B3D74AD6AC8A213419EAB76C9DAED7FC7B3658E6A57466809B4BC2ACD7BF9C181";
    constexpr const char* known_prime_0 = "F326E2E1BCEBD3757A3652B7A0256D085436650EEC63D309555771582E344EF90C0453E556DD6FBBF2A977C9182D181078F7736D6823DB0AA0A26C5972011B31";
    constexpr const char* known_prime_1 = "D3F13DBCD208A2F517C74A0DD05FC8658C71392BAFD7D0148E1BB79F72ABE44F09607CBEFBCD67E7E6F072AEBF7686067AD4A98BCDAC71DEE713C560BACC91E9";
    constexpr const char* known_exponent_0 = "A9B4B15396950DD0878CD4ACBEC48055F8D8C22DE9453EECDBB89D2E4B894CB93BEEE553430E6846C8760959BC5AF4D9C2640B1708414647239859811035BF71";
    constexpr const char* known_exponent_1 = "0DC94871CADBC2F5DC691B3910CFDA3238D3789B8DE1A4E861620E2CB201DA6505C01FFD3055E9275D8F50766B7FE331F2B5789DDE70142C22ADD78DF638AE59";
    constexpr const char* known_coefficient = "66F1979B96C1ACB3B740A9EBE220447212914D3459063890E5C23CCBB020CCE4F8F8728427B77C2D24D15E093E2530D78BB60341AC30627CA6BCDB6215D8C6D7";

    gtl::rsa::key_type known_keys() {
        return gtl::rsa::key_type{
            gtl::rsa::public_key_type{
                hex_2_big_unsigned(known_modulus),
                gtl::big_unsigned(65537) },
            gtl::rsa::private_key_type{
                hex_2_big_unsigned(known_modulus),
                gtl::big_unsigned(65537),
                hex_2_big_unsigned(known_private_exponent),
                { hex_2_big_unsigned(known_prime_0), hex_2_big_unsigned(known_prime_1) },
                { hex_2_big_unsigned(known_exponent_0), hex_2_big_unsigned(known_exponent_1) },
                hex_2_big_unsigned(known_coefficient) }
        };
    }

    // The message that is encrypted in the padding tests.
    constexpr const char* known_message = "54686520717569636B2062726F776E20666F78206A756D7073206F76657220746865206C617A7920646F672E";

    /// @brief  A random generator that returns the same sequence every time, the bytes 0x00, 0x01, 0x02, and so on.
    std::function<unsigned int()> sequence_generator() {
        unsigned int counter = 0;
        return [counter]() mutable -> unsigned int {
            return 0x00010203u + (0x04040404u * counter++);
        };
    }

    /// @brief  A random generator of reasonable quality with a fixed seed.
    std::function<unsigned int()> pcg_generator(const unsigned long long int seed) {
        unsigned long long int state = 0x853C49E6748FEA9Bull ^ seed;
        return [state]() mutable -> unsigned int {
            const unsigned long long int state_previous = state;
            state = state_previous * 0x5851F42D4C957F2Dull + 0xDA3E39CB94B95BDBull;
            const unsigned int state_shift_xor_shift = static_cast<unsigned int>(((state_previous >> 18u) ^ state_previous) >> 27u);
            const unsigned int rotation = static_cast<unsigned int>(state_previous >> 59u);
            return (state_shift_xor_shift >> rotation) | (state_shift_xor_shift << ((32 - rotation) & 31));
        };
    }
}

TEST(rsa, evaluate, known_key_128) {
    const gtl::rsa::key_type keys = known_keys();
    const std::vector<unsigned char> known_decrypted = hex_2_bytes("01060B10151A1F24292E33383D42474C51565B60656A6F74797E83888D92979CA1A6ABB0B5BABFC4C9CED3D8DDE2E7ECF1F6FB00050A0F14191E23282D32373C41464B50555A5F64696E73787D82878C91969BA0A5AAAFB4B9BEC3C8CDD2D7DCE1E6EBF0F5FAFF04090E13181D22272C31363B40454A4F54595E63686D72777C");
    const std::vector<unsigned char> known_encrypted = hex_2_bytes("5D3D4806F12D419D30B13B41FD92A8B153DCDFDFF08EE89766CA509B7CBB107FEC7B0AE0E29440B814AE0DB463012F9CB97C92FC4B774115D5ECC559A8AC78B973B2D675303E60EDEA4D7D51875C84D46C727C12BDCB845630C617E67FF4299F16F31E853847B225A8E6CDCD6CC8CC27B6DEE720FBDA07BDAD18B0EA098B7B78");

    unsigned char test_encrypted[known_length] = {};
    REQUIRE(gtl::rsa::encrypt(known_decrypted.data(), known_length, keys.public_key, test_encrypted) == true);
    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted.data(), known_length));

    // The private key has its primes, so this uses the chinese remainder theorem.
    unsigned char test_decrypted[known_length] = {};
    REQUIRE(gtl::rsa::decrypt(known_encrypted.data(), known_length, keys.private_key, test_decrypted) == true);
    REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted.data(), known_length));

    // Blinding the private key operation must not change the result.
    unsigned char test_decrypted_blinded[known_length] = {};
    REQUIRE(gtl::rsa::decrypt(pcg_generator(1), known_encrypted.data(), known_length, keys.private_key, test_decrypted_blinded) == true);
    REQUIRE(testbench::is_memory_same(test_decrypted_blinded, known_decrypted.data(), known_length));

    // Without the primes the private exponent is used directly.
    gtl::rsa::private_key_type private_key_without_primes = keys.private_key;
    private_key_without_primes.primes[0] = gtl::big_unsigned();
    private_key_without_primes.primes[1] = gtl::big_unsigned();
    unsigned char test_decrypted_without_primes[known_length] = {};
    REQUIRE(gtl::rsa::decrypt(known_encrypted.data(), known_length, private_key_without_primes, test_decrypted_without_primes) == true);
    REQUIRE(testbench::is_memory_same(test_decrypted_without_primes, known_decrypted.data(), known_length));
}

TEST(rsa, evaluate, chinese_remainder_theorem) {
    const gtl::rsa::key_type keys = known_keys();
    const std::vector<unsigned char> known_decrypted = hex_2_bytes("01060B10151A1F24292E33383D42474C51565B60656A6F74797E83888D92979CA1A6ABB0B5BABFC4C9CED3D8DDE2E7ECF1F6FB00050A0F14191E23282D32373C41464B50555A5F64696E73787D82878C91969BA0A5AAAFB4B9BEC3C8CDD2D7DCE1E6EBF0F5FAFF04090E13181D22272C31363B40454A4F54595E63686D72777C");
    const std::vector<unsigned char> known_encrypted = hex_2_bytes("5D3D4806F12D419D30B13B41FD92A8B153DCDFDFF08EE89766CA509B7CBB107FEC7B0AE0E29440B814AE0DB463012F9CB97C92FC4B774115D5ECC559A8AC78B973B2D675303E60EDEA4D7D51875C84D46C727C12BDCB845630C617E67FF4299F16F31E853847B225A8E6CDCD6CC8CC27B6DEE720FBDA07BDAD18B0EA098B7B78");

    // With an incorrect private exponent the data can only be decrypted using the primes.
    {
        gtl::rsa::private_key_type private_key = keys.private_key;
        private_key.private_exponent = gtl::big_unsigned(3);
        unsigned char test_decrypted[known_length] = {};
        REQUIRE(gtl::rsa::decrypt(known_encrypted.data(), known_length, private_key, test_decrypted) == true);
        REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted.data(), known_length));
    }

    // With incorrect values derived from the primes the result is detected as wrong, and the private exponent is used.
    {
        gtl::rsa::private_key_type private_key = keys.private_key;
        private_key.coefficient = private_key.coefficient + 1;
        unsigned char test_decrypted[known_length] = {};
        REQUIRE(gtl::rsa::decrypt(known_encrypted.data(), known_length, private_key, test_decrypted) == true);
        REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted.data(), known_length));
    }
    {
        gtl::rsa::private_key_type private_key = keys.private_key;
        private_key.exponents[1] = private_key.exponents[1] + 2;
        unsigned char test_decrypted[known_length] = {};
        REQUIRE(gtl::rsa::decrypt(known_encrypted.data(), known_length, private_key, test_decrypted) == true);
        REQUIRE(testbench::is_memory_same(test_decrypted, known_decrypted.data(), known_length));
    }
}

TEST(rsa, evaluate, invalid_values) {
    // The classic small key: modulus 3233 = 61 * 53, public exponent 17, private exponent 2753.
    const unsigned char key_public[2] = { 0x0C, 0xA1 };
    const unsigned char key_private[2] = { 0x0A, 0xC1 };
    const unsigned char key_exponent[2] = { 0x00, 0x11 };
    const unsigned char untouched[2] = { 0xA5, 0xA5 };

    gtl::rsa::key_type keys{
        gtl::rsa::public_key_type{
            gtl::big_unsigned(key_public, 2),
            gtl::big_unsigned(key_exponent, 2) },
        gtl::rsa::private_key_type{
            gtl::big_unsigned(key_public, 2),
            gtl::big_unsigned(key_exponent, 2),
            gtl::big_unsigned(key_private, 2),
            { gtl::big_unsigned(61), gtl::big_unsigned(53) },
            { gtl::big_unsigned(2753 % 60), gtl::big_unsigned(2753 % 52) },
            gtl::big_unsigned(38) }
    };

    // The largest valid value is one less than the modulus.
    {
        const unsigned char largest[2] = { 0x0C, 0xA0 };
        unsigned char output[2] = { 0xA5, 0xA5 };
        REQUIRE(gtl::rsa::encrypt(largest, 2, keys.public_key, output) == true);
        REQUIRE(gtl::rsa::decrypt(output, 2, keys.private_key, output) == true);
        REQUIRE(testbench::is_memory_same(output, largest, 2));
        REQUIRE(gtl::rsa::transform_block(largest, 2, key_exponent, key_public, output) == true);
        REQUIRE(gtl::rsa::decrypt(pcg_generator(2), output, 2, keys.private_key, output) == true);
        REQUIRE(testbench::is_memory_same(output, largest, 2));
    }

    // Values that are not less than the modulus are rejected and not silently reduced, and the output is not written.
    const unsigned char invalid[3][2] = { { 0x0C, 0xA1 }, { 0x0C, 0xA2 }, { 0xFF, 0xFF } };
    for (unsigned int i = 0; i < 3; ++i) {
        unsigned char output[2] = { 0xA5, 0xA5 };
        REQUIRE(gtl::rsa::encrypt(invalid[i], 2, keys.public_key, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
        REQUIRE(gtl::rsa::decrypt(invalid[i], 2, keys.private_key, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
        REQUIRE(gtl::rsa::decrypt(pcg_generator(3), invalid[i], 2, keys.private_key, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
        REQUIRE(gtl::rsa::transform_block(invalid[i], 2, key_exponent, key_public, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
    }

    // A buffer that is shorter than the key is rejected, as the result may not fit.
    {
        const unsigned char data[1] = { 0x02 };
        unsigned char output[2] = { 0xA5, 0xA5 };
        REQUIRE(gtl::rsa::encrypt(data, 1, keys.public_key, output) == false);
        REQUIRE(gtl::rsa::decrypt(data, 1, keys.private_key, output) == false);
        REQUIRE(gtl::rsa::decrypt(pcg_generator(4), data, 1, keys.private_key, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
    }

    // A key without a modulus is rejected.
    {
        const unsigned char data[2] = { 0x00, 0x02 };
        const unsigned char zero[2] = { 0x00, 0x00 };
        unsigned char output[2] = { 0xA5, 0xA5 };
        REQUIRE(gtl::rsa::encrypt(data, 2, gtl::rsa::public_key_type{}, output) == false);
        REQUIRE(gtl::rsa::decrypt(data, 2, gtl::rsa::private_key_type{}, output) == false);
        REQUIRE(gtl::rsa::transform_block(data, 2, key_exponent, zero, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
    }

    // Blinding needs the public exponent and a random generator.
    {
        const unsigned char data[2] = { 0x00, 0x02 };
        unsigned char output[2] = { 0xA5, 0xA5 };
        gtl::rsa::private_key_type private_key = keys.private_key;
        private_key.public_exponent = gtl::big_unsigned();
        REQUIRE(gtl::rsa::decrypt(pcg_generator(5), data, 2, private_key, output) == false);
        REQUIRE(gtl::rsa::decrypt(std::function<unsigned int()>(), data, 2, keys.private_key, output) == false);
        REQUIRE(testbench::is_memory_same(output, untouched, 2));
        // Without blinding the same key is usable.
        REQUIRE(gtl::rsa::decrypt(data, 2, private_key, output) == true);
    }
}

TEST(rsa, evaluate, oaep_known) {
    const gtl::rsa::key_type keys = known_keys();
    const std::vector<unsigned char> message = hex_2_bytes(known_message);
    const unsigned char label[5] = { 'l', 'a', 'b', 'e', 'l' };
    std::vector<unsigned char> message_longest;
    for (unsigned int i = 0; i < known_length - gtl::rsa::oaep_overhead; ++i) {
        message_longest.push_back(static_cast<unsigned char>((i * 3 + 7) % 256));
    }

    // Encrypting with a known sequence in place of random data gives known encrypted data.
    const std::vector<unsigned char> known_encrypted = hex_2_bytes("9B9E9C73A3EB1ADCD6455ECCF4DB28CF3DBE26528FA5F94C806E2AC64BEB494F774F1F3C5BEF875CFFFC2E4784366B19B964047C991F75991D4E83A6EAB5E5D436607FD0951F0987B10E2E1DCD8591457E14F0A944AB22C3E4D979FE400D6C29F7A38CC04969AD8637A389EFCC051C8C79E79A23B5E77C6006DE61C3BD612048");
    const std::vector<unsigned char> known_encrypted_label = hex_2_bytes("52F2FD9702D80D5A2B438542BC64BB9B87E5E0EEB61B93369AAC9717D8D9DBE06BAEB42DC36198754D6CB5EBA86A1B963F5DF0B2F1C925FEC2BC0EB9934C1CCCAFE823CFBAB584FB427A8B17B88E43476A90E0BBA9313C5D4D7E85A18346640BB57B3F40883C502D6EE2A81C50AB527B904D130EC45C8A8D9F071D3BBF8AD227");
    const std::vector<unsigned char> known_encrypted_empty = hex_2_bytes("967BA6591BC74160BE98C78D047E08CC8BB091D94E992009BCD01630503E29C4466E73D68CDD9330D2A6D8A4A7E3B5320117BBCE3C0DB90825AFB6B8F37E06A3E810985F768F721B07C52826500364278E6A956E22457C847C487D4666FD81B45D4EB925282974BB220568A69C48D0404104088F13723C5700AD2C6CC212618E");
    const std::vector<unsigned char> known_encrypted_longest = hex_2_bytes("015878EBAA73D95CF63E204E3AF8B7A99B1BF96555E1618CA304CDBECA1876532D77D33CF5F919A67DC5B70BA9104CB8EB0313BEBC2ECA88F786BBCC4507B0C07D086DA1BAAB2F859E0E989C961C8F00DC7DCFCE0981EC7C9E7F4EC5A05CDE6E156FB1523C20CB08DB149FBF7B97A2EE186F2850892CADAA1BB94784F42D73B7");

    unsigned char test_encrypted[known_length] = {};
    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), message.data(), static_cast<unsigned int>(message.size()), keys.public_key, test_encrypted, known_length) == true);
    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted.data(), known_length));

    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), message.data(), static_cast<unsigned int>(message.size()), keys.public_key, test_encrypted, known_length, label, 5) == true);
    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted_label.data(), known_length));

    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), nullptr, 0, keys.public_key, test_encrypted, known_length) == true);
    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted_empty.data(), known_length));

    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), message_longest.data(), static_cast<unsigned int>(message_longest.size()), keys.public_key, test_encrypted, known_length) == true);
    REQUIRE(testbench::is_memory_same(test_encrypted, known_encrypted_longest.data(), known_length));

    // Decrypting the known encrypted data gives the messages.
    unsigned char test_decrypted[known_length] = {};
    unsigned int test_decrypted_length = 0;
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted.data(), known_length, keys.private_key, test_decrypted, test_decrypted_length) == true);
    REQUIRE(test_decrypted_length == message.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message.data(), static_cast<unsigned int>(message.size())));

    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted_label.data(), known_length, keys.private_key, test_decrypted, test_decrypted_length, label, 5) == true);
    REQUIRE(test_decrypted_length == message.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message.data(), static_cast<unsigned int>(message.size())));

    test_decrypted_length = 1;
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted_empty.data(), known_length, keys.private_key, test_decrypted, test_decrypted_length) == true);
    REQUIRE(test_decrypted_length == 0);

    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted_longest.data(), known_length, keys.private_key, test_decrypted, test_decrypted_length) == true);
    REQUIRE(test_decrypted_length == message_longest.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message_longest.data(), static_cast<unsigned int>(message_longest.size())));

    // Data that was encrypted and padded by another implementation, without and with a label.
    const std::vector<unsigned char> reference_encrypted = hex_2_bytes("53785EBCA46E241D02B34D5106667E0789AE977FAA4690E16083377BF048E6930AECB8F74A7BCF9C106802545E44B7DECDED8B56340BEF8E97C60AEE1FA873BD483FC5CC18015EEB54DB8E600E9526140EC0289FC445FDA8B6D9874183B2C476163301A046170F93D3419AC2C3353AB3CB78907ABB7D3B253F4EE372F59F23D1");
    const std::vector<unsigned char> reference_encrypted_label = hex_2_bytes("42CE41DF6FEA2DF7AB0E7FE5E95A03118365AA475067390418C2E3896E6C135857FAD0FA447B9FE02431B4F928E8BEB8554F29616056B3A9E006B72B5C30B42D382B62863801F4856DFCD421295AFF742773FE30DBB473FD9E36B07F2D13FD485F008C315FFF7377E1DC9F81BB7412FD9B78C8836EBA467B07BDBD864D4AC434");

    REQUIRE(gtl::rsa::decrypt_oaep(reference_encrypted.data(), known_length, keys.private_key, test_decrypted, test_decrypted_length) == true);
    REQUIRE(test_decrypted_length == message.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message.data(), static_cast<unsigned int>(message.size())));

    REQUIRE(gtl::rsa::decrypt_oaep(pcg_generator(6), reference_encrypted_label.data(), known_length, keys.private_key, test_decrypted, test_decrypted_length, label, 5) == true);
    REQUIRE(test_decrypted_length == message.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message.data(), static_cast<unsigned int>(message.size())));
}

TEST(rsa, evaluate, oaep_invalid) {
    const gtl::rsa::key_type keys = known_keys();
    const std::vector<unsigned char> message = hex_2_bytes(known_message);
    const unsigned char label[5] = { 'l', 'a', 'b', 'e', 'l' };
    const std::vector<unsigned char> known_encrypted = hex_2_bytes("9B9E9C73A3EB1ADCD6455ECCF4DB28CF3DBE26528FA5F94C806E2AC64BEB494F774F1F3C5BEF875CFFFC2E4784366B19B964047C991F75991D4E83A6EAB5E5D436607FD0951F0987B10E2E1DCD8591457E14F0A944AB22C3E4D979FE400D6C29F7A38CC04969AD8637A389EFCC051C8C79E79A23B5E77C6006DE61C3BD612048");
    const std::vector<unsigned char> known_encrypted_label = hex_2_bytes("52F2FD9702D80D5A2B438542BC64BB9B87E5E0EEB61B93369AAC9717D8D9DBE06BAEB42DC36198754D6CB5EBA86A1B963F5DF0B2F1C925FEC2BC0EB9934C1CCCAFE823CFBAB584FB427A8B17B88E43476A90E0BBA9313C5D4D7E85A18346640BB57B3F40883C502D6EE2A81C50AB527B904D130EC45C8A8D9F071D3BBF8AD227");

    unsigned char untouched[known_length];
    unsigned char output[known_length];
    for (unsigned int i = 0; i < known_length; ++i) {
        untouched[i] = 0xA5;
        output[i] = 0xA5;
    }
    unsigned int output_length = 1;

    // A message that is one byte too long, or an output that is not the length of the key, cannot be encrypted.
    const unsigned char message_too_long[known_length] = {};
    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), message_too_long, known_length - gtl::rsa::oaep_overhead + 1, keys.public_key, output, known_length) == false);
    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), message.data(), static_cast<unsigned int>(message.size()), keys.public_key, output, known_length - 1) == false);
    REQUIRE(gtl::rsa::encrypt_oaep(sequence_generator(), message.data(), static_cast<unsigned int>(message.size()), keys.public_key, output, known_length + 1) == false);
    REQUIRE(gtl::rsa::encrypt_oaep(std::function<unsigned int()>(), message.data(), static_cast<unsigned int>(message.size()), keys.public_key, output, known_length) == false);
    REQUIRE(testbench::is_memory_same(output, untouched, known_length));

    // The label must match the label that was used to encrypt.
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted.data(), known_length, keys.private_key, output, output_length, label, 5) == false);
    REQUIRE(output_length == 0);
    output_length = 1;
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted_label.data(), known_length, keys.private_key, output, output_length) == false);
    REQUIRE(output_length == 0);
    output_length = 1;
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted_label.data(), known_length, keys.private_key, output, output_length, label, 4) == false);
    REQUIRE(output_length == 0);

    // Data of the wrong length is rejected.
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted.data(), known_length - 1, keys.private_key, output, output_length) == false);
    REQUIRE(gtl::rsa::decrypt_oaep(known_encrypted.data(), 0, keys.private_key, output, output_length) == false);

    // Data that is not less than the modulus is rejected.
    const std::vector<unsigned char> modulus = hex_2_bytes(known_modulus);
    REQUIRE(gtl::rsa::decrypt_oaep(modulus.data(), known_length, keys.private_key, output, output_length) == false);

    // Tampered data is rejected.
    for (unsigned int i = 0; i < known_length; i += 17) {
        std::vector<unsigned char> tampered = known_encrypted;
        tampered[i] ^= 0x04;
        output_length = 1;
        REQUIRE(gtl::rsa::decrypt_oaep(tampered.data(), known_length, keys.private_key, output, output_length) == false, "Tampered byte %u was not detected.", i);
        REQUIRE(output_length == 0);
    }

    // Blocks that decrypt to padding that is not valid: a first byte that is not zero, no separator before the message, and a padding byte that is not zero.
    const std::vector<unsigned char> invalid_first_byte = hex_2_bytes("2D2663B20A7EDC01C51B8D698D4168E17B6B0D419FA290CC44B7C80813F79C255B1F581AE6F6ED7C0E70AD2FAA0BD59A7146A429DA0E958C58AE0276E32CC778EC9028C3414BA04A9EAD27F161BFF415892627C52E22988183ADCECECAD742A39B877073A8C53C4A7D7C25DF1B43701B441845EA7C23D8CAC4DD61FFC3AB8FEB");
    const std::vector<unsigned char> invalid_no_separator = hex_2_bytes("249BFF13798205CD640BE1CDB93B6B287BEECB95A77A054A73ADCCAC2B52C0B233AA664165330A4A43C785643E4512CF61E85F89F19FCBA7F88EBFDC32EB873E27D2C929CD96FC5C8F606DB3212CE72437D8BAE154C06755872B7DC2727A5AF034FF67AFBE101F999FCF0DAB265EF91C92201D36A384F481371E08A088363E6F");
    const std::vector<unsigned char> invalid_padding = hex_2_bytes("22458E3164132982B536A928F27FE51F78D6D4C16D8F45B24AD1421DB8822CFA6910D28108CEFC6CC57ACE0F5C9EF24D7229BA534AE3622C5CB044B8EAEE6F711A4165A763C72B7E2C84C4BF8BF76B52BC3B38FA3876FED7E09E1D6A137E39D20BDE516C46D7E2DAC6290F0E6379156883317C9963B1A7D62AFE1631F870F14B");
    REQUIRE(gtl::rsa::decrypt_oaep(invalid_first_byte.data(), known_length, keys.private_key, output, output_length) == false);
    REQUIRE(gtl::rsa::decrypt_oaep(invalid_no_separator.data(), known_length, keys.private_key, output, output_length) == false);
    REQUIRE(gtl::rsa::decrypt_oaep(invalid_padding.data(), known_length, keys.private_key, output, output_length) == false);
    REQUIRE(gtl::rsa::decrypt_oaep(pcg_generator(7), invalid_padding.data(), known_length, keys.private_key, output, output_length) == false);

    // Nothing above may have written to the output.
    REQUIRE(testbench::is_memory_same(output, untouched, known_length));
}

TEST(rsa, evaluate, oaep_random) {
    const gtl::rsa::key_type keys = known_keys();
    const std::vector<unsigned char> message = hex_2_bytes(known_message);
    const std::function<unsigned int()> random_generator = pcg_generator(8);

    // Encrypting the same message twice must give different data that decrypts to the same message.
    unsigned char test_encrypted_0[known_length] = {};
    unsigned char test_encrypted_1[known_length] = {};
    REQUIRE(gtl::rsa::encrypt_oaep(random_generator, message.data(), static_cast<unsigned int>(message.size()), keys.public_key, test_encrypted_0, known_length) == true);
    REQUIRE(gtl::rsa::encrypt_oaep(random_generator, message.data(), static_cast<unsigned int>(message.size()), keys.public_key, test_encrypted_1, known_length) == true);
    REQUIRE(testbench::is_memory_same(test_encrypted_0, test_encrypted_1, known_length) == false);

    unsigned char test_decrypted[known_length] = {};
    unsigned int test_decrypted_length = 0;
    REQUIRE(gtl::rsa::decrypt_oaep(test_encrypted_0, known_length, keys.private_key, test_decrypted, test_decrypted_length) == true);
    REQUIRE(test_decrypted_length == message.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message.data(), static_cast<unsigned int>(message.size())));

    REQUIRE(gtl::rsa::decrypt_oaep(random_generator, test_encrypted_1, known_length, keys.private_key, test_decrypted, test_decrypted_length) == true);
    REQUIRE(test_decrypted_length == message.size());
    REQUIRE(testbench::is_memory_same(test_decrypted, message.data(), static_cast<unsigned int>(message.size())));
}

TEST(rsa, evaluate, generate_keys_sizes) {
    const unsigned int sizes[10] = { 2, 2, 4, 4, 8, 8, 16, 16, 16, 32 };
    for (unsigned int index = 0; index < 10; ++index) {
        const unsigned int size_bytes = sizes[index];
        const gtl::rsa::key_type keys = gtl::rsa::generate_key_pair(pcg_generator(100 + index), size_bytes);

        // The modulus must be exactly the requested size, which needs both primes to have their two most significant bits set.
        REQUIRE(keys.public_key.public_modulus.get_length_bits() == size_bytes * 8, "Key %u has a modulus of %llu bits.", index, keys.public_key.public_modulus.get_length_bits());
        REQUIRE(keys.public_key.public_modulus == keys.private_key.public_modulus);
        REQUIRE(keys.public_key.public_modulus == keys.private_key.primes[0] * keys.private_key.primes[1]);
        REQUIRE(keys.private_key.primes[0] != keys.private_key.primes[1]);
        REQUIRE(keys.private_key.primes[0].get_length_bits() == size_bytes * 4);
        REQUIRE(keys.private_key.primes[1].get_length_bits() == size_bytes * 4);
        REQUIRE(keys.public_key.public_exponent == 65537);

        // A message with a leading zero byte is always less than the modulus.
        unsigned char message[32] = {};
        for (unsigned int i = 1; i < size_bytes; ++i) {
            message[i] = static_cast<unsigned char>(i * 29 + index);
        }

        unsigned char test_encrypted[32] = {};
        REQUIRE(gtl::rsa::encrypt(message, size_bytes, keys.public_key, test_encrypted) == true);

        unsigned char test_decrypted[32] = {};
        REQUIRE(gtl::rsa::decrypt(test_encrypted, size_bytes, keys.private_key, test_decrypted) == true);
        REQUIRE(testbench::is_memory_same(test_decrypted, message, size_bytes), "Key %u did not round trip.", index);

        // The private exponent alone must also decrypt.
        gtl::rsa::private_key_type private_key_without_primes = keys.private_key;
        private_key_without_primes.primes[0] = gtl::big_unsigned();
        private_key_without_primes.primes[1] = gtl::big_unsigned();
        unsigned char test_decrypted_without_primes[32] = {};
        REQUIRE(gtl::rsa::decrypt(test_encrypted, size_bytes, private_key_without_primes, test_decrypted_without_primes) == true);
        REQUIRE(testbench::is_memory_same(test_decrypted_without_primes, message, size_bytes), "Key %u did not round trip without its primes.", index);

        unsigned char test_decrypted_blinded[32] = {};
        REQUIRE(gtl::rsa::decrypt(pcg_generator(200 + index), test_encrypted, size_bytes, keys.private_key, test_decrypted_blinded) == true);
        REQUIRE(testbench::is_memory_same(test_decrypted_blinded, message, size_bytes), "Key %u did not round trip when blinded.", index);
    }
}

TEST(rsa, evaluate, generate_keys_exponent) {
    // A small public exponent must also generate a working key.
    const gtl::rsa::key_type keys = gtl::rsa::generate_key_pair(pcg_generator(300), 16, 3);
    REQUIRE(keys.public_key.public_exponent == 3);
    REQUIRE(keys.public_key.public_modulus.get_length_bits() == 16 * 8);

    const unsigned char message[16] = { 0x00, 0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A, 0x69, 0x78, 0x87, 0x96, 0xA5, 0xB4, 0xC3, 0xD2, 0xE1 };
    unsigned char test_encrypted[16] = {};
    REQUIRE(gtl::rsa::encrypt(message, 16, keys.public_key, test_encrypted) == true);
    unsigned char test_decrypted[16] = {};
    REQUIRE(gtl::rsa::decrypt(test_encrypted, 16, keys.private_key, test_decrypted) == true);
    REQUIRE(testbench::is_memory_same(test_decrypted, message, 16));
}

TEST(rsa, evaluate, generate_keys_invalid) {
    const auto is_empty = [](const gtl::rsa::key_type& keys) -> bool {
        return (keys.public_key.public_modulus == 0) &&
               (keys.public_key.public_exponent == 0) &&
               (keys.private_key.public_modulus == 0) &&
               (keys.private_key.public_exponent == 0) &&
               (keys.private_key.private_exponent == 0) &&
               (keys.private_key.primes[0] == 0) &&
               (keys.private_key.primes[1] == 0) &&
               (keys.private_key.exponents[0] == 0) &&
               (keys.private_key.exponents[1] == 0) &&
               (keys.private_key.coefficient == 0);
    };

    // Sizes that cannot be split into two primes of whole bytes.
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 0)));
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 1)));
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 3)));
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 15)));

    // Exponents that can never have an inverse, which would otherwise never return.
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 16, 0)));
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 16, 1)));
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 16, 2)));
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 16, 65536)));

    // A missing random generator.
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(std::function<unsigned int()>(), 16)));

    // The same arguments with a valid size and exponent do generate a key.
    REQUIRE(is_empty(gtl::rsa::generate_key_pair(pcg_generator(400), 16)) == false);
}

namespace {
    // The digests of the known message, and signatures of it made with the known key, all computed independently of this library.
    struct known_signature_type {
        gtl::rsa::signature_hash_type hash_type;
        const char* digest;
        const char* pkcs1;
        const char* pss_reference;
        const char* pss_fixed;
    };

    // The pss_reference signatures have a random salt as long as the hash, and the pss_fixed signatures have a salt of the bytes 0x00, 0x01, 0x02, and so on, as long as the hash.
    // The known key is too short to make a pss signature using sha2-512 with a salt as long as the hash.
    constexpr const known_signature_type known_signatures[] = {
        { gtl::rsa::signature_hash_type::sha1,
          "408D94384216F890FF7A0C3528E8BED1E0B01621",
          "3EC4D9F16106781FE8C28E9957F0F9BA3ABB5A466CB21F9BECF72AEA2C560CD644333AA5A3D670CD5349BC475FD36727D032143327AB204ADBAE8A5CBFEB7B942A324DCF61BEC2F1FD018693707E4C9E37CCFFB3FD442AFA226F916A0FCDC0D5CE4E96F3AC7FA84060EC41AF22DF0CF3B2F07C68899EE77442B5702A0CBD47F0",
          "763D1E15CA4491C3B175CE197A283642001A3D1338CFCD8B1C4B408930668DB630C9E6BB50B66DA61D8D6AC6C18EBBCEF7E30E683BC936B26F65BEE7A26EC3399FD65B27E7C3B3242F18364A217DC42E4C89404DAEC242371CEB5EE010502080C531B78E08B3B19F3795E2939767AD7F7E6E433BFBE465275900B5268F97DA24",
          "3E3FD9CFA4384B326D806E0FC92AA444569274C10B03FFF6083F3595E716C3D3DA28095F97EB6BB7C1C2AF338DF88942C0B1C57942A465F0F676C34522E2F9FA493325DC7C007B73B067091540BD6BD8A27A33222F157BED559F8145390E97DE7D689E85B9337805C48ECDB1A15112ECA176104AAA65DF20B5EE9E5A6DE99CC4" },
        { gtl::rsa::signature_hash_type::sha2_256,
          "EF537F25C895BFA782526529A9B63D97AA631564D5D789C2B765448C8635FB6C",
          "6686A729D0800F4F474E37DE875BFE82D26B971737D062299592E8AC468F8BE79A15ABA96BAFBA6FEE5B4DF802EAD99098DD644AA9C0BC87DE2391273097047761B1EC6DDA8CB984918F3AEA7FE06170B925848B8D10BEB5AA540D51D8342A0B94825A3682400E1D55BC3213D37C6BD00AF4BFB3A89EAE0092B8D11731E56994",
          "A21FC38DE403674FE8A0D5A8365B5FA6F5C80AEB0AEE1883DB9EC3FACCAD46F563597A35B6AE18C9C064AB3CA31ABF84DF30966EBA43815B9B65E3B491BA63CC8B9DD2414551CBE9C3CFC22B78B43AB84497619213F0509CFE8DDFD6C3F4A4F33DD261156561AD163C47B6C2F0C542C900479F02EF1AECF3BE1D2C0A2BDA999D",
          "419EB77BA7B503D17BA3E2968D69CC5609BCD07E859E0B8E317377E9CF16B12AF7BBCC0D3842D0ABFFD61CB40D20DBAB9A73B7AB80BE8427B2CF870FFDF044AEE4D18AC87DDED5A28C5F53C2A1CB282DE220A3C5895DAF9D458CA84F3C382D1DA9A375E8130440E50947F39F3F76E5E8FC064983591207121D707ED07E0013D6" },
        { gtl::rsa::signature_hash_type::sha2_384,
          "ED892481D8272CA6DF370BF706E4D7BC1B5739FA2177AAE6C50E946678718FC67A7AF2819A021C2FC34E91BDB63409D7",
          "93E088AE6C1FB8F3DC88ACD262BFD56D00FDA6A7DEA9E52F0193A7337BB162FC06D6DC3419C5449FF3A299DDEE7C216A02FE5EC194D05068BCAEE7FC49CF639DE476F4147585C8B556AF08E8F9612F21836CD0B9F224F3A892C95515AB0EF3465C70CAF81E136AEDA8B7B30B4B12A8D4F723442ECC659D9BAE2A567D507229A5",
          "63D9A5E7D7E816BD842E948FD8D91DE675C7AF88E0E1144785DCBD6D2F88DE3B8CA191C645D306148A8E90E78758043AB8633311205D1D341A4FEA29DEE32725624131351662BDB4C6D4CBB5B4A33B7FD8967F5548228C8E02D24B6C9DD9AF524E8CA235DE529B3DF98C92B0174CE55FF27CAC91E11A52FF529A269748856CEB",
          "7AFAAF7C962EEF4610D82A5194F46EE4AC1680A7B94BD06DCE1E215440E7299B55314F099F00A6805E9A73F96A41D4CF90B7E596A33AE66DE3B963BB6F700795A36F9A7A56358812A369795DEF6A9689451ACFCFFC54F530A22982DE5CEB695BC67763CA520BC4A2E7FB97C90CD18A9D4B431A6080030EA546D0CFEB3110C123" },
        { gtl::rsa::signature_hash_type::sha2_512,
          "91EA1245F20D46AE9A037A989F54F1F790F0A47607EEB8A14D12890CEA77A1BBC6C7ED9CF205E67B7F2B8FD4C7DFD3A7A8617E45F3C463D481C7E586C39AC1ED",
          "87345DC62C6F442BCEC2AED0E3522DE5131C7154A278C81BE4CC4F88A46529123409FCD88E3F9D125A302C11D936B27B5C1D8DFF380F41E620A9528FD58CC9F418C4D3146DA0E6AFC439433105207BB46CBCA026A607DE48ECDF5CB8CAD9EC38EB138F931E6282FF99AB6D0D339F637DB6DE2686B3D94DE2449D67E244ACCE52",
          nullptr,
          nullptr },
    };

    // Signatures using sha2-256 with no salt, and with a salt of the twenty bytes 0x00, 0x01, 0x02, and so on.
    constexpr const char* known_pss_salt_0 = "04DFB046E36D5550761883F0438169638E46D0CB256A295AD9B4FDA04F07D55F86D4CE3E41B4B42CC3E182ADAA67AB9AF75CC4CEC24892A284AEB81F3035ED076B83E0E9DFFF9A9AF20173321F56506F35AFE8F44F67690DD5AC91255B914BAE6108DC0F49D909CE1C906DD709A497271D6C797B19D4C300E8B5FC52872B9AE6";
    constexpr const char* known_pss_salt_20 = "939D2B0ECA8455D171794CB731A15D1D596D5D14B54F1FA75788A47960F8F23703940EBDB06665F9B1904517AC5739BC0A7BC8BE020C1E74015D51D721C05E74E36E146F4BDFE2509A9811469831CB2D36866996F8AF43335521BF5B2074FE82699EC19FDBA55BD11712219CA51307F53AFC30E5DBE7DE873A7B5996A8155570";

    // A 1025 bit key, for which the encoded message of a pss signature is one byte shorter than the key.
    constexpr const unsigned int odd_length = 129;
    constexpr const char* odd_modulus = "01E61535721C2141CFA5B5FA1A1253F3FAC51E089326494E646C2E4D45A88AF19160D5B3DCAE5FB6BEDE4317863C12B3C179C792460A5790D76BDD4E955AC878FAD50DC39DB2B900CA9DA4630F92ED00E7B2244BD54E458432B5913097221E2E78ABEF2F09101E2D6A92C6022FE3D54F4E9489738E58D2FB2184423CA20D141AA7";
    constexpr const char* odd_private_exponent = "00FD6BE14974D101F6B8767FC647F7ED23B8D147B73026D7F01356B082B8B477ED4B871F16D6F29FDC3314D796EC8AF39CD358F132030254497BBB0DD56057547A2BFAFD90D4661A48C3C82428BF203469F89566384B7F943E818418BD41FA8F6FC7B783284AFF67D2ABDBA950E0959C54BF00BD8589C9FCABB1F71E02B50CF001";
    constexpr const char* odd_pkcs1 = "010625B90E5B0AE5D64069B71B57F6DA05B9CDD8241B8424B39211923F72A440E1C766BE3A0A2D390F39148C20BC3A5E8A57FF28ADD50E3DA63587EF4D9345837C83051063FBB8FD9CE2C630026A582177495E800EE53EA420904B5305A25277CABE9096CFEF41B14DF495E1F4ECF84030AD3355132AC57003CDF451700E4F83E2";
    constexpr const char* odd_pss_reference = "015E833B8C836A4845DC72E832984C7491AFEF605B8AF427B96A68B8FA64100225BEEFD0D0BDF4E22E95AD2366CFDFBDD2CEE0656A70B902CE6473C2BD0F83DBF141ABAB9F5617358AC9C1CB4C755A19A53E96CF3EC7B3E61BB1A8B7A92037709C089404E4464BDD01DA8A2F53EC6432DB6413F332506B88B5D5B0B9E68AA04859";
    constexpr const char* odd_pss_fixed = "002E73CF850684660B02707CBD6F2A1F87FD0D3D6925869DC3A9F87427D15722A5129312682882C7524A4E3191A5247D1D505C4D787D3CF89F8306301169EC63292DA7F0DDEA8F04AD2E3E5C9551D80A184935D988B6885CF25D4F6F3438F97567206F3742F094B4B81C10D3E35E2325BBFB81A1A6D9B0F8B71F13255A7512A13C";

    // A 2048 bit key, which is long enough for every hash.
    constexpr const unsigned int long_length = 256;
    constexpr const char* long_modulus = "BF636134A68B3EE8653B1777A15E6BEEC9ED8C5DB27FE186E11FBB5D473854808F546E5925D6CA7B1C743F8D3D8A8FDCC8A0B08D2A583ECD70B8D642780338294A23A147C902CD8E671A4EAD589D41BAE41E7A6DE3B3AAA8AA8564505B896E8A9019685DD4035DFD2CDA70FB753E1073E91CB2A42907E07B1527253C9DA77507CEE892C63A7C803A70BA253C2889E011073C76B407092DD4F76B2F73521114204D095EFA5154BEF52A51C8715C4F1CFBCEA7608EE082E9804B57C0B479826308B9FD7D10CAF8A9301B69894CE18EAE5DE010778C36F9B95AB196D462E23DF4DC8034979AD89E0EFF9304F25874D74A6D9D8EF64045821216B95C29D63226C91B";
    constexpr const char* long_private_exponent = "0E56560A592D963D5794092A8392ACB83B3549EE1CF89A7EFDAF605C5900FDC791B52BFD0EAA1C0A03F42B6C9BBF1D0ED792BE171D247633F2AD4E13B5A4DC18AFBEA28EDB9E93A286D3C0301932D2A13B04937C56F1F727627AD2AC7866FFBA6E9810AAF2BE7BD482FF2C36922E5F922754E88706B1D9DDAE947BA42255509D6F76E7281FBD26F174BF709E9F63AD56CEC82273B7481DC69DB2C2E955C9DD6099F837B3A4E2FE7EF4666477093F4C3BD02EA79F4E5EB6E35B7EC37A9793B30BA47285531931E6AD680D7C20AE316E46ED887B3E80C0A10970F44AB6470F9FD465505D389431C346A3289AE571498AE2064260D3ED10CF93B98A019DDAAA9E21";
    constexpr const char* long_pkcs1_sha2_512 = "1AD15B3D8BBB4D29B6D06AD6868C84A96FD089B95A2A5D51A75918D8596B6F08B1A9472946B7BD4B837BD56D305582859BA7E2FA71AE05792A599C1EC05203B4BC095821D2F47BB1F267380AE0FACE0A9F515337FFB84B41CAB10B2B441EAE374F54A9839C8972DCD1874F29E3ADECBEF927E7431EEE15752F778E0D377B490C051711E6300447D4872560512006C5066F0C3A98BC50CA29466E9F209E48245F11EBCEBCC6A769D9168955F54C8DCC949687BD4A7B1F25D83FB0CE7BBDE8AB24D77BF96EBD7FA40344BB3D508522875AAD8D93B6C6CE8E41C31A603920170EB755096AF67D0BAC59935F12318FA31B8C55641616BDDCC4675390085DCE944510";
    constexpr const char* long_pss_reference_sha2_256 = "60F34272544192E7564BDA54B4BAD53DB89E1D5F9A3DA9C258A596E125FC2AAF53229493C2E32C3D4B8497E205AAD04736F34740421BB47D2B16B470BF4A01131D810103A6B53216F5D94B22A687079C915457A7F2B162DCCE64CC37857B1DCEE6B23D3BF68A4B567D7F456D20BBA52F9EBB4F258F604C738CC8AD8F1046E314D79E496216BA0D5EA0A0BE24A73BD8B2D0EB772539E0203638D230DCE9B212F190B0EDCE39E83FC6B67FE09B73452BA5E1FD58829C8E29EA6D0B8FF84E946EE67166F50646029BA582AB4C528D962A421D01C128631C381106CFF60FFA85A7BA10386A9531509806A771CC4E7B13A02CA7A6935C1D3245D329C5464D149E8173";
    constexpr const char* long_pss_reference_sha2_512 = "0F0A565391E91453F21A2F57999B63C80E9B85BD19FCB5BD893AFC94174F1D426808092B656BFAB9B339479B43D9465031F29FB448047BD6FE3CC4AAAFDFE31F92D39504066A8D45516C9DF9E261C2C478178D00ACE891D5FA8EB93BDDE0DB25512337B72EE8B47CAE01DE5BAB39CB7F59D936E02ED91457BC178E1F0B28FCBF0088FDF88532E527DE8A98FB2A9D8B8C19AB044C4BA68F6D02AC70152A4AE45F6E132229F3F04302795CD3D23A86065F910F646189AEE7613051888787A252CABACA55B3F2FB59738CE60FD455AC6E79543DB70379596999E564DDCB271E8BC8EECDE561AA6FE9A068E654391B509A0BC11BBCD2241E47C32B50389CC906E64E";
    constexpr const char* long_pss_fixed_sha2_512 = "558D540C8F3FF83DEF8F6941E9A98861B700A27F433C885AB63A9323637E1EDA9CDA71A4775C089755AE24C92E504E372A36DB0A1E0686E7DD5F37E0EF8D65394CEFCB5CEFFDC91B3919523688A07D82D94E6BAC45A6CAD13A84DD3DAA1353EA34B7F8EFD6CCBCA43E7F2F664CC62F1E0EB8D29DE2D8168E8E24602E185F0762EEF0916DDD66767458BE348331D40B54636BF6F95A3FBA7A8D8AED58F58253A212552E4AEA931F7C979FC129917FF89F3A5DE5C05A70EA245851356D054A8528FF91A6F4DB11100F42E74E1BA83FD477E01032EF742E4EE95CCF798C3DE25616797BF8584D0413973AD5B0B7E02C3681CD30F4870551BBCF2CA2D4622831A0BD";

    gtl::rsa::key_type simple_keys(const char* modulus, const char* private_exponent) {
        return gtl::rsa::key_type{
            gtl::rsa::public_key_type{
                hex_2_big_unsigned(modulus),
                gtl::big_unsigned(65537) },
            gtl::rsa::private_key_type{
                hex_2_big_unsigned(modulus),
                gtl::big_unsigned(65537),
                hex_2_big_unsigned(private_exponent),
                { gtl::big_unsigned(0), gtl::big_unsigned(0) },
                { gtl::big_unsigned(0), gtl::big_unsigned(0) },
                gtl::big_unsigned(0) }
        };
    }
}

TEST(rsa, evaluate, signature_hash) {
    REQUIRE(gtl::rsa::get_signature_hash_size(gtl::rsa::signature_hash_type::sha1) == 20);
    REQUIRE(gtl::rsa::get_signature_hash_size(gtl::rsa::signature_hash_type::sha2_256) == 32);
    REQUIRE(gtl::rsa::get_signature_hash_size(gtl::rsa::signature_hash_type::sha2_384) == 48);
    REQUIRE(gtl::rsa::get_signature_hash_size(gtl::rsa::signature_hash_type::sha2_512) == 64);
    REQUIRE(gtl::rsa::signature_hash_size_max == 64);

    const std::vector<unsigned char> message = hex_2_bytes(known_message);
    for (const known_signature_type& known : known_signatures) {
        const std::vector<unsigned char> digest = hex_2_bytes(known.digest);
        REQUIRE(digest.size() == gtl::rsa::get_signature_hash_size(known.hash_type));
        unsigned char test_digest[gtl::rsa::signature_hash_size_max] = {};
        gtl::rsa::hash_message(known.hash_type, message.data(), message.size(), test_digest);
        REQUIRE(testbench::is_memory_same(test_digest, digest.data(), digest.size()));
    }
}

TEST(rsa, evaluate, pkcs1_known) {
    const gtl::rsa::key_type keys = known_keys();
    const gtl::rsa::key_type keys_simple = simple_keys(known_modulus, known_private_exponent);
    for (const known_signature_type& known : known_signatures) {
        const std::vector<unsigned char> digest = hex_2_bytes(known.digest);
        const std::vector<unsigned char> signature = hex_2_bytes(known.pkcs1);
        const unsigned int digest_length = static_cast<unsigned int>(digest.size());
        REQUIRE(signature.size() == known_length);

        // The scheme is deterministic, so signing gives the known signature, with or without the primes or blinding.
        unsigned char test_signature[known_length] = {};
        REQUIRE(gtl::rsa::sign_pkcs1(known.hash_type, digest.data(), digest_length, keys.private_key, test_signature, known_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature.data(), known_length));

        unsigned char test_signature_simple[known_length] = {};
        REQUIRE(gtl::rsa::sign_pkcs1(known.hash_type, digest.data(), digest_length, keys_simple.private_key, test_signature_simple, known_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature_simple, signature.data(), known_length));

        unsigned char test_signature_blinded[known_length] = {};
        REQUIRE(gtl::rsa::sign_pkcs1(pcg_generator(11), known.hash_type, digest.data(), digest_length, keys.private_key, test_signature_blinded, known_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature_blinded, signature.data(), known_length));

        REQUIRE(gtl::rsa::verify_pkcs1(known.hash_type, digest.data(), digest_length, keys.public_key, signature.data(), known_length) == true);
    }

    // Keys of other lengths.
    const std::vector<unsigned char> digest_sha2_256 = hex_2_bytes(known_signatures[1].digest);
    const std::vector<unsigned char> digest_sha2_512 = hex_2_bytes(known_signatures[3].digest);
    {
        const gtl::rsa::key_type keys_odd = simple_keys(odd_modulus, odd_private_exponent);
        const std::vector<unsigned char> signature = hex_2_bytes(odd_pkcs1);
        unsigned char test_signature[odd_length] = {};
        REQUIRE(gtl::rsa::sign_pkcs1(gtl::rsa::signature_hash_type::sha2_256, digest_sha2_256.data(), 32, keys_odd.private_key, test_signature, odd_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature.data(), odd_length));
        REQUIRE(gtl::rsa::verify_pkcs1(gtl::rsa::signature_hash_type::sha2_256, digest_sha2_256.data(), 32, keys_odd.public_key, signature.data(), odd_length) == true);
    }
    {
        const gtl::rsa::key_type keys_long = simple_keys(long_modulus, long_private_exponent);
        const std::vector<unsigned char> signature = hex_2_bytes(long_pkcs1_sha2_512);
        unsigned char test_signature[long_length] = {};
        REQUIRE(gtl::rsa::sign_pkcs1(gtl::rsa::signature_hash_type::sha2_512, digest_sha2_512.data(), 64, keys_long.private_key, test_signature, long_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature.data(), long_length));
        REQUIRE(gtl::rsa::verify_pkcs1(gtl::rsa::signature_hash_type::sha2_512, digest_sha2_512.data(), 64, keys_long.public_key, signature.data(), long_length) == true);
    }
}

TEST(rsa, evaluate, pkcs1_invalid) {
    const gtl::rsa::key_type keys = known_keys();
    const gtl::rsa::signature_hash_type sha2_256 = gtl::rsa::signature_hash_type::sha2_256;
    const std::vector<unsigned char> digest = hex_2_bytes(known_signatures[1].digest);
    const std::vector<unsigned char> signature = hex_2_bytes(known_signatures[1].pkcs1);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length) == true);

    // Changing any bit of the signature or the digest makes the signature invalid.
    for (unsigned int bit = 0; bit < (known_length * 8); bit += 7) {
        std::vector<unsigned char> signature_changed = signature;
        signature_changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, signature_changed.data(), known_length) == false);
    }
    for (unsigned int bit = 0; bit < (32 * 8); ++bit) {
        std::vector<unsigned char> digest_changed = digest;
        digest_changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest_changed.data(), 32, keys.public_key, signature.data(), known_length) == false);
    }

    // A signature is only valid for the hash function it was made with, and the digest must be the size of the hash.
    REQUIRE(gtl::rsa::verify_pkcs1(gtl::rsa::signature_hash_type::sha1, digest.data(), 20, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(gtl::rsa::signature_hash_type::sha1, digest.data(), 32, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(gtl::rsa::signature_hash_type::sha2_384, digest.data(), 32, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 31, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 33, keys.public_key, signature.data(), known_length) == false);

    // The signature must be the length of the key, and less than the modulus.
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length - 1) == false);
    std::vector<unsigned char> signature_long(known_length + 1, 0);
    for (unsigned int i = 0; i < known_length; ++i) {
        signature_long[i + 1] = signature[i];
    }
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, signature_long.data(), known_length + 1) == false);
    const std::vector<unsigned char> modulus = hex_2_bytes(known_modulus);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, modulus.data(), known_length) == false);
    const std::vector<unsigned char> zeros(known_length, 0);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, zeros.data(), known_length) == false);

    // Null pointers are rejected.
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, nullptr, 32, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, nullptr, known_length) == false);

    // A different key does not verify the signature.
    const gtl::rsa::key_type keys_other = gtl::rsa::generate_key_pair(pcg_generator(12), known_length, 65537, 8);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys_other.public_key, signature.data(), known_length) == false);

    // Blocks that hold the digest but are not exactly the expected encoding are rejected.
    // These are made with the raw private key operation, so each one is a correctly signed block with an incorrect layout.
    const unsigned char prefix[19] = { 0x30, 0x31, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20 };
    const auto make_block = [&](const unsigned int padding_length, const unsigned char type, const unsigned char padding, const unsigned char trailing) -> std::vector<unsigned char> {
        std::vector<unsigned char> block;
        block.push_back(0x00);
        block.push_back(type);
        for (unsigned int i = 0; i < padding_length; ++i) {
            block.push_back(padding);
        }
        block.push_back(0x00);
        for (unsigned int i = 0; i < 19; ++i) {
            block.push_back(prefix[i]);
        }
        for (unsigned int i = 0; i < 32; ++i) {
            block.push_back(digest[i]);
        }
        while (block.size() < known_length) {
            block.push_back(trailing);
        }
        return block;
    };
    const auto sign_block = [&](const std::vector<unsigned char>& block) -> std::vector<unsigned char> {
        std::vector<unsigned char> block_signature(known_length, 0);
        REQUIRE(block.size() == known_length);
        REQUIRE(gtl::rsa::decrypt(block.data(), known_length, keys.private_key, block_signature.data()) == true);
        return block_signature;
    };
    const unsigned int padding_length = known_length - 3 - 19 - 32;
    // The correct layout is accepted.
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(padding_length, 0x01, 0xFF, 0x00)).data(), known_length) == true);
    // Short padding with the digest followed by other data, the layout used to forge signatures for keys with a small exponent.
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(8, 0x01, 0xFF, 0x00)).data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(8, 0x01, 0xFF, 0xA5)).data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(padding_length - 1, 0x01, 0xFF, 0x00)).data(), known_length) == false);
    // The wrong block type or padding bytes.
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(padding_length, 0x02, 0xFF, 0x00)).data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(padding_length, 0x00, 0xFF, 0x00)).data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(make_block(padding_length, 0x01, 0xFE, 0x00)).data(), known_length) == false);
    // An algorithm identifier that leaves out the null parameters.
    {
        const unsigned char prefix_short[17] = { 0x30, 0x2F, 0x30, 0x0B, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x04, 0x20 };
        std::vector<unsigned char> block;
        block.push_back(0x00);
        block.push_back(0x01);
        for (unsigned int i = 0; i < (padding_length + 2); ++i) {
            block.push_back(0xFF);
        }
        block.push_back(0x00);
        for (unsigned int i = 0; i < 17; ++i) {
            block.push_back(prefix_short[i]);
        }
        for (unsigned int i = 0; i < 32; ++i) {
            block.push_back(digest[i]);
        }
        REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, sign_block(block).data(), known_length) == false);
    }

    // Signing fails without writing the signature when the arguments are invalid.
    unsigned char untouched[known_length];
    unsigned char output[known_length];
    for (unsigned int i = 0; i < known_length; ++i) {
        untouched[i] = 0xA5;
        output[i] = 0xA5;
    }
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, digest.data(), 31, keys.private_key, output, known_length) == false);
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, digest.data(), 32, keys.private_key, output, known_length - 1) == false);
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, digest.data(), 32, keys.private_key, output, known_length + 1) == false);
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, nullptr, 32, keys.private_key, output, known_length) == false);
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, digest.data(), 32, keys.private_key, nullptr, known_length) == false);
    // Blinding needs a random generator and the public exponent.
    REQUIRE(gtl::rsa::sign_pkcs1(std::function<unsigned int()>(), sha2_256, digest.data(), 32, keys.private_key, output, known_length) == false);
    gtl::rsa::private_key_type private_key_no_exponent = keys.private_key;
    private_key_no_exponent.public_exponent = 0;
    REQUIRE(gtl::rsa::sign_pkcs1(pcg_generator(13), sha2_256, digest.data(), 32, private_key_no_exponent, output, known_length) == false);
    // A key that is too short to hold the encoded digest, which needs 62 bytes for sha2-256.
    const gtl::rsa::key_type keys_short = gtl::rsa::generate_key_pair(pcg_generator(14), 60, 65537, 8);
    REQUIRE(keys_short.public_key.public_modulus.get_length_bytes() == 60);
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, digest.data(), 32, keys_short.private_key, output, 60) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys_short.public_key, signature.data(), 60) == false);
    REQUIRE(testbench::is_memory_same(output, untouched, known_length));
    // The shortest key that can hold the encoded digest.
    const gtl::rsa::key_type keys_shortest = gtl::rsa::generate_key_pair(pcg_generator(15), 62, 65537, 8);
    REQUIRE(gtl::rsa::sign_pkcs1(sha2_256, digest.data(), 32, keys_shortest.private_key, output, 62) == true);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys_shortest.public_key, output, 62) == true);
}

TEST(rsa, evaluate, pss_known) {
    const gtl::rsa::key_type keys = known_keys();
    unsigned char salt[gtl::rsa::signature_hash_size_max] = {};
    for (unsigned int i = 0; i < gtl::rsa::signature_hash_size_max; ++i) {
        salt[i] = static_cast<unsigned char>(i);
    }
    for (const known_signature_type& known : known_signatures) {
        const std::vector<unsigned char> digest = hex_2_bytes(known.digest);
        const unsigned int digest_length = static_cast<unsigned int>(digest.size());
        unsigned char test_signature[known_length];
        for (unsigned int i = 0; i < known_length; ++i) {
            test_signature[i] = 0xA5;
        }
        if (known.pss_reference == nullptr) {
            // The key is too short for this hash with a salt of the same length, but not with a shorter salt.
            REQUIRE(gtl::rsa::sign_pss(known.hash_type, digest.data(), digest_length, salt, digest_length, keys.private_key, test_signature, known_length) == false);
            REQUIRE(gtl::rsa::sign_pss(pcg_generator(16), known.hash_type, digest.data(), digest_length, keys.private_key, test_signature, known_length) == false);
            for (unsigned int i = 0; i < known_length; ++i) {
                REQUIRE(test_signature[i] == 0xA5);
            }
            REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, test_signature, known_length) == false);
            REQUIRE(gtl::rsa::sign_pss(known.hash_type, digest.data(), digest_length, salt, known_length - digest_length - 2, keys.private_key, test_signature, known_length) == true);
            REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, test_signature, known_length, known_length - digest_length - 2) == true);
            REQUIRE(gtl::rsa::sign_pss(known.hash_type, digest.data(), digest_length, salt, known_length - digest_length - 1, keys.private_key, test_signature, known_length) == false);
            continue;
        }
        const std::vector<unsigned char> signature_reference = hex_2_bytes(known.pss_reference);
        const std::vector<unsigned char> signature_fixed = hex_2_bytes(known.pss_fixed);
        REQUIRE(signature_reference.size() == known_length);
        REQUIRE(signature_fixed.size() == known_length);

        // Signatures made by another implementation are valid.
        REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, signature_reference.data(), known_length) == true);
        REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, signature_reference.data(), known_length, digest_length) == true);
        REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, signature_fixed.data(), known_length) == true);

        // Signing with a known salt gives the known signature.
        REQUIRE(gtl::rsa::sign_pss(known.hash_type, digest.data(), digest_length, salt, digest_length, keys.private_key, test_signature, known_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature_fixed.data(), known_length));

        // The salt must be the expected length.
        REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, signature_fixed.data(), known_length, digest_length - 1) == false);
        REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, signature_fixed.data(), known_length, digest_length + 1) == false);
        REQUIRE(gtl::rsa::verify_pss(known.hash_type, digest.data(), digest_length, keys.public_key, signature_fixed.data(), known_length, 0) == false);
    }

    // Other salt lengths, including no salt.
    {
        const gtl::rsa::signature_hash_type sha2_256 = gtl::rsa::signature_hash_type::sha2_256;
        const std::vector<unsigned char> digest = hex_2_bytes(known_signatures[1].digest);
        const std::vector<unsigned char> signature_salt_0 = hex_2_bytes(known_pss_salt_0);
        const std::vector<unsigned char> signature_salt_20 = hex_2_bytes(known_pss_salt_20);
        unsigned char test_signature[known_length] = {};
        REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 32, nullptr, 0, keys.private_key, test_signature, known_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature_salt_0.data(), known_length));
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_salt_0.data(), known_length, 0) == true);
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_salt_0.data(), known_length) == false);
        REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 32, salt, 20, keys.private_key, test_signature, known_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature_salt_20.data(), known_length));
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_salt_20.data(), known_length, 20) == true);
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_salt_20.data(), known_length, 0) == false);
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_salt_20.data(), known_length) == false);
        REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 32, nullptr, 20, keys.private_key, test_signature, known_length) == false);
    }

    // Keys of other lengths.
    const std::vector<unsigned char> digest_sha2_256 = hex_2_bytes(known_signatures[1].digest);
    const std::vector<unsigned char> digest_sha2_512 = hex_2_bytes(known_signatures[3].digest);
    {
        const gtl::rsa::signature_hash_type sha2_256 = gtl::rsa::signature_hash_type::sha2_256;
        const gtl::rsa::key_type keys_odd = simple_keys(odd_modulus, odd_private_exponent);
        REQUIRE(keys_odd.public_key.public_modulus.get_length_bits() == 1025);
        const std::vector<unsigned char> signature_reference = hex_2_bytes(odd_pss_reference);
        const std::vector<unsigned char> signature_fixed = hex_2_bytes(odd_pss_fixed);
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest_sha2_256.data(), 32, keys_odd.public_key, signature_reference.data(), odd_length) == true);
        unsigned char test_signature[odd_length] = {};
        REQUIRE(gtl::rsa::sign_pss(sha2_256, digest_sha2_256.data(), 32, salt, 32, keys_odd.private_key, test_signature, odd_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature_fixed.data(), odd_length));
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest_sha2_256.data(), 32, keys_odd.public_key, signature_fixed.data(), odd_length) == true);
        for (unsigned int bit = 0; bit < (odd_length * 8); bit += 5) {
            std::vector<unsigned char> signature_changed = signature_fixed;
            signature_changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
            REQUIRE(gtl::rsa::verify_pss(sha2_256, digest_sha2_256.data(), 32, keys_odd.public_key, signature_changed.data(), odd_length) == false);
        }
    }
    {
        const gtl::rsa::signature_hash_type sha2_512 = gtl::rsa::signature_hash_type::sha2_512;
        const gtl::rsa::key_type keys_long = simple_keys(long_modulus, long_private_exponent);
        const std::vector<unsigned char> signature_reference_sha2_256 = hex_2_bytes(long_pss_reference_sha2_256);
        const std::vector<unsigned char> signature_reference_sha2_512 = hex_2_bytes(long_pss_reference_sha2_512);
        const std::vector<unsigned char> signature_fixed = hex_2_bytes(long_pss_fixed_sha2_512);
        REQUIRE(gtl::rsa::verify_pss(gtl::rsa::signature_hash_type::sha2_256, digest_sha2_256.data(), 32, keys_long.public_key, signature_reference_sha2_256.data(), long_length) == true);
        REQUIRE(gtl::rsa::verify_pss(sha2_512, digest_sha2_512.data(), 64, keys_long.public_key, signature_reference_sha2_512.data(), long_length) == true);
        unsigned char test_signature[long_length] = {};
        REQUIRE(gtl::rsa::sign_pss(sha2_512, digest_sha2_512.data(), 64, salt, 64, keys_long.private_key, test_signature, long_length) == true);
        REQUIRE(testbench::is_memory_same(test_signature, signature_fixed.data(), long_length));
        REQUIRE(gtl::rsa::verify_pss(sha2_512, digest_sha2_512.data(), 64, keys_long.public_key, signature_fixed.data(), long_length) == true);
        // A signature made with one hash is not valid for another.
        REQUIRE(gtl::rsa::verify_pss(sha2_512, digest_sha2_512.data(), 64, keys_long.public_key, signature_reference_sha2_256.data(), long_length) == false);
        REQUIRE(gtl::rsa::verify_pss(sha2_512, digest_sha2_512.data(), 64, keys_long.public_key, signature_reference_sha2_256.data(), long_length, 32) == false);
    }
}

TEST(rsa, evaluate, pss_invalid) {
    const gtl::rsa::key_type keys = known_keys();
    const gtl::rsa::signature_hash_type sha2_256 = gtl::rsa::signature_hash_type::sha2_256;
    const std::vector<unsigned char> digest = hex_2_bytes(known_signatures[1].digest);
    const std::vector<unsigned char> signature = hex_2_bytes(known_signatures[1].pss_reference);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length) == true);

    // Changing any bit of the signature or the digest makes the signature invalid.
    for (unsigned int bit = 0; bit < (known_length * 8); bit += 7) {
        std::vector<unsigned char> signature_changed = signature;
        signature_changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_changed.data(), known_length) == false);
    }
    for (unsigned int bit = 0; bit < (32 * 8); ++bit) {
        std::vector<unsigned char> digest_changed = digest;
        digest_changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::rsa::verify_pss(sha2_256, digest_changed.data(), 32, keys.public_key, signature.data(), known_length) == false);
    }

    // The wrong hash, lengths, or key.
    REQUIRE(gtl::rsa::verify_pss(gtl::rsa::signature_hash_type::sha1, digest.data(), 20, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pss(gtl::rsa::signature_hash_type::sha1, digest.data(), 20, keys.public_key, signature.data(), known_length, 32) == false);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 31, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length - 1) == false);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length, 0xFFFFFFFFu) == false);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length, known_length) == false);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, nullptr, 32, keys.public_key, signature.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, nullptr, known_length) == false);
    const std::vector<unsigned char> modulus = hex_2_bytes(known_modulus);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, modulus.data(), known_length) == false);
    const std::vector<unsigned char> zeros(known_length, 0);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, zeros.data(), known_length) == false);
    const gtl::rsa::key_type keys_other = gtl::rsa::generate_key_pair(pcg_generator(17), known_length, 65537, 8);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys_other.public_key, signature.data(), known_length) == false);

    // A signature of the other scheme is not valid.
    const std::vector<unsigned char> signature_pkcs1 = hex_2_bytes(known_signatures[1].pkcs1);
    REQUIRE(gtl::rsa::verify_pss(sha2_256, digest.data(), 32, keys.public_key, signature_pkcs1.data(), known_length) == false);
    REQUIRE(gtl::rsa::verify_pkcs1(sha2_256, digest.data(), 32, keys.public_key, signature.data(), known_length) == false);

    // Signing fails without writing the signature when the arguments are invalid.
    unsigned char untouched[known_length];
    unsigned char output[known_length];
    for (unsigned int i = 0; i < known_length; ++i) {
        untouched[i] = 0xA5;
        output[i] = 0xA5;
    }
    const unsigned char salt[32] = {};
    REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 31, salt, 32, keys.private_key, output, known_length) == false);
    REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 32, salt, 32, keys.private_key, output, known_length - 1) == false);
    REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 32, salt, 0xFFFFFFFFu, keys.private_key, output, known_length) == false);
    REQUIRE(gtl::rsa::sign_pss(sha2_256, nullptr, 32, salt, 32, keys.private_key, output, known_length) == false);
    REQUIRE(gtl::rsa::sign_pss(sha2_256, digest.data(), 32, salt, 32, keys.private_key, nullptr, known_length) == false);
    REQUIRE(gtl::rsa::sign_pss(std::function<unsigned int()>(), sha2_256, digest.data(), 32, keys.private_key, output, known_length) == false);
    gtl::rsa::private_key_type private_key_no_exponent = keys.private_key;
    private_key_no_exponent.public_exponent = 0;
    REQUIRE(gtl::rsa::sign_pss(pcg_generator(18), sha2_256, digest.data(), 32, private_key_no_exponent, output, known_length) == false);
    REQUIRE(testbench::is_memory_same(output, untouched, known_length));
}

TEST(rsa, evaluate, pss_random) {
    const std::function<unsigned int()> random_generator = pcg_generator(19);
    const gtl::rsa::signature_hash_type hash_types[4] = {
        gtl::rsa::signature_hash_type::sha1,
        gtl::rsa::signature_hash_type::sha2_256,
        gtl::rsa::signature_hash_type::sha2_384,
        gtl::rsa::signature_hash_type::sha2_512
    };
    // Freshly generated keys of a few lengths, all long enough for the longest hash with a salt of the same length.
    for (unsigned int length = 130; length <= 134; length += 2) {
        const gtl::rsa::key_type keys = gtl::rsa::generate_key_pair(random_generator, length, 65537, 8);
        REQUIRE(keys.public_key.public_modulus.get_length_bytes() == length);
        for (const gtl::rsa::signature_hash_type hash_type : hash_types) {
            const unsigned int hash_size = gtl::rsa::get_signature_hash_size(hash_type);
            unsigned char message[100] = {};
            for (unsigned int i = 0; i < 100; ++i) {
                message[i] = static_cast<unsigned char>(random_generator());
            }
            unsigned char digest[gtl::rsa::signature_hash_size_max] = {};
            gtl::rsa::hash_message(hash_type, message, 100, digest);

            std::vector<unsigned char> signature_0(length, 0);
            std::vector<unsigned char> signature_1(length, 0);
            REQUIRE(gtl::rsa::sign_pss(random_generator, hash_type, digest, hash_size, keys.private_key, signature_0.data(), length) == true);
            REQUIRE(gtl::rsa::sign_pss(random_generator, hash_type, digest, hash_size, keys.private_key, signature_1.data(), length) == true);
            // The salt is random, so signing the same digest twice gives different signatures that are both valid.
            REQUIRE(testbench::is_memory_same(signature_0.data(), signature_1.data(), length) == false);
            REQUIRE(gtl::rsa::verify_pss(hash_type, digest, hash_size, keys.public_key, signature_0.data(), length) == true);
            REQUIRE(gtl::rsa::verify_pss(hash_type, digest, hash_size, keys.public_key, signature_1.data(), length) == true);
            digest[0] ^= 0x01;
            REQUIRE(gtl::rsa::verify_pss(hash_type, digest, hash_size, keys.public_key, signature_0.data(), length) == false);
            digest[0] ^= 0x01;

            REQUIRE(gtl::rsa::sign_pkcs1(random_generator, hash_type, digest, hash_size, keys.private_key, signature_0.data(), length) == true);
            REQUIRE(gtl::rsa::sign_pkcs1(hash_type, digest, hash_size, keys.private_key, signature_1.data(), length) == true);
            REQUIRE(testbench::is_memory_same(signature_0.data(), signature_1.data(), length) == true);
            REQUIRE(gtl::rsa::verify_pkcs1(hash_type, digest, hash_size, keys.public_key, signature_0.data(), length) == true);
            digest[hash_size - 1] ^= 0x80;
            REQUIRE(gtl::rsa::verify_pkcs1(hash_type, digest, hash_size, keys.public_key, signature_0.data(), length) == false);
        }
    }
}
