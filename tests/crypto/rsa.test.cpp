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

#include <crypto/rsa>

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
