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
#include <testbench/optimise.tests.hpp>
#include <testbench/comparison.tests.hpp>
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <hash/crc>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

using crc_types = testbench::value_collection<8, 16, 32, 64>;

TEST(crc, traits, standard) {
    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            REQUIRE((std::is_pod<gtl::crc<value>>::value == false));

            REQUIRE((std::is_trivial<gtl::crc<value>>::value == false));

            #if defined(_MSC_VER)
                #if 0
                // Not reliable across compiler / os.
                    REQUIRE((std::is_trivially_copyable<gtl::crc<value>>::value == false));
                #endif
            #else
                REQUIRE((std::is_trivially_copyable<gtl::crc<value>>::value == true));
            #endif

            REQUIRE((std::is_standard_layout<gtl::crc<value>>::value == true));
        }
    );
}

TEST(crc, function, reset) {
    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            gtl::crc<value> crc;
            crc.reset();
        }
    );
}

TEST(crc, function, consume) {
    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            gtl::crc<value> crc;
            crc.consume("", 0);
            crc.consume("123456781234567812345678123456781234567812345678123456781234567", 63);
            crc.consume("1234567812345678123456781234567812345678123456781234567812345678", 64);
            crc.consume("12345678123456781234567812345678123456781234567812345678123456781", 65);
        }
        );
}

TEST(crc, function, finalise) {
    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            gtl::crc<value> crc;
            crc.finalise();
        }
    );
}

TEST(crc, function, get_hash) {
    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            gtl::crc<value> crc;
            crc.finalise();
            crc.get_hash();
        }
    );
}

TEST(crc, function, hash_buffer) {
    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            gtl::crc<value>::hash_buffer("", 0);
            gtl::crc<value>::hash_buffer("123456781234567812345678123456781234567812345678123456781234567", 63);
            gtl::crc<value>::hash_buffer("1234567812345678123456781234567812345678123456781234567812345678", 64);
            gtl::crc<value>::hash_buffer("12345678123456781234567812345678123456781234567812345678123456781", 65);
        }
    );
}

TEST(crc, evaluate, hash_as_integer) {
    constexpr static const unsigned int data_count = 7;

    constexpr static const char* data[data_count] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
    };
    UNUSED(data);

    constexpr static const unsigned char result_crc8[data_count] = {
         0x00, 0x20, 0x5F, 0x07, 0xBD, 0x26, 0x74
    };
    UNUSED(result_crc8);

    constexpr static const unsigned short result_crc16[data_count] = {
         0x0000, 0x8145, 0xCADB, 0x5986, 0x5D25, 0xC53B, 0x6066
    };
    UNUSED(result_crc16);

    constexpr static const unsigned int result_crc32[data_count] = {
         0x00000000, 0xE8B7BE43, 0x352441C2, 0x20159D7F,
         0x4C2750BD, 0x1FC2E6D2, 0x7CA94A72
    };
    UNUSED(result_crc32);

    constexpr static const unsigned long long int result_crc64[data_count] = {
        0x0000000000000000, 0x548F120162451C62, 0x66501A349A0E0855,
        0xC04D61278997BA5E, 0x97A2566B552FCC4E, 0x5CA18585B92C58B9,
        0xBE4C46263774953C
    };
    UNUSED(result_crc64);

    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::crc<value> crc;
            for (unsigned int i = 0; i < data_count; ++i) {
                crc.reset();
                crc.consume(&data[i][0], testbench::string_length(&data[i][0]));
                crc.finalise();

                typename gtl::crc<value>::hash_type hash = crc.get_hash();

                if constexpr (value == 8) {
                    char hash_string[2 + 4 + 2 + 1] = {};
                    snprintf(
                        hash_string,
                        2 + 4 + 2 + 1,
                        "%02X == %02X",
                        hash.data[0],
                        result_crc8[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc8[i], gtl::crc<value>::hash_size) == true);
                }

                if constexpr (value == 16) {
                    char hash_string[4 + 4 + 4 + 1] = {};
                    snprintf(
                        hash_string,
                        4 + 4 + 4 + 1,
                        "%04X == %04X",
                        hash.data[0],
                        result_crc16[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc16[i], gtl::crc<value>::hash_size) == true);
                }

                if constexpr (value == 32) {
                    char hash_string[8 + 4 + 8 + 1] = {};
                    snprintf(
                        hash_string,
                        8 + 4 + 8 + 1,
                        "%08X == %08X",
                        hash.data[0],
                        result_crc32[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc32[i], gtl::crc<value>::hash_size) == true);
                }

                if constexpr (value == 64) {
                    char hash_string[16 + 4 + 16 + 1] = {};
                    snprintf(
                        hash_string,
                        16 + 4 + 16 + 1,
                        "%016llX == %016llX",
                        hash.data[0],
                        result_crc64[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc64[i], gtl::crc<value>::hash_size) == true);
                }
            }
        }
    );
}

