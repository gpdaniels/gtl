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

#include <hash/sha2>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

using sha2_types = testbench::value_collection<224, 256, 384, 512>;

TEST(sha2, traits, standard) {
    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            REQUIRE((std::is_pod<gtl::sha2<value>>::value == false));

            REQUIRE((std::is_trivial<gtl::sha2<value>>::value == false));

            REQUIRE((std::is_trivially_copyable<gtl::sha2<value>>::value == true));

            REQUIRE((std::is_standard_layout<gtl::sha2<value>>::value == true));
        }
    );
}

TEST(sha2, constructor, empty) {
    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;
            testbench::do_not_optimise_away(sha2);
        }
    );
}

TEST(sha2, function, reset) {
    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;
            sha2.reset();
        }
    );
}

TEST(sha2, function, consume) {
    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;
            sha2.consume("", 0);
            sha2.consume("123456781234567812345678123456781234567812345678123456781234567", 63);
            sha2.consume("1234567812345678123456781234567812345678123456781234567812345678", 64);
            sha2.consume("12345678123456781234567812345678123456781234567812345678123456781", 65);
        }
    );
}

TEST(sha2, function, finalise) {
    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;
            sha2.finalise();
        }
    );
}

TEST(sha2, function, get_hash) {
    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;
            sha2.finalise();
            sha2.get_hash();
        }
    );
}

