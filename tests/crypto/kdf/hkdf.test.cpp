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
#include <crypto/kdf/hkdf>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(hkdf, traits, standard) {
    REQUIRE((std::is_pod<gtl::hkdf<gtl::sha2<256>>>::value == false));

    REQUIRE((std::is_trivial<gtl::hkdf<gtl::sha2<256>>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::hkdf<gtl::sha2<256>>>::value == true));

    REQUIRE((std::is_standard_layout<gtl::hkdf<gtl::sha2<256>>>::value == true));
}

TEST(hkdf, constructor, extract) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    testbench::do_not_optimise_away(hkdf);
}

TEST(hkdf, constructor, copy) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    gtl::hkdf<gtl::sha2<256>> hkdf2(hkdf, nullptr, 0);
    testbench::do_not_optimise_away(hkdf2);
}

TEST(hkdf, function, expand) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    hkdf.expand(nullptr, 0);
}

TEST(hkdf, function, output) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    hkdf.expand(nullptr, 0);
    unsigned char out[32] = {};
    hkdf.output(out, 32);
}

TEST(hkdf, function, expand_label) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    hkdf.expand_label("test", gtl::hkdf<gtl::sha2<256>>::null, 0, 32);
}

TEST(hkdf, function, expand_label_output) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    unsigned char out[32] = {};
    hkdf.expand_label("test", gtl::hkdf<gtl::sha2<256>>::null, 0, 32, out, 32);
}

TEST(hkdf, function, get_digest) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    hkdf.get_digest();
}

TEST(hkdf, function, reset) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    hkdf.reset();
}

TEST(hkdf, evaluate, hash_to_string) {
    gtl::hkdf<gtl::sha2<256>>::key_type key = {};
    gtl::hkdf<gtl::sha2<256>> hkdf(key.data, gtl::hkdf<gtl::sha2<256>>::hash_size, nullptr, 0);
    hkdf.expand(reinterpret_cast<const unsigned char*>("test"), 4);
    unsigned char out[32] = {};
    hkdf.output(out, 32);
    gtl::hkdf<gtl::sha2<256>>::hash_type hash = {};
    for (unsigned long long int i = 0; i < 32; ++i) {
        hash.data[i] = out[i];
    }
    gtl::hkdf<gtl::sha2<256>>::hash_string_type str = gtl::hkdf<gtl::sha2<256>>::hash_to_string(hash);
    PRINT("%s\n", str.hash);
}

TEST(hkdf, evaluate, rfc5869_sha256) {
    // RFC 5869 Test Case 1 (SHA-256)
    constexpr static unsigned char ikm[] = {
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b,
        0x0b
    };
    constexpr static unsigned char salt[] = {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0a,
        0x0b,
        0x0c
    };
    constexpr static unsigned char info[] = {
        0xf0,
        0xf1,
        0xf2,
        0xf3,
        0xf4,
        0xf5,
        0xf6,
        0xf7,
        0xf8,
        0xf9
    };
    constexpr static unsigned char expected_prk[32] = {
        0x07,
        0x77,
        0x09,
        0x36,
        0x2c,
        0x2e,
        0x32,
        0xdf,
        0x0d,
        0xdc,
        0x3f,
        0x0d,
        0xc4,
        0x7b,
        0xba,
        0x63,
        0x90,
        0xb6,
        0xc7,
        0x3b,
        0xb5,
        0x0f,
        0x9c,
        0x31,
        0x22,
        0xec,
        0x84,
        0x4a,
        0xd7,
        0xc2,
        0xb3,
        0xe5
    };
    constexpr static unsigned char expected_okm[42] = {
        0x3c,
        0xb2,
        0x5f,
        0x25,
        0xfa,
        0xac,
        0xd5,
        0x7a,
        0x90,
        0x43,
        0x4f,
        0x64,
        0xd0,
        0x36,
        0x2f,
        0x2a,
        0x2d,
        0x2d,
        0x0a,
        0x90,
        0xcf,
        0x1a,
        0x5a,
        0x4c,
        0x5d,
        0xb0,
        0x2d,
        0x56,
        0xec,
        0xc4,
        0xc5,
        0xbf,
        0x34,
        0x00,
        0x72,
        0x08,
        0xd5,
        0xb8,
        0x87,
        0x18,
        0x58,
        0x65
    };

    gtl::hkdf<gtl::sha2<256>> hkdf(salt, sizeof(salt), ikm, sizeof(ikm));
    REQUIRE(testbench::is_memory_same(hkdf.get_digest(), expected_prk, 32) == true);

    hkdf.expand(info, sizeof(info));
    unsigned char out[42] = {};
    hkdf.output(out, 42);
    REQUIRE(testbench::is_memory_same(out, expected_okm, 42) == true);
}
