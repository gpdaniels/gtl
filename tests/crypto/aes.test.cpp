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

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

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

using aes_types = testbench::value_collection<128, 192, 256>;

TEST(aes, traits, standard) {
    testbench::test_template<aes_types>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            REQUIRE((std::is_pod<gtl::aes<value>>::value == true));

            REQUIRE((std::is_trivial<gtl::aes<value>>::value == true));

            REQUIRE((std::is_trivially_copyable<gtl::aes<value>>::value == true));

            REQUIRE((std::is_standard_layout<gtl::aes<value>>::value == true));
        }
    );
}

TEST(aes, constructor, empty) {
    testbench::test_template<aes_types>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::aes<value> aes;
            testbench::do_not_optimise_away(aes);
        }
    );
}

TEST(aes, evaluate, key_length_128) {
    gtl::aes<128> aes;
    constexpr static const unsigned int length = 16;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key_data[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const unsigned char encrypted[length] = { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A };

    typename gtl::aes<128>::key_type key = {};
    for (unsigned int i = 0; i < aes.key_size; ++i) {
        key.data[i] = key_data[i];
    }

    typename gtl::aes<128>::block_type input_block = {};
    for (unsigned int j = 0; j < aes.block_size; ++j) {
        for (unsigned int k = 0; k < aes.block_size; ++k) {
            input_block.data[k].data[j] = plain[j * aes.block_size + k];
        }
    }

    typename gtl::aes<128>::block_type output_block = aes.encrypt_block(key, input_block);

    unsigned char output[length];
    for (unsigned int j = 0; j < aes.block_size; ++j) {
        for (unsigned int k = 0; k < aes.block_size; ++k) {
            output[j * aes.block_size + k] = output_block.data[k].data[j];
        }
    }

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, evaluate, key_length_192) {
    gtl::aes<192> aes;
    constexpr static const unsigned int length = 16;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key_data[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
    const unsigned char encrypted[length] = { 0xDD, 0xA9, 0x7C, 0xA4, 0x86, 0x4C, 0xDF, 0xE0, 0x6E, 0xAF, 0x70, 0xA0, 0xEC, 0x0D, 0x71, 0x91 };

    typename gtl::aes<192>::key_type key = {};
    for (unsigned int i = 0; i < aes.key_size; ++i) {
        key.data[i] = key_data[i];
    }

    typename gtl::aes<192>::block_type input_block = {};
    for (unsigned int j = 0; j < aes.block_size; ++j) {
        for (unsigned int k = 0; k < aes.block_size; ++k) {
            input_block.data[k].data[j] = plain[j * aes.block_size + k];
        }
    }

    typename gtl::aes<192>::block_type output_block = aes.encrypt_block(key, input_block);

    unsigned char output[length];
    for (unsigned int j = 0; j < aes.block_size; ++j) {
        for (unsigned int k = 0; k < aes.block_size; ++k) {
            output[j * aes.block_size + k] = output_block.data[k].data[j];
        }
    }

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, evaluate, key_length_256) {
    gtl::aes<256> aes;
    constexpr static const unsigned int length = 16;
    const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    const unsigned char key_data[aes.key_size] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };
    const unsigned char encrypted[length] = { 0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45, 0xBF, 0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89 };

    typename gtl::aes<256>::key_type key = {};
    for (unsigned int i = 0; i < aes.key_size; ++i) {
        key.data[i] = key_data[i];
    }

    typename gtl::aes<256>::block_type input_block = {};
    for (unsigned int j = 0; j < aes.block_size; ++j) {
        for (unsigned int k = 0; k < aes.block_size; ++k) {
            input_block.data[k].data[j] = plain[j * aes.block_size + k];
        }
    }

    typename gtl::aes<256>::block_type output_block = aes.encrypt_block(key, input_block);

    unsigned char output[length];
    for (unsigned int j = 0; j < aes.block_size; ++j) {
        for (unsigned int k = 0; k < aes.block_size; ++k) {
            output[j * aes.block_size + k] = output_block.data[k].data[j];
        }
    }

    REQUIRE(testbench::is_memory_same(encrypted, output, length), "%s != %s\n", hex_2_string(encrypted, length).c_str(), hex_2_string(output, length).c_str());
}

TEST(aes, evaluate, encrypt_decrypt) {
    testbench::test_template<aes_types>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::aes<value> aes;
            typename gtl::aes<value>::key_type key = {};
            for (unsigned int i = 0; i < aes.key_size; ++i) {
                key.data[i] = static_cast<unsigned char>(i);
            }

            typename gtl::aes<value>::block_type plain = {};
            for (unsigned int i = 0; i < aes.block_size * aes.block_size; ++i) {
                plain.data[i / aes.block_size].data[i % aes.block_size] = static_cast<unsigned char>(i);
            }

            typename gtl::aes<value>::block_type output = aes.encrypt_block(key, plain);
            typename gtl::aes<value>::block_type result = aes.decrypt_block(key, output);

            REQUIRE(testbench::is_memory_same(&plain, &result, sizeof(plain)));
        }
    );
}
