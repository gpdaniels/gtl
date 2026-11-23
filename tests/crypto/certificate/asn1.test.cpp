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

#include <crypto/certificate/asn1>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {
    /// @brief  Convert a string of hexadecimal characters into bytes.
    std::vector<unsigned char> hex_2_bytes(const char* hex) {
        const auto nibble = [](const char character) -> unsigned char {
            return static_cast<unsigned char>((character >= 'A') ? (character - 'A' + 10) : (character - '0'));
        };
        std::vector<unsigned char> bytes;
        for (unsigned int i = 0; (hex[i] != 0) && (hex[i + 1] != 0); i += 2) {
            bytes.push_back(static_cast<unsigned char>((nibble(hex[i]) << 4) | nibble(hex[i + 1])));
        }
        return bytes;
    }

    /// @brief  Parse a buffer that must hold exactly one element.
    bool parse_whole(const std::vector<unsigned char>& bytes, gtl::asn1::element& element) {
        return gtl::asn1::parse(bytes.data(), bytes.size(), element) && (element.encoded_length == bytes.size());
    }

    // An rsa public key encoded by another implementation.
    // It is a sequence of an algorithm identifier (a sequence of an object identifier and a null) and a string of bits that holds a sequence of two integers.
    constexpr const char* reference_public_key = "30819F300D06092A864886F70D010101050003818D0030818902818100C94E2F3F4155BB327E4C3A14EED197EDB7D034D8950693DEF85A82764D718D56BECE43DA68294BE4DB0CECE54C31A3CCCFF43D2FFB282BCA5AAEA444124FFD0AF370C3CE96C037F89E316E3A7C2803785E4BDA6E15F8FF8EBD29711764C9BA79D4B1C5B4832D1E8ABF2C6717B7EAB5E72361E6B7338F87315D48D4FEA87480990203010001";
    constexpr const char* reference_modulus = "C94E2F3F4155BB327E4C3A14EED197EDB7D034D8950693DEF85A82764D718D56BECE43DA68294BE4DB0CECE54C31A3CCCFF43D2FFB282BCA5AAEA444124FFD0AF370C3CE96C037F89E316E3A7C2803785E4BDA6E15F8FF8EBD29711764C9BA79D4B1C5B4832D1E8ABF2C6717B7EAB5E72361E6B7338F87315D48D4FEA8748099";
    constexpr const unsigned char object_identifier_rsa[9] = { 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01 };

    // Signatures, which are a sequence of two integers, encoded by another implementation.
    constexpr const char* reference_signature = "3045022100EFD48B2AACB6A8FD1140DD9CD45E81D69D2C877B56AAF991C34D0EA84EAF37160220019F4113742A2B14BD25926B49C649155F267E60D3814B4C0CC84250E46F0083";
    constexpr const char* reference_signature_r = "EFD48B2AACB6A8FD1140DD9CD45E81D69D2C877B56AAF991C34D0EA84EAF3716";
    constexpr const char* reference_signature_s = "019F4113742A2B14BD25926B49C649155F267E60D3814B4C0CC84250E46F0083";
    constexpr const char* reference_signature_1_127 = "300602010102017F";
    constexpr const char* reference_signature_128_0 = "300702020080020100";

    // Times computed by another implementation.
    struct reference_time_type {
        long long int seconds;
        unsigned int year;
        unsigned int month;
        unsigned int day;
        unsigned int hour;
        unsigned int minute;
        unsigned int second;
    };

    constexpr const reference_time_type reference_times[] = {
        { 0ll, 1970, 1, 1, 0, 0, 0 },
        { 1ll, 1970, 1, 1, 0, 0, 1 },
        { 59ll, 1970, 1, 1, 0, 0, 59 },
        { 60ll, 1970, 1, 1, 0, 1, 0 },
        { 86399ll, 1970, 1, 1, 23, 59, 59 },
        { 86400ll, 1970, 1, 2, 0, 0, 0 },
        { 951782399ll, 2000, 2, 28, 23, 59, 59 },
        { 951782400ll, 2000, 2, 29, 0, 0, 0 },
        { 951868800ll, 2000, 3, 1, 0, 0, 0 },
        { 946684799ll, 1999, 12, 31, 23, 59, 59 },
        { 946684800ll, 2000, 1, 1, 0, 0, 0 },
        { 1234567890ll, 2009, 2, 13, 23, 31, 30 },
        { 2147483647ll, 2038, 1, 19, 3, 14, 7 },
        { 2147483648ll, 2038, 1, 19, 3, 14, 8 },
        { 2524607999ll, 2049, 12, 31, 23, 59, 59 },
        { 2524608000ll, 2050, 1, 1, 0, 0, 0 },
        { 4102444800ll, 2100, 1, 1, 0, 0, 0 },
        { 4107542399ll, 2100, 2, 28, 23, 59, 59 },
        { 4107542400ll, 2100, 3, 1, 0, 0, 0 },
        { 253402300799ll, 9999, 12, 31, 23, 59, 59 },
        { -1ll, 1969, 12, 31, 23, 59, 59 },
        { -86400ll, 1969, 12, 31, 0, 0, 0 },
        { -631152000ll, 1950, 1, 1, 0, 0, 0 },
        { -2208988800ll, 1900, 1, 1, 0, 0, 0 },
        { 1709164800ll, 2024, 2, 29, 0, 0, 0 },
        { 1740787199ll, 2025, 2, 28, 23, 59, 59 },
        { -62135596800ll, 1, 1, 1, 0, 0, 0 },
    };

    /// @brief  Read every element in a buffer, and the contents of every constructed element, counting the elements.
    /// @return false if anything in the buffer is not correctly encoded.
    bool walk(const unsigned char* data, const unsigned long long int length, const unsigned int depth, unsigned int& count) {
        gtl::asn1::reader reader(data, length);
        while (!reader.is_end()) {
            gtl::asn1::element element;
            if (!reader.read(element)) {
                return false;
            }
            ++count;
            // Every element must lie inside the buffer it was read from.
            if ((element.encoded < data) || ((element.encoded + element.encoded_length) > (data + length)) || ((element.content + element.content_length) != (element.encoded + element.encoded_length))) {
                return false;
            }
            if (element.is_constructed() && (depth < 16)) {
                if (!walk(element.content, element.content_length, depth + 1, count)) {
                    return false;
                }
            }
        }
        return true;
    }
}

