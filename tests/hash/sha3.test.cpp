/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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
#include <optimise.tests.hpp>
#include <comparison.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <hash/sha3>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cstdio>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

using sha3_types = testbench::value_collection<224, 256, 384, 512>;

TEST(sha3, traits, standard) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            REQUIRE((std::is_pod<gtl::sha3<value>>::value == false));

            REQUIRE((std::is_trivial<gtl::sha3<value>>::value == false));

            REQUIRE((std::is_trivially_copyable<gtl::sha3<value>>::value == true));

            REQUIRE((std::is_standard_layout<gtl::sha3<value>>::value == true));
        }
    );
}

TEST(sha3, constructor, empty) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;
            testbench::do_not_optimise_away(sha3);
        }
    );
}

TEST(sha3, function, reset) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;
            sha3.reset();
        }
    );
}

TEST(sha3, function, consume) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;
            sha3.consume("", 0);
            sha3.consume("123456781234567812345678123456781234567812345678123456781234567", 63);
            sha3.consume("1234567812345678123456781234567812345678123456781234567812345678", 64);
            sha3.consume("12345678123456781234567812345678123456781234567812345678123456781", 65);
        }
    );
}

TEST(sha3, function, finalise) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;
            sha3.finalise();
        }
    );
}


TEST(sha3, function, get_hash) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;
            sha3.finalise();
            sha3.get_hash();
        }
    );
}

TEST(sha3, function, hash_buffer) {
    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;
            gtl::sha3<value>::hash_buffer("", 0);
            gtl::sha3<value>::hash_buffer("123456781234567812345678123456781234567812345678123456781234567", 63);
            gtl::sha3<value>::hash_buffer("1234567812345678123456781234567812345678123456781234567812345678", 64);
            gtl::sha3<value>::hash_buffer("12345678123456781234567812345678123456781234567812345678123456781", 65);
        }
    );
}

