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
#include <testbench/print.tests.hpp>
#include <testbench/require.tests.hpp>

#include <crypto/certificate/pem>
#include <crypto/certificate/x509>
#include <io/socket>
#include <protocol/http>
#include <protocol/tls/tls_client>
#include <protocol/tls/tls_server>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <random>
#include <string>
#include <thread>
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

    // The bytes sent by the client of another implementation, OpenSSL 3.0, in handshakes with a server that used the random generator with these seeds.
    // Each client then sent the line "hello from the openssl client".
    struct recording_type {
        const char* description;
        unsigned long long int seed;
        bool is_rsa;
        bool allow_tls12;
        bool allow_tls13;
        unsigned int version;
        unsigned int suite;
        const char* bytes;
    };

    constexpr const recording_type recordings[] = {
        { "TLS 1.3, ecdsa, aes-128-gcm", 201, false, true, true, 0x0304, 0x1301, "16030100D8010000D403030A240703D260AA63EFC185F410121465BA8BFAC53880AD8901433C6CF87F9BBE20481500F9D7FEB03048691DA48F0D834113B1A441102FB701C6CD8B5344B2AF5D0004130100FF01000087000B000403000102000A00160014001D0017001E0019001801000101010201030104002300000016000000170000000D001E001C040305030603080708080809080A080B080408050806040105010601002B0003020304002D00020101003300260024001D002004A53583D07DEB17964B112B65CFED54FF7042B01FB38E96DE67930DBFF9F83E14030300010117030300350BFA41FF454744E0F882A0EB4B0AC6D64FCBE5048FD2385916D0F7CEA3C9D56760FA102B405993A2930769BBF0C30D32BA7A8F15A5170303002FF8C40221C2A383E44E061774E1B217A4C0F8ADE6900D3BB842929A8B0CF4AAE2EB89F05BFE5F032DF0884D2D0C7DEC" },
        { "TLS 1.3, rsa, aes-256-gcm, secp384r1", 202, true, true, true, 0x0304, 0x1302, "16030101070100010303032C694ECEA818801445F0A0A4B069C2EFE644E07CC1562C4C6590A0F86A89EBE920BF3B45890E142283E0D0025419B07D45CD55D65B147BD0BC3DAA62A07C4FBEFE0004130200FF010000B6000B000403000102000A000400020018002300000016000000170000000D001E001C040305030603080708080809080A080B080408050806040105010601002B0003020304002D00020101003300670065001800610459BE82D1676FE08EC6E4CB9412C4318683D628E85DF29BDDD18F29B4D9553B527B4F330C8306E45B5A3FBC1F08492BFB8AB9C920D12C71030D7D42C1D976C22DB84149FB5C88C822471F189C74461FBBB3FF06D39A0F829BE0C612382E44B72A140303000101170303004546412F5A9204E2C60B88B9C1685A832F8C32B3E4483B24B4AB0C77F9A6DA8CBEA807F8A635F143E0B477B651CE64F6DCB1CA295717785C0C4B5997F4532ABE7D4CD54B75F0170303002FA57C581CC7563A5ABEB2794B6D7D7EB35E62E8287B1D3F3E86E086FEBDDBA9230457BDD84223F8CBC2061BB2C47622" },
        { "TLS 1.3, ecdsa, chacha20-poly1305", 203, false, false, true, 0x0304, 0x1303, "160301011C01000118030315BB365CC0FE7A1D08E247AF0E97A7D82BAB4849257833F117D5AF2A469DCA9920647A226CC8187EB1312340D98DE7647C603D3A70B4DA3994FAB67C039854AD55003A1303C02CC030009FCCA9CCA8CCAAC02BC02F009EC024C028006BC023C0270067C00AC0140039C009C0130033009D009C003D003C0035002F00FF01000095000B000403000102000A00160014001D0017001E0019001801000101010201030104002300000016000000170000000D002A0028040305030603080708080809080A080B080408050806040105010601030303010302040205020602002B00050403040303002D00020101003300260024001D0020BD651EEF313105592250F6FACE2148A0B572B91D6E188A0B0761D1B3E3D0C76B14030300010117030300350AB0205643BD7081076CBFDAC0B6A538D4604EE37CC1E1B3BD3F10EBE6D184CDF21CE99A623583532503926B9EF38AD35538F1B79E170303002F2CEAF14533A28E73E617EBA89B3DCBDD94434AC28E530EE63D8031633A1C60B4D18A24FECBA6C435967F044CD8E3E3" },
        { "TLS 1.2, ecdsa, chacha20-poly1305", 204, false, true, true, 0x0303, 0xCCA9, "16030100830100007F0303B3F53F0DF1A537DAB529B5CCC18803E8B44B6996D367913B524B6D0497C61587000004CCA900FF01000052000B000403000102000A000C000A001D0017001E00190018002300000016000000170000000D002A0028040305030603080708080809080A080B080408050806040105010601030303010302040205020602160303002510000021200308763C07F9E9AC52EAB7D62C843EC48DAC63FF69EA97F1B67422873625CD721403030001011603030020E5D4308D38016B460684ADDAC8FC53E033A048416CD680896C7788C3EF16F3A4170303002EAD025B66E17C51CE81DCAED624910F7C62C0ED0784F96E9B92199A3B1CA9364638C86D53B9CB445C857DF7D0B72D" },
        { "TLS 1.2, rsa, aes-128-gcm, secp256r1", 205, true, true, true, 0x0303, 0xC02F, "160301007B0100007703032D659D4FE9679110C57108BC5FC2F58541900DAFFF379AF96E3635A99A5393E1000004C02F00FF0100004A000B000403000102000A000400020017002300000016000000170000000D002A0028040305030603080708080809080A080B08040805080604010501060103030301030204020502060216030300461000004241048BC236C21C9861AF166C5662D64FA612B74CF2C675C02DE279903F720D3340FD38813BF2E953A1EA2D9EFDBBC79B7728560DA2B68B8633C2848FAA0168F779AB1403030001011603030028F2DC964FF8D3E58C9958118567DC8877C519321AE0C68825AF8EE2CDAA677F9F6913D04FEDA6854A1703030036F2DC964FF8D3E58D6D791677C7BE5C565B7C622A09441497A05CC75B50F418A67D8B04B48AF31156FB899119A0E6C1C1CFE2856FE362" },
        { "TLS 1.2, rsa, aes-256-gcm, older rsa padding", 206, true, true, false, 0x0303, 0xC030, "16030100C6010000C2030372CAF077069806E03B46979CF2004599080944BFDA472E54F447F02E4CA09797209B99790725A44FD09586FFAB35F5421DF4B408BB09A53E3BA56F63636B1AAF55000A130213031301C03000FF0100006F000B000403000102000A00160014001D0017001E0019001801000101010201030104002300000016000000170000000D000400020501002B00050403040303002D00020101003300260024001D0020425949F14AFEEB5E33B519913A257516EA2C6A4F4C02EDB4A7637F6473DD8F1E1603030025100000212051FB22AAF7C92DDBEADDDBEFC2E451B2C868C628A18C07DE20EDBDC3CCAF743F14030300010116030300286162FA110A6E481BBBB6C9BF6D515F8F0E29C8B46E861B22C0D007DAD20ACD040C2D92F0C0ECE39017030300366162FA110A6E481C3704BFB2EDA3C7A53781037B888B4977323CA9B7E0E819BAF37E6AEAF081CFFD616D3193D686E9303B56B140125C" },
    };

    /// @brief  Move every byte of output from one end of a connection to the other.
    template <typename from_type, typename to_type>
    bool pump(from_type& from, to_type& to) {
        unsigned char buffer[1024];
        bool moved = false;
        for (unsigned long long int length = from.get_output(buffer, sizeof(buffer)); length != 0; length = from.get_output(buffer, sizeof(buffer))) {
            static_cast<void>(to.process_input(buffer, length));
            moved = true;
        }
        return moved;
    }

    /// @brief  Read everything that is waiting to be read as a string.
    template <typename connection_type>
    std::string read_all(connection_type& connection) {
        std::string data;
        unsigned char buffer[256];
        for (unsigned long long int length = connection.read(buffer, sizeof(buffer)); length != 0; length = connection.read(buffer, sizeof(buffer))) {
            data.append(reinterpret_cast<const char*>(buffer), length);
        }
        return data;
    }

    /// @brief  Make the configuration of a server from an identity.
    gtl::tls_server::configuration_type make_configuration(const server_identity& identity, const unsigned long long int seed) {
        gtl::tls_server::configuration_type configuration;
        configuration.random_generator = pcg_generator(seed);
        configuration.certificates = identity.certificates;
        configuration.certificate_lengths = identity.certificate_lengths;
        configuration.certificates_length = 2;
        configuration.private_key = &identity.private_key;
        return configuration;
    }
}

