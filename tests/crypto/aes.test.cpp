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

template <unsigned long long int size>
static typename gtl::aes<size>::block_type bytes_2_block(const unsigned char* bytes) {
    typename gtl::aes<size>::block_type block = {};
    for (unsigned int j = 0; j < gtl::aes<size>::block_size; ++j) {
        for (unsigned int k = 0; k < gtl::aes<size>::block_size; ++k) {
            block.data[k].data[j] = bytes[j * gtl::aes<size>::block_size + k];
        }
    }
    return block;
}

template <unsigned long long int size>
static void block_2_bytes(const typename gtl::aes<size>::block_type& block, unsigned char* bytes) {
    for (unsigned int j = 0; j < gtl::aes<size>::block_size; ++j) {
        for (unsigned int k = 0; k < gtl::aes<size>::block_size; ++k) {
            bytes[j * gtl::aes<size>::block_size + k] = block.data[k].data[j];
        }
    }
}

TEST(aes, evaluate, decrypt_known) {
    // The example vectors of FIPS 197 appendix C, decrypted.
    testbench::test_template<aes_types>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const unsigned int length = 16;
            const unsigned char plain[length] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
            const unsigned char encrypted[3][length] = {
                { 0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30, 0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A },
                { 0xDD, 0xA9, 0x7C, 0xA4, 0x86, 0x4C, 0xDF, 0xE0, 0x6E, 0xAF, 0x70, 0xA0, 0xEC, 0x0D, 0x71, 0x91 },
                { 0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45, 0xBF, 0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89 }
            };
            const unsigned int index = (value == 128) ? 0 : ((value == 192) ? 1 : 2);

            typename gtl::aes<value>::key_type key = {};
            for (unsigned int i = 0; i < gtl::aes<value>::key_size; ++i) {
                key.data[i] = static_cast<unsigned char>(i);
            }

            unsigned char output[length] = {};
            block_2_bytes<value>(gtl::aes<value>::decrypt_block(key, bytes_2_block<value>(encrypted[index])), output);
            REQUIRE(testbench::is_memory_same(plain, output, length), "%s != %s\n", hex_2_string(plain, length).c_str(), hex_2_string(output, length).c_str());

            block_2_bytes<value>(gtl::aes<value>::encrypt_block(key, bytes_2_block<value>(plain)), output);
            REQUIRE(testbench::is_memory_same(encrypted[index], output, length), "%s != %s\n", hex_2_string(encrypted[index], length).c_str(), hex_2_string(output, length).c_str());
        }
    );
}

TEST(aes, evaluate, expanded_key) {
    // A key that is expanded once must give the same results as the key itself.
    testbench::test_template<aes_types>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const unsigned int length = 16;

            typename gtl::aes<value>::key_type key = {};
            for (unsigned int i = 0; i < gtl::aes<value>::key_size; ++i) {
                key.data[i] = static_cast<unsigned char>(i * 11 + 5);
            }
            const typename gtl::aes<value>::round_keys_type round_keys = gtl::aes<value>::expand_key(key);

            for (unsigned int block_index = 0; block_index < 32; ++block_index) {
                unsigned char plain[length];
                for (unsigned int i = 0; i < length; ++i) {
                    plain[i] = static_cast<unsigned char>(block_index * 17 + i * 3);
                }
                const typename gtl::aes<value>::block_type block = bytes_2_block<value>(plain);

                const typename gtl::aes<value>::block_type encrypted_key = gtl::aes<value>::encrypt_block(key, block);
                const typename gtl::aes<value>::block_type encrypted_round_keys = gtl::aes<value>::encrypt_block(round_keys, block);
                REQUIRE(testbench::is_memory_same(&encrypted_key, &encrypted_round_keys, sizeof(encrypted_key)));

                const typename gtl::aes<value>::block_type decrypted_key = gtl::aes<value>::decrypt_block(key, encrypted_key);
                const typename gtl::aes<value>::block_type decrypted_round_keys = gtl::aes<value>::decrypt_block(round_keys, encrypted_key);
                REQUIRE(testbench::is_memory_same(&decrypted_key, &decrypted_round_keys, sizeof(decrypted_key)));
                REQUIRE(testbench::is_memory_same(&block, &decrypted_round_keys, sizeof(block)));
            }
        }
    );
}