TEST(sha3, evaluate, hash_as_integer) {

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

    constexpr static const unsigned int result_sha3_224[data_count][gtl::sha3<224>::hash_size / sizeof(unsigned int)] = {
        { 0x6B4E0342, 0x3667DBB7, 0x3B6E1545, 0x4F0EB1AB, 0xD4597F9A, 0x1B078E3F, 0x5B5A6BC7 },
        { 0x9E86FF69, 0x557CA95F, 0x405F0812, 0x69685B38, 0xE3A819B3, 0x09EE942F, 0x482B6A8B },
        { 0xE642824C, 0x3F8CF24A, 0xD09234EE, 0x7D3C766F, 0xC9A3A516, 0x8D0C94AD, 0x73B46FDF },
        { 0x18768BB4, 0xC48EB7FC, 0x88E5DDB1, 0x7EFCF296, 0x4ABD7798, 0xA39D86A4, 0xB4A1E4C8 },
        { 0x5CDECA81, 0xE123F87C, 0xAD96B9CB, 0xA999F16F, 0x6D415496, 0x08D4E0F4, 0x681B8239 },
        { 0xA67C289B, 0x8250A6F4, 0x37A20137, 0x985D6055, 0x89A8C163, 0xD45261B1, 0x5419556E },
        { 0x0526898E, 0x185869F9, 0x1B3E2A76, 0xDD72A15D, 0xC6940A67, 0xC8164A04, 0x4CD25CC8 }
    };
    UNUSED(result_sha3_224);

    constexpr static const unsigned int result_sha3_256[data_count][gtl::sha3<256>::hash_size / sizeof(unsigned int)] = {
        { 0xA7FFC6F8, 0xBF1ED766, 0x51C14756, 0xA061D662, 0xF580FF4D, 0xE43B49FA, 0x82D80A4B, 0x80F8434A },
        { 0x80084BF2, 0xFBA02475, 0x726FEB2C, 0xAB2D8215, 0xEAB14BC6, 0xBDD8BFB2, 0xC8151257, 0x032ECD8B },
        { 0x3A985DA7, 0x4FE225B2, 0x045C172D, 0x6BD390BD, 0x855F086E, 0x3E9D525B, 0x46BFE245, 0x11431532 },
        { 0xEDCDB206, 0x9366E752, 0x43860C18, 0xC3A11465, 0xECA34BCE, 0x6143D30C, 0x8665CEFC, 0xFD32BFFD },
        { 0x7CAB2DC7, 0x65E21B24, 0x1DBC1C25, 0x5CE620B2, 0x9F527C6D, 0x5E7F5F84, 0x3E56288F, 0x0D707521 },
        { 0xA79D6A9D, 0xA47F04A3, 0xB9A9323E, 0xC9991F21, 0x05D4C78A, 0x7BC7BEEB, 0x103855A7, 0xA11DFB9F },
        { 0x293E5CE4, 0xCE54EE71, 0x990AB06E, 0x511B7CCD, 0x62722B1B, 0xEB414F5F, 0xF65C8274, 0xE0F5BE1D }
    };
    UNUSED(result_sha3_256);

    constexpr static const unsigned long long int result_sha3_384[data_count][gtl::sha3<384>::hash_size / sizeof(unsigned long long int)] = {
        { 0x0C63A75B845E4F7D, 0x01107D852E4C2485, 0xC51A50AAAA94FC61, 0x995E71BBEE983A2A, 0xC3713831264ADB47, 0xFB6BD1E058D5F004 },
        { 0x1815F774F320491B, 0x48569EFEC794D249, 0xEEB59AAE46D22BF7, 0x7DAFE25C5EDC28D7, 0xEA44F93EE1234AA8, 0x8F61C91912A4CCD9 },
        { 0xEC01498288516FC9, 0x26459F58E2C6AD8D, 0xF9B473CB0FC08C25, 0x96DA7CF0E49BE4B2, 0x98D88CEA927AC7F5, 0x39F1EDF228376D25 },
        { 0xD9519709F44AF73E, 0x2C8E291109A979DE, 0x3D61DC02BF69DEF7, 0xFBFFDFFFE6627515, 0x13F19AD57E17D4B9, 0x3BA1E484FC1980D5 },
        { 0xFED399D2217AAF4C, 0x717AD0C5102C1558, 0x9E1C990CC2B9A502, 0x9056A7F7485888D6, 0xAB65DB2370077A5C, 0xADB53FC9280D278F },
        { 0xD5B972302F5080D0, 0x830E0DE7B6B2CF38, 0x3665A008F4C4F386, 0xA61112652C742D20, 0xCB45AA51BD4F542F, 0xC733E2719E999291 },
        { 0x3C213A17F514638A, 0xCB3BF17F109F3E24, 0xC16F9F14F085B52A, 0x2F2B81ADC0DB83DF, 0x1A58DB2CE013191B, 0x8BA72D8FAE7E2A5E }
    };
    UNUSED(result_sha3_384);

    constexpr static const unsigned long long int result_sha3_512[data_count][gtl::sha3<512>::hash_size / sizeof(unsigned long long int)] = {
        { 0xA69F73CCA23A9AC5, 0xC8B567DC185A756E, 0x97C982164FE25859, 0xE0D1DCC1475C80A6, 0x15B2123AF1F5F94C, 0x11E3E9402C3AC558, 0xF500199D95B6D3E3, 0x01758586281DCD26 },
        { 0x697F2D856172CB83, 0x09D6B8B97DAC4DE3, 0x44B549D4DEE61EDF, 0xB4962D8698B7FA80, 0x3F4F93FF24393586, 0xE28B5B957AC3D1D3, 0x69420CE53332712F, 0x997BD336D09AB02A },
        { 0xB751850B1A57168A, 0x5693CD924B6B096E, 0x08F621827444F70D, 0x884F5D0240D2712E, 0x10E116E9192AF3C9, 0x1A7EC57647E39340, 0x57340B4CF408D5A5, 0x6592F8274EEC53F0 },
        { 0x3444E155881FA155, 0x11F57726C7D7CFE8, 0x0302A7433067B29D, 0x59A71415CA9DD141, 0xAC892D310BC4D781, 0x28C98FDA839D18D7, 0xF0556F2FE7ACB3C0, 0xCDA4BFF3A25F5F59 },
        { 0xAF328D17FA28753A, 0x3C9F5CB72E376B90, 0x440B96F0289E5703, 0xB729324A975AB384, 0xEDA565FC92AADED1, 0x43669900D7618616, 0x87ACDC0A5FFA358B, 0xD0571AAAD80ACA68 },
        { 0xD1DB17B4745B255E, 0x5EB159F66593CC9C, 0x143850979FC7A395, 0x1796ABA80165AAB5, 0x36B46174CE19E3F7, 0x07F0E5C6487F5F03, 0x084BC0EC9461691E, 0xF20113E42AD28163 },
        { 0x9524B9A5536B9106, 0x9526B4F6196B7E94, 0x75B4DA69E01F0C85, 0x5797F224CD7335DD, 0xB286FD99B9B32FFE, 0x33B59AD424CC1744, 0xF6EB59137F5FB860, 0x1932E8A8AF0AE930 }
    };
    UNUSED(result_sha3_512);

    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;

            for (unsigned int i = 0; i < data_count; ++i) {
                sha3.reset();
                sha3.consume(data[i], testbench::string_length(data[i]));
                sha3.finalise();
                typename gtl::sha3<value>::hash_type hash = sha3.get_hash();

                if constexpr (value == 224) {
                    char hash_string[56 + 4 + 56 + 1] = {};
                    snprintf(
                        hash_string,
                        56 + 4 + 56 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6],
                        result_sha3_224[i][0], result_sha3_224[i][1], result_sha3_224[i][2], result_sha3_224[i][3],
                        result_sha3_224[i][4], result_sha3_224[i][5], result_sha3_224[i][6]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_224[i], gtl::sha3<value>::hash_size) == true);
                }
                if constexpr (value == 256) {
                    char hash_string[64 + 4 + 64 + 1] = {};
                    snprintf(
                        hash_string,
                        64 + 4 + 64 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha3_256[i][0], result_sha3_256[i][1], result_sha3_256[i][2], result_sha3_256[i][3],
                        result_sha3_256[i][4], result_sha3_256[i][5], result_sha3_256[i][6], result_sha3_256[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_256[i], gtl::sha3<value>::hash_size) == true);
                }
                if constexpr (value == 384) {
                    char hash_string[96 + 4 + 96 + 1] = {};
                    snprintf(
                        hash_string,
                        96 + 4 + 96 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5],
                        result_sha3_384[i][0], result_sha3_384[i][1], result_sha3_384[i][2], result_sha3_384[i][3],
                        result_sha3_384[i][4], result_sha3_384[i][5]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_384[i], gtl::sha3<value>::hash_size) == true);
                }
                if constexpr (value == 512) {
                    char hash_string[128 + 4 + 128 + 1] = {};
                    snprintf(
                        hash_string,
                        128 + 4 + 128 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha3_512[i][0], result_sha3_512[i][1], result_sha3_512[i][2], result_sha3_512[i][3],
                        result_sha3_512[i][4], result_sha3_512[i][5], result_sha3_512[i][6], result_sha3_512[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_512[i], gtl::sha3<value>::hash_size) == true);
                }
            }
        }
    );
}