TEST(tls_server, traits, standard) {
    REQUIRE((std::is_pod<gtl::tls_server>::value == false));

    REQUIRE((std::is_trivial<gtl::tls_server>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::tls_server>::value == false));

    REQUIRE((std::is_copy_constructible<gtl::tls_server>::value == false));
}

TEST(tls_server, constructor, empty) {
    gtl::tls_server server;
    REQUIRE(server.get_state() == gtl::tls_server::state_type::idle);
    REQUIRE(server.is_connected() == false);
    REQUIRE(server.get_version() == 0);
    REQUIRE(server.get_suite() == 0);
    REQUIRE(server.get_output_length() == 0);
    REQUIRE(server.get_read_length() == 0);
    // Nothing can be done with a server that has not been started.
    const unsigned char data[4] = { 1, 2, 3, 4 };
    unsigned char buffer[16] = {};
    REQUIRE(server.process_input(data, 4) == false);
    REQUIRE(server.write(data, 4) == false);
    REQUIRE(server.read(buffer, sizeof(buffer)) == 0);
    REQUIRE(server.get_output(buffer, sizeof(buffer)) == 0);
    server.close();
    REQUIRE(server.get_state() == gtl::tls_server::state_type::idle);
}

TEST(tls_server, function, start) {
    const server_identity identity(false);
    const gtl::tls_server::configuration_type valid = make_configuration(identity, 1);
    {
        gtl::tls_server server;
        REQUIRE(server.start(valid) == true);
        REQUIRE(server.get_state() == gtl::tls_server::state_type::handshake);
        // A server waits for the client to send first, and cannot be started twice.
        REQUIRE(server.get_output_length() == 0);
        REQUIRE(server.start(valid) == false);
    }
    // A random generator, a certificate, and a private key are all required, and at least one version must be allowed.
    for (unsigned int missing = 0; missing < 6; ++missing) {
        gtl::tls_server::configuration_type configuration = valid;
        if (missing == 0) {
            configuration.random_generator = std::function<unsigned int()>();
        }
        else if (missing == 1) {
            configuration.certificates = nullptr;
        }
        else if (missing == 2) {
            configuration.certificate_lengths = nullptr;
        }
        else if (missing == 3) {
            configuration.certificates_length = 0;
        }
        else if (missing == 4) {
            configuration.private_key = nullptr;
        }
        else {
            configuration.allow_tls12 = false;
            configuration.allow_tls13 = false;
        }
        gtl::tls_server server;
        REQUIRE(server.start(configuration) == false);
        REQUIRE(server.get_state() == gtl::tls_server::state_type::idle);
    }
}