TEST(asn1, traits, standard) {
    REQUIRE((std::is_pod<gtl::asn1>::value == true));

    REQUIRE((std::is_trivial<gtl::asn1>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::asn1>::value == true));

    REQUIRE((std::is_standard_layout<gtl::asn1>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::asn1::element>::value == true));
    REQUIRE((std::is_trivially_copyable<gtl::asn1::time>::value == true));
    REQUIRE((std::is_trivially_copyable<gtl::asn1::reader>::value == true));
    REQUIRE((std::is_trivially_copyable<gtl::asn1::writer>::value == true));
}

TEST(asn1, constructor, empty) {
    gtl::asn1 asn1;
    testbench::do_not_optimise_away(asn1);
    gtl::asn1::element element;
    REQUIRE(element.tag == 0);
    REQUIRE(element.encoded == nullptr);
    REQUIRE(element.encoded_length == 0);
    REQUIRE(element.content == nullptr);
    REQUIRE(element.content_length == 0);
    gtl::asn1::reader reader;
    REQUIRE(reader.is_end() == true);
    REQUIRE(reader.read(element) == false);
    gtl::asn1::time time;
    REQUIRE(time.is_valid() == true);
    REQUIRE(time.to_seconds() == 0);
}

TEST(asn1, parameter, tags) {
    REQUIRE(gtl::asn1::tag_boolean == 0x01);
    REQUIRE(gtl::asn1::tag_integer == 0x02);
    REQUIRE(gtl::asn1::tag_bit_string == 0x03);
    REQUIRE(gtl::asn1::tag_octet_string == 0x04);
    REQUIRE(gtl::asn1::tag_null == 0x05);
    REQUIRE(gtl::asn1::tag_object_identifier == 0x06);
    REQUIRE(gtl::asn1::tag_utf8_string == 0x0C);
    REQUIRE(gtl::asn1::tag_printable_string == 0x13);
    REQUIRE(gtl::asn1::tag_t61_string == 0x14);
    REQUIRE(gtl::asn1::tag_ia5_string == 0x16);
    REQUIRE(gtl::asn1::tag_utc_time == 0x17);
    REQUIRE(gtl::asn1::tag_generalized_time == 0x18);
    REQUIRE(gtl::asn1::tag_bmp_string == 0x1E);
    REQUIRE(gtl::asn1::tag_sequence == 0x30);
    REQUIRE(gtl::asn1::tag_set == 0x31);
    REQUIRE(gtl::asn1::tag_context_constructed(0) == 0xA0);
    REQUIRE(gtl::asn1::tag_context_constructed(3) == 0xA3);
    REQUIRE(gtl::asn1::tag_context_primitive(0) == 0x80);
    REQUIRE(gtl::asn1::tag_context_primitive(2) == 0x82);
}

TEST(asn1, function, parse) {
    gtl::asn1::element element;

    // The short form of the length.
    {
        const std::vector<unsigned char> bytes = hex_2_bytes("0403AABBCC");
        REQUIRE(gtl::asn1::parse(bytes.data(), bytes.size(), element) == true);
        REQUIRE(element.tag == gtl::asn1::tag_octet_string);
        REQUIRE(element.encoded == bytes.data());
        REQUIRE(element.encoded_length == 5);
        REQUIRE(element.content == bytes.data() + 2);
        REQUIRE(element.content_length == 3);
        REQUIRE(element.is_constructed() == false);
        REQUIRE(element.is_content(bytes.data() + 2, 3) == true);
        REQUIRE(element.is_content(bytes.data() + 2, 2) == false);
        REQUIRE(element.is_content(bytes.data() + 1, 3) == false);
    }

    // No contents.
    {
        const std::vector<unsigned char> bytes = hex_2_bytes("0500");
        REQUIRE(gtl::asn1::parse(bytes.data(), bytes.size(), element) == true);
        REQUIRE(element.tag == gtl::asn1::tag_null);
        REQUIRE(element.encoded_length == 2);
        REQUIRE(element.content_length == 0);
        REQUIRE(element.is_content(nullptr, 0) == true);
    }

    // Data after the element is not part of it.
    {
        const std::vector<unsigned char> bytes = hex_2_bytes("0201070201080000");
        REQUIRE(gtl::asn1::parse(bytes.data(), bytes.size(), element) == true);
        REQUIRE(element.encoded_length == 3);
        REQUIRE(element.content_length == 1);
        REQUIRE(element.content[0] == 0x07);
    }

    // The long form of the length, at the boundaries of each number of length bytes.
    const unsigned long long int lengths[8] = { 127, 128, 255, 256, 65535, 65536, 70000, 16777216 };
    for (const unsigned long long int length : lengths) {
        std::vector<unsigned char> bytes;
        bytes.push_back(gtl::asn1::tag_sequence);
        unsigned long long int header_length = 2;
        if (length < 128) {
            bytes.push_back(static_cast<unsigned char>(length));
        }
        else {
            unsigned int length_bytes = 0;
            for (unsigned long long int remaining = length; remaining != 0; remaining >>= 8) {
                ++length_bytes;
            }
            bytes.push_back(static_cast<unsigned char>(0x80 | length_bytes));
            for (unsigned int i = length_bytes; i-- > 0;) {
                bytes.push_back(static_cast<unsigned char>((length >> (8 * i)) & 0xFF));
            }
            header_length += length_bytes;
        }
        bytes.resize(header_length + length, 0x5A);
        REQUIRE(gtl::asn1::parse(bytes.data(), bytes.size(), element) == true);
        REQUIRE(element.tag == gtl::asn1::tag_sequence);
        REQUIRE(element.is_constructed() == true);
        REQUIRE(element.encoded_length == bytes.size());
        REQUIRE(element.content == bytes.data() + header_length);
        REQUIRE(element.content_length == length);
        // One byte short of the contents, or of the header, is not a complete element.
        REQUIRE(gtl::asn1::parse(bytes.data(), bytes.size() - 1, element) == false);
        REQUIRE(gtl::asn1::parse(bytes.data(), header_length - 1, element) == false);
        REQUIRE(gtl::asn1::parse(bytes.data(), header_length, element) == false);
    }
}

