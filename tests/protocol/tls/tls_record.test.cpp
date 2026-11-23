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

#include <protocol/tls/tls_record>

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
            return static_cast<unsigned char>((character >= 'a') ? (character - 'a' + 10) : ((character >= 'A') ? (character - 'A' + 10) : (character - '0')));
        };
        std::vector<unsigned char> bytes;
        for (unsigned int i = 0; (hex[i] != 0) && (hex[i + 1] != 0); i += 2) {
            bytes.push_back(static_cast<unsigned char>((nibble(hex[i]) << 4) | nibble(hex[i + 1])));
        }
        return bytes;
    }

    /// @brief  Generate a buffer where each byte is its index times a multiplier plus an offset.
    std::vector<unsigned char> pattern(const unsigned int length, const unsigned int multiplier, const unsigned int offset) {
        std::vector<unsigned char> bytes;
        for (unsigned int i = 0; i < length; ++i) {
            bytes.push_back(static_cast<unsigned char>(((i * multiplier) + offset) & 0xFF));
        }
        return bytes;
    }

    /// @brief  Get a pointer to the data of a buffer that is valid to pass even when the buffer is empty.
    const unsigned char* data_of(const std::vector<unsigned char>& bytes) {
        static const unsigned char empty[1] = {};
        return bytes.empty() ? empty : bytes.data();
    }

    // Records built with the ciphers of another implementation, five consecutive records for each cipher and version.
    // The key is pattern(key_size, 7, 3), the initialisation vector is pattern(iv_size, 11, 5), and each fragment is pattern(length, 13, 17 + sequence).
    struct reference_type {
        gtl::tls_record::version_type version;
        gtl::tls_record::cipher_type cipher;
        unsigned long long int sequence;
        unsigned char content_type;
        unsigned int length;
        const char* record;
    };

    constexpr const reference_type references[] = {
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, 0, 22, 0, "1703030011F98E6A841E7976462BE64E0A56D1D5BDD3" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, 1, 22, 1, "17030300123BFF8A3D599389158F03F8D00E98E7BBC25F" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, 2, 23, 17, "17030300220C6A90ADFE10A466605B9324CB2ED30C644CCE8E2FF813B6A60B371EDB61F7603FC5" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, 3, 21, 2, "1703030013DE9D2B8C39D211B5C9730E54352F17E7CC5CF3" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, 4, 23, 300, "170303013DE6E8A9B92522C0F93338D8D2B953A0AF4AC27725DF68892A9DEE6ECC11B6AF7C472AEB99F0B4CEE1D3513A0B6D8C880FCC0F0DFA8E02D788C229E92CF3B4358ADE8885B3F34B9E9C8F5FB347B70326BC100FFF4B4DDF2ACCA593544EBCB309A110BF07ECFCA47634D25512FAC912BEF5A4E4C03B275297595DBB01C4F98C4AB78A3A8A5C80B1765EAB91FC1C76A60868CD10464343916222E627345B5EAE8F858008147E029E13AE3AACE507E4D82D433D394C7A35E6DD34031DD6E868DBC5B0D774737F8ECF3186365D8A6B14D6C033975B90549B00884CDBACF8CBE526589B11860F6CF49CFD1E93013469856367AECC39BC7682C6F785698D3E0C20FC9FBD3B3B3918BDAED165C0D9B454B75A4BB14B8DE15C27F2D177C18B913A634A0D878B9FD9D425707D5A39FFAEA7038046200F8A57FB34ECE3E9F0F508F9C8" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_128_gcm, 0, 22, 0, "16030300180000000000000000E68B550C048FBA212CCD55EC4E511033" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_128_gcm, 1, 22, 1, "16030300190000000000000001D77733F20B650D4159F7F86A0D29870244" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_128_gcm, 2, 23, 17, "17030300290000000000000002D3B3FF06C104A8FEC7BCA6C52131164163D85A3500F974AFACE2E045CD08E10EEC" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_128_gcm, 3, 21, 2, "150303001A0000000000000003DD2C007DA45BFED114A1442C2976939799EC" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_128_gcm, 4, 23, 300, "17030301440000000000000004AAE65CC75FE009452925C9FDD7B41E2414FB381539D823EFA74252D15A76853142242F9C30EA910E50E8B495A76DEB85D5B6BD2F15605947E7D67AF51CDFB94E325D0D54FF7FC2177F4C536C7CD1509CD70811F76B58D32A94C8DDFF3575F0ED53A4D6336384C0757D56BD07BE5FEBDA6D8FE1722996E9117EF791A9469195846D1724037B4B10033F31D6C68A13390E6DF54D1FC79625DF65CE475E2575EBDD500574A00E394107E26A007EEEE071320ABAFAAC8A7C29A3FE5E1CACB54B764A023F8B9367AB4F0AC70501A4773CE8B48599BA7BE314B12E6FBE02EBE274B19CAD79C8D9A17E7DAE67FAF00253EEFF41352411B175CBD7C2D0F25597C387522603CB028ED28E222FC0C5D7E7567AA4B176844DE3A6C1FB0FFE83E61CB0630DAF0D2DDCAAB41346F80D9E5D9FC67035B51148CEE683848BCBF4777BFD" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_256_gcm, 0, 22, 0, "170303001186BE323E566314E38CCCEDC07E5A735DCD" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_256_gcm, 1, 22, 1, "1703030012D64A659CF136E8DF7B0F0315EEFCD68565C4" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_256_gcm, 2, 23, 17, "1703030022017A608EE15F4602838B577FDBEDE6EC86E9B69BCD85A2CB17D1B10641562F42A22A" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_256_gcm, 3, 21, 2, "1703030013F3E750CF1314DB14D005996BCE1981504D7549" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_256_gcm, 4, 23, 300, "170303013DDCD01FDD741489D4B89B6C00D5EB20E0EDDDF25421315ED85F49BF6B192EC0A1D1AFFAE67BFCFA23056D3BEE2179138E8D3AB65D9F4AC3703FEF367F1751BE3C032ADF1F49FED4BB2999D1FAE6F317415591BD15921F7074429102F6B64A46D4D652807B54EDA1F973F2220212A658F656A7F263CE49E7BAC1100068D2E77118D37B74872B07519E81B9B5119AA1D93E5960D63D7E5328064FFCB49FF2B41FAC8A81F8357648153D444BEEAFAE1D24036F1A1AE008BF7C29EFA32D137A24C288FDA28EAFC68D85949162FACCBD2EDA909CE37C22FE949118463C70BDF95588B8A3440DD234311B70514B64BCA75A80140EAF270DE85B81966E5705BB074171FE9695533C20127B5DB4635354026C4EEBAB80C45C7B3F7DF368659D126E18A1A95F8D19354D0E296DADA65485AA4C16B4D64A10EE9A3886E4E015A263B9" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_256_gcm, 0, 22, 0, "160303001800000000000000005068080ED4A7FE403BEB7C16F88A2BD2" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_256_gcm, 1, 22, 1, "160303001900000000000000013F08F477FF621314480A6D4BD9428A7670" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_256_gcm, 2, 23, 17, "17030300290000000000000002B2E7A56DBCCE8C5C8F568649696E0BBB63630EA4921B59B99E080CE449B8EE079D" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_256_gcm, 3, 21, 2, "150303001A0000000000000003A67DACD35DDC247188F059C83C024F0B1F20" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::aes_256_gcm, 4, 23, 300, "17030301440000000000000004F49C146EB310B1CE6CB4D6F0E57FB1BFA9AF7708743560B0EBF553C3BBBD778887167DA4B79FBEEBA6B9D2E4BEC39B01F5CADBF9391586A50009D1B5ED4AD6A9757F78DF7C1145E8AFB7836AB9327CBFF74294880AD45BD0C8879795FBB25B6FE29FA280383111DA004D0057A6CAFB2BC4726D9817E5C92C27C1CC532CAB8D407ADC5ACE7F5A9D84FB9DFE3E0B325D8D342574B0B4180D98AA51A016F41EED36FD833DBFF73631F7B2FC4FB2E555A64C943E0DA791C3E5CADA7107CCE634D3D1096A5CBBAF2E8F334E7716FF78953946F180D6B0E393763A5041B1F244B63679033F2189F530FD994E0317590DB950635DF383BCF9AF8FBDD6A44E2CC03795D6E485E7A388D54C47400E1A440BA3E288ED4320ABF76C19FDE1EFA5A6F41E1ECFC14619561DEBD55863140DF9584F6FFF49B679994CBCE3DDDC969010" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::chacha20_poly1305, 0, 22, 0, "170303001120BC980C66C6D00E949A4115FF627579C1" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::chacha20_poly1305, 1, 22, 1, "17030300123076CD67D83553E74F4645589EABEEBF1D28" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::chacha20_poly1305, 2, 23, 17, "1703030022B14E228BAA317669C96928DB06CE8C6CF0D8D3D8D3C85A8C964FFC4FDF048ABBA55E" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::chacha20_poly1305, 3, 21, 2, "1703030013020A29C23025458685B2311A4F7B932DC00C9B" },
        { gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::chacha20_poly1305, 4, 23, 300, "170303013DC4C0EEC6071A99B710BC3DF401187D0CD7200A79875C92859243E50E5555BB1CC19AC5AB2645F3719D1228E3289DE8A86BA06CB2AACF3C4553EE1C0B40DF6E1F7CB956C2FD9AAA7AA04E6EAD788BEF8C861402D1BBCF9986BACE6C0864628B469FE4D8940E4FE9539424CFFDAF4418A80EE12CA4E69FD91E81A62724DCC0B4A01446A5168F5F4949040F8986C4D5FA2AFD298B1AD488D5866551FAC862534C715612AA006E07019AC60A6980336823939280328E27BF22F0359D805A39622B55A47D22CE9F7078B2640ABEA1A5A2D9018FC328B2F80D4109807A39E00F97BE77F29CEC0EEB51F2BE6E43505078E010EAA199F0B9A516F268408EF8589D03C7C7B8280DBDA545E88ABBBD08EA7546AD64D8564CDD6301B143A60CF42EF267C319E76D4DA6369F3C099D5DD9E7426E987998A1088DF06530BB7C3A2563E2" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::chacha20_poly1305, 0, 22, 0, "1603030010B10D2FE84B260F7386F186C3EEA61E99" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::chacha20_poly1305, 1, 22, 1, "160303001130293513770A4BCD5886CD83161D9B8E07" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::chacha20_poly1305, 2, 23, 17, "1703030021B14E228BAA317669C96928DB06CE8C6CF09A524E0557AFE79C5BC5AEFECA504A33" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::chacha20_poly1305, 3, 21, 2, "1503030012020A5159320A0DD1D940A2ED691947C612F4" },
        { gtl::tls_record::version_type::tls12, gtl::tls_record::cipher_type::chacha20_poly1305, 4, 23, 300, "170303013CC4C0EEC6071A99B710BC3DF401187D0CD7200A79875C92859243E50E5555BB1CC19AC5AB2645F3719D1228E3289DE8A86BA06CB2AACF3C4553EE1C0B40DF6E1F7CB956C2FD9AAA7AA04E6EAD788BEF8C861402D1BBCF9986BACE6C0864628B469FE4D8940E4FE9539424CFFDAF4418A80EE12CA4E69FD91E81A62724DCC0B4A01446A5168F5F4949040F8986C4D5FA2AFD298B1AD488D5866551FAC862534C715612AA006E07019AC60A6980336823939280328E27BF22F0359D805A39622B55A47D22CE9F7078B2640ABEA1A5A2D9018FC328B2F80D4109807A39E00F97BE77F29CEC0EEB51F2BE6E43505078E010EAA199F0B9A516F268408EF8589D03C7C7B8280DBDA545E88ABBBD08EA7546AD64D8564CDD6301B143A60CF42EF267C319E76D4DA6369F3C099D5DD9E7F86E78E0472795D6122F0CABD71ACD53" },
    };

    constexpr const gtl::tls_record::cipher_type ciphers[3] = {
        gtl::tls_record::cipher_type::aes_128_gcm,
        gtl::tls_record::cipher_type::aes_256_gcm,
        gtl::tls_record::cipher_type::chacha20_poly1305
    };

    constexpr const gtl::tls_record::version_type versions[2] = {
        gtl::tls_record::version_type::tls12,
        gtl::tls_record::version_type::tls13
    };

    /// @brief  Set the reference keys for a cipher and version.
    bool set_reference_keys(gtl::tls_record& record, const gtl::tls_record::version_type version, const gtl::tls_record::cipher_type cipher) {
        const std::vector<unsigned char> key = pattern(gtl::tls_record::get_key_size(cipher), 7, 3);
        const std::vector<unsigned char> iv = pattern(gtl::tls_record::get_iv_size(version, cipher), 11, 5);
        return record.set_keys(version, cipher, key.data(), iv.data());
    }
}

