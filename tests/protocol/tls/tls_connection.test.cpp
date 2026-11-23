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

#include <protocol/tls/tls_connection>

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

    /// @brief  Move every byte of output from one connection to another, in chunks of a size.
    bool pump(gtl::tls_connection& from, gtl::tls_connection& to, const unsigned long long int chunk = 1024) {
        std::vector<unsigned char> buffer(chunk, 0);
        bool moved = false;
        for (unsigned long long int length = from.get_output(buffer.data(), chunk); length != 0; length = from.get_output(buffer.data(), chunk)) {
            static_cast<void>(to.process_input(buffer.data(), length));
            moved = true;
        }
        return moved;
    }

    /// @brief  Run two connections against each other until neither has anything more to send.
    void exchange(gtl::tls_connection& lhs, gtl::tls_connection& rhs, const unsigned long long int chunk = 1024) {
        for (unsigned int i = 0; i < 32; ++i) {
            const bool moved_lhs = pump(lhs, rhs, chunk);
            const bool moved_rhs = pump(rhs, lhs, chunk);
            if (!moved_lhs && !moved_rhs) {
                break;
            }
        }
    }

    /// @brief  Read everything that is waiting to be read.
    std::vector<unsigned char> read_all(gtl::tls_connection& connection) {
        std::vector<unsigned char> data;
        unsigned char buffer[1000];
        for (unsigned long long int length = connection.read(buffer, sizeof(buffer)); length != 0; length = connection.read(buffer, sizeof(buffer))) {
            data.insert(data.end(), buffer, buffer + length);
        }
        return data;
    }

    /// @brief  Make the settings of a client.
    gtl::tls_handshake::settings_type make_client_settings(const trusted_root& root, const server_identity& identity, const unsigned long long int seed) {
        gtl::tls_handshake::settings_type settings;
        settings.is_client = true;
        settings.random_generator = pcg_generator(seed);
        settings.host = identity.host;
        settings.trusted = &root.parsed;
        settings.trusted_length = 1;
        settings.time = time_now;
        return settings;
    }

    /// @brief  Make the settings of a server.
    gtl::tls_handshake::settings_type make_server_settings(const server_identity& identity, const unsigned long long int seed) {
        gtl::tls_handshake::settings_type settings;
        settings.is_client = false;
        settings.random_generator = pcg_generator(seed);
        settings.certificates = identity.certificates;
        settings.certificate_lengths = identity.certificate_lengths;
        settings.certificates_length = 2;
        settings.private_key = &identity.private_key;
        return settings;
    }

    using state_type = gtl::tls_connection::state_type;
}

TEST(tls_connection, traits, standard) {
    REQUIRE((std::is_pod<gtl::tls_connection>::value == false));

    REQUIRE((std::is_trivial<gtl::tls_connection>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::tls_connection>::value == false));

    REQUIRE((std::is_copy_constructible<gtl::tls_connection>::value == false));
}

TEST(tls_connection, constructor, empty) {
    gtl::tls_connection connection;
    REQUIRE(connection.get_state() == state_type::idle);
    REQUIRE(connection.is_connected() == false);
    REQUIRE(connection.get_alert() == 0);
    REQUIRE(connection.is_alert_received() == false);
    REQUIRE(connection.get_version() == 0);
    REQUIRE(connection.get_suite() == 0);
    REQUIRE(connection.get_output_length() == 0);
    REQUIRE(connection.get_read_length() == 0);
    REQUIRE(connection.get_peer_certificates().empty() == true);
}

TEST(tls_connection, function, start) {
    const trusted_root root;
    const server_identity identity(false);
    {
        gtl::tls_connection connection;
        gtl::tls_handshake::settings_type settings = make_client_settings(root, identity, 1);
        REQUIRE(connection.start(settings, false, false) == false);
        settings.random_generator = std::function<unsigned int()>();
        REQUIRE(connection.start(settings) == false);
        REQUIRE(connection.get_state() == state_type::idle);
        settings.random_generator = pcg_generator(1);
        REQUIRE(connection.start(settings) == true);
        REQUIRE(connection.get_state() == state_type::handshake);
        REQUIRE(connection.get_output_length() > 0);
        REQUIRE(connection.start(settings) == false);
    }
    {
        gtl::tls_connection connection;
        gtl::tls_handshake::settings_type settings = make_server_settings(identity, 2);
        settings.private_key = nullptr;
        REQUIRE(connection.start(settings) == false);
        settings.private_key = &identity.private_key;
        settings.certificates_length = 0;
        REQUIRE(connection.start(settings) == false);
        settings.certificates_length = 2;
        REQUIRE(connection.start(settings) == true);
        REQUIRE(connection.get_state() == state_type::handshake);
        REQUIRE(connection.get_output_length() == 0);
    }
}