TEST(asn1, function, parse_invalid) {
    gtl::asn1::element element;
    element.tag = 0x77;
    const unsigned char one[1] = { 0x05 };
    REQUIRE(gtl::asn1::parse(nullptr, 0, element) == false);
    REQUIRE(gtl::asn1::parse(nullptr, 10, element) == false);
    REQUIRE(gtl::asn1::parse(one, 0, element) == false);
    REQUIRE(gtl::asn1::parse(one, 1, element) == false);

    const char* invalid[] = {
        // Contents that are longer than the data.
        "0401",
        "0403AABB",
        "30820100",
        // A length that is not definite.
        "30800000",
        // Lengths that are not the shortest encoding.
        "048103AABBCC",
        "0482000301AABBCC",
        "04820003AABBCC",
        "0483000080",
        // Length bytes that are missing.
        "0481",
        "0482FF",
        "0484010000",
        // A length that needs more than eight bytes, and one that is far longer than the data.
        "0489010000000000000000",
        "0488FFFFFFFFFFFFFFFF",
        "0488800000000000000000",
        // A tag that needs more than one byte.
        "1F8101AA",
        "3F8101AA",
        "BF8101AA",
    };
    for (const char* hex : invalid) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        REQUIRE(gtl::asn1::parse(bytes.data(), bytes.size(), element) == false, "Parsed the invalid encoding '%s'.", hex);
    }
    // A failed parse does not change the element.
    REQUIRE(element.tag == 0x77);
    REQUIRE(element.encoded == nullptr);
}

TEST(asn1, function, reader) {
    const std::vector<unsigned char> bytes = hex_2_bytes("02010705000402AABB30060101FF020109");
    gtl::asn1::reader reader(bytes.data(), bytes.size());
    gtl::asn1::element element;
    REQUIRE(reader.is_end() == false);

    // Peeking does not move the reader.
    REQUIRE(reader.peek(element) == true);
    REQUIRE(element.tag == gtl::asn1::tag_integer);
    REQUIRE(reader.is_next(gtl::asn1::tag_integer) == true);
    REQUIRE(reader.is_next(gtl::asn1::tag_null) == false);
    REQUIRE(reader.peek(element) == true);
    REQUIRE(element.tag == gtl::asn1::tag_integer);

    // Reading with the wrong tag does not move the reader or change the element.
    element.tag = 0x77;
    REQUIRE(reader.read(gtl::asn1::tag_null, element) == false);
    REQUIRE(element.tag == 0x77);
    REQUIRE(reader.read(gtl::asn1::tag_integer, element) == true);
    REQUIRE(element.content_length == 1);
    REQUIRE(element.content[0] == 0x07);

    REQUIRE(reader.read(element) == true);
    REQUIRE(element.tag == gtl::asn1::tag_null);
    REQUIRE(reader.read(gtl::asn1::tag_octet_string, element) == true);
    REQUIRE(element.content_length == 2);
    REQUIRE(reader.is_end() == false);

    gtl::asn1::element sequence;
    REQUIRE(reader.read(gtl::asn1::tag_sequence, sequence) == true);
    REQUIRE(reader.is_end() == true);
    REQUIRE(reader.read(element) == false);
    REQUIRE(reader.peek(element) == false);
    REQUIRE(reader.is_next(gtl::asn1::tag_integer) == false);

    // Read the contents of the sequence.
    gtl::asn1::reader reader_sequence(sequence);
    bool boolean = false;
    REQUIRE(reader_sequence.read(gtl::asn1::tag_boolean, element) == true);
    REQUIRE(gtl::asn1::decode_boolean(element, boolean) == true);
    REQUIRE(boolean == true);
    unsigned long long int integer = 0;
    REQUIRE(reader_sequence.read(gtl::asn1::tag_integer, element) == true);
    REQUIRE(gtl::asn1::decode_integer(element, integer) == true);
    REQUIRE(integer == 9);
    REQUIRE(reader_sequence.is_end() == true);

    // A reader stops at an element that is not correctly encoded.
    const std::vector<unsigned char> truncated = hex_2_bytes("02010702");
    gtl::asn1::reader reader_truncated(truncated.data(), truncated.size());
    REQUIRE(reader_truncated.read(element) == true);
    REQUIRE(reader_truncated.is_end() == false);
    REQUIRE(reader_truncated.read(element) == false);
    REQUIRE(reader_truncated.is_end() == false);

    // A reader of nothing.
    gtl::asn1::reader reader_null(nullptr, 100);
    REQUIRE(reader_null.is_end() == true);
    gtl::asn1::reader reader_empty(bytes.data(), 0);
    REQUIRE(reader_empty.is_end() == true);
    REQUIRE(reader_empty.read(element) == false);
}

TEST(asn1, function, decode_boolean) {
    gtl::asn1::element element;
    bool value = false;
    REQUIRE(parse_whole(hex_2_bytes("0101FF"), element) == true);
    const std::vector<unsigned char> bytes_true = hex_2_bytes("0101FF");
    REQUIRE(parse_whole(bytes_true, element) == true);
    REQUIRE(gtl::asn1::decode_boolean(element, value) == true);
    REQUIRE(value == true);
    const std::vector<unsigned char> bytes_false = hex_2_bytes("010100");
    REQUIRE(parse_whole(bytes_false, element) == true);
    REQUIRE(gtl::asn1::decode_boolean(element, value) == true);
    REQUIRE(value == false);

    // True must be encoded as all bits set, the length must be one, and the tag must be correct.
    const char* invalid[] = { "010101", "010180", "0100", "0102FFFF", "0201FF" };
    for (const char* hex : invalid) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        REQUIRE(parse_whole(bytes, element) == true);
        value = true;
        REQUIRE(gtl::asn1::decode_boolean(element, value) == false, "Decoded the invalid boolean '%s'.", hex);
        REQUIRE(value == true);
    }
}