TEST(tls_record, traits, standard) {
    REQUIRE((std::is_pod<gtl::tls_record>::value == false));

    REQUIRE((std::is_trivial<gtl::tls_record>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::tls_record>::value == false));

    REQUIRE((std::is_standard_layout<gtl::tls_record>::value == true));
}

TEST(tls_record, constructor, empty) {
    gtl::tls_record record;
    REQUIRE(record.is_protected() == false);
    REQUIRE(record.get_sequence() == 0);
    REQUIRE(record.get_record_size(0) == gtl::tls_record::header_size);
    REQUIRE(record.get_record_size(100) == gtl::tls_record::header_size + 100);
}

TEST(tls_record, parameter, sizes) {
    REQUIRE(gtl::tls_record::content_change_cipher_spec == 20);
    REQUIRE(gtl::tls_record::content_alert == 21);
    REQUIRE(gtl::tls_record::content_handshake == 22);
    REQUIRE(gtl::tls_record::content_application_data == 23);
    REQUIRE(gtl::tls_record::header_size == 5);
    REQUIRE(gtl::tls_record::fragment_size_max == 16384);

    using cipher_type = gtl::tls_record::cipher_type;
    using version_type = gtl::tls_record::version_type;
    REQUIRE(gtl::tls_record::get_key_size(cipher_type::none) == 0);
    REQUIRE(gtl::tls_record::get_key_size(cipher_type::aes_128_gcm) == 16);
    REQUIRE(gtl::tls_record::get_key_size(cipher_type::aes_256_gcm) == 32);
    REQUIRE(gtl::tls_record::get_key_size(cipher_type::chacha20_poly1305) == 32);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls12, cipher_type::none) == 0);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls13, cipher_type::none) == 0);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls12, cipher_type::aes_128_gcm) == 4);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls12, cipher_type::aes_256_gcm) == 4);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls12, cipher_type::chacha20_poly1305) == 12);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls13, cipher_type::aes_128_gcm) == 12);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls13, cipher_type::aes_256_gcm) == 12);
    REQUIRE(gtl::tls_record::get_iv_size(version_type::tls13, cipher_type::chacha20_poly1305) == 12);
    for (const cipher_type cipher : ciphers) {
        REQUIRE(gtl::tls_record::get_key_size(cipher) <= gtl::tls_record::key_size_max);
        for (const version_type version : versions) {
            REQUIRE(gtl::tls_record::get_iv_size(version, cipher) <= gtl::tls_record::iv_size_max);
            gtl::tls_record record;
            REQUIRE(set_reference_keys(record, version, cipher) == true);
            REQUIRE(record.get_record_size(0) <= (gtl::tls_record::header_size + gtl::tls_record::overhead_size_max));
            REQUIRE(record.get_record_size(gtl::tls_record::fragment_size_max) <= (gtl::tls_record::header_size + gtl::tls_record::body_size_max));
        }
    }
}