TEST(tls_server, evaluate, recorded_clients) {
    // Replay what real clients sent, which only works if every message, key, and record of the server is exactly right.
    for (const recording_type& recording : recordings) {
        const server_identity identity(recording.is_rsa);
        const std::vector<unsigned char> bytes = hex_2_bytes(recording.bytes);
        gtl::tls_server::configuration_type configuration = make_configuration(identity, recording.seed);
        configuration.allow_tls12 = recording.allow_tls12;
        configuration.allow_tls13 = recording.allow_tls13;
        // Deliver the bytes all at once, and then again one byte at a time.
        for (unsigned int pass = 0; pass < 2; ++pass) {
            gtl::tls_server server;
            REQUIRE(server.start(configuration) == true);
            if (pass == 0) {
                REQUIRE(server.process_input(bytes.data(), bytes.size()) == true, "Failed to replay '%s', alert %u.", recording.description, server.get_alert());
            }
            else {
                for (const unsigned char& byte : bytes) {
                    REQUIRE_SILENT(server.process_input(&byte, 1) == true);
                }
            }
            REQUIRE(server.is_connected() == true, "Not connected after replaying '%s', alert %u.", recording.description, server.get_alert());
            REQUIRE(server.get_version() == recording.version);
            REQUIRE(server.get_suite() == recording.suite);
            REQUIRE(read_all(server) == "hello from the openssl client\n");
            REQUIRE(server.get_output_length() > 0);
        }

        // A server with a different random generator has different keys, so cannot use the same bytes.
        {
            configuration.random_generator = pcg_generator(recording.seed + 1000);
            gtl::tls_server server;
            REQUIRE(server.start(configuration) == true);
            REQUIRE(server.process_input(bytes.data(), bytes.size()) == false);
            REQUIRE(server.get_state() == gtl::tls_server::state_type::failed);
            REQUIRE(server.is_alert_received() == false);
            configuration.random_generator = pcg_generator(recording.seed);
        }

        // Changing a byte in the middle of any record that the client sent never gives the server different data, and changing the hello always fails the handshake.
        unsigned int failures = 0;
        unsigned int record_index = 0;
        for (unsigned long long int offset = 0; (offset + gtl::tls_record::header_size) <= bytes.size(); ++record_index) {
            const unsigned long long int body_length = (static_cast<unsigned long long int>(bytes[offset + 3]) << 8) | bytes[offset + 4];
            std::vector<unsigned char> changed = bytes;
            changed[offset + gtl::tls_record::header_size + (body_length / 2)] ^= 0x04;
            gtl::tls_server server;
            REQUIRE(server.start(configuration) == true);
            static_cast<void>(server.process_input(changed.data(), changed.size()));
            const std::string data = read_all(server);
            REQUIRE(data.empty() || (data == "hello from the openssl client\n"));
            if (record_index == 0) {
                REQUIRE(data.empty() == true);
                REQUIRE(server.get_state() == gtl::tls_server::state_type::failed);
            }
            failures += (server.get_state() == gtl::tls_server::state_type::failed) ? 1 : 0;
            offset += gtl::tls_record::header_size + body_length;
        }
        REQUIRE(record_index >= 3);
        REQUIRE(failures >= 3);
    }
}