TEST(asn1, function, decode_null) {
    gtl::asn1::element element;
    const std::vector<unsigned char> bytes = hex_2_bytes("0500");
    REQUIRE(parse_whole(bytes, element) == true);
    REQUIRE(gtl::asn1::decode_null(element) == true);
    const std::vector<unsigned char> bytes_content = hex_2_bytes("050100");
    REQUIRE(parse_whole(bytes_content, element) == true);
    REQUIRE(gtl::asn1::decode_null(element) == false);
    const std::vector<unsigned char> bytes_tag = hex_2_bytes("0400");
    REQUIRE(parse_whole(bytes_tag, element) == true);
    REQUIRE(gtl::asn1::decode_null(element) == false);
}

TEST(asn1, function, decode_integer) {
    struct known_type {
        const char* hex;
        unsigned long long int value;
        unsigned long long int length;
    };

    const known_type knowns[] = {
        { "020100", 0x00ull, 1 },
        { "020101", 0x01ull, 1 },
        { "02017F", 0x7Full, 1 },
        { "02020080", 0x80ull, 1 },
        { "020200FF", 0xFFull, 1 },
        { "02020100", 0x0100ull, 2 },
        { "02027FFF", 0x7FFFull, 2 },
        { "0203008000", 0x8000ull, 2 },
        { "020301E240", 123456ull, 3 },
        { "02087FFFFFFFFFFFFFFF", 0x7FFFFFFFFFFFFFFFull, 8 },
        { "020900FFFFFFFFFFFFFFFF", 0xFFFFFFFFFFFFFFFFull, 8 },
    };
    gtl::asn1::element element;
    for (const known_type& known : knowns) {
        const std::vector<unsigned char> bytes = hex_2_bytes(known.hex);
        REQUIRE(parse_whole(bytes, element) == true);
        unsigned long long int value = 12345;
        REQUIRE(gtl::asn1::decode_integer(element, value) == true, "Failed to decode the integer '%s'.", known.hex);
        REQUIRE(value == known.value);
        const unsigned char* unsigned_bytes = nullptr;
        unsigned long long int unsigned_length = 0;
        REQUIRE(gtl::asn1::decode_unsigned(element, unsigned_bytes, unsigned_length) == true);
        REQUIRE(unsigned_length == known.length);
        REQUIRE(unsigned_bytes == (bytes.data() + bytes.size() - known.length));
    }

    // Negative integers, integers with unnecessary leading zeros, empty integers, and the wrong tag.
    const char* invalid[] = { "020180", "0201FF", "0202FF7F", "02020001", "0202007F", "020300007F", "0200", "040101" };
    for (const char* hex : invalid) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        REQUIRE(parse_whole(bytes, element) == true);
        unsigned long long int value = 12345;
        REQUIRE(gtl::asn1::decode_integer(element, value) == false, "Decoded the invalid integer '%s'.", hex);
        REQUIRE(value == 12345);
        const unsigned char* unsigned_bytes = nullptr;
        unsigned long long int unsigned_length = 0;
        REQUIRE(gtl::asn1::decode_unsigned(element, unsigned_bytes, unsigned_length) == false);
        REQUIRE(unsigned_bytes == nullptr);
    }

    // Integers that are valid but do not fit in sixty four bits.
    const char* large[] = { "0209010000000000000000", "020A00FFFFFFFFFFFFFFFFFF" };
    for (const char* hex : large) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        REQUIRE(parse_whole(bytes, element) == true);
        unsigned long long int value = 12345;
        REQUIRE(gtl::asn1::decode_integer(element, value) == false);
        REQUIRE(value == 12345);
        const unsigned char* unsigned_bytes = nullptr;
        unsigned long long int unsigned_length = 0;
        REQUIRE(gtl::asn1::decode_unsigned(element, unsigned_bytes, unsigned_length) == true);
        REQUIRE(unsigned_length == 9);
    }
}

TEST(asn1, function, decode_bit_string) {
    gtl::asn1::element element;
    const unsigned char* bits = nullptr;
    unsigned long long int length = 0;
    unsigned int unused_bits = 99;
    {
        const std::vector<unsigned char> bytes = hex_2_bytes("030300AABB");
        REQUIRE(parse_whole(bytes, element) == true);
        REQUIRE(gtl::asn1::decode_bit_string(element, bits, length, unused_bits) == true);
        REQUIRE(bits == bytes.data() + 3);
        REQUIRE(length == 2);
        REQUIRE(unused_bits == 0);
    }
    {
        const std::vector<unsigned char> bytes = hex_2_bytes("03020780");
        REQUIRE(parse_whole(bytes, element) == true);
        REQUIRE(gtl::asn1::decode_bit_string(element, bits, length, unused_bits) == true);
        REQUIRE(length == 1);
        REQUIRE(bits[0] == 0x80);
        REQUIRE(unused_bits == 7);
    }
    {
        const std::vector<unsigned char> bytes = hex_2_bytes("030100");
        REQUIRE(parse_whole(bytes, element) == true);
        REQUIRE(gtl::asn1::decode_bit_string(element, bits, length, unused_bits) == true);
        REQUIRE(length == 0);
        REQUIRE(unused_bits == 0);
    }
    // No unused bit count, a count above seven, unused bits in an empty string, unused bits that are set, and the wrong tag.
    const char* invalid[] = { "0300", "030208FF", "030101", "030201FF", "03020740", "040200AA" };
    for (const char* hex : invalid) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        REQUIRE(parse_whole(bytes, element) == true);
        length = 1234;
        REQUIRE(gtl::asn1::decode_bit_string(element, bits, length, unused_bits) == false, "Decoded the invalid bit string '%s'.", hex);
        REQUIRE(length == 1234);
    }
}