TEST(tls_record, function, parse_header) {
    unsigned char content_type = 0;
    unsigned int body_length = 0;
    {
        const unsigned char header[5] = { 22, 3, 3, 0x01, 0x02 };
        REQUIRE(gtl::tls_record::parse_header(header, content_type, body_length) == true);
        REQUIRE(content_type == 22);
        REQUIRE(body_length == 0x0102);
    }
    {
        // The first record of a connection may have an older minor version.
        const unsigned char header[5] = { 22, 3, 1, 0x00, 0x00 };
        REQUIRE(gtl::tls_record::parse_header(header, content_type, body_length) == true);
        REQUIRE(body_length == 0);
    }
    {
        const unsigned char header[5] = { 23, 3, 3, 0x41, 0x00 };
        REQUIRE(gtl::tls_record::parse_header(header, content_type, body_length) == true);
        REQUIRE(content_type == 23);
        REQUIRE(body_length == gtl::tls_record::body_size_max);
    }
    content_type = 77;
    body_length = 1234;
    {
        // A body that is too long.
        const unsigned char header[5] = { 23, 3, 3, 0x41, 0x01 };
        REQUIRE(gtl::tls_record::parse_header(header, content_type, body_length) == false);
    }
    {
        // Content types that do not exist.
        const unsigned char header_low[5] = { 19, 3, 3, 0x00, 0x10 };
        REQUIRE(gtl::tls_record::parse_header(header_low, content_type, body_length) == false);
        const unsigned char header_high[5] = { 24, 3, 3, 0x00, 0x10 };
        REQUIRE(gtl::tls_record::parse_header(header_high, content_type, body_length) == false);
    }
    {
        // Not the protocol at all, for example the start of an unencrypted request.
        const unsigned char header_version[5] = { 22, 2, 0, 0x00, 0x10 };
        REQUIRE(gtl::tls_record::parse_header(header_version, content_type, body_length) == false);
        const unsigned char header_text[5] = { 'G', 'E', 'T', ' ', '/' };
        REQUIRE(gtl::tls_record::parse_header(header_text, content_type, body_length) == false);
    }
    REQUIRE(content_type == 77);
    REQUIRE(body_length == 1234);
}

