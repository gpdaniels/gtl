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

#include <protocol/tls/tls13>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {
    // A small hierarchy of test certificates made by another implementation: a root, an intermediate issued by the root, and two server certificates issued by the intermediate.
    constexpr const char* certificate_root = "30820311308201F9A003020102020203E9300D06092A864886F70D01010B05003039310B300906035504061302474231123010060355040A0C0967746C2074657374733116301406035504030C0D67746C207465737420726F6F74301E170D3230303130313030303030305A170D3430303130313030303030305A3039310B300906035504061302474231123010060355040A0C0967746C2074657374733116301406035504030C0D67746C207465737420726F6F7430820122300D06092A864886F70D01010105000382010F003082010A0282010100F5F18CA84CA5BD749E9B44A380EBF87AC7B923C3609364B9DEB53FA1790CC1C6F13BC440772F8D15EAAA26D79782E6505AF36855EE758EB6C2CC6EF709F0A3C085C8A6DC5CEC4C3FEBB27AB1CAE7D715C49DF1832D74B41C7D5D8EF8B1A2B9105BF8CADCEDCD6992CC03D5721CEAA1C8CD7474515222AA3ACD708F213F7A8C980C31A3BC670CCD0F7A2A3248EACFB39F53DF2F3A773486245B2C8F742C579CD9146648777CBA58B5EA9F213193BDE86167CD50405107EC3F94F55CA3A8D224E825AF2C1FFEFC2ADDBE6C511DE041629D3CDFEC21917C56763428C6094E988361C9423DCA7915A9A9481AB16B5AF8E7830F9E74E955E09E4E4F6B8C88E1A6C2BF0203010001A3233021300F0603551D130101FF040530030101FF300E0603551D0F0101FF040403020186300D06092A864886F70D01010B0500038201010030072A0ADEED62F04CE2CDD23ED2F73F874D8DD556529D9C23E958E5A42612517C4BC06CCE7DA3208558CD5714845DC2F50A6B797D4642A8B381F164BF4B4C468CC13FCABF84A6392A87F6E052F1B5F44A9C45852A02DF13C33D995E08BF574C57CC5AB1DC628CE69D265B724D504FCD5AC68F444A0EDCC7FB4C2EF0A588E7672F589472A8A15DDDD950F284FD4B59CFC9D35676E8A3922B718205F53D15B154C5ADFE7751AF6DE44ED4990A1CA22D9ADADA0B5A2CDF66A9528B6BFC35678B532113B72C559CBDB3D7916A53E3B225F776C43C9546D09C50A288C17CE9221E43B1C31D03D33BCDFA93CAF0EFBA9E18BB0AC6A13AB90ADB4F10F7B36C221A6C99";
    constexpr const char* certificate_intermediate = "3082026E30820156A003020102020203EA300D06092A864886F70D01010C05003039310B300906035504061302474231123010060355040A0C0967746C2074657374733116301406035504030C0D67746C207465737420726F6F74301E170D3234303130313030303030305A170D3334303130313030303030305A3041310B300906035504061302474231123010060355040A0C0967746C207465737473311E301C06035504030C1567746C207465737420696E7465726D6564696174653076301006072A8648CE3D020106052B8104002203620004F19D05001BF40B06F728C47CFF4F3C50556B66AB81E27346CD3D3FCA9A920D284E51FB55642C22F03336F8FCEC255B9F2FEAD93F56CB43D6489E74CF5835E9D5ADDDD96D094E525282B8E8BE14451E8ABFD1E9A782A07CB2B147A4C55DB59A82A326302430120603551D130101FF040830060101FF020100300E0603551D0F0101FF040403020186300D06092A864886F70D01010C05000382010100E29F5E71CF20D2854FDC1007D264C5B8CE17CB0C766215590F096C02229408FF3DAE9A2A38DA3425C8FE12BB76D005D7127AF943ED567BD44C04E61DB656A9EC1ED19A7AF9405FAEEF0A7D8C85D973B713557FCCB923AD13FB0EA257F761960CDDDF2FFE7BB6B849259106F5B720C91E335E9C103886765995FAB3969D463D15136A5E8A872827C09A7C85309B3E33B86F7579D1B430FF45CA46A0881094507DA601474CC660A25249B402F6EFD164FBD179D886423FB097E18DC63C45FB27192A5CF245158E8122FA26E403F375F7453AB2D0795DC79CD46E33212EC0D5FD562819401C58D29454115FBFD0A48A70ACF3ED724F6C49B6E8A56707B0A85BF30B";
    constexpr const char* certificate_leaf = "308202083082018EA003020102020203EB300A06082A8648CE3D0403033041310B300906035504061302474231123010060355040A0C0967746C207465737473311E301C06035504030C1567746C207465737420696E7465726D656469617465301E170D3234303130313030303030305A170D3334303130313030303030305A3038310B300906035504061302474231123010060355040A0C0967746C2074657374733115301306035504030C0C6578616D706C652E746573743059301306072A8648CE3D020106082A8648CE3D03010703420004C3DE93124C9367BB2B0DC1E97B9D3930C39AD40BC1FD2044A622F3680E1D6BA3E98962A5CDFB45A087D26261DF40B574C9C22328D8870FC6922EB3D9F3A1BD34A37F307D300C0603551D130101FF04023000300E0603551D0F0101FF04040302078030480603551D110441303F820C6578616D706C652E7465737482132A2E77696C642E6578616D706C652E7465737487047F0000018114736F6D656F6E65406578616D706C652E7465737430130603551D25040C300A06082B06010505070301300A06082A8648CE3D0403030368003065023100B201A470D08F7AF56322D396F9939C7AD7CAC0CD79C2160E103217E25220FABF6CD6CD8500C907DE73C2D26A52FF9CEF02301DDFEA4CBF29C3596A5926E608A68AAC7F951E25F9E19E43F23A585AD2DAE173195BF1E25024518433FF94A8793AD261";
    constexpr const char* certificate_leaf_rsa = "308202A930820230A003020102020203EC300A06082A8648CE3D0403023041310B300906035504061302474231123010060355040A0C0967746C207465737473311E301C06035504030C1567746C207465737420696E7465726D656469617465301E170D3234303130313030303030305A170D3334303130313030303030305A303C310B300906035504061302474231123010060355040A0C0967746C2074657374733119301706035504030C107273612E6578616D706C652E7465737430820122300D06092A864886F70D01010105000382010F003082010A0282010100EA59633FCE7239FA498695E68DC5748278927FB584596EF822FC23D9B7A008ABB7A0A6D7B0914DD0F68C961A7C7FB2CFBDD988DDCCF3DE4C403CABA94D1870B6E3CF4EC8C8AC8137FEB4A3E0D3E2EB7AA1DD3680D4E3550A425D4B2168A34523A0B81733F36C18EA788C1A22D1FBB25B735B9FF512BDA692E34A02E19FFC99B484BFB0EBB942A6498891CC9F73D4D175C84AD63C376391032F1E82854810F550C61DD6E66F32940F35924FEA0D9B4BECB57D213B53BAB5A6C2A8CD27E01E1467DB267CAB3909ADED43A7CAE7C22768AE3FFD7DF1717285C110B048E14A737F016470028A3C63A6457EC81AE1077CDB4C32573C084FFEB53A80B7FDA0EC5BE91D0203010001A3523050300C0603551D130101FF04023000300E0603551D0F0101FF040403020780301B0603551D110414301282107273612E6578616D706C652E7465737430130603551D25040C300A06082B06010505070301300A06082A8648CE3D040302036700306402300799927A2AE82BCA4A2BD194FD9C306F94712C09F712BB43B39734AB5FC645DAA0F832530A5F1C11F31E26E9B0C3CB1E02303C3983AC1C604C5072CBBE4B5F34331E421192A02D7F1E60E4BC8EA46905B6876BB80809ACF052FBB458F5FD95B5141F";
    constexpr const char* key_leaf = "308187020100301306072A8648CE3D020106082A8648CE3D030107046D306B0201010420AA0D2C9ED42E0953FF618D821996B6237EA83F40B24260CC51EEAB1055371A78A14403420004C3DE93124C9367BB2B0DC1E97B9D3930C39AD40BC1FD2044A622F3680E1D6BA3E98962A5CDFB45A087D26261DF40B574C9C22328D8870FC6922EB3D9F3A1BD34";
    constexpr const char* key_leaf_rsa = "308204BD020100300D06092A864886F70D0101010500048204A7308204A30201000282010100EA59633FCE7239FA498695E68DC5748278927FB584596EF822FC23D9B7A008ABB7A0A6D7B0914DD0F68C961A7C7FB2CFBDD988DDCCF3DE4C403CABA94D1870B6E3CF4EC8C8AC8137FEB4A3E0D3E2EB7AA1DD3680D4E3550A425D4B2168A34523A0B81733F36C18EA788C1A22D1FBB25B735B9FF512BDA692E34A02E19FFC99B484BFB0EBB942A6498891CC9F73D4D175C84AD63C376391032F1E82854810F550C61DD6E66F32940F35924FEA0D9B4BECB57D213B53BAB5A6C2A8CD27E01E1467DB267CAB3909ADED43A7CAE7C22768AE3FFD7DF1717285C110B048E14A737F016470028A3C63A6457EC81AE1077CDB4C32573C084FFEB53A80B7FDA0EC5BE91D02030100010282010070F98AB935E327441BB2000DD786BB3632BE1D46FA78CAE060AD40F3326CBA015E52DA5BDCC90FC48F37131DCDAB1A1EC01D60B7DC982C30823F3D5A2ECA7B7C0FB2DEFE388FC0F3A79C99EC46F97C1FF06C5E5C194546B7DD6A704FB8CC0D06B1F24D6D440A76B08250B840D50E0BDBBE6F0B5BF01EF1709A96117187D914975FBCD4B88F5B0F90390FB8FFAF8DB32A8D34FBABD4B3D263B6DD63FB7CA3C782DD78F1FD610E0C97469DF8EAB674727F569B2538528A37A792D0D1DA7A281DE0511EA11952BF3F46F4DAC2F151A1DCEF54B91653D86AE53F745E603BC14A10DF74EC938F0F7856654CCA7FEFD61D6406891D2CC99C16590E3F6E022D899ECC5902818100F968767F9E7AFF06A9C01008170A70602E3BE1FC60931181877A1B08118E953610ADF21581B409308C3372E71F0AFD5F194149A5F91F84C6E47842956858DE45C360E06DD860E8E3A5E21DF41A56AFB021C0D6576E9F89B3F9BA013BA6530235605E6A5FC05E0F5B141F681447718EB3E01AF46C7C24E337D8C467B9F131CE1302818100F08B08A8E84D707DF7991D22035F7B30885129C0F2388FC7D8EE4336BEF3433034A2EDFC61F7F95529D6A8B1B6E77C3D0F398EFC91C4070886242FEC751FC07E3EFF7EB6A95E83F01FB1DFBD503F311108679328A592327FC7A7353438D097AF97C857548BBF03E2988E0CEEC3EC6D27155A7492AA0E93990622ECE1A6AE920F02818100EF95F61F6A3265421557370F4E72ECB8B8313649A6CC2088F987C6E7DD55C6E884E554246CD071F37CFAFF6A1E5C0441929531168B5647CA78373E5F203A1E6B8DE04637BF20791816863C97FDA54068464464A2E0D5C90666E2D3C46566603CBE3274A53EBA1CAE358A6604F50C95351E1746ADE023DF520A9DC1B6D188329902818044EB00E32C13ABEC5B79621071B8F499B624D2FAB62BA07E58D80327F753C967515D7124438981C575C44BF706F991EE08A3AC250F3A83ED5C4997F57EA54748A4921D630669CF11887335308F825B738EDDE61ADA3D65FC985E7EEEE46C575976FA6E9309AB03D8F36E97A3FD3C8B158C97AB18AE6298C55A2DBD82F577C4DF0281802E97458BFE6783697CAB25B1E23B5B9537435A5B9E9DBD7B3DC3E05DC909CF40E86425CDB4502089C1F0B063DE2D8438A6359DEDF356E88EFD93EC67CD45ADCD154B38D6021A05EE6F2E68F3CB13428E0D0176E3C5C03B4C251D6C8DC40F2FC249A123785EDCF39CA28FD5B19375E8703B718466D418CC59C8FF323D73FF6C7B";

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

    /// @brief  A random generator of reasonable quality with a fixed seed, which makes every handshake repeatable.
    std::function<unsigned int()> pcg_generator(const unsigned long long int seed) {
        unsigned long long int state = 0x853C49E6748FEA9Bull ^ seed;
        return [state]() mutable -> unsigned int {
            const unsigned long long int state_previous = state;
            state = state_previous * 0x5851F42D4C957F2Dull + 0xDA3E39CB94B95BDBull;
            const unsigned int state_shift_xor_shift = static_cast<unsigned int>(((state_previous >> 18u) ^ state_previous) >> 27u);
            const unsigned int rotation = static_cast<unsigned int>(state_previous >> 59u);
            return (state_shift_xor_shift >> rotation) | (state_shift_xor_shift << ((32 - rotation) & 31));
        };
    }

    // The time used to verify certificates, the start of 2027, when every test certificate is valid.
    constexpr const long long int time_now = 1798761600ll;

    /// @brief  The trusted root certificate.
    class trusted_root final {
    public:
        std::vector<unsigned char> encoded;
        gtl::x509::certificate_type parsed;

    public:
        explicit trusted_root(const char* hex = certificate_root)
            : encoded(hex_2_bytes(hex)) {
            REQUIRE(gtl::x509::parse_certificate(this->encoded.data(), this->encoded.size(), this->parsed) == true);
        }

        trusted_root(const trusted_root&) = delete;
        trusted_root& operator=(const trusted_root&) = delete;
    };

    /// @brief  The certificates and private key of a server, with either an elliptic curve key or an rsa key.
    class server_identity final {
    public:
        std::vector<unsigned char> leaf;
        std::vector<unsigned char> intermediate;
        gtl::x509::private_key_type private_key;
        const unsigned char* certificates[2] = {};
        unsigned long long int certificate_lengths[2] = {};
        const char* host = nullptr;

    public:
        explicit server_identity(const bool is_rsa)
            : leaf(hex_2_bytes(is_rsa ? certificate_leaf_rsa : certificate_leaf))
            , intermediate(hex_2_bytes(certificate_intermediate))
            , host(is_rsa ? "rsa.example.test" : "example.test") {
            const std::vector<unsigned char> key = hex_2_bytes(is_rsa ? key_leaf_rsa : key_leaf);
            REQUIRE(gtl::x509::parse_private_key(key.data(), key.size(), this->private_key) == true);
            this->certificates[0] = this->leaf.data();
            this->certificates[1] = this->intermediate.data();
            this->certificate_lengths[0] = this->leaf.size();
            this->certificate_lengths[1] = this->intermediate.size();
        }

        server_identity(const server_identity&) = delete;
        server_identity& operator=(const server_identity&) = delete;
    };

    using handshake = gtl::tls_handshake;

    /// @brief  One end of a handshake, with what a connection would provide to it.
    class endpoint final {
    public:
        handshake::settings_type settings;
        handshake::sender_type sender;
        gtl::tls_record receiver;
        gtl::tls13 machine;
        std::vector<unsigned char> pending;
        unsigned int change_cipher_specs = 0;
        bool is_handshake_only = true;

    public:
        endpoint() = default;
        endpoint(const endpoint&) = delete;
        endpoint& operator=(const endpoint&) = delete;
    };

    /// @brief  Deliver every record that one end has sent to the other end, as a connection would.
    /// @return false if a record could not be read, or the handshake of the receiving end failed.
    bool deliver(endpoint& from, endpoint& to) {
        std::vector<unsigned char> bytes;
        bytes.swap(from.sender.buffer);
        for (unsigned long long int offset = 0; offset < bytes.size();) {
            unsigned char record_type = 0;
            unsigned int body_length = 0;
            if (((bytes.size() - offset) < gtl::tls_record::header_size) || !gtl::tls_record::parse_header(&bytes[offset], record_type, body_length)) {
                return false;
            }
            unsigned char* body = &bytes[offset + gtl::tls_record::header_size];
            offset += gtl::tls_record::header_size + body_length;
            if (record_type == gtl::tls_record::content_change_cipher_spec) {
                ++to.change_cipher_specs;
                if (!true) {
                    return false;
                }
                continue;
            }
            unsigned int length = 0;
            unsigned char content_type = 0;
            if (!to.receiver.unprotect(record_type, body, body_length, body, length, content_type)) {
                return false;
            }
            if (content_type != gtl::tls_record::content_handshake) {
                to.is_handshake_only = false;
                continue;
            }
            to.pending.insert(to.pending.end(), body, body + length);
            while (to.pending.size() >= 4) {
                const unsigned long long int message_length = 4 + ((static_cast<unsigned long long int>(to.pending[1]) << 16) | (static_cast<unsigned long long int>(to.pending[2]) << 8) | to.pending[3]);
                if (to.pending.size() < message_length) {
                    break;
                }
                const std::vector<unsigned char> message(to.pending.begin(), to.pending.begin() + static_cast<long long int>(message_length));
                to.pending.erase(to.pending.begin(), to.pending.begin() + static_cast<long long int>(message_length));
                if (!to.machine.process_message(message.data(), message.size(), to.pending.empty())) {
                    return false;
                }
            }
        }
        return true;
    }

    /// @brief  Check that application data sent by one end is received by the other.
    bool can_send(endpoint& from, endpoint& to) {
        const unsigned char data[5] = { 'h', 'e', 'l', 'l', 'o' };
        if (!from.sender.record.is_protected() || !from.sender.send(gtl::tls_record::content_application_data, data, 5)) {
            return false;
        }
        std::vector<unsigned char> bytes;
        bytes.swap(from.sender.buffer);
        unsigned int length = 0;
        unsigned char content_type = 0;
        return (bytes.size() > 5) && to.receiver.unprotect(bytes[0], &bytes[5], static_cast<unsigned int>(bytes.size() - 5), &bytes[5], length, content_type) && (content_type == gtl::tls_record::content_application_data) && (length == 5) && testbench::is_memory_same(&bytes[5], data, 5);
    }

    /// @brief  The contents of a hello message of a client, made to offer exactly one cipher suite and one group.
    class client_hello final {
    public:
        std::vector<unsigned char> message;
        unsigned char random[handshake::random_size] = {};
        unsigned char session_id[handshake::session_id_size_max] = {};
        unsigned int group = 0;
        unsigned char private_value[handshake::exchange_private_size_max] = {};
        handshake::client_hello_type parsed;

    public:
        client_hello(const unsigned int suite, const unsigned int offered_group, const bool is_tls13, const bool with_extended_master_secret, const bool with_share, const std::function<unsigned int()>& random_generator)
            : group(offered_group) {
            handshake::fill_random(random_generator, this->random, handshake::random_size);
            handshake::fill_random(random_generator, this->session_id, handshake::session_id_size_max);
            unsigned char public_value[handshake::exchange_public_size_max] = {};
            unsigned int public_length = 0;
            REQUIRE(handshake::generate_exchange(offered_group, random_generator, this->private_value, public_value, public_length) == true);
            handshake::writer output(this->message);
            const unsigned long long int body = output.begin_message(handshake::message_client_hello);
            output.write_integer(handshake::version_tls12, 2);
            output.write_bytes(this->random, handshake::random_size);
            output.write_integer(handshake::session_id_size_max, 1);
            output.write_bytes(this->session_id, handshake::session_id_size_max);
            output.write_integer(2, 2);
            output.write_integer(suite, 2);
            output.write_integer(1, 1);
            output.write_integer(0, 1);
            const unsigned long long int extensions = output.begin_vector(2);
            output.write_integer(handshake::extension_supported_groups, 2);
            output.write_integer(4, 2);
            output.write_integer(2, 2);
            output.write_integer(offered_group, 2);
            output.write_integer(handshake::extension_signature_algorithms, 2);
            output.write_integer(16, 2);
            output.write_integer(14, 2);
            const unsigned int signatures[7] = { handshake::signature_ecdsa_sha256, handshake::signature_ecdsa_sha384, handshake::signature_rsa_pss_sha256, handshake::signature_rsa_pss_sha384, handshake::signature_rsa_pss_sha512, handshake::signature_rsa_pkcs1_sha256, handshake::signature_rsa_pkcs1_sha384 };
            for (const unsigned int signature : signatures) {
                output.write_integer(signature, 2);
            }
            if (with_extended_master_secret) {
                output.write_integer(handshake::extension_extended_master_secret, 2);
                output.write_integer(0, 2);
            }
            if (is_tls13) {
                output.write_integer(handshake::extension_supported_versions, 2);
                output.write_integer(3, 2);
                output.write_integer(2, 1);
                output.write_integer(handshake::version_tls13, 2);
            }
            if (with_share) {
                output.write_integer(handshake::extension_key_share, 2);
                output.write_integer(public_length + 6, 2);
                output.write_integer(public_length + 4, 2);
                output.write_integer(offered_group, 2);
                output.write_integer(public_length, 2);
                output.write_bytes(public_value, public_length);
            }
            output.end_vector(extensions, 2);
            output.end_message(body);
            REQUIRE(handshake::parse_client_hello(this->message.data() + 4, this->message.size() - 4, this->parsed) == true);
        }

        client_hello(const client_hello&) = delete;
        client_hello& operator=(const client_hello&) = delete;
    };

    /// @brief  Set up the settings of a client and a server.
    void configure(endpoint& client, endpoint& server, const trusted_root& root, const server_identity& identity, const unsigned long long int seed) {
        client.settings.is_client = true;
        client.settings.random_generator = pcg_generator(seed);
        client.settings.host = identity.host;
        client.settings.trusted = &root.parsed;
        client.settings.trusted_length = 1;
        client.settings.time = time_now;
        server.settings.is_client = false;
        server.settings.random_generator = pcg_generator(seed + 1);
        server.settings.certificates = identity.certificates;
        server.settings.certificate_lengths = identity.certificate_lengths;
        server.settings.certificates_length = 2;
        server.settings.private_key = &identity.private_key;
    }

    using state_type = gtl::tls13::state_type;

    /// @brief  Start a client and a server from a hello message, and run the handshake.
    /// @return true if both ends completed the handshake.
    bool run(endpoint& client, endpoint& server, const client_hello& hello) {
        gtl::tls13::share_type share;
        share.group = hello.group;
        for (unsigned int i = 0; i < handshake::exchange_private_size_max; ++i) {
            share.private_value[i] = hello.private_value[i];
        }
        if (!client.machine.start_client(client.settings, client.sender, client.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, &share, 1)) {
            return false;
        }
        if (!server.machine.start_server(server.settings, server.sender, server.receiver, hello.parsed, hello.message.data(), hello.message.size())) {
            return false;
        }
        return deliver(server, client) && deliver(client, server) && client.machine.is_connected() && server.machine.is_connected();
    }

    /// @brief  Build a hello message of a server, which may be wrong in a chosen way.
    std::vector<unsigned char> make_server_hello(const client_hello& hello, const unsigned int suite, const unsigned int version, const unsigned int group, const bool wrong_session_id, const bool is_retry) {
        constexpr const unsigned char retry_random[handshake::random_size] = {
            0xCF,
            0x21,
            0xAD,
            0x74,
            0xE5,
            0x9A,
            0x61,
            0x11,
            0xBE,
            0x1D,
            0x8C,
            0x02,
            0x1E,
            0x65,
            0xB8,
            0x91,
            0xC2,
            0xA2,
            0x11,
            0x16,
            0x7A,
            0xBB,
            0x8C,
            0x5E,
            0x07,
            0x9E,
            0x09,
            0xE2,
            0xC8,
            0xA8,
            0x33,
            0x9C
        };
        unsigned char private_value[handshake::exchange_private_size_max] = {};
        unsigned char public_value[handshake::exchange_public_size_max] = {};
        unsigned int public_length = 0;
        if (group != 0) {
            REQUIRE(handshake::generate_exchange(group, pcg_generator(77), private_value, public_value, public_length) == true);
        }
        std::vector<unsigned char> message;
        handshake::writer output(message);
        const unsigned long long int body = output.begin_message(handshake::message_server_hello);
        output.write_integer(handshake::version_tls12, 2);
        if (is_retry) {
            output.write_bytes(retry_random, handshake::random_size);
        }
        else {
            output.write_bytes(hello.random, handshake::random_size);
        }
        output.write_integer(handshake::session_id_size_max, 1);
        output.write_bytes(hello.session_id, handshake::session_id_size_max);
        if (wrong_session_id) {
            message.back() ^= 0x01;
        }
        output.write_integer(suite, 2);
        output.write_integer(0, 1);
        const unsigned long long int extensions = output.begin_vector(2);
        if (version != 0) {
            output.write_integer(handshake::extension_supported_versions, 2);
            output.write_integer(2, 2);
            output.write_integer(version, 2);
        }
        if (group != 0) {
            output.write_integer(handshake::extension_key_share, 2);
            if (is_retry) {
                output.write_integer(2, 2);
                output.write_integer(group, 2);
            }
            else {
                output.write_integer(public_length + 4, 2);
                output.write_integer(group, 2);
                output.write_integer(public_length, 2);
                output.write_bytes(public_value, public_length);
            }
        }
        output.end_vector(extensions, 2);
        output.end_message(body);
        return message;
    }
}