TEST(asn1, function, decode_time) {
    struct known_type {
        const char* hex;
        gtl::asn1::time time;
    };

    known_type knowns[5] = {
        // 700101000000Z
        { "170D3730303130313030303030305A", {} },
        // 491231235959Z, which is in 2049.
        { "170D3439313233313233353935395A", {} },
        // 500101000000Z, which is in 1950.
        { "170D3530303130313030303030305A", {} },
        // 20500101000000Z
        { "180F32303530303130313030303030305A", {} },
        // 20240229123456Z
        { "180F32303234303232393132333435365A", {} },
    };
    knowns[1].time.year = 2049;
    knowns[1].time.month = 12;
    knowns[1].time.day = 31;
    knowns[1].time.hour = 23;
    knowns[1].time.minute = 59;
    knowns[1].time.second = 59;
    knowns[2].time.year = 1950;
    knowns[3].time.year = 2050;
    knowns[4].time.year = 2024;
    knowns[4].time.month = 2;
    knowns[4].time.day = 29;
    knowns[4].time.hour = 12;
    knowns[4].time.minute = 34;
    knowns[4].time.second = 56;
    gtl::asn1::element element;
    for (const known_type& known : knowns) {
        const std::vector<unsigned char> bytes = hex_2_bytes(known.hex);
        REQUIRE(parse_whole(bytes, element) == true);
        gtl::asn1::time time;
        REQUIRE(gtl::asn1::decode_time(element, time) == true, "Failed to decode the time '%s'.", known.hex);
        REQUIRE(time.year == known.time.year);
        REQUIRE(time.month == known.time.month);
        REQUIRE(time.day == known.time.day);
        REQUIRE(time.hour == known.time.hour);
        REQUIRE(time.minute == known.time.minute);
        REQUIRE(time.second == known.time.second);
    }

    const char* invalid[] = {
        // No seconds: 7001010000Z.
        "170B373030313031303030305A",
        // A time zone offset in place of the Z: 700101000000+0100.
        "17113730303130313030303030302B30313030",
        // No Z: 7001010000000.
        "170D37303031303130303030303030",
        // A character that is not a digit: 70010100000aZ.
        "170D3730303130313030303030615A",
        // Month zero, month thirteen, day zero, and the thirtieth of february in a leap year.
        "170D3730303030313030303030305A",
        "170D3730313330313030303030305A",
        "170D3730303130303030303030305A",
        "180F32303234303233303030303030305A",
        // The twenty ninth of february in years that are not leap years, 2023 and 2100.
        "180F32303233303232393030303030305A",
        "180F32313030303232393030303030305A",
        // Hour twenty four, minute sixty, and second sixty.
        "170D3730303130313234303030305A",
        "170D3730303130313030363030305A",
        "170D3730303130313030303036305A",
        // A four digit year with the two digit tag, and a two digit year with the four digit tag.
        "170F32303234303232393132333435365A",
        "180D3730303130313030303030305A",
        // Fractions of a second: 20240229123456.5Z.
        "181132303234303232393132333435362E355A",
        // The year zero.
        "180F30303030303130313030303030305A",
        // The wrong tag.
        "0C0D3730303130313030303030305A",
        // Empty.
        "1700",
    };
    for (const char* hex : invalid) {
        const std::vector<unsigned char> bytes = hex_2_bytes(hex);
        REQUIRE(parse_whole(bytes, element) == true);
        gtl::asn1::time time;
        time.year = 1234;
        REQUIRE(gtl::asn1::decode_time(element, time) == false, "Decoded the invalid time '%s'.", hex);
        REQUIRE(time.year == 1234);
    }
}

TEST(asn1, function, writer) {
    unsigned char buffer[64] = {};
    gtl::asn1::writer writer(buffer, sizeof(buffer));
    REQUIRE(writer.is_valid() == true);
    REQUIRE(writer.get_length() == 0);

    const unsigned long long int sequence = writer.begin(gtl::asn1::tag_sequence);
    writer.write_boolean(true);
    writer.write_boolean(false);
    writer.write_null();
    writer.write_integer(0);
    writer.write_integer(127);
    writer.write_integer(128);
    writer.write_integer(0xFFFFFFFFFFFFFFFFull);
    const unsigned char bytes[3] = { 0xAA, 0xBB, 0xCC };
    writer.write_octet_string(bytes, 3);
    writer.write_bit_string(bytes, 3);
    writer.write_object_identifier(object_identifier_rsa, sizeof(object_identifier_rsa));
    writer.write_string(gtl::asn1::tag_utf8_string, "gtl");
    writer.write(gtl::asn1::tag_context_primitive(2), bytes, 2);
    writer.end(sequence);
    REQUIRE(writer.is_valid() == true);

    const std::vector<unsigned char> expected = hex_2_bytes("303C0101FF010100050002010002017F02020080020900FFFFFFFFFFFFFFFF0403AABBCC030400AABBCC06092A864886F70D0101010C0367746C8202AABB");
    REQUIRE(writer.get_length() == expected.size());
    REQUIRE(testbench::is_memory_same(buffer, expected.data(), expected.size()));
}

TEST(asn1, function, writer_time) {
    unsigned char buffer[64] = {};
    gtl::asn1::writer writer(buffer, sizeof(buffer));
    // Years from 1950 to 2049 are written with two digits, and other years with four.
    writer.write_time(gtl::asn1::time::from_seconds(-631152000ll));
    writer.write_time(gtl::asn1::time::from_seconds(2524607999ll));
    writer.write_time(gtl::asn1::time::from_seconds(2524608000ll));
    writer.write_time(gtl::asn1::time::from_seconds(-631152001ll));
    REQUIRE(writer.is_valid() == true);
    const std::vector<unsigned char> expected = hex_2_bytes(
        "170D3530303130313030303030305A"
        "170D3439313233313233353935395A"
        "180F32303530303130313030303030305A"
        "180F31393439313233313233353935395A"
    );
    REQUIRE(writer.get_length() == expected.size());
    REQUIRE(testbench::is_memory_same(buffer, expected.data(), expected.size()));

    // Every time that is written can be read back.
    for (const reference_time_type& reference : reference_times) {
        unsigned char time_buffer[32] = {};
        gtl::asn1::writer time_writer(time_buffer, sizeof(time_buffer));
        time_writer.write_time(gtl::asn1::time::from_seconds(reference.seconds));
        REQUIRE(time_writer.is_valid() == true);
        gtl::asn1::element element;
        REQUIRE(gtl::asn1::parse(time_buffer, time_writer.get_length(), element) == true);
        REQUIRE(element.encoded_length == time_writer.get_length());
        gtl::asn1::time time;
        REQUIRE(gtl::asn1::decode_time(element, time) == true);
        REQUIRE(time.to_seconds() == reference.seconds);
    }

    // A time that is not valid is not written.
    gtl::asn1::time invalid;
    invalid.month = 13;
    writer.write_time(invalid);
    REQUIRE(writer.is_valid() == false);
    REQUIRE(writer.get_length() == 0);
}