TEST(tls_record, function, set_keys) {
    const std::vector<unsigned char> key = pattern(32, 7, 3);
    const std::vector<unsigned char> iv = pattern(12, 11, 5);
    gtl::tls_record record;
    REQUIRE(record.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::none, key.data(), iv.data()) == false);
    REQUIRE(record.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, nullptr, iv.data()) == false);
    REQUIRE(record.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, key.data(), nullptr) == false);
    REQUIRE(record.is_protected() == false);
    REQUIRE(record.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, key.data(), iv.data()) == true);
    REQUIRE(record.is_protected() == true);
    REQUIRE(record.get_sequence() == 0);

    // Setting keys restarts the sequence number, and resetting removes the protection.
    unsigned char output[64] = {};
    unsigned long long int output_length = 0;
    REQUIRE(record.protect(gtl::tls_record::content_handshake, key.data(), 4, output, sizeof(output), output_length) == true);
    REQUIRE(record.get_sequence() == 1);
    REQUIRE(record.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::chacha20_poly1305, key.data(), iv.data()) == true);
    REQUIRE(record.get_sequence() == 0);
    REQUIRE(record.protect(gtl::tls_record::content_handshake, key.data(), 4, output, sizeof(output), output_length) == true);
    record.reset();
    REQUIRE(record.is_protected() == false);
    REQUIRE(record.get_sequence() == 0);
}