TEST(tls_connection, evaluate, handshake) {
    const trusted_root root;
    // Each kind of server key, with each combination of versions that leads to each version, delivered in chunks of different sizes.
    unsigned long long int seed = 100;
    for (unsigned int is_rsa = 0; is_rsa < 2; ++is_rsa) {
        const server_identity identity(is_rsa != 0);
        for (unsigned int versions = 0; versions < 3; ++versions) {
            const unsigned long long int chunks[3] = { 1, 37, 4096 };
            const gtl::tls_handshake::settings_type client_settings = make_client_settings(root, identity, seed++);
            const gtl::tls_handshake::settings_type server_settings = make_server_settings(identity, seed++);
            gtl::tls_connection client;
            gtl::tls_connection server;
            REQUIRE(server.start(server_settings, true, versions != 1) == true);
            REQUIRE(client.start(client_settings, true, versions != 2) == true);
            exchange(client, server, chunks[versions]);
            REQUIRE(client.is_connected() == true, "The client did not connect, alert %u.", client.get_alert());
            REQUIRE(server.is_connected() == true, "The server did not connect, alert %u.", server.get_alert());
            const unsigned int version = (versions == 0) ? gtl::tls_handshake::version_tls13 : gtl::tls_handshake::version_tls12;
            REQUIRE(client.get_version() == version);
            REQUIRE(server.get_version() == version);
            REQUIRE(client.get_suite() != 0);
            REQUIRE(client.get_suite() == server.get_suite());
            REQUIRE(client.get_peer_certificates().size() == 2);
            REQUIRE(client.get_peer_certificates()[0] == identity.leaf);
            REQUIRE(server.get_peer_certificates().empty() == true);
            REQUIRE(client.get_output_length() == 0);
            REQUIRE(server.get_output_length() == 0);

            // Data of many sizes in both directions.
            const unsigned long long int sizes[7] = { 1, 100, 16383, 16384, 16385, 40000, 100000 };
            for (const unsigned long long int size : sizes) {
                std::vector<unsigned char> data(size, 0);
                for (unsigned long long int i = 0; i < size; ++i) {
                    data[i] = static_cast<unsigned char>((i * 7) + size);
                }
                REQUIRE(client.write(data.data(), size) == true);
                REQUIRE(pump(client, server, 3000) == true);
                REQUIRE(server.get_read_length() == size);
                REQUIRE(read_all(server) == data);
                REQUIRE(server.write(data.data(), size) == true);
                REQUIRE(pump(server, client, 3000) == true);
                REQUIRE(read_all(client) == data);
            }

            // The server closes, and the client closes in reply.
            server.close();
            REQUIRE(server.get_state() == state_type::closed);
            exchange(server, client);
            REQUIRE(client.get_state() == state_type::closed);
            REQUIRE(client.write(root.encoded.data(), 4) == false);
            REQUIRE(server.write(root.encoded.data(), 4) == false);
            // Input after closing is ignored.
            REQUIRE(client.process_input(root.encoded.data(), 16) == true);
            REQUIRE(client.get_state() == state_type::closed);
        }
    }
}