TEST(tls13, traits, standard) {
    REQUIRE((std::is_pod<gtl::tls13>::value == false));

    REQUIRE((std::is_trivial<gtl::tls13>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::tls13>::value == false));

    REQUIRE((std::is_copy_constructible<gtl::tls13>::value == false));
}

TEST(tls13, constructor, empty) {
    gtl::tls13 machine;
    REQUIRE(machine.get_state() == state_type::idle);
    REQUIRE(machine.is_connected() == false);
    REQUIRE(machine.get_alert() == 0);
    REQUIRE(machine.get_suite() == 0);
    REQUIRE(machine.get_peer_certificates().empty() == true);
    REQUIRE(machine.update_keys(true) == false);
    // A handshake that has not been started accepts no messages.
    const unsigned char message[4] = { handshake::message_finished, 0, 0, 0 };
    REQUIRE(machine.process_message(message, 4, true) == false);
    REQUIRE(machine.get_state() == state_type::failed);
    REQUIRE(machine.get_alert() == handshake::alert_unexpected_message);
}

TEST(tls13, evaluate, handshake) {
    // Every cipher suite, with every group, for each kind of server key.
    const trusted_root root;
    const unsigned int suites[3] = { handshake::suite_aes_128_gcm_sha256, handshake::suite_aes_256_gcm_sha384, handshake::suite_chacha20_poly1305_sha256 };
    const unsigned int groups[3] = { handshake::group_x25519, handshake::group_secp256r1, handshake::group_secp384r1 };
    unsigned long long int seed = 1;
    for (unsigned int is_rsa = 0; is_rsa < 2; ++is_rsa) {
        const server_identity identity(is_rsa != 0);
        for (const unsigned int suite : suites) {
            for (const unsigned int group : groups) {
                endpoint client;
                endpoint server;
                configure(client, server, root, identity, seed += 2);
                const client_hello hello(suite, group, true, false, true, client.settings.random_generator);
                REQUIRE(run(client, server, hello) == true, "Handshake failed for suite %04X group %u, alerts %u and %u.", suite, group, client.machine.get_alert(), server.machine.get_alert());
                REQUIRE(client.machine.get_state() == state_type::connected);
                REQUIRE(server.machine.get_state() == state_type::connected);
                REQUIRE(client.machine.get_suite() == suite);
                REQUIRE(server.machine.get_suite() == suite);
                REQUIRE(client.machine.get_peer_certificates().size() == 2);
                REQUIRE(client.machine.get_peer_certificates()[0] == identity.leaf);
                // The record that makes the connection look like an older version is sent by both ends, once.
                REQUIRE(client.change_cipher_specs == 1);
                REQUIRE(server.change_cipher_specs == 1);
                REQUIRE(client.is_handshake_only == true);
                REQUIRE(server.is_handshake_only == true);
                // The keys of each direction match.
                REQUIRE(can_send(client, server) == true);
                REQUIRE(can_send(server, client) == true);
                REQUIRE(can_send(client, server) == true);
            }
        }
    }
}