TEST(tls_record, function, protect_unprotected) {
    // Without keys a record is the header followed by the fragment.
    gtl::tls_record record;
    const std::vector<unsigned char> fragment = pattern(100, 3, 1);
    unsigned char output[128];
    for (unsigned char& byte : output) {
        byte = 0xA5;
    }
    unsigned long long int output_length = 0;
    REQUIRE(record.protect(gtl::tls_record::content_handshake, fragment.data(), 100, output, 105, output_length) == true);
    REQUIRE(output_length == 105);
    REQUIRE(output[0] == 22);
    REQUIRE(output[1] == 3);
    REQUIRE(output[2] == 3);
    REQUIRE(output[3] == 0);
    REQUIRE(output[4] == 100);
    REQUIRE(testbench::is_memory_same(&output[5], fragment.data(), 100));
    REQUIRE(output[105] == 0xA5);
    // The sequence number only counts protected records.
    REQUIRE(record.get_sequence() == 0);

    unsigned char recovered[128] = {};
    unsigned int recovered_length = 0;
    unsigned char content_type = 0;
    REQUIRE(record.unprotect(output[0], &output[5], 100, recovered, recovered_length, content_type) == true);
    REQUIRE(recovered_length == 100);
    REQUIRE(content_type == 22);
    REQUIRE(testbench::is_memory_same(recovered, fragment.data(), 100));

    // An empty fragment, and a buffer that is too small.
    REQUIRE(record.protect(gtl::tls_record::content_application_data, nullptr, 0, output, sizeof(output), output_length) == true);
    REQUIRE(output_length == 5);
    output_length = 1234;
    REQUIRE(record.protect(gtl::tls_record::content_handshake, fragment.data(), 100, output, 104, output_length) == false);
    REQUIRE(record.protect(gtl::tls_record::content_handshake, nullptr, 100, output, sizeof(output), output_length) == false);
    REQUIRE(record.protect(gtl::tls_record::content_handshake, fragment.data(), 100, nullptr, sizeof(output), output_length) == false);
    REQUIRE(output_length == 1234);
}