TEST(asn1, function, time) {
    for (const reference_time_type& reference : reference_times) {
        const gtl::asn1::time time = gtl::asn1::time::from_seconds(reference.seconds);
        REQUIRE(time.is_valid() == true);
        REQUIRE(time.year == reference.year, "Year mismatch for %lld seconds.", reference.seconds);
        REQUIRE(time.month == reference.month, "Month mismatch for %lld seconds.", reference.seconds);
        REQUIRE(time.day == reference.day, "Day mismatch for %lld seconds.", reference.seconds);
        REQUIRE(time.hour == reference.hour);
        REQUIRE(time.minute == reference.minute);
        REQUIRE(time.second == reference.second);
        REQUIRE(time.to_seconds() == reference.seconds, "Seconds mismatch for %lld seconds.", reference.seconds);
    }

    // Converting to a time and back gives the same number of seconds, and the days follow each other in order.
    gtl::asn1::time previous = gtl::asn1::time::from_seconds(-62135596800ll);
    for (long long int day = 1; day < 3652059; day += 1) {
        const long long int seconds = -62135596800ll + (day * 86400) + (day % 86400);
        const gtl::asn1::time time = gtl::asn1::time::from_seconds(seconds);
        REQUIRE_SILENT(time.is_valid() == true);
        REQUIRE_SILENT(time.to_seconds() == seconds);
        const bool next_day = (time.year == previous.year) && (time.month == previous.month) && (time.day == (previous.day + 1));
        const bool next_month = (time.year == previous.year) && (time.month == (previous.month + 1)) && (time.day == 1);
        const bool next_year = (time.year == (previous.year + 1)) && (time.month == 1) && (time.day == 1) && (previous.month == 12) && (previous.day == 31);
        REQUIRE_SILENT(next_day || next_month || next_year);
        previous = time;
    }
    REQUIRE(previous.year == 9999);
    REQUIRE(previous.month == 12);
    REQUIRE(previous.day == 31);

    // The fields of a time must be in range.
    gtl::asn1::time time;
    REQUIRE(time.is_valid() == true);
    time.year = 0;
    REQUIRE(time.is_valid() == false);
    time.year = 10000;
    REQUIRE(time.is_valid() == false);
    time.year = 2023;
    time.month = 2;
    time.day = 29;
    REQUIRE(time.is_valid() == false);
    time.year = 2024;
    REQUIRE(time.is_valid() == true);
    time.year = 1900;
    REQUIRE(time.is_valid() == false);
    time.year = 2000;
    REQUIRE(time.is_valid() == true);
    time.day = 30;
    REQUIRE(time.is_valid() == false);
    time.month = 4;
    REQUIRE(time.is_valid() == true);
    time.day = 31;
    REQUIRE(time.is_valid() == false);
    time.day = 0;
    REQUIRE(time.is_valid() == false);
    time.day = 1;
    time.month = 0;
    REQUIRE(time.is_valid() == false);
    time.month = 13;
    REQUIRE(time.is_valid() == false);
    time.month = 12;
    time.hour = 24;
    REQUIRE(time.is_valid() == false);
    time.hour = 23;
    time.minute = 60;
    REQUIRE(time.is_valid() == false);
    time.minute = 59;
    time.second = 60;
    REQUIRE(time.is_valid() == false);
    time.second = 59;
    REQUIRE(time.is_valid() == true);
}

TEST(asn1, evaluate, read_public_key) {
    const std::vector<unsigned char> bytes = hex_2_bytes(reference_public_key);
    const std::vector<unsigned char> modulus = hex_2_bytes(reference_modulus);
    gtl::asn1::reader reader(bytes.data(), bytes.size());
    gtl::asn1::element public_key;
    REQUIRE(reader.read(gtl::asn1::tag_sequence, public_key) == true);
    REQUIRE(reader.is_end() == true);
    REQUIRE(public_key.encoded_length == bytes.size());

    gtl::asn1::reader reader_public_key(public_key);
    gtl::asn1::element algorithm;
    gtl::asn1::element key;
    REQUIRE(reader_public_key.read(gtl::asn1::tag_sequence, algorithm) == true);
    REQUIRE(reader_public_key.read(gtl::asn1::tag_bit_string, key) == true);
    REQUIRE(reader_public_key.is_end() == true);

    gtl::asn1::reader reader_algorithm(algorithm);
    gtl::asn1::element element;
    REQUIRE(reader_algorithm.read(gtl::asn1::tag_object_identifier, element) == true);
    REQUIRE(element.is_content(object_identifier_rsa, sizeof(object_identifier_rsa)) == true);
    REQUIRE(reader_algorithm.read(gtl::asn1::tag_null, element) == true);
    REQUIRE(gtl::asn1::decode_null(element) == true);
    REQUIRE(reader_algorithm.is_end() == true);

    const unsigned char* key_bytes = nullptr;
    unsigned long long int key_length = 0;
    unsigned int unused_bits = 99;
    REQUIRE(gtl::asn1::decode_bit_string(key, key_bytes, key_length, unused_bits) == true);
    REQUIRE(unused_bits == 0);
    gtl::asn1::reader reader_key(key_bytes, key_length);
    gtl::asn1::element numbers;
    REQUIRE(reader_key.read(gtl::asn1::tag_sequence, numbers) == true);
    REQUIRE(reader_key.is_end() == true);

    gtl::asn1::reader reader_numbers(numbers);
    const unsigned char* modulus_bytes = nullptr;
    unsigned long long int modulus_length = 0;
    REQUIRE(reader_numbers.read(gtl::asn1::tag_integer, element) == true);
    REQUIRE(gtl::asn1::decode_unsigned(element, modulus_bytes, modulus_length) == true);
    REQUIRE(modulus_length == modulus.size());
    REQUIRE(testbench::is_memory_same(modulus_bytes, modulus.data(), modulus.size()));
    unsigned long long int exponent = 0;
    REQUIRE(reader_numbers.read(gtl::asn1::tag_integer, element) == true);
    REQUIRE(gtl::asn1::decode_integer(element, exponent) == true);
    REQUIRE(exponent == 65537);
    REQUIRE(reader_numbers.is_end() == true);

    unsigned int count = 0;
    REQUIRE(walk(bytes.data(), bytes.size(), 0, count) == true);
    REQUIRE(count == 5);
}