TEST(tls_connection, evaluate, corrupted) {
    const trusted_root root;
    const server_identity identity(false);
    for (unsigned int versions = 0; versions < 2; ++versions) {
        // Record everything each end sends in a handshake and a short exchange of data.
        std::vector<unsigned char> sent[2];
        const auto run = [&](const unsigned int corrupt_end, const unsigned long long int corrupt_index, bool& data_arrived) -> bool {
            const gtl::tls_handshake::settings_type client_settings = make_client_settings(root, identity, 200);
            const gtl::tls_handshake::settings_type server_settings = make_server_settings(identity, 201);
            gtl::tls_connection ends[2];
            REQUIRE(ends[0].start(client_settings, true, versions == 0) == true);
            REQUIRE(ends[1].start(server_settings, true, versions == 0) == true);
            unsigned long long int counts[2] = { 0, 0 };
            bool wrote = false;
            data_arrived = false;
            for (unsigned int round = 0; round < 32; ++round) {
                bool moved = false;
                for (unsigned int end = 0; end < 2; ++end) {
                    unsigned char byte = 0;
                    while (ends[end].get_output(&byte, 1) == 1) {
                        if (corrupt_end == 2) {
                            sent[end].push_back(byte);
                        }
                        else if ((corrupt_end == end) && (counts[end] == corrupt_index)) {
                            byte ^= 0x10;
                        }
                        ++counts[end];
                        static_cast<void>(ends[1 - end].process_input(&byte, 1));
                        moved = true;
                    }
                }
                if (!wrote && ends[0].is_connected() && ends[1].is_connected()) {
                    wrote = true;
                    REQUIRE(ends[0].write(root.encoded.data(), 50) == true);
                    REQUIRE(ends[1].write(root.encoded.data(), 60) == true);
                    moved = true;
                }
                if (!moved) {
                    break;
                }
            }
            data_arrived = (read_all(ends[0]).size() == 60) && (read_all(ends[1]).size() == 50);
            return ends[0].is_connected() && ends[1].is_connected();
        };
        bool data_arrived = false;
        REQUIRE(run(2, 0, data_arrived) == true);
        REQUIRE(data_arrived == true);
        REQUIRE(sent[0].size() > 200);
        REQUIRE(sent[1].size() > 1000);

        // Changing a byte of any record that either end sends is detected: the connection fails, or at least the data does not arrive.
        // The first, middle, and last byte of the body of each record are changed, as the version in the header of a record is ignored.
        for (unsigned int end = 0; end < 2; ++end) {
            unsigned int records = 0;
            for (unsigned long long int offset = 0; (offset + gtl::tls_record::header_size) <= sent[end].size(); ++records) {
                const unsigned long long int body_length = (static_cast<unsigned long long int>(sent[end][offset + 3]) << 8) | sent[end][offset + 4];
                const unsigned long long int body = offset + gtl::tls_record::header_size;
                const unsigned long long int indices[4] = { offset, body, body + (body_length / 2), body + body_length - 1 };
                for (const unsigned long long int index : indices) {
                    const bool connected = run(end, index, data_arrived);
                    REQUIRE(!(connected && data_arrived), "A change to byte %llu sent by end %u was not detected.", index, end);
                }
                offset = body + body_length;
            }
            REQUIRE(records >= 3);
        }
    }
}

TEST(tls_connection, evaluate, alerts) {
    const trusted_root root;
    const server_identity identity(false);
    for (unsigned int versions = 0; versions < 2; ++versions) {
        const gtl::tls_handshake::settings_type client_settings = make_client_settings(root, identity, 300);
        const gtl::tls_handshake::settings_type server_settings = make_server_settings(identity, 301);
        gtl::tls_connection client;
        gtl::tls_connection server;
        REQUIRE(server.start(server_settings, true, versions == 0) == true);
        REQUIRE(client.start(client_settings, true, versions == 0) == true);
        exchange(client, server);
        REQUIRE(client.is_connected() == true);
        REQUIRE(server.is_connected() == true);

        // A record that is not authentic fails the connection, and the alert that reports it fails the peer.
        REQUIRE(client.write(root.encoded.data(), 32) == true);
        std::vector<unsigned char> record(client.get_output_length(), 0);
        REQUIRE(client.get_output(record.data(), record.size()) == record.size());
        record[record.size() - 1] ^= 0x01;
        REQUIRE(server.process_input(record.data(), record.size()) == false);
        REQUIRE(server.get_state() == state_type::failed);
        REQUIRE(server.get_alert() == gtl::tls_handshake::alert_bad_record_mac);
        REQUIRE(server.is_alert_received() == false);
        REQUIRE(server.get_read_length() == 0);
        REQUIRE(pump(server, client) == true);
        REQUIRE(client.get_state() == state_type::failed);
        REQUIRE(client.get_alert() == gtl::tls_handshake::alert_bad_record_mac);
        REQUIRE(client.is_alert_received() == true);
        // Nothing more can be sent or received.
        REQUIRE(client.write(root.encoded.data(), 4) == false);
        REQUIRE(client.process_input(record.data(), record.size()) == false);
        REQUIRE(server.write(root.encoded.data(), 4) == false);
        REQUIRE(client.get_output_length() == 0);
    }
}
