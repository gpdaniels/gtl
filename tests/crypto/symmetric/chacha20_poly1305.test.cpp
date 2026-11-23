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

#include <crypto/symmetric/chacha20_poly1305>

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

    // The example of RFC 8439 section 2.8.2.
    constexpr const char* rfc_example_key = "808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9F";
    constexpr const char* rfc_example_iv = "070000004041424344454647";
    constexpr const char* rfc_example_aad = "50515253C0C1C2C3C4C5C6C7";
    constexpr const char* rfc_example_plain = "4C616469657320616E642047656E746C656D656E206F662074686520636C617373206F66202739393A204966204920636F756C64206F6666657220796F75206F6E6C79206F6E652074697020666F7220746865206675747572652C2073756E73637265656E20776F756C642062652069742E";
    constexpr const char* rfc_example_encrypted = "D31A8D34648E60DB7B86AFBC53EF7EC2A4ADED51296E08FEA9E2B5A736EE62D63DBEA45E8CA9671282FAFB69DA92728B1A71DE0A9E060B2905D6A5B67ECD3B3692DDBD7F2D778B8C9803AEE328091B58FAB324E4FAD675945585808B4831D7BC3FF4DEF08E4B7A9DE576D26586CEC64B6116";
    constexpr const char* rfc_example_tag = "1AE10B594F09E26A7E902ECBD0600691";

    // The test vector of RFC 8439 appendix A.5.
    constexpr const char* rfc_vector_key = "1C9240A5EB55D38AF333888604F6B5F0473917C1402B80099DCA5CBC207075C0";
    constexpr const char* rfc_vector_iv = "000000000102030405060708";
    constexpr const char* rfc_vector_aad = "F33388860000000000004E91";
    constexpr const char* rfc_vector_plain = "496E7465726E65742D4472616674732061726520647261667420646F63756D656E74732076616C696420666F722061206D6178696D756D206F6620736978206D6F6E74687320616E64206D617920626520757064617465642C207265706C616365642C206F72206F62736F6C65746564206279206F7468657220646F63756D656E747320617420616E792074696D652E20497420697320696E617070726F70726961746520746F2075736520496E7465726E65742D447261667473206173207265666572656E6365206D6174657269616C206F7220746F2063697465207468656D206F74686572207468616E206173202FE2809C776F726B20696E2070726F67726573732E2FE2809D";
    constexpr const char* rfc_vector_encrypted = "64A0861575861AF460F062C79BE643BD5E805CFD345CF389F108670AC76C8CB24C6CFC18755D43EEA09EE94E382D26B0BDB7B73C321B0100D4F03B7F355894CF332F830E710B97CE98C8A84ABD0B948114AD176E008D33BD60F982B1FF37C8559797A06EF4F0EF61C186324E2B3506383606907B6A7C02B0F9F6157B53C867E4B9166C767B804D46A59B5216CDE7A4E99040C5A40433225EE282A1B0A06C523EAF4534D7F83FA1155B0047718CBC546A0D072B04B3564EEA1B422273F548271A0BB2316053FA76991955EBD63159434ECEBB4E466DAE5A1073A6727627097A1049E617D91D361094FA68F0FF77987130305BEABA2EDA04DF997B714D6C6F2C29A6AD5CB4022B02709B";
    constexpr const char* rfc_vector_tag = "EEAD9D67890CBB22392336FEA1851F38";

    // Data encrypted by another implementation, for lengths either side of the block sizes of the cipher and the authenticator.
    // The key is pattern(32, 7, 3), the nonce is pattern(12, 11, 5), the data is pattern(length, 13, 17), and the additional data is pattern(aad_length, 17, 29).
    struct reference_type {
        unsigned int length;
        unsigned int aad_length;
        const char* encrypted;
        const char* tag;
    };

    constexpr const reference_type references[] = {
        { 0, 0, "", "8DDA000CF4057E4B5850F2226B360AC1" },
        { 0, 5, "", "B448FCB275CB0CEB2C619833B265CCF5" },
        { 0, 13, "", "ABA12A476391C5CEB219ADE59603FD15" },
        { 0, 16, "", "B18AA730FA65C9A800B4FDDE400BC7F4" },
        { 0, 17, "", "F949672D568C4E600A2B9CFE02F610D3" },
        { 1, 0, "27", "4C6C6B27AF3C57C6C3565F8A482E1DC1" },
        { 1, 5, "27", "26956C39425FB0B41EC97EABE1F9F3D1" },
        { 1, 13, "27", "7A3E32D964212E765585091F6D1956AE" },
        { 1, 16, "27", "C0638CACA19097B3931AE50DC5DA26A7" },
        { 1, 17, "27", "A00113301EFD1811BC7D2537A57F0CAF" },
        { 15, 0, "27BDAE3D1E16D68DE46DD437CDA86B", "209D7E5A11DAB636D012B7646C0A04B9" },
        { 15, 5, "27BDAE3D1E16D68DE46DD437CDA86B", "F5C57F6CA4FC0F252B85D68505D6DAC9" },
        { 15, 13, "27BDAE3D1E16D68DE46DD437CDA86B", "496F450CC7BE8DE6614161F990F53CA6" },
        { 15, 16, "27BDAE3D1E16D68DE46DD437CDA86B", "8F949FDF032EF723A0D63CE8E8B60D9F" },
        { 15, 17, "27BDAE3D1E16D68DE46DD437CDA86B", "74322663809A7881C8397D11C95BF3A6" },
        { 16, 0, "27BDAE3D1E16D68DE46DD437CDA86B64", "4EDE1C3D7686E3798CBAE1B6156AC73B" },
        { 16, 5, "27BDAE3D1E16D68DE46DD437CDA86B64", "23071E4F09A93C68E72C01D8AE359E4C" },
        { 16, 13, "27BDAE3D1E16D68DE46DD437CDA86B64", "77B0E3EE2B6BBA291EE98B4B3A550029" },
        { 16, 16, "27BDAE3D1E16D68DE46DD437CDA86B64", "BDD53DC268DA23675C7E673A9216D121" },
        { 16, 17, "27BDAE3D1E16D68DE46DD437CDA86B64", "A273C445E546A5C484E1A76372BBB629" },
        { 17, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB", "320C7C6EF1DE06B11A0F43F8DCD2964A" },
        { 17, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB", "544F8D9C2659961DCA0D8E4ABE73625C" },
        { 17, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB", "4806E0D302848173FFFAD62A8A528CE3" },
        { 17, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB", "AF7B826C0614B8337DBD95B07ED13543" },
        { 17, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB", "B047C5D320B7DFCB556AEA6B17EFBE76" },
        { 31, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2", "4C480119DC613A88A20318DB791EB09A" },
        { 31, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2", "6E8B124711DCC9F45102632D5BBF7BAC" },
        { 31, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2", "6242657EED06B54A87EFAB0D279EA533" },
        { 31, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2", "C9B70717F196EB0A05B26A931B1D4F93" },
        { 31, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2", "CA834A7E0B3A13A3DD5EBF4EB43AD8C6" },
        { 32, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C273", "4874351C6CF340A0A18555F865002762" },
        { 32, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C273", "65B7464AA16DD00C5184A04A47A1F273" },
        { 32, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C273", "5E6E99817D98BB628671E92A13801CFB" },
        { 32, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C273", "C5E33B1A8128F2220434A8B007FFC55A" },
        { 32, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C273", "C1AF7E819BCB19BBDCE0FC6BA01C4F8E" },
        { 33, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B", "2CA8EA927B1B1239ED3DBF8760CF97CC" },
        { 33, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B", "6D47D2535E38DF609EBEA6313E8F95DA" },
        { 33, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B", "1226429F4EA5549E88CD48127866AEE8" },
        { 33, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B", "AA3826CBFF0DC1D29E1FD17001980169" },
        { 33, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B", "122DB32D9380ABA4FD229CE58718F7E9" },
        { 63, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCD", "7421A0281B37F65EE55C65E8A466741E" },
        { 63, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCD", "EA06E9CDC76E24707448BEF6AA7A6771" },
        { 63, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCD", "7272584F1F22618B3F0D8334E4E41F86" },
        { 63, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCD", "991623AAF354FC326140E0D348D4FAE7" },
        { 63, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCD", "5BFF41067048B005246D12DAAE54D347" },
        { 64, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3", "202CFA9525506FFC5CED814C09579B9D" },
        { 64, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3", "9611433BD2879D0DECD8DA5A0F6B8EF0" },
        { 64, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3", "237DB2BC293BDA28B79D9F9848D54605" },
        { 64, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3", "45217D17FE6D75D0D8D0FC37ADC42167" },
        { 64, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3", "070A9C737A6129A39BFD2E3E1345FAC6" },
        { 65, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1", "32C6151C1DDEA3C905B1782B741FB37F" },
        { 65, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1", "7DCE921FC38602410656B6EB645CDE2C" },
        { 65, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1", "A632916495965E9C64178B083E3F6C92" },
        { 65, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1", "9B27A8D545D4717884F24FCC37148075" },
        { 65, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1", "B731F99CCDD9021AAD45D0720677A3BC" },
        { 127, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C0", "8CA5D5BA28AB1E51E8ADEFBD83EB1058" },
        { 127, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C0", "77357678E5E4D8F80E13F1DBC8A1780D" },
        { 127, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C0", "EA2FFA1F0B6D468797ED845D07496B98" },
        { 127, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C0", "B83327F49A556AD6DCB3A89266F0080C" },
        { 127, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C0", "3444787496A67D70F787D43E32CB1DC6" },
        { 128, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D", "7ECE0B5B57A8E51F4167EF4A741F1A10" },
        { 128, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D", "695EAC1814E29FC767CCF068B9D581C5" },
        { 128, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D", "DC5830C0396A0D56F0A684EAF77C7450" },
        { 128, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D", "AA5C5D94C95231A5356DA81F572412C4" },
        { 128, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D", "216DAE14C5A3443F5041D4CB22FF267E" },
        { 129, 0, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67", "1B37BE9A853A99C09070849B85169C46" },
        { 129, 5, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67", "E849E3E8CC80825A920A32D49AB36213" },
        { 129, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67", "E95FD100757A59421C63AF533DD84B93" },
        { 129, 16, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67", "7669B3D57E6CCA905F042C80FF08C8A4" },
        { 129, 17, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67", "CE9885CD9D2B76DC0DF9541F8ABBF685" },
        { 255, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67CF4CC324AC759949ED0E5FED74179659185A85853D17A05E53BC550B1AC85130D670B3DB3A05DDD9FD10B639DC562E25308A7224B91B3DF6869A2105D2BCD0CDF6D915E72F38C5486A204F958D74EF99D8E23BA09E9E9BD719C6D24BBCA7A92902C67A5E00174E3C8DF3ACF6FB1EA6B4E11C89D43BAA5231650C48E4425E", "C7BCA9092B39039ABEBD86762A1B378A" },
        { 256, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67CF4CC324AC759949ED0E5FED74179659185A85853D17A05E53BC550B1AC85130D670B3DB3A05DDD9FD10B639DC562E25308A7224B91B3DF6869A2105D2BCD0CDF6D915E72F38C5486A204F958D74EF99D8E23BA09E9E9BD719C6D24BBCA7A92902C67A5E00174E3C8DF3ACF6FB1EA6B4E11C89D43BAA5231650C48E4425E45", "0D6E560F7B946D4779B3CEF17C4B54D4" },
        { 1000, 13, "27BDAE3D1E16D68DE46DD437CDA86B64BB71F65C63B293EC2CF2DF044979C2739B2BBBAC3144E08F912B376370404A98A2A17EF35B77D8B8E3B33AFC17DFCDD3F1459B9D92ED9C03441573721D4CD202044066E2A78A141C0B72C648F32A7CBD50DBDF333F9D211513B56089C536EBE29C7AD63D6424E484FBDF692E9A19C05D67CF4CC324AC759949ED0E5FED74179659185A85853D17A05E53BC550B1AC85130D670B3DB3A05DDD9FD10B639DC562E25308A7224B91B3DF6869A2105D2BCD0CDF6D915E72F38C5486A204F958D74EF99D8E23BA09E9E9BD719C6D24BBCA7A92902C67A5E00174E3C8DF3ACF6FB1EA6B4E11C89D43BAA5231650C48E4425E45FBDE644E96FAC17FA903A4F870D7854B66F3A184BE53AE85205E8CC82849A089353A9E06B79E2E51681D00BA5177E6002C0BD5CCC52C2E8985AA7291962A43C3905D104113DB8E4DDCDFD276693CBF50DF2C64AC54E56979C17572F7EA6489297D4041F33340C23042ACDD29FA258D3ED1C7B24DB4D9A27591A8C1E1C16062ED0AF7F771063409A9E42CE54FA5B3CB6ABAB14F925CA3022B6AAB0414AB239368AC4F03839F0FBFC63AC0C2652B351A2FEE60C00C5B9A7897A5F8017352673376C91ADE85123701FBC5E59B3C2582361C79CF7A25A852BB4D0790A7788412FDD453B47AD6BF54C316DCFC1D677BA22F01017F62924E50C4CD05CF5D0D39D0B79F024D1D0E01ED9C7270D31BC8BD0247654C88E09CE4B2B9C376F607FFB4B1E1061377D02584EDE0680646A5F90D9126AE61748D62EC935020BA2E006A8FFC56A4D3EF3B89D5D4283AA983F954FA4C5ED00103889443AD858E02E430D2A4C065787FFE08E81DCA14F78787E62F9407A22ADE53DC8C6ECD2C519C35143CD9951EA2FF4BF6C873500FC30263BED8D87DA81B27075D99DE87DD478DFD0CA07D65AF5B1EC8B23CE5906ECFB276D417C5374A8B555494C756262CFA94D0AD0888FF06ECBB879808139BC555B6D07206DB117F0D45FD8A89E613A0DC86B50C60EDA9799D1DCF1C1577B0CD8D86786C6CDA5DB8F91A3564CC40E453EBBCCC560DB5F9BF8791B7A8B8011AD34416DCAFA303EA4D1999ACB518B4D63D8E1E7E74074879365E65E54A82A6C76CCB45C06D12BBF7E1866360EA85F656AEA53F1689375D347C067478E549C73D0621E08EE82FD027F31DD33847512B0D6E7EDECE96D62BD254B4543A48A785340C1E1BA83C9736B01B835E38FF67B190DE5EC4ED21578C9BDA907B034718C683FF6E2957EE7ACAA706282F4F0339990E232EB8AD5EA326CE573A1298912325C70A04837DAE2911DE968AC8240B4CB663D8D9ADF07695EA08FC6EAF2F9B346E9518F17C82F49A75B0E8115886231F3A300408C2898317B948B6814A22033B1EF561B3", "F96D6162C4771D2B31BFB27CB7933516" },
    };
}

TEST(chacha20_poly1305, traits, standard) {
    REQUIRE((std::is_pod<gtl::chacha20_poly1305>::value == true));

    REQUIRE((std::is_trivial<gtl::chacha20_poly1305>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::chacha20_poly1305>::value == true));

    REQUIRE((std::is_standard_layout<gtl::chacha20_poly1305>::value == true));
}

TEST(chacha20_poly1305, constructor, empty) {
    gtl::chacha20_poly1305 chacha20_poly1305;
    testbench::do_not_optimise_away(chacha20_poly1305);
}

TEST(chacha20_poly1305, parameter, sizes) {
    REQUIRE(gtl::chacha20_poly1305::key_size == 32);
    REQUIRE(gtl::chacha20_poly1305::iv_size == 12);
    REQUIRE(gtl::chacha20_poly1305::tag_size == 16);
}

TEST(chacha20_poly1305, function, encrypt) {
    const unsigned char key[gtl::chacha20_poly1305::key_size] = {};
    const unsigned char iv[gtl::chacha20_poly1305::iv_size] = {};
    const unsigned char data[4] = { 1, 2, 3, 4 };
    unsigned char output[4] = {};
    unsigned char tag[gtl::chacha20_poly1305::tag_size] = {};
    gtl::chacha20_poly1305::encrypt(data, 4, key, iv, output, tag);
    gtl::chacha20_poly1305::encrypt(data, 4, key, iv, output, tag, data, 4);
}

TEST(chacha20_poly1305, function, decrypt) {
    const unsigned char key[gtl::chacha20_poly1305::key_size] = {};
    const unsigned char iv[gtl::chacha20_poly1305::iv_size] = {};
    const unsigned char data[4] = { 1, 2, 3, 4 };
    unsigned char encrypted[4] = {};
    unsigned char output[4] = {};
    unsigned char tag[gtl::chacha20_poly1305::tag_size] = {};
    gtl::chacha20_poly1305::encrypt(data, 4, key, iv, encrypted, tag);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted, 4, key, iv, output, tag) == true);
    REQUIRE(testbench::is_memory_same(output, data, 4));
}

TEST(chacha20_poly1305, evaluate, rfc_8439_example) {
    const std::vector<unsigned char> key = hex_2_bytes(rfc_example_key);
    const std::vector<unsigned char> iv = hex_2_bytes(rfc_example_iv);
    const std::vector<unsigned char> aad = hex_2_bytes(rfc_example_aad);
    const std::vector<unsigned char> plain = hex_2_bytes(rfc_example_plain);
    const std::vector<unsigned char> encrypted = hex_2_bytes(rfc_example_encrypted);
    const std::vector<unsigned char> tag = hex_2_bytes(rfc_example_tag);
    const unsigned int length = static_cast<unsigned int>(plain.size());
    REQUIRE(length == 114);
    REQUIRE(encrypted.size() == length);

    std::vector<unsigned char> test_encrypted(length, 0);
    unsigned char test_tag[gtl::chacha20_poly1305::tag_size] = {};
    gtl::chacha20_poly1305::encrypt(plain.data(), length, key.data(), iv.data(), test_encrypted.data(), test_tag, aad.data(), static_cast<unsigned int>(aad.size()));
    REQUIRE(testbench::is_memory_same(test_encrypted.data(), encrypted.data(), length));
    REQUIRE(testbench::is_memory_same(test_tag, tag.data(), gtl::chacha20_poly1305::tag_size));

    std::vector<unsigned char> test_plain(length, 0);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), test_plain.data(), tag.data(), aad.data(), static_cast<unsigned int>(aad.size())) == true);
    REQUIRE(testbench::is_memory_same(test_plain.data(), plain.data(), length));
}

TEST(chacha20_poly1305, evaluate, rfc_8439_vector) {
    const std::vector<unsigned char> key = hex_2_bytes(rfc_vector_key);
    const std::vector<unsigned char> iv = hex_2_bytes(rfc_vector_iv);
    const std::vector<unsigned char> aad = hex_2_bytes(rfc_vector_aad);
    const std::vector<unsigned char> plain = hex_2_bytes(rfc_vector_plain);
    const std::vector<unsigned char> encrypted = hex_2_bytes(rfc_vector_encrypted);
    const std::vector<unsigned char> tag = hex_2_bytes(rfc_vector_tag);
    const unsigned int length = static_cast<unsigned int>(encrypted.size());
    REQUIRE(length == 265);
    REQUIRE(plain.size() == length);

    std::vector<unsigned char> test_plain(length, 0);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), test_plain.data(), tag.data(), aad.data(), static_cast<unsigned int>(aad.size())) == true);
    REQUIRE(testbench::is_memory_same(test_plain.data(), plain.data(), length));

    std::vector<unsigned char> test_encrypted(length, 0);
    unsigned char test_tag[gtl::chacha20_poly1305::tag_size] = {};
    gtl::chacha20_poly1305::encrypt(plain.data(), length, key.data(), iv.data(), test_encrypted.data(), test_tag, aad.data(), static_cast<unsigned int>(aad.size()));
    REQUIRE(testbench::is_memory_same(test_encrypted.data(), encrypted.data(), length));
    REQUIRE(testbench::is_memory_same(test_tag, tag.data(), gtl::chacha20_poly1305::tag_size));
}

TEST(chacha20_poly1305, evaluate, reference_lengths) {
    const std::vector<unsigned char> key = pattern(32, 7, 3);
    const std::vector<unsigned char> iv = pattern(12, 11, 5);
    for (const reference_type& reference : references) {
        const std::vector<unsigned char> plain = pattern(reference.length, 13, 17);
        const std::vector<unsigned char> aad = pattern(reference.aad_length, 17, 29);
        const std::vector<unsigned char> encrypted = hex_2_bytes(reference.encrypted);
        const std::vector<unsigned char> tag = hex_2_bytes(reference.tag);
        REQUIRE(encrypted.size() == reference.length);
        REQUIRE(tag.size() == gtl::chacha20_poly1305::tag_size);

        std::vector<unsigned char> test_encrypted(reference.length + 1, 0xA5);
        unsigned char test_tag[gtl::chacha20_poly1305::tag_size] = {};
        gtl::chacha20_poly1305::encrypt(data_of(plain), reference.length, key.data(), iv.data(), test_encrypted.data(), test_tag, (reference.aad_length == 0) ? nullptr : aad.data(), reference.aad_length);
        REQUIRE(testbench::is_memory_same(test_encrypted.data(), data_of(encrypted), reference.length), "Encrypted data mismatch for length %u with %u bytes of additional data.", reference.length, reference.aad_length);
        REQUIRE(testbench::is_memory_same(test_tag, tag.data(), gtl::chacha20_poly1305::tag_size), "Tag mismatch for length %u with %u bytes of additional data.", reference.length, reference.aad_length);
        // Nothing is written past the end of the output.
        REQUIRE(test_encrypted[reference.length] == 0xA5);

        std::vector<unsigned char> test_plain(reference.length + 1, 0xA5);
        REQUIRE(gtl::chacha20_poly1305::decrypt(data_of(encrypted), reference.length, key.data(), iv.data(), test_plain.data(), tag.data(), (reference.aad_length == 0) ? nullptr : aad.data(), reference.aad_length) == true);
        REQUIRE(testbench::is_memory_same(test_plain.data(), data_of(plain), reference.length));
        REQUIRE(test_plain[reference.length] == 0xA5);

        // Encrypting and decrypting in place gives the same results.
        std::vector<unsigned char> in_place = plain;
        in_place.push_back(0xA5);
        gtl::chacha20_poly1305::encrypt(in_place.data(), reference.length, key.data(), iv.data(), in_place.data(), test_tag, aad.empty() ? nullptr : aad.data(), reference.aad_length);
        REQUIRE(testbench::is_memory_same(in_place.data(), data_of(encrypted), reference.length));
        REQUIRE(testbench::is_memory_same(test_tag, tag.data(), gtl::chacha20_poly1305::tag_size));
        REQUIRE(gtl::chacha20_poly1305::decrypt(in_place.data(), reference.length, key.data(), iv.data(), in_place.data(), test_tag, aad.empty() ? nullptr : aad.data(), reference.aad_length) == true);
        REQUIRE(testbench::is_memory_same(in_place.data(), data_of(plain), reference.length));
        REQUIRE(in_place[reference.length] == 0xA5);
    }
}

TEST(chacha20_poly1305, evaluate, authenticate) {
    const std::vector<unsigned char> key = hex_2_bytes(rfc_example_key);
    const std::vector<unsigned char> iv = hex_2_bytes(rfc_example_iv);
    const std::vector<unsigned char> aad = hex_2_bytes(rfc_example_aad);
    const std::vector<unsigned char> plain = hex_2_bytes(rfc_example_plain);
    const std::vector<unsigned char> encrypted = hex_2_bytes(rfc_example_encrypted);
    const std::vector<unsigned char> tag = hex_2_bytes(rfc_example_tag);
    const unsigned int length = static_cast<unsigned int>(encrypted.size());
    const unsigned int aad_length = static_cast<unsigned int>(aad.size());

    std::vector<unsigned char> untouched(length, 0xA5);
    std::vector<unsigned char> output(length, 0xA5);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), output.data(), tag.data(), aad.data(), aad_length) == true);
    REQUIRE(testbench::is_memory_same(output.data(), plain.data(), length));
    output = untouched;

    // Changing any bit of the encrypted data, the tag, the additional data, the nonce, or the key is detected, and nothing is decrypted.
    for (unsigned int bit = 0; bit < (length * 8); ++bit) {
        std::vector<unsigned char> changed = encrypted;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::chacha20_poly1305::decrypt(changed.data(), length, key.data(), iv.data(), output.data(), tag.data(), aad.data(), aad_length) == false);
    }
    for (unsigned int bit = 0; bit < (gtl::chacha20_poly1305::tag_size * 8); ++bit) {
        std::vector<unsigned char> changed = tag;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), output.data(), changed.data(), aad.data(), aad_length) == false);
    }
    for (unsigned int bit = 0; bit < (aad_length * 8); ++bit) {
        std::vector<unsigned char> changed = aad;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), output.data(), tag.data(), changed.data(), aad_length) == false);
    }
    for (unsigned int bit = 0; bit < (gtl::chacha20_poly1305::iv_size * 8); ++bit) {
        std::vector<unsigned char> changed = iv;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), changed.data(), output.data(), tag.data(), aad.data(), aad_length) == false);
    }
    for (unsigned int bit = 0; bit < (gtl::chacha20_poly1305::key_size * 8); ++bit) {
        std::vector<unsigned char> changed = key;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, changed.data(), iv.data(), output.data(), tag.data(), aad.data(), aad_length) == false);
    }

    // Truncating the data or the additional data, moving bytes between them, or leaving out the additional data is detected.
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length - 1, key.data(), iv.data(), output.data(), tag.data(), aad.data(), aad_length) == false);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), 0, key.data(), iv.data(), output.data(), tag.data(), aad.data(), aad_length) == false);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), output.data(), tag.data(), aad.data(), aad_length - 1) == false);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), output.data(), tag.data()) == false);
    std::vector<unsigned char> joined = aad;
    joined.insert(joined.end(), encrypted.begin(), encrypted.end());
    REQUIRE(gtl::chacha20_poly1305::decrypt(&joined[aad_length + 4], length - 4, key.data(), iv.data(), output.data(), tag.data(), joined.data(), aad_length + 4) == false);
    REQUIRE(gtl::chacha20_poly1305::decrypt(&joined[aad_length - 4], length + 4, key.data(), iv.data(), joined.data(), tag.data(), joined.data(), aad_length - 4) == false);

    // An all zero tag is not accepted.
    const unsigned char zero_tag[gtl::chacha20_poly1305::tag_size] = {};
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), length, key.data(), iv.data(), output.data(), zero_tag, aad.data(), aad_length) == false);
    REQUIRE(gtl::chacha20_poly1305::decrypt(encrypted.data(), 0, key.data(), iv.data(), output.data(), zero_tag) == false);

    REQUIRE(testbench::is_memory_same(output.data(), untouched.data(), length));
}