TEST(asn1, evaluate, write_public_key) {
    // Writing the same structure gives the same bytes as the other implementation.
    const std::vector<unsigned char> expected = hex_2_bytes(reference_public_key);
    const std::vector<unsigned char> modulus = hex_2_bytes(reference_modulus);
    unsigned char buffer[256] = {};
    gtl::asn1::writer writer(buffer, sizeof(buffer));
    const unsigned long long int public_key = writer.begin(gtl::asn1::tag_sequence);
    {
        const unsigned long long int algorithm = writer.begin(gtl::asn1::tag_sequence);
        writer.write_object_identifier(object_identifier_rsa, sizeof(object_identifier_rsa));
        writer.write_null();
        writer.end(algorithm);

        const unsigned long long int key = writer.begin(gtl::asn1::tag_bit_string);
        const unsigned char unused_bits[1] = { 0 };
        writer.write_encoded(unused_bits, 1);
        {
            const unsigned long long int numbers = writer.begin(gtl::asn1::tag_sequence);
            writer.write_unsigned(modulus.data(), modulus.size());
            writer.write_integer(65537);
            writer.end(numbers);
        }
        writer.end(key);
    }
    writer.end(public_key);
    REQUIRE(writer.is_valid() == true);
    REQUIRE(writer.get_length() == expected.size());
    REQUIRE(testbench::is_memory_same(buffer, expected.data(), expected.size()));
}

TEST(asn1, evaluate, signature) {
    const std::vector<unsigned char> expected = hex_2_bytes(reference_signature);
    const std::vector<unsigned char> r = hex_2_bytes(reference_signature_r);
    const std::vector<unsigned char> s = hex_2_bytes(reference_signature_s);

    // The first value has its top bit set so needs a leading zero, and the second does not.
    unsigned char buffer[128] = {};
    {
        gtl::asn1::writer writer(buffer, sizeof(buffer));
        const unsigned long long int sequence = writer.begin(gtl::asn1::tag_sequence);
        writer.write_unsigned(r.data(), r.size());
        writer.write_unsigned(s.data(), s.size());
        writer.end(sequence);
        REQUIRE(writer.is_valid() == true);
        REQUIRE(writer.get_length() == expected.size());
        REQUIRE(testbench::is_memory_same(buffer, expected.data(), expected.size()));
    }
    {
        gtl::asn1::reader reader(expected.data(), expected.size());
        gtl::asn1::element sequence;
        gtl::asn1::element element;
        REQUIRE(reader.read(gtl::asn1::tag_sequence, sequence) == true);
        gtl::asn1::reader reader_sequence(sequence);
        const unsigned char* bytes = nullptr;
        unsigned long long int length = 0;
        REQUIRE(reader_sequence.read(gtl::asn1::tag_integer, element) == true);
        REQUIRE(gtl::asn1::decode_unsigned(element, bytes, length) == true);
        REQUIRE(length == r.size());
        REQUIRE(testbench::is_memory_same(bytes, r.data(), r.size()));
        REQUIRE(reader_sequence.read(gtl::asn1::tag_integer, element) == true);
        REQUIRE(gtl::asn1::decode_unsigned(element, bytes, length) == true);
        REQUIRE(length == s.size());
        REQUIRE(testbench::is_memory_same(bytes, s.data(), s.size()));
        REQUIRE(reader_sequence.is_end() == true);
    }

    // Small values, written from buffers with leading zeros.
    {
        const std::vector<unsigned char> expected_small = hex_2_bytes(reference_signature_1_127);
        const unsigned char one[4] = { 0, 0, 0, 1 };
        const unsigned char value[4] = { 0, 0, 0, 127 };
        gtl::asn1::writer writer(buffer, sizeof(buffer));
        const unsigned long long int sequence = writer.begin(gtl::asn1::tag_sequence);
        writer.write_unsigned(one, 4);
        writer.write_unsigned(value, 4);
        writer.end(sequence);
        REQUIRE(writer.get_length() == expected_small.size());
        REQUIRE(testbench::is_memory_same(buffer, expected_small.data(), expected_small.size()));
    }
    {
        const std::vector<unsigned char> expected_small = hex_2_bytes(reference_signature_128_0);
        const unsigned char value[4] = { 0, 0, 0, 128 };
        const unsigned char zero[4] = { 0, 0, 0, 0 };
        gtl::asn1::writer writer(buffer, sizeof(buffer));
        const unsigned long long int sequence = writer.begin(gtl::asn1::tag_sequence);
        writer.write_unsigned(value, 4);
        writer.write_unsigned(zero, 4);
        writer.end(sequence);
        REQUIRE(writer.get_length() == expected_small.size());
        REQUIRE(testbench::is_memory_same(buffer, expected_small.data(), expected_small.size()));
        gtl::asn1::writer writer_empty(buffer, sizeof(buffer));
        writer_empty.write_unsigned(nullptr, 0);
        REQUIRE(writer_empty.get_length() == 3);
        REQUIRE(buffer[2] == 0x00);
    }
}