TEST(tls_server, evaluate, versions) {
    const trusted_root root;
    const server_identity identity(false);

    struct case_type {
        bool server_tls12;
        bool server_tls13;
        bool client_tls12;
        bool client_tls13;
        unsigned int version;
    };

    const case_type cases[] = {
        { true, true, true, true, 0x0304 },
        { true, true, true, false, 0x0303 },
        { true, true, false, true, 0x0304 },
        { true, false, true, true, 0x0303 },
        { false, true, true, true, 0x0304 },
        { true, false, true, false, 0x0303 },
        { false, true, false, true, 0x0304 },
        { true, false, false, true, 0 },
        { false, true, true, false, 0 },
    };
    for (const case_type& test : cases) {
        gtl::tls_server::configuration_type server_configuration = make_configuration(identity, 30);
        server_configuration.allow_tls12 = test.server_tls12;
        server_configuration.allow_tls13 = test.server_tls13;
        gtl::tls_client::configuration_type client_configuration;
        client_configuration.random_generator = pcg_generator(31);
        client_configuration.host = identity.host;
        client_configuration.trusted = &root.parsed;
        client_configuration.trusted_length = 1;
        client_configuration.time = time_now;
        client_configuration.allow_tls12 = test.client_tls12;
        client_configuration.allow_tls13 = test.client_tls13;
        gtl::tls_server server;
        gtl::tls_client client;
        REQUIRE(server.start(server_configuration) == true);
        REQUIRE(client.start(client_configuration) == true);
        for (unsigned int i = 0; i < 16; ++i) {
            const bool moved_client = pump(client, server);
            const bool moved_server = pump(server, client);
            if (!moved_client && !moved_server) {
                break;
            }
        }
        if (test.version != 0) {
            REQUIRE(server.is_connected() == true);
            REQUIRE(client.is_connected() == true);
            REQUIRE(server.get_version() == test.version);
            REQUIRE(client.get_version() == test.version);
            REQUIRE(server.get_suite() == client.get_suite());
        }
        else {
            // With no version in common the server refuses the client, and tells it why.
            REQUIRE(server.get_state() == gtl::tls_server::state_type::failed);
            REQUIRE(server.get_alert() == gtl::tls_handshake::alert_protocol_version);
            REQUIRE(server.is_alert_received() == false);
            REQUIRE(client.get_state() == gtl::tls_client::state_type::failed);
            REQUIRE(client.get_alert() == gtl::tls_handshake::alert_protocol_version);
            REQUIRE(client.is_alert_received() == true);
        }
    }
}

