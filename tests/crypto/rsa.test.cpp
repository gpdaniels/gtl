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

#include <type_traits>

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