TEST(tls_record, evaluate, rfc_8448) {
    // The client application data record of the simple handshake of RFC 8448 section 3.
    const std::vector<unsigned char> key = hex_2_bytes("17422dda596ed5d9acd890e3c63f5051");
    const std::vector<unsigned char> iv = hex_2_bytes("5b78923dee08579033e523d9");
    const std::vector<unsigned char> expected = hex_2_bytes("1703030043a23f7054b62c94d0affafe8228ba55cbefacea42f914aa66bcab3f2b9819a8a5b46b395bd54a9a20441e2b62974e1f5a6292a2977014bd1e3deae63aeebb21694915e4");
    std::vector<unsigned char> fragment;
    for (unsigned char i = 0; i < 50; ++i) {
        fragment.push_back(i);
    }

    gtl::tls_record record_send;
    REQUIRE(record_send.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, key.data(), iv.data()) == true);
    REQUIRE(record_send.get_record_size(50) == expected.size());
    unsigned char output[128] = {};
    unsigned long long int output_length = 0;
    REQUIRE(record_send.protect(gtl::tls_record::content_application_data, fragment.data(), 50, output, sizeof(output), output_length) == true);
    REQUIRE(output_length == expected.size());
    REQUIRE(testbench::is_memory_same(output, expected.data(), expected.size()));

    gtl::tls_record record_receive;
    REQUIRE(record_receive.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, key.data(), iv.data()) == true);
    unsigned char recovered[128] = {};
    unsigned int recovered_length = 0;
    unsigned char content_type = 0;
    REQUIRE(record_receive.unprotect(expected[0], &expected[5], static_cast<unsigned int>(expected.size() - 5), recovered, recovered_length, content_type) == true);
    REQUIRE(content_type == gtl::tls_record::content_application_data);
    REQUIRE(recovered_length == 50);
    REQUIRE(testbench::is_memory_same(recovered, fragment.data(), 50));
}

TEST(tls_record, evaluate, reference) {
    gtl::tls_record record_send;
    gtl::tls_record record_receive;
    for (const reference_type& reference : references) {
        if (reference.sequence == 0) {
            REQUIRE(set_reference_keys(record_send, reference.version, reference.cipher) == true);
            REQUIRE(set_reference_keys(record_receive, reference.version, reference.cipher) == true);
        }
        REQUIRE(record_send.get_sequence() == reference.sequence);
        REQUIRE(record_receive.get_sequence() == reference.sequence);
        const std::vector<unsigned char> fragment = pattern(reference.length, 13, 17 + static_cast<unsigned int>(reference.sequence));
        const std::vector<unsigned char> expected = hex_2_bytes(reference.record);
        REQUIRE(record_send.get_record_size(reference.length) == expected.size());

        std::vector<unsigned char> output(expected.size() + 1, 0xA5);
        unsigned long long int output_length = 0;
        REQUIRE(record_send.protect(reference.content_type, data_of(fragment), reference.length, output.data(), expected.size(), output_length) == true);
        REQUIRE(output_length == expected.size());
        REQUIRE(testbench::is_memory_same(output.data(), expected.data(), expected.size()), "Record mismatch for sequence %llu.", reference.sequence);
        REQUIRE(output[expected.size()] == 0xA5);

        unsigned char header_type = 0;
        unsigned int body_length = 0;
        REQUIRE(gtl::tls_record::parse_header(expected.data(), header_type, body_length) == true);
        REQUIRE(body_length == (expected.size() - gtl::tls_record::header_size));
        std::vector<unsigned char> recovered(body_length + 1, 0xA5);
        unsigned int recovered_length = 0;
        unsigned char content_type = 0;
        REQUIRE(record_receive.unprotect(header_type, &expected[gtl::tls_record::header_size], body_length, recovered.data(), recovered_length, content_type) == true);
        REQUIRE(content_type == reference.content_type);
        REQUIRE(recovered_length == reference.length);
        REQUIRE(testbench::is_memory_same(recovered.data(), data_of(fragment), reference.length));
        REQUIRE(recovered[body_length] == 0xA5);
    }
}