TEST(tls_server, evaluate, invalid_input) {
    const server_identity identity(false);
    const gtl::tls_server::configuration_type configuration = make_configuration(identity, 40);
    const char* inputs[] = {
        // A request that is not encrypted.
        "GET / HTTP/1.1\r\nHost: example.test\r\n\r\n",
        // A record of application data before any handshake.
        "\x17\x03\x03\x00\x04"
        "abcd",
        // A handshake record that holds a message that is not a hello.
        "\x16\x03\x03\x00\x04\x0B\x00\x00\x00",
        // A hello message that is cut short.
        "\x16\x03\x03\x00\x06\x01\x00\x00\x02\x03\x03",
        // A handshake record with nothing in it.
        "\x16\x03\x03\x00\x00\x16\x03\x03\x00\x00",
        // A record that is longer than is allowed.
        "\x16\x03\x03\xFF\xFF",
        // A change of cipher before any handshake.
        "\x14\x03\x03\x00\x01\x01",
    };
    const unsigned long long int lengths[] = { 38, 9, 9, 11, 10, 5, 6 };
    unsigned int index = 0;
    for (const char* input : inputs) {
        gtl::tls_server server;
        REQUIRE(server.start(configuration) == true);
        REQUIRE(server.process_input(reinterpret_cast<const unsigned char*>(input), lengths[index]) == false, "Accepted invalid input %u.", index);
        REQUIRE(server.get_state() == gtl::tls_server::state_type::failed);
        REQUIRE(server.is_alert_received() == false);
        // The client is sent a fatal alert, and nothing more is accepted.
        unsigned char output[16] = {};
        REQUIRE(server.get_output(output, sizeof(output)) == 7);
        REQUIRE(output[0] == gtl::tls_record::content_alert);
        REQUIRE(output[5] == gtl::tls_handshake::alert_level_fatal);
        REQUIRE(output[6] == server.get_alert());
        REQUIRE(server.process_input(reinterpret_cast<const unsigned char*>(input), lengths[index]) == false);
        REQUIRE(server.get_output_length() == 0);
        ++index;
    }
    // A fatal alert from the client ends the connection without a reply.
    {
        const unsigned char alert[7] = { 0x15, 0x03, 0x03, 0x00, 0x02, 0x02, 0x28 };
        gtl::tls_server server;
        REQUIRE(server.start(configuration) == true);
        REQUIRE(server.process_input(alert, sizeof(alert)) == false);
        REQUIRE(server.get_state() == gtl::tls_server::state_type::failed);
        REQUIRE(server.get_alert() == gtl::tls_handshake::alert_handshake_failure);
        REQUIRE(server.is_alert_received() == true);
        REQUIRE(server.get_output_length() == 0);
    }
}

namespace {
    // The sample server below shows how to use the tls_server class with a socket and the http class, which together make an https server.
    // It can be tried against the sample client in the tests of the tls_client class, or against other programs.
    // By default the server only serves a client that is already trying to connect, and the client only tries to connect once, so that the tests do not wait.
    // Set this to true, here and in the tests of the tls_client class, to make the server wait for a client, and the client wait for a server.
    // The server then also writes its certificate to a file, so that other programs can be told to trust it:
    //     curl --cacert gtl_sample_certificate.pem https://localhost:48443/hello
    constexpr const bool sample_wait = false;

    // The longest time that the sample server and client wait for each other.
    constexpr const unsigned int sample_wait_seconds = 60;