TEST(sha2, evaluate, hash_as_integer) {

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

    constexpr static const unsigned int result_sha2_224[data_count][gtl::sha2<224>::hash_size / sizeof(unsigned int)] = {
         { 0xD14A028C, 0x2A3A2BC9, 0x476102BB, 0x288234C4, 0x15A2B01F, 0x828EA62A, 0xC5B3E42F },
         { 0xABD37534, 0xC7D9A2EF, 0xB9465DE9, 0x31CD7055, 0xFFDB8879, 0x563AE980, 0x78D6D6D5 },
         { 0x23097D22, 0x3405D822, 0x8642A477, 0xBDA255B3, 0x2AADBCE4, 0xBDA0B3F7, 0xE36C9DA7 },
         { 0x2CB21C83, 0xAE2F004D, 0xE7E81C3C, 0x7019CBCB, 0x65B71AB6, 0x56B22D6D, 0x0C39B8EB },
         { 0x45A5F72C, 0x39C5CFF2, 0x522EB342, 0x9799E49E, 0x5F44B356, 0xEF926BCF, 0x390DCCC2 },
         { 0xBFF72B4F, 0xCB7D75E5, 0x632900AC, 0x5F90D219, 0xE05E97A7, 0xBDE72E74, 0x0DB393D9 },
         { 0xB50AECBE, 0x4E9BB0B5, 0x7BC5F3AE, 0x760A8E01, 0xDB24F203, 0xFB3CDCD1, 0x3148046E }
    };
    UNUSED(result_sha2_224);

    constexpr static const unsigned int result_sha2_256[data_count][gtl::sha2<256>::hash_size / sizeof(unsigned int)] = {
         { 0xE3B0C442, 0x98FC1C14, 0x9AFBF4C8, 0x996FB924, 0x27AE41E4, 0x649B934C, 0xA495991B, 0x7852B855 },
         { 0xCA978112, 0xCA1BBDCA, 0xFAC231B3, 0x9A23DC4D, 0xA786EFF8, 0x147C4E72, 0xB9807785, 0xAFEE48BB },
         { 0xBA7816BF, 0x8F01CFEA, 0x414140DE, 0x5DAE2223, 0xB00361A3, 0x96177A9C, 0xB410FF61, 0xF20015AD },
         { 0xF7846F55, 0xCF23E14E, 0xEBEAB5B4, 0xE1550CAD, 0x5B509E33, 0x48FBC4EF, 0xA3A1413D, 0x393CB650 },
         { 0x71C480DF, 0x93D6AE2F, 0x1EFAD144, 0x7C66C952, 0x5E316218, 0xCF51FC8D, 0x9ED832F2, 0xDAF18B73 },
         { 0xDB4BFCBD, 0x4DA0CD85, 0xA60C3C37, 0xD3FBD880, 0x5C77F15F, 0xC6B1FDFE, 0x614EE0A7, 0xC8FDB4C0 },
         { 0xF371BC4A, 0x311F2B00, 0x9EEF952D, 0xD83CA80E, 0x2B60026C, 0x8E935592, 0xD0F9C308, 0x453C813E }
    };
    UNUSED(result_sha2_256);

    constexpr static const unsigned long long int result_sha2_384[data_count][gtl::sha2<384>::hash_size / sizeof(unsigned long long int)] = {
        { 0x38B060A751AC9638, 0x4CD9327EB1B1E36A, 0x21FDB71114BE0743, 0x4C0CC7BF63F6E1DA, 0x274EDEBFE76F65FB, 0xD51AD2F14898B95B },
        { 0x54A59B9F22B0B808, 0x80D8427E548B7C23, 0xABD873486E1F035D, 0xCE9CD697E8517503, 0x3CAA88E6D57BC35E, 0xFAE0B5AFD3145F31 },
        { 0xCB00753F45A35E8B, 0xB5A03D699AC65007, 0x272C32AB0EDED163, 0x1A8B605A43FF5BED, 0x8086072BA1E7CC23, 0x58BAECA134C825A7 },
        { 0x473ED35167EC1F5D, 0x8E550368A3DB39BE, 0x54639F828868E945, 0x4C239FC8B52E3C61, 0xDBD0D8B4DE1390C2, 0x56DCBB5D5FD99CD5 },
        { 0xFEB67349DF3DB6F5, 0x924815D6C3DC133F, 0x091809213731FE5C, 0x7B5F4999E463479F, 0xF2877F5F2936FA63, 0xBB43784B12F3EBB4 },
        { 0x1761336E3F7CBFE5, 0x1DEB137F026F89E0, 0x1A448E3B1FAFA640, 0x39C1464EE8732F11, 0xA5341A6F41E0C202, 0x294736ED64DB1A84 },
        { 0xB12932B0627D1C06, 0x0942F54477641556, 0x55BD4DA0C9AFA6DD, 0x9B9EF53129AF1B8F, 0xB0195996D2DE9CA0, 0xDF9D821FFEE67026 }
    };
    UNUSED(result_sha2_384);

    constexpr static const unsigned long long int result_sha2_512[data_count][gtl::sha2<512>::hash_size / sizeof(unsigned long long int)] = {
        { 0xCF83E1357EEFB8BD, 0xF1542850D66D8007, 0xD620E4050B5715DC, 0x83F4A921D36CE9CE, 0x47D0D13C5D85F2B0, 0xFF8318D2877EEC2F, 0x63B931BD47417A81, 0xA538327AF927DA3E },
        { 0x1F40FC92DA241694, 0x750979EE6CF582F2, 0xD5D7D28E18335DE0, 0x5ABC54D0560E0F53, 0x02860C652BF08D56, 0x0252AA5E74210546, 0xF369FBBBCE8C12CF, 0xC7957B2652FE9A75 },
        { 0xDDAF35A193617ABA, 0xCC417349AE204131, 0x12E6FA4E89A97EA2, 0x0A9EEEE64B55D39A, 0x2192992A274FC1A8, 0x36BA3C23A3FEEBBD, 0x454D4423643CE80E, 0x2A9AC94FA54CA49F },
        { 0x107DBF389D9E9F71, 0xA3A95F6C055B9251, 0xBC5268C2BE16D6C1, 0x3492EA45B0199F33, 0x09E16455AB1E9611, 0x8E8A905D5597B720, 0x38DDB372A8982604, 0x6DE66687BB420E7C },
        { 0x4DBFF86CC2CA1BAE, 0x1E16468A05CB9881, 0xC97F1753BCE36190, 0x34898FAA1AABE429, 0x955A1BF8EC483D74, 0x21FE3C1646613A59, 0xED5441FB0F321389, 0xF77F48A879C7B1F1 },
        { 0x1E07BE23C26A86EA, 0x37EA810C8EC78093, 0x52515A970E9253C2, 0x6F536CFC7A9996C4, 0x5C8370583E0A78FA, 0x4A90041D71A4CEAB, 0x7423F19C71B9D5A3, 0xE01249F0BEBD5894 },
        { 0x72EC1EF1124A45B0, 0x47E8B7C75A932195, 0x135BB61DE24EC0D1, 0x914042246E0AEC3A, 0x2354E093D76F3048, 0xB456764346900CB1, 0x30D2A4FD5DD16ABB, 0x5E30BCB850DEE843 }
    };
    UNUSED(result_sha2_512);

    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;

            for (unsigned int i = 0; i < data_count; ++i) {
                sha2.reset();
                sha2.consume(data[i], strlen(data[i]));
                sha2.finalise();
                typename gtl::sha2<value>::hash_type hash = sha2.get_hash();

                if constexpr (value == 224) {
                    char hash_string[56 + 4 + 56 + 1] = {};
                    snprintf(
                        hash_string,
                        56 + 4 + 56 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6],
                        result_sha2_224[i][0], result_sha2_224[i][1], result_sha2_224[i][2], result_sha2_224[i][3],
                        result_sha2_224[i][4], result_sha2_224[i][5], result_sha2_224[i][6]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_224[i], gtl::sha2<value>::hash_size) == true);
                }
                if constexpr (value == 256) {
                    char hash_string[64 + 4 + 64 + 1] = {};
                    snprintf(
                        hash_string,
                        64 + 4 + 64 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha2_256[i][0], result_sha2_256[i][1], result_sha2_256[i][2], result_sha2_256[i][3],
                        result_sha2_256[i][4], result_sha2_256[i][5], result_sha2_256[i][6], result_sha2_256[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_256[i], gtl::sha2<value>::hash_size) == true);
                }
                if constexpr (value == 384) {
                    char hash_string[96 + 4 + 96 + 1] = {};
                    snprintf(
                        hash_string,
                        96 + 4 + 96 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5],
                        result_sha2_384[i][0], result_sha2_384[i][1], result_sha2_384[i][2], result_sha2_384[i][3],
                        result_sha2_384[i][4], result_sha2_384[i][5]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_384[i], gtl::sha2<value>::hash_size) == true);
                }
                if constexpr (value == 512) {
                    char hash_string[128 + 4 + 128 + 1] = {};
                    snprintf(
                        hash_string,
                        128 + 4 + 128 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha2_512[i][0], result_sha2_512[i][1], result_sha2_512[i][2], result_sha2_512[i][3],
                        result_sha2_512[i][4], result_sha2_512[i][5], result_sha2_512[i][6], result_sha2_512[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_512[i], gtl::sha2<value>::hash_size) == true);
                }
            }
        }
    );
}