TEST(aes, evaluate, every_byte_value) {
    // Transform 256 blocks, each filled with one byte value, so that every input of the substitution boxes is used.
    // The expected values are the xor of the 256 outputs, computed independently of this library.
    testbench::test_template<aes_types>(
        [](auto test_value) -> void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            constexpr static const unsigned int length = 16;
            const unsigned char expected_encrypted[3][length] = {
                { 0x01, 0xED, 0x1B, 0x08, 0x1B, 0xD5, 0xD6, 0x1A, 0x56, 0x3B, 0xBA, 0x08, 0x07, 0xBB, 0xF3, 0xB8 },
                { 0x89, 0xC2, 0x6D, 0x1B, 0xDC, 0x28, 0x63, 0xF3, 0x72, 0xFE, 0xDA, 0x4F, 0x63, 0xF4, 0x68, 0x4C },
                { 0xFB, 0x6D, 0xE0, 0xA8, 0x7F, 0x3B, 0x7C, 0x3E, 0x87, 0x57, 0xBB, 0x44, 0xDB, 0x63, 0x4D, 0xEB }
            };
            const unsigned char expected_decrypted[3][length] = {
                { 0xD7, 0xFF, 0xA5, 0xDC, 0xE7, 0xCB, 0x72, 0x46, 0xD3, 0xD2, 0xDC, 0x1F, 0xC9, 0x29, 0x9B, 0x0D },
                { 0xBB, 0x6C, 0x61, 0x75, 0x38, 0x10, 0xBD, 0x8D, 0x4B, 0x72, 0x46, 0x26, 0x8B, 0xE0, 0xF1, 0x63 },
                { 0x63, 0xBE, 0x0D, 0xAC, 0x40, 0x00, 0x0A, 0x3B, 0xE2, 0xDA, 0xF7, 0x58, 0xC7, 0x3F, 0x13, 0x7E }
            };
            const unsigned int index = (value == 128) ? 0 : ((value == 192) ? 1 : 2);

            typename gtl::aes<value>::key_type key = {};
            for (unsigned int i = 0; i < gtl::aes<value>::key_size; ++i) {
                key.data[i] = static_cast<unsigned char>(i);
            }
            const typename gtl::aes<value>::round_keys_type round_keys = gtl::aes<value>::expand_key(key);

            unsigned char sum_encrypted[length] = {};
            unsigned char sum_decrypted[length] = {};
            for (unsigned int byte_value = 0; byte_value < 256; ++byte_value) {
                unsigned char input[length];
                for (unsigned int i = 0; i < length; ++i) {
                    input[i] = static_cast<unsigned char>(byte_value);
                }
                const typename gtl::aes<value>::block_type block = bytes_2_block<value>(input);

                unsigned char output[length] = {};
                block_2_bytes<value>(gtl::aes<value>::encrypt_block(round_keys, block), output);
                for (unsigned int i = 0; i < length; ++i) {
                    sum_encrypted[i] ^= output[i];
                }

                block_2_bytes<value>(gtl::aes<value>::decrypt_block(round_keys, block), output);
                for (unsigned int i = 0; i < length; ++i) {
                    sum_decrypted[i] ^= output[i];
                }

                // Each transformation must also be undone by the other.
                const typename gtl::aes<value>::block_type round_trip = gtl::aes<value>::decrypt_block(round_keys, gtl::aes<value>::encrypt_block(round_keys, block));
                REQUIRE(testbench::is_memory_same(&block, &round_trip, sizeof(block)), "Round trip failed for byte value %u.", byte_value);
            }

            REQUIRE(testbench::is_memory_same(expected_encrypted[index], sum_encrypted, length), "%s != %s\n", hex_2_string(expected_encrypted[index], length).c_str(), hex_2_string(sum_encrypted, length).c_str());
            REQUIRE(testbench::is_memory_same(expected_decrypted[index], sum_decrypted, length), "%s != %s\n", hex_2_string(expected_decrypted[index], length).c_str(), hex_2_string(sum_decrypted, length).c_str());
        }
    );
}

TEST(aes, evaluate, constant_expression) {
    // The cipher must be usable at compile time, zeros encrypted with a zero key is a well known value.
    constexpr const gtl::aes<128>::block_type encrypted = gtl::aes<128>::encrypt_block(gtl::aes<128>::key_type{}, gtl::aes<128>::block_type{});
    constexpr const gtl::aes<128>::block_type decrypted = gtl::aes<128>::decrypt_block(gtl::aes<128>::key_type{}, encrypted);
    const unsigned char expected[16] = { 0x66, 0xE9, 0x4B, 0xD4, 0xEF, 0x8A, 0x2C, 0x3B, 0x88, 0x4C, 0xFA, 0x59, 0xCA, 0x34, 0x2B, 0x2E };
    const unsigned char zeros[16] = {};

    unsigned char output[16] = {};
    block_2_bytes<128>(encrypted, output);
    REQUIRE(testbench::is_memory_same(expected, output, 16), "%s != %s\n", hex_2_string(expected, 16).c_str(), hex_2_string(output, 16).c_str());

    block_2_bytes<128>(decrypted, output);
    REQUIRE(testbench::is_memory_same(zeros, output, 16), "%s != %s\n", hex_2_string(zeros, 16).c_str(), hex_2_string(output, 16).c_str());
}