TEST(tls_record, evaluate, round_trip) {
    const unsigned int lengths[9] = { 0, 1, 15, 16, 17, 255, 1000, gtl::tls_record::fragment_size_max - 1, gtl::tls_record::fragment_size_max };
    const unsigned char content_types[3] = { gtl::tls_record::content_alert, gtl::tls_record::content_handshake, gtl::tls_record::content_application_data };
    for (const gtl::tls_record::version_type version : versions) {
        for (const gtl::tls_record::cipher_type cipher : ciphers) {
            gtl::tls_record record_send;
            gtl::tls_record record_receive;
            REQUIRE(set_reference_keys(record_send, version, cipher) == true);
            REQUIRE(set_reference_keys(record_receive, version, cipher) == true);
            unsigned int index = 0;
            for (const unsigned int length : lengths) {
                const unsigned char content_type = content_types[index++ % 3];
                const std::vector<unsigned char> fragment = pattern(length, 5, index);
                std::vector<unsigned char> output(record_send.get_record_size(length) + 1, 0xA5);
                unsigned long long int output_length = 0;
                REQUIRE(record_send.protect(content_type, data_of(fragment), length, output.data(), output.size() - 1, output_length) == true);
                REQUIRE(output_length == (output.size() - 1));
                REQUIRE(output[output_length] == 0xA5);
                // A buffer that is one byte too small is refused, without using up a sequence number.
                unsigned long long int small_length = 0;
                std::vector<unsigned char> small(output.size(), 0xA5);
                REQUIRE(record_send.protect(content_type, data_of(fragment), length, small.data(), output_length - 1, small_length) == false);
                REQUIRE(small[0] == 0xA5);

                unsigned char header_type = 0;
                unsigned int body_length = 0;
                REQUIRE(gtl::tls_record::parse_header(output.data(), header_type, body_length) == true);
                // The fragment is recovered in place.
                unsigned int recovered_length = 0;
                unsigned char recovered_type = 0;
                REQUIRE(record_receive.unprotect(header_type, &output[5], body_length, &output[5], recovered_length, recovered_type) == true);
                REQUIRE(recovered_type == content_type);
                REQUIRE(recovered_length == length);
                REQUIRE(testbench::is_memory_same(&output[5], data_of(fragment), length));
            }
            REQUIRE(record_send.get_sequence() == 9);
            REQUIRE(record_receive.get_sequence() == 9);

            // A fragment that is too long is refused.
            std::vector<unsigned char> large(gtl::tls_record::fragment_size_max + 1, 0x11);
            std::vector<unsigned char> output(gtl::tls_record::fragment_size_max + 100, 0);
            unsigned long long int output_length = 0;
            REQUIRE(record_send.protect(gtl::tls_record::content_application_data, large.data(), gtl::tls_record::fragment_size_max + 1, output.data(), output.size(), output_length) == false);
            REQUIRE(record_send.get_sequence() == 9);
        }
    }
}

TEST(tls_record, evaluate, authenticate) {
    for (const gtl::tls_record::version_type version : versions) {
        for (const gtl::tls_record::cipher_type cipher : ciphers) {
            gtl::tls_record record_send;
            REQUIRE(set_reference_keys(record_send, version, cipher) == true);
            const std::vector<unsigned char> fragment = pattern(40, 9, 2);
            std::vector<unsigned char> record(record_send.get_record_size(40), 0);
            unsigned long long int record_length = 0;
            // Use the second record, so that the sequence number is not zero.
            REQUIRE(record_send.protect(gtl::tls_record::content_handshake, fragment.data(), 40, record.data(), record.size(), record_length) == true);
            REQUIRE(record_send.protect(gtl::tls_record::content_handshake, fragment.data(), 40, record.data(), record.size(), record_length) == true);
            const unsigned int body_length = static_cast<unsigned int>(record_length) - 5;

            const auto receive = [&](const std::vector<unsigned char>& received, const unsigned char header_type, const unsigned int received_body_length, const unsigned int records_before) -> bool {
                gtl::tls_record record_receive;
                REQUIRE(set_reference_keys(record_receive, version, cipher) == true);
                // Move the sequence number forward by receiving records made with the same keys.
                gtl::tls_record record_other;
                REQUIRE(set_reference_keys(record_other, version, cipher) == true);
                for (unsigned int i = 0; i < records_before; ++i) {
                    std::vector<unsigned char> other(record_other.get_record_size(40), 0);
                    unsigned long long int other_length = 0;
                    REQUIRE(record_other.protect(gtl::tls_record::content_handshake, fragment.data(), 40, other.data(), other.size(), other_length) == true);
                    unsigned int ignored_length = 0;
                    unsigned char ignored_type = 0;
                    REQUIRE(record_receive.unprotect(other[0], &other[5], static_cast<unsigned int>(other_length) - 5, &other[5], ignored_length, ignored_type) == true);
                }
                std::vector<unsigned char> output(received.size() + 1, 0);
                unsigned int output_length = 0;
                unsigned char content_type = 0;
                const bool result = record_receive.unprotect(header_type, received.data() + 5, received_body_length, output.data(), output_length, content_type);
                // A record that is refused does not use up a sequence number.
                REQUIRE(record_receive.get_sequence() == (records_before + (result ? 1u : 0u)));
                return result && (output_length == 40) && testbench::is_memory_same(output.data(), fragment.data(), 40);
            };

            REQUIRE(receive(record, record[0], body_length, 1) == true);
            // A record that arrives out of order, or is repeated, is refused.
            REQUIRE(receive(record, record[0], body_length, 0) == false);
            REQUIRE(receive(record, record[0], body_length, 2) == false);
            // Changing any bit of the body is detected.
            for (unsigned int bit = 0; bit < (body_length * 8); bit += 3) {
                std::vector<unsigned char> changed = record;
                changed[5 + (bit / 8)] ^= static_cast<unsigned char>(1 << (bit % 8));
                REQUIRE_SILENT(receive(changed, changed[0], body_length, 1) == false);
            }
            // A truncated body, including one that is too short to hold a tag.
            for (unsigned int length = 0; length < body_length; ++length) {
                REQUIRE_SILENT(receive(record, record[0], length, 1) == false);
            }
            // A different content type in the header.
            REQUIRE(receive(record, gtl::tls_record::content_alert, body_length, 1) == false);
            if (version == gtl::tls_record::version_type::tls13) {
                REQUIRE(record[0] == gtl::tls_record::content_application_data);
                REQUIRE(receive(record, gtl::tls_record::content_handshake, body_length, 1) == false);
            }
            else {
                REQUIRE(record[0] == gtl::tls_record::content_handshake);
                REQUIRE(receive(record, gtl::tls_record::content_application_data, body_length, 1) == false);
            }
            // Different keys.
            {
                gtl::tls_record record_receive;
                const std::vector<unsigned char> key = pattern(gtl::tls_record::get_key_size(cipher), 7, 4);
                const std::vector<unsigned char> iv = pattern(gtl::tls_record::get_iv_size(version, cipher), 11, 5);
                REQUIRE(record_receive.set_keys(version, cipher, key.data(), iv.data()) == true);
                std::vector<unsigned char> output(record.size(), 0);
                unsigned int output_length = 0;
                unsigned char content_type = 0;
                REQUIRE(record_receive.unprotect(record[0], &record[5], body_length, output.data(), output_length, content_type) == false);
            }
        }
    }
}