TEST(sha2, evaluate, hash_as_string) {

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
            "D14A028C2A3A2BC9476102BB288234C415A2B01F828EA62AC5B3E42F",
            "ABD37534C7D9A2EFB9465DE931CD7055FFDB8879563AE98078D6D6D5",
            "23097D223405D8228642A477BDA255B32AADBCE4BDA0B3F7E36C9DA7",
            "2CB21C83AE2F004DE7E81C3C7019CBCB65B71AB656B22D6D0C39B8EB",
            "45A5F72C39C5CFF2522EB3429799E49E5F44B356EF926BCF390DCCC2",
            "BFF72B4FCB7D75E5632900AC5F90D219E05E97A7BDE72E740DB393D9",
            "B50AECBE4E9BB0B57BC5F3AE760A8E01DB24F203FB3CDCD13148046E"
        },
        { // 256
            "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855",
            "CA978112CA1BBDCAFAC231B39A23DC4DA786EFF8147C4E72B9807785AFEE48BB",
            "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD",
            "F7846F55CF23E14EEBEAB5B4E1550CAD5B509E3348FBC4EFA3A1413D393CB650",
            "71C480DF93D6AE2F1EFAD1447C66C9525E316218CF51FC8D9ED832F2DAF18B73",
            "DB4BFCBD4DA0CD85A60C3C37D3FBD8805C77F15FC6B1FDFE614EE0A7C8FDB4C0",
            "F371BC4A311F2B009EEF952DD83CA80E2B60026C8E935592D0F9C308453C813E"
        },
        { // 382
            "38B060A751AC96384CD9327EB1B1E36A21FDB71114BE07434C0CC7BF63F6E1DA274EDEBFE76F65FBD51AD2F14898B95B",
            "54A59B9F22B0B80880D8427E548B7C23ABD873486E1F035DCE9CD697E85175033CAA88E6D57BC35EFAE0B5AFD3145F31",
            "CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED1631A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7",
            "473ED35167EC1F5D8E550368A3DB39BE54639F828868E9454C239FC8B52E3C61DBD0D8B4DE1390C256DCBB5D5FD99CD5",
            "FEB67349DF3DB6F5924815D6C3DC133F091809213731FE5C7B5F4999E463479FF2877F5F2936FA63BB43784B12F3EBB4",
            "1761336E3F7CBFE51DEB137F026F89E01A448E3B1FAFA64039C1464EE8732F11A5341A6F41E0C202294736ED64DB1A84",
            "B12932B0627D1C060942F5447764155655BD4DA0C9AFA6DD9B9EF53129AF1B8FB0195996D2DE9CA0DF9D821FFEE67026"
        },
        { // 512
            "CF83E1357EEFB8BDF1542850D66D8007D620E4050B5715DC83F4A921D36CE9CE47D0D13C5D85F2B0FF8318D2877EEC2F63B931BD47417A81A538327AF927DA3E",
            "1F40FC92DA241694750979EE6CF582F2D5D7D28E18335DE05ABC54D0560E0F5302860C652BF08D560252AA5E74210546F369FBBBCE8C12CFC7957B2652FE9A75",
            "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F",
            "107DBF389D9E9F71A3A95F6C055B9251BC5268C2BE16D6C13492EA45B0199F3309E16455AB1E96118E8A905D5597B72038DDB372A89826046DE66687BB420E7C",
            "4DBFF86CC2CA1BAE1E16468A05CB9881C97F1753BCE3619034898FAA1AABE429955A1BF8EC483D7421FE3C1646613A59ED5441FB0F321389F77F48A879C7B1F1",
            "1E07BE23C26A86EA37EA810C8EC7809352515A970E9253C26F536CFC7A9996C45C8370583E0A78FA4A90041D71A4CEAB7423F19C71B9D5A3E01249F0BEBD5894",
            "72EC1EF1124A45B047E8B7C75A932195135BB61DE24EC0D1914042246E0AEC3A2354E093D76F3048B456764346900CB130D2A4FD5DD16ABB5E30BCB850DEE843"
        }
    };
    UNUSED(result);

    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;
            for (unsigned int i = 0; i < data_count; ++i) {
                sha2.reset();
                sha2.consume(data[i], strlen(data[i]));
                sha2.finalise();
                typename gtl::sha2<value>::hash_type hash = sha2.get_hash();

                unsigned long long int result_index = value == 512 ? 3 : value == 384 ? 2 : value == 256 ? 1 : 0;

                PRINT("%s == %s\n", gtl::sha2<value>::hash_to_string(hash).hash, result[result_index][i]);
                REQUIRE(testbench::is_string_same(gtl::sha2<value>::hash_to_string(hash).hash, result[result_index][i]) == true);
            }
        }
    );
}


