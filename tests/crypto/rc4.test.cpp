/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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

#include <main.tests.hpp>
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <crypto/rc4>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <string>
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

TEST(rc4, constructor, empty) {
    gtl::rc4 rc4;
    testbench::do_not_optimise_away(rc4);
}

TEST(aes, evaluate, examples) {
    constexpr static const unsigned int length_data = 16;
    constexpr static const unsigned int length_key = 16;
    const unsigned char plain[length_data] =     { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key[length_key] =        { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length_data] = { 0xE9, 0x8D, 0x62, 0xCA, 0x03, 0xB7, 0x7F, 0xBB, 0x8E, 0x42, 0x3D, 0x7D, 0xC2, 0x00, 0xC4, 0xB0 };

    unsigned char output[length_data];
    gtl::rc4 rc4;
    rc4.encrypt(plain, length_data, key, length_key, output);

    REQUIRE(testbench::is_memory_same(encrypted, output, length_data), "%s != %s\n", hex_2_string(encrypted, length_data).c_str(), hex_2_string(output, length_data).c_str());
}

TEST(aes, evaluate, roundtrip) {

    class random_pcg final {
    private:
        unsigned long long int state = 0x853C49E6748FEA9Bull;
        unsigned long long int increment = 0xDA3E39CB94B95BDBull;
    public:
        unsigned int get_random_raw() {
            unsigned long long int state_previous = this->state;
            this->state = state_previous * 0x5851F42D4C957F2Dull + this->increment;
            unsigned int state_shift_xor_shift = static_cast<unsigned int>(((state_previous >> 18u) ^ state_previous) >> 27u);
            int rotation = state_previous >> 59u;
            return (state_shift_xor_shift >> rotation) | (state_shift_xor_shift << ((-rotation) & 31));
        }
    };

    random_pcg rng;

    for (unsigned int length_data = 1; length_data < 257; ++length_data) {
        for (unsigned int length_key = 1; length_key < 256; ++length_key) {
            unsigned char* plain = new unsigned char[length_data];
            unsigned char* key = new unsigned char[length_key];
            unsigned char* output = new unsigned char[length_data];
            unsigned char* input = new unsigned char[length_data];

            for (unsigned int i = 0; i < length_data; ++i) {
                plain[i] = rng.get_random_raw() & 0xFF;
            }
            for (unsigned int i = 0; i < length_key; ++i) {
                key[i] = rng.get_random_raw() & 0xFF;
            }

            gtl::rc4 rc4;
            rc4.encrypt(plain, length_data, key, length_key, output);
            rc4.decrypt(output, length_data, key, length_key, input);

            REQUIRE(testbench::is_memory_same(plain, input, length_data), "%s != %s\n", hex_2_string(plain, length_data).c_str(), hex_2_string(input, length_data).c_str());

            delete[] plain;
            delete[] key;
            delete[] output;
            delete[] input;
        }
    }
}