TEST(sha3, evaluate, hash_as_string) {

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
        { // 224
         "6B4E03423667DBB73B6E15454F0EB1ABD4597F9A1B078E3F5B5A6BC7",
         "9E86FF69557CA95F405F081269685B38E3A819B309EE942F482B6A8B",
         "E642824C3F8CF24AD09234EE7D3C766FC9A3A5168D0C94AD73B46FDF",
         "18768BB4C48EB7FC88E5DDB17EFCF2964ABD7798A39D86A4B4A1E4C8",
         "5CDECA81E123F87CAD96B9CBA999F16F6D41549608D4E0F4681B8239",
         "A67C289B8250A6F437A20137985D605589A8C163D45261B15419556E",
         "0526898E185869F91B3E2A76DD72A15DC6940A67C8164A044CD25CC8"
        },
        { // 256
         "A7FFC6F8BF1ED76651C14756A061D662F580FF4DE43B49FA82D80A4B80F8434A",
         "80084BF2FBA02475726FEB2CAB2D8215EAB14BC6BDD8BFB2C8151257032ECD8B",
         "3A985DA74FE225B2045C172D6BD390BD855F086E3E9D525B46BFE24511431532",
         "EDCDB2069366E75243860C18C3A11465ECA34BCE6143D30C8665CEFCFD32BFFD",
         "7CAB2DC765E21B241DBC1C255CE620B29F527C6D5E7F5F843E56288F0D707521",
         "A79D6A9DA47F04A3B9A9323EC9991F2105D4C78A7BC7BEEB103855A7A11DFB9F",
         "293E5CE4CE54EE71990AB06E511B7CCD62722B1BEB414F5FF65C8274E0F5BE1D"
        },
        { // 382
         "0C63A75B845E4F7D01107D852E4C2485C51A50AAAA94FC61995E71BBEE983A2AC3713831264ADB47FB6BD1E058D5F004",
         "1815F774F320491B48569EFEC794D249EEB59AAE46D22BF77DAFE25C5EDC28D7EA44F93EE1234AA88F61C91912A4CCD9",
         "EC01498288516FC926459F58E2C6AD8DF9B473CB0FC08C2596DA7CF0E49BE4B298D88CEA927AC7F539F1EDF228376D25",
         "D9519709F44AF73E2C8E291109A979DE3D61DC02BF69DEF7FBFFDFFFE662751513F19AD57E17D4B93BA1E484FC1980D5",
         "FED399D2217AAF4C717AD0C5102C15589E1C990CC2B9A5029056A7F7485888D6AB65DB2370077A5CADB53FC9280D278F",
         "D5B972302F5080D0830E0DE7B6B2CF383665A008F4C4F386A61112652C742D20CB45AA51BD4F542FC733E2719E999291",
         "3C213A17F514638ACB3BF17F109F3E24C16F9F14F085B52A2F2B81ADC0DB83DF1A58DB2CE013191B8BA72D8FAE7E2A5E"
        },
        { // 512
         "A69F73CCA23A9AC5C8B567DC185A756E97C982164FE25859E0D1DCC1475C80A615B2123AF1F5F94C11E3E9402C3AC558F500199D95B6D3E301758586281DCD26",
         "697F2D856172CB8309D6B8B97DAC4DE344B549D4DEE61EDFB4962D8698B7FA803F4F93FF24393586E28B5B957AC3D1D369420CE53332712F997BD336D09AB02A",
         "B751850B1A57168A5693CD924B6B096E08F621827444F70D884F5D0240D2712E10E116E9192AF3C91A7EC57647E3934057340B4CF408D5A56592F8274EEC53F0",
         "3444E155881FA15511F57726C7D7CFE80302A7433067B29D59A71415CA9DD141AC892D310BC4D78128C98FDA839D18D7F0556F2FE7ACB3C0CDA4BFF3A25F5F59",
         "AF328D17FA28753A3C9F5CB72E376B90440B96F0289E5703B729324A975AB384EDA565FC92AADED143669900D761861687ACDC0A5FFA358BD0571AAAD80ACA68",
         "D1DB17B4745B255E5EB159F66593CC9C143850979FC7A3951796ABA80165AAB536B46174CE19E3F707F0E5C6487F5F03084BC0EC9461691EF20113E42AD28163",
         "9524B9A5536B91069526B4F6196B7E9475B4DA69E01F0C855797F224CD7335DDB286FD99B9B32FFE33B59AD424CC1744F6EB59137F5FB8601932E8A8AF0AE930"
        }
    };
    UNUSED(result);

    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;
            for (unsigned int i = 0; i < data_count; ++i) {
                sha3.reset();
                sha3.consume(data[i], testbench::string_length(data[i]));
                sha3.finalise();
                typename gtl::sha3<value>::hash_type hash = sha3.get_hash();

                unsigned long long int result_index = value == 512 ? 3 : value == 384 ? 2 : value == 256 ? 1 : 0;

                PRINT("%s == %s\n", gtl::sha3<value>::hash_to_string(hash).hash, result[result_index][i]);
                REQUIRE(testbench::is_string_same(gtl::sha3<value>::hash_to_string(hash).hash, result[result_index][i]) == true);
            }
        }
    );
}