TEST(tls13, evaluate, update_keys) {
    const trusted_root root;
    const server_identity identity(false);
    endpoint client;
    endpoint server;
    configure(client, server, root, identity, 100);
    const client_hello hello(handshake::suite_aes_256_gcm_sha384, handshake::group_x25519, true, false, true, client.settings.random_generator);
    REQUIRE(run(client, server, hello) == true);

    // The client changes the keys it sends with, without asking the server to change its own.
    REQUIRE(client.machine.update_keys(false) == true);
    REQUIRE(client.sender.record.get_sequence() == 0);
    REQUIRE(deliver(client, server) == true);
    REQUIRE(server.sender.buffer.empty() == true);
    REQUIRE(server.receiver.get_sequence() == 0);
    REQUIRE(can_send(client, server) == true);
    REQUIRE(can_send(server, client) == true);

    // The server changes its keys and asks the client to do the same, which the client does in reply.
    REQUIRE(server.machine.update_keys(true) == true);
    REQUIRE(deliver(server, client) == true);
    REQUIRE(client.sender.buffer.empty() == false);
    REQUIRE(deliver(client, server) == true);
    REQUIRE(server.sender.buffer.empty() == true);
    REQUIRE(can_send(client, server) == true);
    REQUIRE(can_send(server, client) == true);
    for (unsigned int i = 0; i < 3; ++i) {
        REQUIRE(client.machine.update_keys(true) == true);
        REQUIRE(deliver(client, server) == true);
        REQUIRE(deliver(server, client) == true);
        REQUIRE(can_send(client, server) == true);
        REQUIRE(can_send(server, client) == true);
    }

    // An update that is not valid, or that shares a record with another message, fails the handshake.
    const unsigned char invalid[5] = { handshake::message_key_update, 0, 0, 1, 2 };
    REQUIRE(server.machine.process_message(invalid, 5, true) == false);
    REQUIRE(server.machine.get_alert() == handshake::alert_decode_error);
    const unsigned char valid[5] = { handshake::message_key_update, 0, 0, 1, 0 };
    REQUIRE(client.machine.process_message(valid, 5, false) == false);
    REQUIRE(client.machine.get_alert() == handshake::alert_unexpected_message);
    REQUIRE(client.machine.update_keys(true) == false);
}