TEST(sha2, evaluate, partial_insert) {

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

    constexpr static const unsigned int result_sha2_224[data_count][gtl::sha2<224>::hash_size / sizeof(unsigned int)] = {
        { 0xD14A028C, 0x2A3A2BC9, 0x476102BB, 0x288234C4, 0x15A2B01F, 0x828EA62A, 0xC5B3E42F },
        { 0xABD37534, 0xC7D9A2EF, 0xB9465DE9, 0x31CD7055, 0xFFDB8879, 0x563AE980, 0x78D6D6D5 },
        { 0x23097D22, 0x3405D822, 0x8642A477, 0xBDA255B3, 0x2AADBCE4, 0xBDA0B3F7, 0xE36C9DA7 },
        { 0x2CB21C83, 0xAE2F004D, 0xE7E81C3C, 0x7019CBCB, 0x65B71AB6, 0x56B22D6D, 0x0C39B8EB },
        { 0x45A5F72C, 0x39C5CFF2, 0x522EB342, 0x9799E49E, 0x5F44B356, 0xEF926BCF, 0x390DCCC2 },
        { 0xBFF72B4F, 0xCB7D75E5, 0x632900AC, 0x5F90D219, 0xE05E97A7, 0xBDE72E74, 0x0DB393D9 },
        { 0xB50AECBE, 0x4E9BB0B5, 0x7BC5F3AE, 0x760A8E01, 0xDB24F203, 0xFB3CDCD1, 0x3148046E }
    };
    UNUSED(result_sha2_224);

    constexpr static const unsigned int result_sha2_256[data_count][gtl::sha2<256>::hash_size / sizeof(unsigned int)] = {
        { 0xE3B0C442, 0x98FC1C14, 0x9AFBF4C8, 0x996FB924, 0x27AE41E4, 0x649B934C, 0xA495991B, 0x7852B855 },
        { 0xCA978112, 0xCA1BBDCA, 0xFAC231B3, 0x9A23DC4D, 0xA786EFF8, 0x147C4E72, 0xB9807785, 0xAFEE48BB },
        { 0xBA7816BF, 0x8F01CFEA, 0x414140DE, 0x5DAE2223, 0xB00361A3, 0x96177A9C, 0xB410FF61, 0xF20015AD },
        { 0xF7846F55, 0xCF23E14E, 0xEBEAB5B4, 0xE1550CAD, 0x5B509E33, 0x48FBC4EF, 0xA3A1413D, 0x393CB650 },
        { 0x71C480DF, 0x93D6AE2F, 0x1EFAD144, 0x7C66C952, 0x5E316218, 0xCF51FC8D, 0x9ED832F2, 0xDAF18B73 },
        { 0xDB4BFCBD, 0x4DA0CD85, 0xA60C3C37, 0xD3FBD880, 0x5C77F15F, 0xC6B1FDFE, 0x614EE0A7, 0xC8FDB4C0 },
        { 0xF371BC4A, 0x311F2B00, 0x9EEF952D, 0xD83CA80E, 0x2B60026C, 0x8E935592, 0xD0F9C308, 0x453C813E }
    };
    UNUSED(result_sha2_256);

    constexpr static const unsigned long long int result_sha2_384[data_count][gtl::sha2<384>::hash_size / sizeof(unsigned long long int)] = {
        { 0x38B060A751AC9638, 0x4CD9327EB1B1E36A, 0x21FDB71114BE0743, 0x4C0CC7BF63F6E1DA, 0x274EDEBFE76F65FB, 0xD51AD2F14898B95B },
        { 0x54A59B9F22B0B808, 0x80D8427E548B7C23, 0xABD873486E1F035D, 0xCE9CD697E8517503, 0x3CAA88E6D57BC35E, 0xFAE0B5AFD3145F31 },
        { 0xCB00753F45A35E8B, 0xB5A03D699AC65007, 0x272C32AB0EDED163, 0x1A8B605A43FF5BED, 0x8086072BA1E7CC23, 0x58BAECA134C825A7 },
        { 0x473ED35167EC1F5D, 0x8E550368A3DB39BE, 0x54639F828868E945, 0x4C239FC8B52E3C61, 0xDBD0D8B4DE1390C2, 0x56DCBB5D5FD99CD5 },
        { 0xFEB67349DF3DB6F5, 0x924815D6C3DC133F, 0x091809213731FE5C, 0x7B5F4999E463479F, 0xF2877F5F2936FA63, 0xBB43784B12F3EBB4 },
        { 0x1761336E3F7CBFE5, 0x1DEB137F026F89E0, 0x1A448E3B1FAFA640, 0x39C1464EE8732F11, 0xA5341A6F41E0C202, 0x294736ED64DB1A84 },
        { 0xB12932B0627D1C06, 0x0942F54477641556, 0x55BD4DA0C9AFA6DD, 0x9B9EF53129AF1B8F, 0xB0195996D2DE9CA0, 0xDF9D821FFEE67026 }
    };
    UNUSED(result_sha2_384);

    constexpr static const unsigned long long int result_sha2_512[data_count][gtl::sha2<512>::hash_size / sizeof(unsigned long long int)] = {
        { 0xCF83E1357EEFB8BD, 0xF1542850D66D8007, 0xD620E4050B5715DC, 0x83F4A921D36CE9CE, 0x47D0D13C5D85F2B0, 0xFF8318D2877EEC2F, 0x63B931BD47417A81, 0xA538327AF927DA3E },
        { 0x1F40FC92DA241694, 0x750979EE6CF582F2, 0xD5D7D28E18335DE0, 0x5ABC54D0560E0F53, 0x02860C652BF08D56, 0x0252AA5E74210546, 0xF369FBBBCE8C12CF, 0xC7957B2652FE9A75 },
        { 0xDDAF35A193617ABA, 0xCC417349AE204131, 0x12E6FA4E89A97EA2, 0x0A9EEEE64B55D39A, 0x2192992A274FC1A8, 0x36BA3C23A3FEEBBD, 0x454D4423643CE80E, 0x2A9AC94FA54CA49F },
        { 0x107DBF389D9E9F71, 0xA3A95F6C055B9251, 0xBC5268C2BE16D6C1, 0x3492EA45B0199F33, 0x09E16455AB1E9611, 0x8E8A905D5597B720, 0x38DDB372A8982604, 0x6DE66687BB420E7C },
        { 0x4DBFF86CC2CA1BAE, 0x1E16468A05CB9881, 0xC97F1753BCE36190, 0x34898FAA1AABE429, 0x955A1BF8EC483D74, 0x21FE3C1646613A59, 0xED5441FB0F321389, 0xF77F48A879C7B1F1 },
        { 0x1E07BE23C26A86EA, 0x37EA810C8EC78093, 0x52515A970E9253C2, 0x6F536CFC7A9996C4, 0x5C8370583E0A78FA, 0x4A90041D71A4CEAB, 0x7423F19C71B9D5A3, 0xE01249F0BEBD5894 },
        { 0x72EC1EF1124A45B0, 0x47E8B7C75A932195, 0x135BB61DE24EC0D1, 0x914042246E0AEC3A, 0x2354E093D76F3048, 0xB456764346900CB1, 0x30D2A4FD5DD16ABB, 0x5E30BCB850DEE843 }
    };
    UNUSED(result_sha2_512);

    testbench::test_template<sha2_types>(
        [](auto test_value)->void {
            constexpr static const unsigned long long int value = decltype(test_value)::value;

            gtl::sha2<value> sha2;

            for (unsigned int i = 0; i < data_count; ++i) {
                sha2.reset();
                sha2.consume(data1[i], strlen(data1[i]));
                sha2.consume(data2[i], strlen(data2[i]));
                sha2.finalise();
                typename gtl::sha2<value>::hash_type hash = sha2.get_hash();

                if constexpr (value == 224) {
                    char hash_string[56 + 4 + 56 + 1] = {};
                    snprintf(
                        hash_string,
                        56 + 4 + 56 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6],
                        result_sha2_224[i][0], result_sha2_224[i][1], result_sha2_224[i][2], result_sha2_224[i][3],
                        result_sha2_224[i][4], result_sha2_224[i][5], result_sha2_224[i][6]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_224[i], gtl::sha2<value>::hash_size) == true);
                }
                if constexpr (value == 256) {
                    char hash_string[64 + 4 + 64 + 1] = {};
                    snprintf(
                        hash_string,
                        64 + 4 + 64 + 1,
                        "%08X%08X%08X%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X%08X%08X%08X",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha2_256[i][0], result_sha2_256[i][1], result_sha2_256[i][2], result_sha2_256[i][3],
                        result_sha2_256[i][4], result_sha2_256[i][5], result_sha2_256[i][6], result_sha2_256[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_256[i], gtl::sha2<value>::hash_size) == true);
                }
                if constexpr (value == 384) {
                    char hash_string[96 + 4 + 96 + 1] = {};
                    snprintf(
                        hash_string,
                        96 + 4 + 96 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5],
                        result_sha2_384[i][0], result_sha2_384[i][1], result_sha2_384[i][2], result_sha2_384[i][3],
                        result_sha2_384[i][4], result_sha2_384[i][5]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_384[i], gtl::sha2<value>::hash_size) == true);
                }
                if constexpr (value == 512) {
                    char hash_string[128 + 4 + 128 + 1] = {};
                    snprintf(
                        hash_string,
                        128 + 4 + 128 + 1,
                        "%016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX == %016llX%016llX%016llX%016llX%016llX%016llX%016llX%016llX",
                        hash.data[0], hash.data[1], hash.data[2], hash.data[3],
                        hash.data[4], hash.data[5], hash.data[6], hash.data[7],
                        result_sha2_512[i][0], result_sha2_512[i][1], result_sha2_512[i][2], result_sha2_512[i][3],
                        result_sha2_512[i][4], result_sha2_512[i][5], result_sha2_512[i][6], result_sha2_512[i][7]
                        );
                    PRINT("%s\n", hash_string);
                    REQUIRE(testbench::is_memory_same(hash.data, result_sha2_512[i], gtl::sha2<value>::hash_size) == true);
                }
            }
        }
    );
}