    // The port of the sample server.
    constexpr const unsigned short sample_port = 48443;

    /// @brief  The private key and certificate of the sample server, which are made here rather than loaded from files.
    /// @note   The private key is fixed so that the sample client can make the same certificate and trust it.
    /// @note   A real server must make its private key from an unpredictable source of random numbers, and keep it secret.
    class sample_identity final {
    public:
        gtl::x509::private_key_type private_key;
        std::vector<unsigned char> certificate;

    public:
        sample_identity() {
            // An elliptic curve private key is just a number, here one whose bytes count up in sevens.
            this->private_key.algorithm = gtl::x509::key_algorithm::secp256r1;
            for (unsigned int i = 0; i < 32; ++i) {
                this->private_key.scalar[i] = static_cast<unsigned char>((i * 7) + 1);
            }
            gtl::x509::public_key_type public_key;
            REQUIRE(gtl::x509::get_public_key(this->private_key, public_key) == true);

            // A certificate for the names that clients use to reach the server, valid from the start of 2026 to the start of 2036, signed with its own key.
            const unsigned char serial_number[1] = { 0x01 };
            const char* hosts[2] = { "localhost", "127.0.0.1" };
            gtl::x509::description_type description;
            description.subject.organisation = "gtl";
            description.subject.common_name = "localhost";
            description.serial_number = serial_number;
            description.serial_number_length = sizeof(serial_number);
            description.not_before = 1767225600ll;
            description.not_after = 2082758400ll;
            description.hosts = hosts;
            description.hosts_length = 2;
            this->certificate.resize(1024);
            unsigned long long int length = 0;
            // An elliptic curve key needs no random numbers to sign, so no random generator is given.
            REQUIRE(gtl::x509::generate_certificate(description, public_key, this->private_key, gtl::x509::signature_scheme::ecdsa_sha2_256, std::function<unsigned int()>(), this->certificate.data(), this->certificate.size(), length) == true);
            this->certificate.resize(length);
        }

        sample_identity(const sample_identity&) = delete;
        sample_identity& operator=(const sample_identity&) = delete;
    };