TEST(sha3, evaluate, partial_insert) {

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

    constexpr static const unsigned int result_sha3_224[data_count][gtl::sha3<224>::hash_size / sizeof(unsigned int)] = {
        { 0x6B4E0342, 0x3667DBB7, 0x3B6E1545, 0x4F0EB1AB, 0xD4597F9A, 0x1B078E3F, 0x5B5A6BC7 },
        { 0x9E86FF69, 0x557CA95F, 0x405F0812, 0x69685B38, 0xE3A819B3, 0x09EE942F, 0x482B6A8B },
        { 0xE642824C, 0x3F8CF24A, 0xD09234EE, 0x7D3C766F, 0xC9A3A516, 0x8D0C94AD, 0x73B46FDF },
        { 0x18768BB4, 0xC48EB7FC, 0x88E5DDB1, 0x7EFCF296, 0x4ABD7798, 0xA39D86A4, 0xB4A1E4C8 },
        { 0x5CDECA81, 0xE123F87C, 0xAD96B9CB, 0xA999F16F, 0x6D415496, 0x08D4E0F4, 0x681B8239 },
        { 0xA67C289B, 0x8250A6F4, 0x37A20137, 0x985D6055, 0x89A8C163, 0xD45261B1, 0x5419556E },
        { 0x0526898E, 0x185869F9, 0x1B3E2A76, 0xDD72A15D, 0xC6940A67, 0xC8164A04, 0x4CD25CC8 }
    };
    UNUSED(result_sha3_224);

    constexpr static const unsigned int result_sha3_256[data_count][gtl::sha3<256>::hash_size / sizeof(unsigned int)] = {
        { 0xA7FFC6F8, 0xBF1ED766, 0x51C14756, 0xA061D662, 0xF580FF4D, 0xE43B49FA, 0x82D80A4B, 0x80F8434A },
        { 0x80084BF2, 0xFBA02475, 0x726FEB2C, 0xAB2D8215, 0xEAB14BC6, 0xBDD8BFB2, 0xC8151257, 0x032ECD8B },
        { 0x3A985DA7, 0x4FE225B2, 0x045C172D, 0x6BD390BD, 0x855F086E, 0x3E9D525B, 0x46BFE245, 0x11431532 },
        { 0xEDCDB206, 0x9366E752, 0x43860C18, 0xC3A11465, 0xECA34BCE, 0x6143D30C, 0x8665CEFC, 0xFD32BFFD },
        { 0x7CAB2DC7, 0x65E21B24, 0x1DBC1C25, 0x5CE620B2, 0x9F527C6D, 0x5E7F5F84, 0x3E56288F, 0x0D707521 },
        { 0xA79D6A9D, 0xA47F04A3, 0xB9A9323E, 0xC9991F21, 0x05D4C78A, 0x7BC7BEEB, 0x103855A7, 0xA11DFB9F },
        { 0x293E5CE4, 0xCE54EE71, 0x990AB06E, 0x511B7CCD, 0x62722B1B, 0xEB414F5F, 0xF65C8274, 0xE0F5BE1D }
    };
    UNUSED(result_sha3_256);

    constexpr static const unsigned long long int result_sha3_384[data_count][gtl::sha3<384>::hash_size / sizeof(unsigned long long int)] = {
        { 0x0C63A75B845E4F7D, 0x01107D852E4C2485, 0xC51A50AAAA94FC61, 0x995E71BBEE983A2A, 0xC3713831264ADB47, 0xFB6BD1E058D5F004 },
        { 0x1815F774F320491B, 0x48569EFEC794D249, 0xEEB59AAE46D22BF7, 0x7DAFE25C5EDC28D7, 0xEA44F93EE1234AA8, 0x8F61C91912A4CCD9 },
        { 0xEC01498288516FC9, 0x26459F58E2C6AD8D, 0xF9B473CB0FC08C25, 0x96DA7CF0E49BE4B2, 0x98D88CEA927AC7F5, 0x39F1EDF228376D25 },
        { 0xD9519709F44AF73E, 0x2C8E291109A979DE, 0x3D61DC02BF69DEF7, 0xFBFFDFFFE6627515, 0x13F19AD57E17D4B9, 0x3BA1E484FC1980D5 },
        { 0xFED399D2217AAF4C, 0x717AD0C5102C1558, 0x9E1C990CC2B9A502, 0x9056A7F7485888D6, 0xAB65DB2370077A5C, 0xADB53FC9280D278F },
        { 0xD5B972302F5080D0, 0x830E0DE7B6B2CF38, 0x3665A008F4C4F386, 0xA61112652C742D20, 0xCB45AA51BD4F542F, 0xC733E2719E999291 },
        { 0x3C213A17F514638A, 0xCB3BF17F109F3E24, 0xC16F9F14F085B52A, 0x2F2B81ADC0DB83DF, 0x1A58DB2CE013191B, 0x8BA72D8FAE7E2A5E }
    };
    UNUSED(result_sha3_384);

    constexpr static const unsigned long long int result_sha3_512[data_count][gtl::sha3<512>::hash_size / sizeof(unsigned long long int)] = {
        { 0xA69F73CCA23A9AC5, 0xC8B567DC185A756E, 0x97C982164FE25859, 0xE0D1DCC1475C80A6, 0x15B2123AF1F5F94C, 0x11E3E9402C3AC558, 0xF500199D95B6D3E3, 0x01758586281DCD26 },
        { 0x697F2D856172CB83, 0x09D6B8B97DAC4DE3, 0x44B549D4DEE61EDF, 0xB4962D8698B7FA80, 0x3F4F93FF24393586, 0xE28B5B957AC3D1D3, 0x69420CE53332712F, 0x997BD336D09AB02A },
        { 0xB751850B1A57168A, 0x5693CD924B6B096E, 0x08F621827444F70D, 0x884F5D0240D2712E, 0x10E116E9192AF3C9, 0x1A7EC57647E39340, 0x57340B4CF408D5A5, 0x6592F8274EEC53F0 },
        { 0x3444E155881FA155, 0x11F57726C7D7CFE8, 0x0302A7433067B29D, 0x59A71415CA9DD141, 0xAC892D310BC4D781, 0x28C98FDA839D18D7, 0xF0556F2FE7ACB3C0, 0xCDA4BFF3A25F5F59 },
        { 0xAF328D17FA28753A, 0x3C9F5CB72E376B90, 0x440B96F0289E5703, 0xB729324A975AB384, 0xEDA565FC92AADED1, 0x43669900D7618616, 0x87ACDC0A5FFA358B, 0xD0571AAAD80ACA68 },
        { 0xD1DB17B4745B255E, 0x5EB159F66593CC9C, 0x143850979FC7A395, 0x1796ABA80165AAB5, 0x36B46174CE19E3F7, 0x07F0E5C6487F5F03, 0x084BC0EC9461691E, 0xF20113E42AD28163 },
        { 0x9524B9A5536B9106, 0x9526B4F6196B7E94, 0x75B4DA69E01F0C85, 0x5797F224CD7335DD, 0xB286FD99B9B32FFE, 0x33B59AD424CC1744, 0xF6EB59137F5FB860, 0x1932E8A8AF0AE930 }
    };
    UNUSED(result_sha3_512);

    testbench::test_template<sha3_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha3<value> sha3;

            for (unsigned int i = 0; i < data_count; ++i) {
                sha3.reset();
                sha3.consume(data1[i], testbench::string_length(data1[i]));
                sha3.consume(data2[i], testbench::string_length(data2[i]));
                sha3.finalise();
                typename gtl::sha3<value>::hash_type hash = sha3.get_hash();

                if constexpr (value == 224) {
                    char hash_string[56 + 4 + 56 + 1] = {};
                    snprintf(
                        hash_string,
                        56 + 4 + 56 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6],
                        result_sha3_224[i][0], result_sha3_224[i][1], result_sha3_224[i][2], result_sha3_224[i][3],
                        result_sha3_224[i][4], result_sha3_224[i][5], result_sha3_224[i][6]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_224[i], gtl::sha3<value>::hash_size) == true);
                }
                if constexpr (value == 256) {
                    char hash_string[64 + 4 + 64 + 1] = {};
                    snprintf(
                        hash_string,
                        64 + 4 + 64 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha3_256[i][0], result_sha3_256[i][1], result_sha3_256[i][2], result_sha3_256[i][3],
                        result_sha3_256[i][4], result_sha3_256[i][5], result_sha3_256[i][6], result_sha3_256[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_256[i], gtl::sha3<value>::hash_size) == true);
                }
                if constexpr (value == 384) {
                    char hash_string[96 + 4 + 96 + 1] = {};
                    snprintf(
                        hash_string,
                        96 + 4 + 96 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5],
                        result_sha3_384[i][0], result_sha3_384[i][1], result_sha3_384[i][2], result_sha3_384[i][3],
                        result_sha3_384[i][4], result_sha3_384[i][5]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_384[i], gtl::sha3<value>::hash_size) == true);
                }
                if constexpr (value == 512) {
                    char hash_string[128 + 4 + 128 + 1] = {};
                    snprintf(
                        hash_string,
                        128 + 4 + 128 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha3_512[i][0], result_sha3_512[i][1], result_sha3_512[i][2], result_sha3_512[i][3],
                        result_sha3_512[i][4], result_sha3_512[i][5], result_sha3_512[i][6], result_sha3_512[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha3_512[i], gtl::sha3<value>::hash_size) == true);
                }
            }
        }
    );
}