TEST(crc, evaluate, hash_as_string) {

    constexpr static const unsigned int hash_count = 4;
    constexpr static const unsigned int data_count = 7;

    constexpr static const char* data[data_count] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
    };
    UNUSED(data);

    constexpr static const char* result[hash_count][data_count] = {
        { // 8
            "00",
            "20",
            "5F",
            "07",
            "BD",
            "26",
            "74"
        },
        { // 16
            "0000",
            "8145",
            "CADB",
            "5986",
            "5D25",
            "C53B",
            "6066"
        },
        { // 32
            "00000000",
            "E8B7BE43",
            "352441C2",
            "20159D7F",
            "4C2750BD",
            "1FC2E6D2",
            "7CA94A72"
        },
        { // 64
            "0000000000000000",
            "548F120162451C62",
            "66501A349A0E0855",
            "C04D61278997BA5E",
            "97A2566B552FCC4E",
            "5CA18585B92C58B9",
            "BE4C46263774953C"
        }
    };
    UNUSED(result);

    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::crc<value> crc;
            for (unsigned int i = 0; i < data_count; ++i) {
                crc.reset();
                crc.consume(&data[i][0], testbench::string_length(&data[i][0]));
                crc.finalise();

                typename gtl::crc<value>::hash_type hash = crc.get_hash();

                unsigned long long int result_index = value == 64 ? 3 : value == 32 ? 2 : value == 16 ? 1 : 0;

                PRINT("%s == %s\n", gtl::crc<value>::hash_to_string(hash).hash, result[result_index][i]);
                REQUIRE(testbench::is_string_same(gtl::crc<value>::hash_to_string(hash).hash, result[result_index][i]) == true);
            }
        }
    );
}


TEST(crc, evaluate, partial_insert) {

    constexpr static const unsigned int data_count = 7;

    constexpr static const char* data1[data_count] = {
        "",
        "a",
        "",
        "message",
        "abcdefghijklm",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "123456789012345678901234567890123456789012345678901234567890"
    };
    UNUSED(data1);

    constexpr static const char* data2[data_count] = {
        "",
        "",
        "abc",
        " digest",
        "nopqrstuvwxyz",
        "",
        "12345678901234567890"
    };
    UNUSED(data2);

    constexpr static const unsigned char result_crc8[data_count] = {
        0x00, 0x20, 0x5F, 0x07, 0xBD, 0x26, 0x74
    };
    UNUSED(result_crc8);

    constexpr static const unsigned short result_crc16[data_count] = {
        0x0000, 0x8145, 0xCADB, 0x5986, 0x5D25, 0xC53B, 0x6066
    };
    UNUSED(result_crc16);

    constexpr static const unsigned int result_crc32[data_count] = {
        0x00000000, 0xE8B7BE43, 0x352441C2, 0x20159D7F,
        0x4C2750BD, 0x1FC2E6D2, 0x7CA94A72
    };
    UNUSED(result_crc32);

    constexpr static const unsigned long long int result_crc64[data_count] = {
        0x0000000000000000, 0x548F120162451C62, 0x66501A349A0E0855,
        0xC04D61278997BA5E, 0x97A2566B552FCC4E, 0x5CA18585B92C58B9,
        0xBE4C46263774953C
    };
    UNUSED(result_crc64);

    testbench::test_template<crc_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::crc<value> crc;
            for (unsigned int i = 0; i < data_count; ++i) {
                crc.reset();
                crc.consume(&data1[i][0], testbench::string_length(&data1[i][0]));
                crc.consume(&data2[i][0], testbench::string_length(&data2[i][0]));
                crc.finalise();

                typename gtl::crc<value>::hash_type hash = crc.get_hash();

                if constexpr (value == 8) {
                    char hash_string[2 + 4 + 2 + 1] = {};
                    snprintf(
                        hash_string,
                        2 + 4 + 2 + 1,
                        "%02X == %02X",
                        hash.data[0],
                        result_crc8[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc8[i], gtl::crc<value>::hash_size) == true);
                }

                if constexpr (value == 16) {
                    char hash_string[4 + 4 + 4 + 1] = {};
                    snprintf(
                        hash_string,
                        4 + 4 + 4 + 1,
                        "%04X == %04X",
                        hash.data[0],
                        result_crc16[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc16[i], gtl::crc<value>::hash_size) == true);
                }

                if constexpr (value == 32) {
                    char hash_string[8 + 4 + 8 + 1] = {};
                    snprintf(
                        hash_string,
                        8 + 4 + 8 + 1,
                        "%08X == %08X",
                        hash.data[0],
                        result_crc32[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc32[i], gtl::crc<value>::hash_size) == true);
                }

                if constexpr (value == 64) {
                    char hash_string[16 + 4 + 16 + 1] = {};
                    snprintf(
                        hash_string,
                        16 + 4 + 16 + 1,
                        "%016llX == %016llX",
                        hash.data[0],
                        result_crc64[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc64[i], gtl::crc<value>::hash_size) == true);
                }
            }
        }
    );
}