    /// @brief  Move bytes between a socket and a tls_client or tls_server, once in each direction.
    /// @return false when the socket has closed.
    template <typename tls_type>
    bool sample_pump(const gtl::socket& socket, tls_type& tls) {
        unsigned char buffer[4096];
        while (tls.get_output_length() != 0) {
            const unsigned long long int length = tls.get_output(buffer, sizeof(buffer));
            for (unsigned long long int sent = 0; sent < length;) {
                const long long int count = socket.tcp_send(buffer + sent, length - sent);
                if (count < 0) {
                    return false;
                }
                sent += static_cast<unsigned long long int>(count);
            }
        }
        const long long int count = socket.tcp_receive(buffer, sizeof(buffer));
        if (count < 0) {
            return false;
        }
        if (count == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        static_cast<void>(tls.process_input(buffer, static_cast<unsigned long long int>(count)));
        return true;
    }

    /// @brief  Read the application data that is waiting on a tls_client or tls_server, adding it to a string.
    template <typename tls_type>
    void sample_read(tls_type& tls, std::string& data) {
        unsigned char buffer[4096];
        for (unsigned long long int length = tls.read(buffer, sizeof(buffer)); length != 0; length = tls.read(buffer, sizeof(buffer))) {
            data.append(reinterpret_cast<const char*>(buffer), length);
        }
    }
}

TEST(tls_server, sample, server) {
    const sample_identity identity;
    const unsigned char* certificates[1] = { identity.certificate.data() };
    const unsigned long long int certificate_lengths[1] = { identity.certificate.size() };

    // The random numbers of a connection must be unpredictable.
    std::random_device random;
    gtl::tls_server::configuration_type configuration;
    configuration.random_generator = [&random]() -> unsigned int {
        return static_cast<unsigned int>(random());
    };
    configuration.certificates = certificates;
    configuration.certificate_lengths = certificate_lengths;
    configuration.certificates_length = 1;
    configuration.private_key = &identity.private_key;

    gtl::socket listener;
    if (!listener.open(gtl::socket::tcp_server{ gtl::socket::ip_loopback, sample_port })) {
        PRINT("The sample server could not listen on port %u, which may be in use by another run of this test.\n", static_cast<unsigned int>(sample_port));
        return;
    }

    if (sample_wait) {
        // Write the certificate as text, for clients that need to be given the certificates they should trust.
        std::vector<char> text(gtl::pem::get_encoded_length(identity.certificate.size(), "CERTIFICATE"));
        unsigned long long int text_length = 0;
        REQUIRE(gtl::pem::encode(identity.certificate.data(), identity.certificate.size(), "CERTIFICATE", text.data(), text.size(), text_length) == true);
        std::ofstream("gtl_sample_certificate.pem", std::ios::binary).write(text.data(), static_cast<std::streamsize>(text_length));
        PRINT("The sample server is waiting for a client on port %u, and has written its certificate to 'gtl_sample_certificate.pem'.\n", static_cast<unsigned int>(sample_port));
    }

    // A browser makes several connections, and only makes a request once it has been told to trust the certificate, so keep going until a request is answered.
    const std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + std::chrono::seconds(sample_wait ? sample_wait_seconds : 0);
    while (true) {
        // Wait for a client to connect, accepting a connection blocks so only do so once there is one.
        while (!listener.is_data_available() && (std::chrono::steady_clock::now() < deadline)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (!listener.is_data_available()) {
            return;
        }
        gtl::socket connection;
        REQUIRE(listener.accept(connection) == true);

        // Each connection has its own tls_server, which is given what arrives on the socket and gives back what to send.
        gtl::tls_server tls;
        REQUIRE(tls.start(configuration) == true);

        // Read until there is a whole request, which also performs the handshake.
        const std::chrono::steady_clock::time_point request_deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
        std::string received;
        gtl::http::request_type request;
        unsigned long long int request_length = 0;
        gtl::http::result_type result = gtl::http::result_type::incomplete;
        while ((result == gtl::http::result_type::incomplete) && (std::chrono::steady_clock::now() < request_deadline)) {
            if (!sample_pump(connection, tls) || (tls.get_state() == gtl::tls_server::state_type::failed) || (tls.get_state() == gtl::tls_server::state_type::closed)) {
                break;
            }
            sample_read(tls, received);
            result = gtl::http::read_request(received.data(), received.size(), request, request_length);
        }
        if (result != gtl::http::result_type::complete) {
            // A client that goes away, does not trust the certificate, or does not use the protocol, is not a failure of the server.
            if (tls.get_state() != gtl::tls_server::state_type::failed) {
                PRINT("The sample server had a connection that ended before it made a request.\n");
            }
            else if (tls.is_alert_received()) {
                PRINT("The sample server had a connection that the client ended with alert %u, which is usually because it has not been told to trust the certificate.\n", static_cast<unsigned int>(tls.get_alert()));
            }
            else if (tls.get_version() == 0) {
                PRINT("The sample server had a connection that it ended with alert %u before a version was agreed, was the client given an address that starts with http rather than https?\n", static_cast<unsigned int>(tls.get_alert()));
            }
            else {
                PRINT("The sample server had a connection that it ended with alert %u.\n", static_cast<unsigned int>(tls.get_alert()));
            }
            continue;
        }

        // Send the response, then close the connection, and keep going until everything has been sent.
        gtl::http::response_type response;
        response.headers.push_back(gtl::http::header_type{ "Content-Type", "text/plain" });
        response.headers.push_back(gtl::http::header_type{ "Connection", "close" });
        response.body = "Hello from the gtl https server, you asked for " + request.target + "\n";
        std::string encoded;
        REQUIRE(gtl::http::write_response(response, encoded) == true);
        REQUIRE(tls.write(reinterpret_cast<const unsigned char*>(encoded.data()), encoded.size()) == true);
        tls.close();
        while ((tls.get_output_length() != 0) && sample_pump(connection, tls)) {
        }
        PRINT("The sample server answered a request for '%s' using version %04X and cipher suite %04X.\n", request.target.c_str(), tls.get_version(), tls.get_suite());
        return;
    }
}
