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

#include <hash/sha0>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(sha0, traits, standard) {
    REQUIRE(sizeof(gtl::sha0) >= 0);

    REQUIRE((std::is_pod<gtl::sha0>::value == false));

    REQUIRE((std::is_trivial<gtl::sha0>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::sha0>::value == true));

    REQUIRE((std::is_standard_layout<gtl::sha0>::value == true));
}

TEST(sha0, constructor, empty) {
    gtl::sha0 sha0;
    testbench::do_not_optimise_away(sha0);
}

TEST(sha0, function, clear) {
    gtl::sha0 sha0;
    sha0.clear();
}

TEST(sha0, function, insert) {
    gtl::sha0 sha0;
    sha0.insert("", 0);
    sha0.insert("123456781234567812345678123456781234567812345678123456781234567", 63);
    sha0.insert("1234567812345678123456781234567812345678123456781234567812345678", 64);
    sha0.insert("12345678123456781234567812345678123456781234567812345678123456781", 65);
}

TEST(sha0, function, finalise) {
    gtl::sha0 sha0;
    sha0.finalise();
}

TEST(sha0, evaluate, hash_as_integer) {

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

    constexpr static unsigned int result[data_count][gtl::sha0::hash_size / sizeof(unsigned int)] = {
        { 0xF96CEA19, 0x8AD1DD56, 0x17AC084A, 0x3D92C610, 0x7708C0EF },
        { 0x37F29777, 0x2FAE4CB1, 0xBA39B6CF, 0x9CF03811, 0x80BD62F2 },
        { 0x0164B8A9, 0x14CD2A5E, 0x74C4F7FF, 0x082C4D97, 0xF1EDF880 },
        { 0xC1B0F222, 0xD150EBB9, 0xAA36A40C, 0xAFDC8BCB, 0xED830B14 },
        { 0xB40CE07A, 0x430CFD3C, 0x033039B9, 0xFE9AFEC9, 0x5DC1BDCD },
        { 0x79E966F7, 0xA3A990DF, 0x33E40E3D, 0x7F8F18D2, 0xCAEBADFA },
        { 0x4AA29D14, 0xD171522E, 0xCE47BEE8, 0x957E35A4, 0x1F3E9CFF }
    };

    gtl::sha0 sha0;

    for (unsigned int i = 0; i < data_count; ++i) {
        sha0.clear();
        sha0.insert(data[i], strlen(data[i]));
        gtl::sha0::hash_type hash = sha0.finalise();

        char hash_string[40 + 4 + 40 + 1] = {};
        snprintf(hash_string, 40 + 4 + 40 + 1, "%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X", hash.data[0], hash.data[1], hash.data[2], hash.data[3], hash.data[4], result[i][0], result[i][1], result[i][2], result[i][3], result[i][4]);
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha0::hash_size) == true);
    }
}

TEST(sha0, evaluate, hash_as_string) {

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

    constexpr static const char* result[data_count] = {
        "F96CEA198AD1DD5617AC084A3D92C6107708C0EF",
        "37F297772FAE4CB1BA39B6CF9CF0381180BD62F2",
        "0164B8A914CD2A5E74C4F7FF082C4D97F1EDF880",
        "C1B0F222D150EBB9AA36A40CAFDC8BCBED830B14",
        "B40CE07A430CFD3C033039B9FE9AFEC95DC1BDCD",
        "79E966F7A3A990DF33E40E3D7F8F18D2CAEBADFA",
        "4AA29D14D171522ECE47BEE8957E35A41F3E9CFF"
    };

    gtl::sha0 sha0;

    for (unsigned int i = 0; i < data_count; ++i) {
        sha0.clear();
        sha0.insert(data[i], strlen(data[i]));
        gtl::sha0::hash_type hash = sha0.finalise();
        PRINT("%s == %s\n", gtl::sha0::hash_to_string(hash).hash, result[i]);
        REQUIRE(testbench::is_string_same(gtl::sha0::hash_to_string(hash).hash, result[i]) == true);
    }
}


TEST(sha0, evaluate, partial_insert) {

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

    constexpr static const char* data2[data_count] = {
        "",
        "",
        "abc",
        " digest",
        "nopqrstuvwxyz",
        "",
        "12345678901234567890"
    };

    constexpr static unsigned int result[data_count][gtl::sha0::hash_size / sizeof(unsigned int)] = {
        { 0xF96CEA19, 0x8AD1DD56, 0x17AC084A, 0x3D92C610, 0x7708C0EF },
        { 0x37F29777, 0x2FAE4CB1, 0xBA39B6CF, 0x9CF03811, 0x80BD62F2 },
        { 0x0164B8A9, 0x14CD2A5E, 0x74C4F7FF, 0x082C4D97, 0xF1EDF880 },
        { 0xC1B0F222, 0xD150EBB9, 0xAA36A40C, 0xAFDC8BCB, 0xED830B14 },
        { 0xB40CE07A, 0x430CFD3C, 0x033039B9, 0xFE9AFEC9, 0x5DC1BDCD },
        { 0x79E966F7, 0xA3A990DF, 0x33E40E3D, 0x7F8F18D2, 0xCAEBADFA },
        { 0x4AA29D14, 0xD171522E, 0xCE47BEE8, 0x957E35A4, 0x1F3E9CFF }
    };

    gtl::sha0 sha0;

    for (unsigned int i = 0; i < data_count; ++i) {
        sha0.clear();
        sha0.insert(data1[i], strlen(data1[i]));
        sha0.insert(data2[i], strlen(data2[i]));
        gtl::sha0::hash_type hash = sha0.finalise();

        char hash_string[40 + 4 + 40 + 1] = {};
        snprintf(hash_string, 40 + 4 + 40 + 1, "%08X%08X%08X%08X%08X == %08X%08X%08X%08X%08X", hash.data[0], hash.data[1], hash.data[2], hash.data[3], hash.data[4], result[i][0], result[i][1], result[i][2], result[i][3], result[i][4]);
        PRINT("%s\n", hash_string);

        REQUIRE(testbench::is_memory_same(hash.data, result[i], gtl::sha0::hash_size) == true);
    }
}
