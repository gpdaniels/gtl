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

#include <hash/crc>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

using crc_types = testbench::enum_collection<gtl::crc_size, gtl::crc_size::bits_8, gtl::crc_size::bits_16, gtl::crc_size::bits_32, gtl::crc_size::bits_64>;

TEST(crc, traits, standard) {
    testbench::test_template<crc_types>(
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            REQUIRE(sizeof(gtl::crc<value_enum>) >= 0);

            REQUIRE((std::is_pod<gtl::crc<value_enum>>::value == false));

            REQUIRE((std::is_trivial<gtl::crc<value_enum>>::value == false));

            #if defined(__clang__)
                // This check failes on gcc7 for some reason.
                REQUIRE((std::is_trivially_copyable<gtl::crc<value_enum>>::value == false));
            #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER))
                REQUIRE((std::is_trivially_copyable<gtl::crc<value_enum>>::value == true));
            #else
                // This check failes on gcc7 for some reason.
                REQUIRE((std::is_trivially_copyable<gtl::crc<value_enum>>::value == false));
            #endif

            REQUIRE((std::is_standard_layout<gtl::crc<value_enum>>::value == true));
        }
    );
}

TEST(crc, constructor, empty) {
    testbench::test_template<crc_types>(
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc = gtl::crc<value_enum>(typename gtl::crc<value_enum>::settings_type());
            testbench::do_not_optimise_away(crc);
        }
    );
}

TEST(crc, function, clear) {
    testbench::test_template<crc_types>(
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc;
            crc.clear();
        }
    );
}

TEST(crc, function, insert) {
    testbench::test_template<crc_types>(
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc;
            crc.insert("", 0);
            crc.insert("123456781234567812345678123456781234567812345678123456781234567", 63);
            crc.insert("1234567812345678123456781234567812345678123456781234567812345678", 64);
            crc.insert("12345678123456781234567812345678123456781234567812345678123456781", 65);
        }
    );
}

TEST(crc, function, finalise) {
    testbench::test_template<crc_types>(
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc;
            crc.finalise();
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
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc;
            for (unsigned int i = 0; i < data_count; ++i) {
                crc.clear();
                crc.insert(data[i], strlen(data[i]));
                typename gtl::crc<value_enum>::hash_type hash = crc.finalise();

                if constexpr (value_enum == gtl::crc_size::bits_8) {
                    char hash_string[2 + 4 + 2 + 1] = {};
                    snprintf(
                        hash_string,
                        2 + 4 + 2 + 1,
                        "%02X == %02X",
                        hash.data[0],
                        result_crc8[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc8[i], gtl::crc<value_enum>::hash_size) == true);
                }

                if constexpr (value_enum == gtl::crc_size::bits_16) {
                    char hash_string[4 + 4 + 4 + 1] = {};
                    snprintf(
                        hash_string,
                        4 + 4 + 4 + 1,
                        "%04X == %04X",
                        hash.data[0],
                        result_crc16[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc16[i], gtl::crc<value_enum>::hash_size) == true);
                }

                if constexpr (value_enum == gtl::crc_size::bits_32) {
                    char hash_string[8 + 4 + 8 + 1] = {};
                    snprintf(
                        hash_string,
                        8 + 4 + 8 + 1,
                        "%08X == %08X",
                        hash.data[0],
                        result_crc32[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc32[i], gtl::crc<value_enum>::hash_size) == true);
                }

                if constexpr (value_enum == gtl::crc_size::bits_64) {
                    char hash_string[16 + 4 + 16 + 1] = {};
                    snprintf(
                        hash_string,
                        16 + 4 + 16 + 1,
                        "%016llX == %016llX",
                        hash.data[0],
                        result_crc64[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc64[i], gtl::crc<value_enum>::hash_size) == true);
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
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc;
            for (unsigned int i = 0; i < data_count; ++i) {
                crc.clear();
                crc.insert(data[i], strlen(data[i]));
                typename gtl::crc<value_enum>::hash_type hash = crc.finalise();
                PRINT("%s == %s\n", gtl::crc<value_enum>::hash_to_string(hash).hash, result[static_cast<unsigned int>(value_enum)][i]);
                REQUIRE(testbench::is_string_same(gtl::crc<value_enum>::hash_to_string(hash).hash, result[static_cast<unsigned int>(value_enum)][i]) == true);
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
        [](auto test_enum)->void {
            using type_enum = typename decltype(test_enum)::type;
            constexpr static const type_enum value_enum = decltype(test_enum)::value;

            gtl::crc<value_enum> crc;
            for (unsigned int i = 0; i < data_count; ++i) {
                crc.clear();
                crc.insert(data1[i], strlen(data1[i]));
                crc.insert(data2[i], strlen(data2[i]));
                typename gtl::crc<value_enum>::hash_type hash = crc.finalise();

                if constexpr (value_enum == gtl::crc_size::bits_8) {
                    char hash_string[2 + 4 + 2 + 1] = {};
                    snprintf(
                        hash_string,
                        2 + 4 + 2 + 1,
                        "%02X == %02X",
                        hash.data[0],
                        result_crc8[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc8[i], gtl::crc<value_enum>::hash_size) == true);
                }

                if constexpr (value_enum == gtl::crc_size::bits_16) {
                    char hash_string[4 + 4 + 4 + 1] = {};
                    snprintf(
                        hash_string,
                        4 + 4 + 4 + 1,
                        "%04X == %04X",
                        hash.data[0],
                        result_crc16[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc16[i], gtl::crc<value_enum>::hash_size) == true);
                }

                if constexpr (value_enum == gtl::crc_size::bits_32) {
                    char hash_string[8 + 4 + 8 + 1] = {};
                    snprintf(
                        hash_string,
                        8 + 4 + 8 + 1,
                        "%08X == %08X",
                        hash.data[0],
                        result_crc32[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc32[i], gtl::crc<value_enum>::hash_size) == true);
                }

                if constexpr (value_enum == gtl::crc_size::bits_64) {
                    char hash_string[16 + 4 + 16 + 1] = {};
                    snprintf(
                        hash_string,
                        16 + 4 + 16 + 1,
                        "%016llX == %016llX",
                        hash.data[0],
                        result_crc64[i]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, &result_crc64[i], gtl::crc<value_enum>::hash_size) == true);
                }
            }
        }
    );
}