TEST(tls13, evaluate, client_failures) {
    const trusted_root root;
    const server_identity identity(false);

    struct case_type {
        const char* description;
        unsigned int suite;
        unsigned int version;
        unsigned int group;
        bool wrong_session_id;
        bool is_retry;
        unsigned char alert;
    };

    const case_type cases[] = {
        { "a request to retry", handshake::suite_aes_128_gcm_sha256, handshake::version_tls13, handshake::group_secp256r1, false, true, handshake::alert_handshake_failure },
        { "a different session identifier", handshake::suite_aes_128_gcm_sha256, handshake::version_tls13, handshake::group_x25519, true, false, handshake::alert_illegal_parameter },
        { "a cipher suite of an older version", handshake::suite_ecdhe_ecdsa_aes_128_gcm_sha256, handshake::version_tls13, handshake::group_x25519, false, false, handshake::alert_illegal_parameter },
        { "an older version", handshake::suite_aes_128_gcm_sha256, handshake::version_tls12, handshake::group_x25519, false, false, handshake::alert_illegal_parameter },
        { "no version", handshake::suite_aes_128_gcm_sha256, 0, handshake::group_x25519, false, false, handshake::alert_illegal_parameter },
        { "no key share", handshake::suite_aes_128_gcm_sha256, handshake::version_tls13, 0, false, false, handshake::alert_missing_extension },
        { "a key share for a group that was not offered", handshake::suite_aes_128_gcm_sha256, handshake::version_tls13, handshake::group_secp256r1, false, false, handshake::alert_illegal_parameter },
    };
    for (const case_type& test : cases) {
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 200);
        const client_hello hello(handshake::suite_aes_128_gcm_sha256, handshake::group_x25519, true, false, true, client.settings.random_generator);
        gtl::tls13::share_type share;
        share.group = hello.group;
        REQUIRE(client.machine.start_client(client.settings, client.sender, client.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, &share, 1) == true);
        REQUIRE(client.machine.get_state() == state_type::wait_server_hello);
        const std::vector<unsigned char> server_hello = make_server_hello(hello, test.suite, test.version, test.group, test.wrong_session_id, test.is_retry);
        REQUIRE(client.machine.process_message(server_hello.data(), server_hello.size(), true) == false, "Accepted a hello with %s.", test.description);
        REQUIRE(client.machine.get_state() == state_type::failed);
        REQUIRE(client.machine.get_alert() == test.alert, "Unexpected alert %u for a hello with %s.", client.machine.get_alert(), test.description);
    }
    {
        // A valid hello must be the last message in its record, and any other message in its place is not expected.
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 200);
        const client_hello hello(handshake::suite_aes_128_gcm_sha256, handshake::group_x25519, true, false, true, client.settings.random_generator);
        gtl::tls13::share_type share;
        share.group = hello.group;
        const std::vector<unsigned char> server_hello = make_server_hello(hello, handshake::suite_aes_128_gcm_sha256, handshake::version_tls13, handshake::group_x25519, false, false);
        REQUIRE(client.machine.start_client(client.settings, client.sender, client.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, &share, 1) == true);
        REQUIRE(client.machine.process_message(server_hello.data(), server_hello.size(), false) == false);
        REQUIRE(client.machine.get_alert() == handshake::alert_unexpected_message);
        REQUIRE(server.machine.start_client(client.settings, client.sender, client.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, &share, 1) == true);
        const unsigned char finished[4] = { handshake::message_finished, 0, 0, 0 };
        REQUIRE(server.machine.process_message(finished, 4, true) == false);
        REQUIRE(server.machine.get_alert() == handshake::alert_unexpected_message);
        REQUIRE(server.machine.process_message(nullptr, 0, true) == false);
        REQUIRE(server.machine.get_alert() == handshake::alert_decode_error);
    }
    {
        // A client cannot be started as a server, twice, or with too many key shares.
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 200);
        const client_hello hello(handshake::suite_aes_128_gcm_sha256, handshake::group_x25519, true, false, true, client.settings.random_generator);
        gtl::tls13::share_type shares[3];
        REQUIRE(server.machine.start_client(server.settings, server.sender, server.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, shares, 1) == false);
        REQUIRE(client.machine.start_client(client.settings, client.sender, client.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, shares, 3) == false);
        endpoint other;
        configure(other, server, root, identity, 200);
        REQUIRE(other.machine.start_client(other.settings, other.sender, other.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, shares, 1) == true);
        REQUIRE(other.machine.start_client(other.settings, other.sender, other.receiver, hello.message.data(), hello.message.size(), hello.session_id, handshake::session_id_size_max, shares, 1) == false);
    }
}