TEST(asn1, evaluate, writer_lengths) {
    // Contents either side of the lengths where the encoding of the length changes size, written as a value and as a nested element.
    const unsigned long long int lengths[9] = { 0, 1, 127, 128, 255, 256, 65535, 65536, 70000 };
    std::vector<unsigned char> content(70000, 0);
    for (unsigned long long int i = 0; i < content.size(); ++i) {
        content[i] = static_cast<unsigned char>((i * 31) + 7);
    }
    std::vector<unsigned char> buffer(70100, 0);
    for (const unsigned long long int length : lengths) {
        const unsigned long long int header_length = (length < 128) ? 2 : ((length < 256) ? 3 : ((length < 65536) ? 4 : 5));
        {
            gtl::asn1::writer writer(buffer.data(), buffer.size());
            writer.write_octet_string(content.data(), length);
            REQUIRE(writer.is_valid() == true);
            REQUIRE(writer.get_length() == (header_length + length));
            gtl::asn1::element element;
            REQUIRE(gtl::asn1::parse(buffer.data(), writer.get_length(), element) == true);
            REQUIRE(element.tag == gtl::asn1::tag_octet_string);
            REQUIRE(element.encoded_length == writer.get_length());
            REQUIRE(element.is_content(content.data(), length) == true);
        }
        {
            // The contents are written before the length is known, so they are moved to make room for a longer length.
            gtl::asn1::writer writer(buffer.data(), buffer.size());
            const unsigned long long int outer = writer.begin(gtl::asn1::tag_sequence);
            const unsigned long long int inner = writer.begin(gtl::asn1::tag_context_constructed(0));
            writer.write_encoded(content.data(), length);
            writer.end(inner);
            writer.write_integer(42);
            writer.end(outer);
            REQUIRE(writer.is_valid() == true);
            gtl::asn1::reader reader(buffer.data(), writer.get_length());
            gtl::asn1::element element_outer;
            gtl::asn1::element element_inner;
            gtl::asn1::element element_integer;
            REQUIRE(reader.read(gtl::asn1::tag_sequence, element_outer) == true);
            REQUIRE(reader.is_end() == true);
            gtl::asn1::reader reader_outer(element_outer);
            REQUIRE(reader_outer.read(gtl::asn1::tag_context_constructed(0), element_inner) == true);
            REQUIRE(element_inner.is_content(content.data(), length) == true);
            REQUIRE(element_inner.encoded_length == (header_length + length));
            unsigned long long int integer = 0;
            REQUIRE(reader_outer.read(gtl::asn1::tag_integer, element_integer) == true);
            REQUIRE(gtl::asn1::decode_integer(element_integer, integer) == true);
            REQUIRE(integer == 42);
            REQUIRE(reader_outer.is_end() == true);
        }
    }
}

TEST(asn1, evaluate, writer_overflow) {
    const unsigned char content[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    // A buffer that is exactly large enough.
    {
        unsigned char buffer[11];
        for (unsigned char& byte : buffer) {
            byte = 0xA5;
        }
        gtl::asn1::writer writer(buffer, 10);
        writer.write_octet_string(content, 8);
        REQUIRE(writer.is_valid() == true);
        REQUIRE(writer.get_length() == 10);
        REQUIRE(buffer[10] == 0xA5);
    }
    // Every buffer that is too small makes the writer invalid without writing past the end of the buffer.
    for (unsigned long long int capacity = 0; capacity < 10; ++capacity) {
        unsigned char buffer[11];
        for (unsigned char& byte : buffer) {
            byte = 0xA5;
        }
        gtl::asn1::writer writer(buffer, capacity);
        writer.write_octet_string(content, 8);
        REQUIRE(writer.is_valid() == false);
        REQUIRE(writer.get_length() == 0);
        for (unsigned long long int i = capacity; i < 11; ++i) {
            REQUIRE(buffer[i] == 0xA5);
        }
        // Once invalid, nothing more is written even if it would fit.
        writer.write_null();
        writer.write_integer(1);
        const unsigned long long int marker = writer.begin(gtl::asn1::tag_sequence);
        writer.end(marker);
        REQUIRE(writer.is_valid() == false);
        for (unsigned long long int i = capacity; i < 11; ++i) {
            REQUIRE(buffer[i] == 0xA5);
        }
    }
    // A nested element whose length needs more bytes than were reserved, in a buffer with no room to move the contents.
    {
        std::vector<unsigned char> buffer(2 + 128 + 1, 0xA5);
        std::vector<unsigned char> large(128, 0x11);
        gtl::asn1::writer writer(buffer.data(), 2 + 128);
        const unsigned long long int marker = writer.begin(gtl::asn1::tag_sequence);
        writer.write_encoded(large.data(), large.size());
        REQUIRE(writer.is_valid() == true);
        writer.end(marker);
        REQUIRE(writer.is_valid() == false);
        REQUIRE(buffer[2 + 128] == 0xA5);
    }
    // A null buffer, and ending an element that was not begun.
    {
        gtl::asn1::writer writer(nullptr, 100);
        writer.write_null();
        REQUIRE(writer.is_valid() == false);
        unsigned char buffer[8] = {};
        gtl::asn1::writer writer_end(buffer, sizeof(buffer));
        writer_end.end(4);
        REQUIRE(writer_end.is_valid() == false);
    }
}

TEST(asn1, evaluate, corrupted) {
    // Reading every element of corrupted data either fails or succeeds, without reading outside the data.
    const std::vector<unsigned char> original = hex_2_bytes(reference_public_key);
    unsigned int count = 0;
    REQUIRE(walk(original.data(), original.size(), 0, count) == true);

    // Every truncation fails.
    for (unsigned long long int length = 1; length < original.size(); ++length) {
        // The data is copied so that tools that detect out of bounds reads see the exact length.
        const std::vector<unsigned char> truncated(original.begin(), original.begin() + static_cast<long long int>(length));
        count = 0;
        REQUIRE(walk(truncated.data(), truncated.size(), 0, count) == false);
    }

    // Every single byte change, to a few values.
    unsigned int failures = 0;
    const unsigned char values[6] = { 0x00, 0x01, 0x7F, 0x80, 0x84, 0xFF };
    for (unsigned long long int index = 0; index < original.size(); ++index) {
        for (const unsigned char value : values) {
            std::vector<unsigned char> changed = original;
            changed[index] = value;
            count = 0;
            if (!walk(changed.data(), changed.size(), 0, count)) {
                ++failures;
            }
        }
    }
    REQUIRE(failures > 0);

    // Pseudo random data.
    unsigned long long int state = 0x853C49E6748FEA9Bull;
    for (unsigned int iteration = 0; iteration < 20000; ++iteration) {
        std::vector<unsigned char> random(1 + (iteration % 40), 0);
        for (unsigned char& byte : random) {
            state = (state * 0x5851F42D4C957F2Dull) + 0xDA3E39CB94B95BDBull;
            byte = static_cast<unsigned char>(state >> 56);
            // Bias the data towards small lengths and constructed tags so that more of it is well formed.
            if ((state & 0x300) == 0) {
                byte &= 0x0F;
            }
            if ((state & 0xC00) == 0) {
                byte = gtl::asn1::tag_sequence;
            }
        }
        count = 0;
        static_cast<void>(walk(random.data(), random.size(), 0, count));
    }
}