TEST(tls_record, evaluate, padding) {
    // In TLS 1.3 a fragment may be followed by zeros that hide its length, which are removed along with the content type.
    const std::vector<unsigned char> key = pattern(16, 7, 3);
    const std::vector<unsigned char> iv = pattern(12, 11, 5);
    // The record layer adds no padding, so the padded contents are sent as the fragment of a record of content type zero, which is itself padding.
    const auto receive_padded = [&](const std::vector<unsigned char>& inner, unsigned int& length, unsigned char& content_type) -> bool {
        gtl::tls_record record_send;
        gtl::tls_record record_receive;
        REQUIRE(record_send.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, key.data(), iv.data()) == true);
        REQUIRE(record_receive.set_keys(gtl::tls_record::version_type::tls13, gtl::tls_record::cipher_type::aes_128_gcm, key.data(), iv.data()) == true);
        std::vector<unsigned char> record(record_send.get_record_size(static_cast<unsigned int>(inner.size())), 0);
        unsigned long long int record_length = 0;
        REQUIRE(record_send.protect(0, data_of(inner), static_cast<unsigned int>(inner.size()), record.data(), record.size(), record_length) == true);
        return record_receive.unprotect(record[0], &record[5], static_cast<unsigned int>(record_length) - 5, &record[5], length, content_type) && ((length == 0) || (record[5] == inner[0]));
    };
    unsigned int length = 0;
    unsigned char content_type = 0;
    // A fragment of three bytes, its content type, and ten bytes of padding.
    std::vector<unsigned char> inner = { 0xAA, 0xBB, 0xCC, gtl::tls_record::content_application_data };
    inner.resize(14, 0x00);
    REQUIRE(receive_padded(inner, length, content_type) == true);
    REQUIRE(length == 3);
    REQUIRE(content_type == gtl::tls_record::content_application_data);
    // An empty fragment with padding.
    inner = { gtl::tls_record::content_handshake, 0x00, 0x00 };
    REQUIRE(receive_padded(inner, length, content_type) == true);
    REQUIRE(length == 0);
    REQUIRE(content_type == gtl::tls_record::content_handshake);
    // A fragment that ends in zeros keeps them, as only the zeros after the content type are padding.
    inner = { 0xAA, 0x00, 0x00, gtl::tls_record::content_application_data, 0x00 };
    REQUIRE(receive_padded(inner, length, content_type) == true);
    REQUIRE(length == 3);
    // Nothing but zeros has no content type, so is refused.
    inner.assign(8, 0x00);
    REQUIRE(receive_padded(inner, length, content_type) == false);
    inner.clear();
    REQUIRE(receive_padded(inner, length, content_type) == false);
}