TEST(tls13, evaluate, server_failures) {
    const trusted_root root;
    const server_identity identity(false);
    const server_identity identity_rsa(true);
    {
        // A client that offers no key share, or only cipher suites of an older version.
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 300);
        const client_hello hello(handshake::suite_aes_128_gcm_sha256, handshake::group_x25519, true, false, false, client.settings.random_generator);
        REQUIRE(server.machine.start_server(server.settings, server.sender, server.receiver, hello.parsed, hello.message.data(), hello.message.size()) == false);
        REQUIRE(server.machine.get_state() == state_type::failed);
        REQUIRE(server.machine.get_alert() == handshake::alert_missing_extension);
        REQUIRE(server.sender.buffer.empty() == true);
    }
    {
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 300);
        const client_hello hello(handshake::suite_ecdhe_ecdsa_aes_128_gcm_sha256, handshake::group_x25519, true, false, true, client.settings.random_generator);
        REQUIRE(server.machine.start_server(server.settings, server.sender, server.receiver, hello.parsed, hello.message.data(), hello.message.size()) == false);
        REQUIRE(server.machine.get_alert() == handshake::alert_handshake_failure);
    }
    {
        // A server needs a certificate and a private key, and cannot be started as a client.
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 300);
        const client_hello hello(handshake::suite_aes_128_gcm_sha256, handshake::group_x25519, true, false, true, client.settings.random_generator);
        REQUIRE(client.machine.start_server(client.settings, client.sender, client.receiver, hello.parsed, hello.message.data(), hello.message.size()) == false);
        server.settings.private_key = nullptr;
        REQUIRE(server.machine.start_server(server.settings, server.sender, server.receiver, hello.parsed, hello.message.data(), hello.message.size()) == false);
        REQUIRE(server.machine.get_alert() == handshake::alert_internal_error);
    }
    {
        // A finished message from the client that is wrong, which is what a client without the right keys would send.
        endpoint client;
        endpoint server;
        configure(client, server, root, identity_rsa, 300);
        const client_hello hello(handshake::suite_chacha20_poly1305_sha256, handshake::group_secp256r1, true, false, true, client.settings.random_generator);
        REQUIRE(server.machine.start_server(server.settings, server.sender, server.receiver, hello.parsed, hello.message.data(), hello.message.size()) == true);
        REQUIRE(server.machine.get_state() == state_type::wait_client_finished);
        unsigned char finished[36] = { handshake::message_finished, 0, 0, 32 };
        REQUIRE(server.machine.process_message(finished, 36, true) == false);
        REQUIRE(server.machine.get_alert() == handshake::alert_decrypt_error);
    }
    {
        // Anything other than a finished message from the client.
        endpoint client;
        endpoint server;
        configure(client, server, root, identity, 300);
        const client_hello hello(handshake::suite_aes_128_gcm_sha256, handshake::group_x25519, true, false, true, client.settings.random_generator);
        REQUIRE(server.machine.start_server(server.settings, server.sender, server.receiver, hello.parsed, hello.message.data(), hello.message.size()) == true);
        const unsigned char certificate[8] = { handshake::message_certificate, 0, 0, 4, 0, 0, 0, 0 };
        REQUIRE(server.machine.process_message(certificate, 8, true) == false);
        REQUIRE(server.machine.get_alert() == handshake::alert_unexpected_message);
    }
}
