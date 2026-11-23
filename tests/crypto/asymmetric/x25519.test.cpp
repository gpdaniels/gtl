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

#include <crypto/asymmetric/x25519>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {
    /// @brief  Simple type to hold a key, public or private, or a shared secret.
    struct key_type {
        unsigned char data[gtl::x25519::key_size] = {};
    };

    /// @brief  Convert a string of hexadecimal characters into a key.
    key_type hex_2_key(const char* hex) {
        const auto nibble = [](const char character) -> unsigned char {
            return static_cast<unsigned char>((character >= 'a') ? (character - 'a' + 10) : ((character >= 'A') ? (character - 'A' + 10) : (character - '0')));
        };
        key_type key;
        for (unsigned int i = 0; i < gtl::x25519::key_size; ++i) {
            key.data[i] = static_cast<unsigned char>((nibble(hex[2 * i]) << 4) | nibble(hex[(2 * i) + 1]));
        }
        return key;
    }

    // Results computed by another implementation for a set of scalars and points.
    // The last four points are not canonical or are edge cases: all bits set, a value above the modulus, the modulus minus three, and two.
    struct reference_type {
        const char* scalar;
        const char* point;
        const char* public_key;
        const char* product;
    };

    constexpr const reference_type references[] = {
        { "D1A5AC9A015FAC2EF7B341673635512A1511F41FE37D111B267F039EEC5D4F58",
          "9DC02223DA426384268A0B489B28B008464099491967F6F0597853E939953EA0",
          "B94F5E076E3E3510A3B5208D174347693E0339A4A490E7E6B846A5A461BA9330",
          "C816042E55B022D0500993A539A44D82821234B1B4F764B373A3B2FADF055D06" },
        { "6AB9F1EB8F7D3388F4F9D586F66E99FD54080DF2C446F0E58668B09C08A16DD0",
          "BB82030DBC2BCABA32A90BF2E207A84A856FC5F033B77C480836AB6F77F40F19",
          "1EE48331A96232E0157C82A4D239DDB3DE4C6966BB870FE4823D94934FA6D455",
          "6D4C519A08C7786702AA6EFED7CA6071769CA5EC7CDF93ADC3576FD10C2A2828" },
        { "015F7E6BC5AEAF483724089E9252CC13B50951A6B69412522765CFF4D780306E",
          "6CA202C88E549DFF68C09BFAFBFC60B2FAC074DEBC1E6777E9BA4B6C703ED114",
          "D22C00D81AD7D6588DD1254815D2F8D7D0B445CC269C9F30B1A13F57733B6D57",
          "09CBAD8AA7E6CAFB18DDD44C1C0E47D138271EF3A9D8791783FF17DFD0CE665A" },
        { "2F5052C9FD15B19A18C584D01363568198613F0C34E84409EF7938709A159EC2",
          "011E39EFE22590F4A339AD19CD180F4D855E32FEBA602D1EC8E154780838C99C",
          "2ACD400C4B0330C135DB12CB5B83C324BC8BCD4294CD8599BEE6B05B3661C460",
          "06664C429DE41B38B4C86DBD17C39D701D0B6426CDF770B4296FC2C882FEDC69" },
        { "94091DD64A21FFE94214BC6D17DEEB43873A5CF2F0A71B4B5CAA9A5C81B6967D",
          "E9C981A479986215BAB0BF6C32EFEFA14852534B138C3509D8369EDD510363DA",
          "272D994702DF37D6A01DAC29BAF3065B11F069F5E8E98A90AD3735B4AB29C820",
          "84AD0845A0C33821023D1C870C8AC82E417F83F47F98274CE5E67C4468181911" },
        { "88DBF612972C594A2C2F6DDE493EF1C4CD27ADBA8C36E6EBD00B89FF143ABA61",
          "5850A03E801FFB108DA1160E3373979443004B9E670ADDF33000DCA9045FA413",
          "C815CAC9FD30FC3D5F93DDE994F1356E216594DE02EB756C065B0EE69F6E5344",
          "B94A21217B054817E2C307109534200E28E1E9035BD4BEA10A615F2E451C2B48" },
        { "1D92AD4B6987FA0347CC5D2FB6CF9E47C83F4F6CAEB3B5EF6F629730528921C3",
          "71EA5F5B962198C5D0532765E7E92CDD0519456BB3D735297E535DCAB17BF84D",
          "A6E99DF0D5A7715B68D7DCBA4B0763802BA63218DBC625AD1D57E67B13730476",
          "7E35B6EEF8D21B80E8A532A86F6BA18508F11C6CD6140070B36535C99E2FB070" },
        { "FB848C99B9A43EC7866A23EA000C1939A168F5FF17314A0B88C7BE711D7EF7D0",
          "E8180000FA67E824043AA522C6743DE57DBC5DE1D39D5483ACB618B699A9DD00",
          "F6C374B64E96627F148ED3997B746258AC9CD42E7715F849E7DD99AF0EB8AD1D",
          "489E5D882B2E6C454581C686053977E452C8533D799ADC965B7597FF76FD2A28" },
        { "5A3DF89DA7BF23D71EC63F111D9171C3F05047380A17812F20248E63CFDED4FB",
          "C89951A24C6CA28C13FD1CFDC646B2B656D69E61A92B91023BE7EB58EB914B6B",
          "61749AAB6A290EFE6F183F60F7DF4D4D1D88EE9ED3E1EF79EDF4471DE249160A",
          "187319DD5B3E2F0AE8E23BD4A3C05A0CD711EB6DE01D47374BECF4C0194E3E79" },
        { "C3C81C2B9A9AE9E358D68FC47DBC8596F19B420011E71C657D1616F14FB77917",
          "553F26ABEEEBE4BE18603F3C15BBF6CD3106002E869CE9275054868FB0D610AF",
          "64215562C0DF6A367015DA03F2FA864AB920FBEED17C28C04179DA17ED2E502C",
          "9423B30E2692051C17B407BB935DDAFECF92C8B7B0B695C89F23FF4515E1E455" },
        { "4AE43FD8358484A65B03CFF3B3F0EBE5478DEA48398D24628F41429518E4BCE0",
          "105A9CEF3903C748EB63D42EC006C9B46CDAB99141B0CD80176026B76CBB69FD",
          "8D3BC92F037A1C829AFCE2B69457D3FE03AA8520A6FD6B6744151BBE98C7665B",
          "AE7B943515C875007FBF4C6B67036F68FAAC8F0BF358AE392E83F1649711157B" },
        { "13876D0627A48F198E5D5EEBA044EAAECE7545A112D872EF1FC7AF768E4DA77F",
          "92EC86FA88925DABC1026BFC9335F5F6848886C98586C005F4A0C02716E3BBAB",
          "BD33829BDEE1FDFAFBB55BBA92A3B34608777D3BFF238278A5AF92C43586723A",
          "4DCF8DC968C7A8BCBC9571D5553733830CE9AF92876D7C6F68DFC6071B34101C" },
        { "58E08F69BA8386E2DB875B1A2D92734A4C9C4F62E44962FD29E59A546FADB366",
          "A50656C6EDF2F06AF2C6C847D13118E34D60FD98FF1925B5B456CDE6067425FF",
          "5292B3A24CAF1A7EC2D650631DBBE7A87878D2958B69245FB818B1415AE3930A",
          "FD7764ED035327955542D1B7684A1D41A5DB05A650660C172955BB0FA071FB78" },
        { "6774E97B32D47E4E848019C7333D0AECFB1EB46F335917F34DF1E5AD9C0116F9",
          "9607720F9F1C07DFC26581CDAA37A002CBDD5587A69090B66F82C649CE7A40C4",
          "8EBA41BE7E4CA72D8CB4F239EB7B616A73E0ED11C754F44DB6A31F57436B7926",
          "005E4B18CA5D46FF2112951EE65EE5FCACC288D93C554372A59B80A82CE0F44D" },
        { "3A8CFE81D50302B031CDE30603C4BBFBE2AF5A32FFFB80A034F4A9DA9D315153",
          "B8C9573532D1CDD04675D02A826940C3BB553AE30CA9997A814CB64C9AA95894",
          "6740B3855A2FF615E891495AEE00064D3C06708ADE9A56307DE595E5FFEF9504",
          "5BF021235A9B0A0DEDF15BD0998AF23AD6686458C4441C2AE453DCE412651A4A" },
        { "5BD4CA497530B4B3490E350F3590BBA9FE216128B35A578F459FC9AB8745AE88",
          "6FD1B3E081A50A236AF0114C66F7AFFA7CF4CD0C627CC71203151D25A0C4D980",
          "8F1A90B0BAE361D5B8C7F26F5712022A908E36888A70C0A063922DFA738DFA2D",
          "B00FB13B13F88E12349057A8B2D2B5CB9B0A020B6102A8EA970C0E40D0DA9073" },
        { "8D68C655677C5C72D8118949E899757191592F3252412E16DC64FA7E44FFA095",
          "29881AAD8730C5AB11D275376323D8E4FF4179AAE8CCB6C13FE4902137E162EF",
          "22E89A224B7AD7E0252E05A704BC539D2A5D9DE79E884D413B5E7AC74266F839",
          "C0B8158662CBDB6AAB6A02F3B20FB21B750407A66B14C36CDED760856EF2577B" },
        { "6C47C1DE66E7E5EBD7B1FE0746EA1A360BCF043929829FB18CBD7A07C25D7639",
          "4EF7D80D0EFAC32F30F950A6AF21FB65345AD8DE0C683548CF00E7A64F2B3820",
          "BD634C2B79D65B22EBF34A4012E16FE437B3A81EE3EEB073361BC466717C8837",
          "CD06D1DF41120E2AB0A3ED5907D97C4A589438A6AD9AA292D6EA3981F5A82B75" },
        { "362EB7F8954E74E3A7B823B3735979BD9A475CECCBCF9471B7736266C4881BD4",
          "D238F8B22B170A316512D14CBFBCE16123895D3ADA7A6C0F77DD3928472229FE",
          "F521533AC67BD82EDEE4656EB640FF91DE9CC9685CE445D31D7A7184444B1903",
          "39A3A66EBE287B67E1D4796D9D9C5118E570CB494C5F200F92337CA5336B4F69" },
        { "1FF17650532C3034FF639402627CDE9429618C1ADC337A678EF5F3A649714D7A",
          "EE1303AC368992EF442A2E8705A0225FD4A1AFC18C14E650DAAAA6D872A146D1",
          "A41FB5D1BA972BB3A9BE137A444D6ABDCF07525FA306F0077F05BD66DF5BFC32",
          "14DB8A746E706034A2E64A0547795EEF0E9C70D9849B895F3E11C37722BC6747" },
        { "CB67903A62D3D4D64E5C0A3B477E52238051CF9D3372AD027DCEB13543FF1573",
          "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
          "60D870BF06B42C99211BA77809F98968E2D672539657EA07002487494FA68F42",
          "B8FA7172010DE146CC5A0EB6C7C466B4252937DBF5EA7D185176DF0B32D52C50" },
        { "D71F238A6B74E3B17302F571E828557A92791DC38470BB399CC2C675D1730217",
          "F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F",
          "31BC889241ADF97F025021B037B7B1FD1EEE8F48812595EE56007EA0127FFE3C",
          "43E4190F11160BB4961D3D8845D3878B8A5E18F6203601A8C3EAA287A4E8593A" },
        { "432B2E7BF4583E776C3AF83E1952ABA7EF3D3E69560A5B5020883990AEAA23C9",
          "EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F",
          "6FBAFD76C53A89145EC4C0E5DA8437DF9459D32F7B6B0B044BB4DF8F36CAB40B",
          "6B7C947CE6071D26058DB7C6AB0F7CE73E3884154B7A32520CDC63B56E5CB962" },
        { "F68F3189E7394D8B39D1B8BDB035229CAAC67EC3E1D71FBE4C4CA9A0B2DFF2FB",
          "0200000000000000000000000000000000000000000000000000000000000000",
          "9E22C3F4A71F80EADA8F83FF2AE539BA369E260D505BB7BA3B25D4A762865A0C",
          "CF68278323F815DCDFF82EEE84BDFED62711CBE3A0B50F5BA1B1F3092C664A52" },
    };

    // The points of small order, for which the shared secret is zero whatever the private key is.
    constexpr const char* small_order_points[] = {
        "0000000000000000000000000000000000000000000000000000000000000000",
        "0100000000000000000000000000000000000000000000000000000000000000",
        "ECFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F",
        "EDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F",
        "EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F",
        "E0EB7A7C3B41B8AE1656E3FAF19FC46ADA098DEB9C32B1FD866205165F49B800",
        "5F9C95BCA3508C24B1D0B1559C83EF5B04445CC4581C8E86D8224EDDD09F1157",
    };
}

TEST(x25519, traits, standard) {
    REQUIRE((std::is_pod<gtl::x25519>::value == true));

    REQUIRE((std::is_trivial<gtl::x25519>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::x25519>::value == true));

    REQUIRE((std::is_standard_layout<gtl::x25519>::value == true));
}

TEST(x25519, constructor, empty) {
    gtl::x25519 x25519;
    testbench::do_not_optimise_away(x25519);
}

TEST(x25519, parameter, sizes) {
    REQUIRE(gtl::x25519::key_size == 32);
}

TEST(x25519, function, scalar_multiply) {
    const key_type scalar = hex_2_key("0100000000000000000000000000000000000000000000000000000000000000");
    const key_type point = hex_2_key("0900000000000000000000000000000000000000000000000000000000000000");
    key_type output;
    gtl::x25519::scalar_multiply(scalar.data, point.data, output.data);
}

TEST(x25519, function, generate_public_key) {
    const key_type private_key = hex_2_key("0100000000000000000000000000000000000000000000000000000000000000");
    key_type public_key;
    gtl::x25519::generate_public_key(private_key.data, public_key.data);
}

TEST(x25519, function, generate_shared_secret) {
    const key_type private_key = hex_2_key("0100000000000000000000000000000000000000000000000000000000000000");
    const key_type peer_public_key = hex_2_key("0900000000000000000000000000000000000000000000000000000000000000");
    key_type shared_secret;
    REQUIRE(gtl::x25519::generate_shared_secret(private_key.data, peer_public_key.data, shared_secret.data) == true);
}

TEST(x25519, evaluate, rfc_7748_vectors) {
    // The test vectors of RFC 7748 section 5.2.
    {
        const key_type scalar = hex_2_key("a546e36bf0527c9d3b16154b82465edd62144c0ac1fc5a18506a2244ba449ac4");
        const key_type point = hex_2_key("e6db6867583030db3594c1a424b15f7c726624ec26b3353b10a903a6d0ab1c4c");
        const key_type expected = hex_2_key("c3da55379de9c6908e94ea4df28d084f32eccf03491c71f754b4075577a28552");
        key_type output;
        gtl::x25519::scalar_multiply(scalar.data, point.data, output.data);
        REQUIRE(testbench::is_memory_same(output.data, expected.data, gtl::x25519::key_size));
    }
    {
        // The most significant bit of this point is set and must be ignored.
        const key_type scalar = hex_2_key("4b66e9d4d1b4673c5ad22691957d6af5c11b6421e0ea01d42ca4169e7918ba0d");
        const key_type point = hex_2_key("e5210f12786811d3f4b7959d0538ae2c31dbe7106fc03c3efc4cd549c715a493");
        const key_type expected = hex_2_key("95cbde9476e8907d7aade45cb4b873f88b595a68799fa152e6f8f7647aac7957");
        key_type output;
        gtl::x25519::scalar_multiply(scalar.data, point.data, output.data);
        REQUIRE(testbench::is_memory_same(output.data, expected.data, gtl::x25519::key_size));
    }
}

TEST(x25519, evaluate, rfc_7748_iterations) {
    // The iterated test of RFC 7748 section 5.2, where each result becomes the next scalar and each scalar the next point.
    const key_type expected_1 = hex_2_key("422c8e7a6227d7bca1350b3e2bb7279f7897b87bb6854b783c60e80311ae3079");
    const key_type expected_1000 = hex_2_key("684cf59ba83309552800ef566f2f4d3c1c3887c49360e3875f2eb94d99532c51");
    key_type scalar = hex_2_key("0900000000000000000000000000000000000000000000000000000000000000");
    key_type point = scalar;
    for (unsigned int iteration = 1; iteration <= 1000; ++iteration) {
        key_type output;
        gtl::x25519::scalar_multiply(scalar.data, point.data, output.data);
        point = scalar;
        scalar = output;
        if (iteration == 1) {
            REQUIRE(testbench::is_memory_same(scalar.data, expected_1.data, gtl::x25519::key_size));
        }
    }
    REQUIRE(testbench::is_memory_same(scalar.data, expected_1000.data, gtl::x25519::key_size));
}

TEST(x25519, evaluate, rfc_7748_key_agreement) {
    // The Diffie-Hellman example of RFC 7748 section 6.1.
    const key_type alice_private_key = hex_2_key("77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a");
    const key_type alice_public_key = hex_2_key("8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a");
    const key_type bob_private_key = hex_2_key("5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb");
    const key_type bob_public_key = hex_2_key("de9edb7d7b7dc1b4d35b61c2ece435373f8343c85b78674dadfc7e146f882b4f");
    const key_type expected_shared_secret = hex_2_key("4a5d9d5ba4ce2de1728e3bf480350f25e07e21c947d19e3376f09b3c1e161742");

    key_type public_key;
    gtl::x25519::generate_public_key(alice_private_key.data, public_key.data);
    REQUIRE(testbench::is_memory_same(public_key.data, alice_public_key.data, gtl::x25519::key_size));
    gtl::x25519::generate_public_key(bob_private_key.data, public_key.data);
    REQUIRE(testbench::is_memory_same(public_key.data, bob_public_key.data, gtl::x25519::key_size));

    key_type shared_secret;
    REQUIRE(gtl::x25519::generate_shared_secret(alice_private_key.data, bob_public_key.data, shared_secret.data) == true);
    REQUIRE(testbench::is_memory_same(shared_secret.data, expected_shared_secret.data, gtl::x25519::key_size));
    REQUIRE(gtl::x25519::generate_shared_secret(bob_private_key.data, alice_public_key.data, shared_secret.data) == true);
    REQUIRE(testbench::is_memory_same(shared_secret.data, expected_shared_secret.data, gtl::x25519::key_size));
}

TEST(x25519, evaluate, reference) {
    for (const reference_type& reference : references) {
        const key_type scalar = hex_2_key(reference.scalar);
        const key_type point = hex_2_key(reference.point);
        const key_type expected_public_key = hex_2_key(reference.public_key);
        const key_type expected_product = hex_2_key(reference.product);

        key_type public_key;
        gtl::x25519::generate_public_key(scalar.data, public_key.data);
        REQUIRE(testbench::is_memory_same(public_key.data, expected_public_key.data, gtl::x25519::key_size));

        key_type product;
        gtl::x25519::scalar_multiply(scalar.data, point.data, product.data);
        REQUIRE(testbench::is_memory_same(product.data, expected_product.data, gtl::x25519::key_size));

        key_type shared_secret;
        REQUIRE(gtl::x25519::generate_shared_secret(scalar.data, point.data, shared_secret.data) == true);
        REQUIRE(testbench::is_memory_same(shared_secret.data, expected_product.data, gtl::x25519::key_size));

        // The output may be either of the inputs.
        key_type in_place = scalar;
        gtl::x25519::scalar_multiply(in_place.data, point.data, in_place.data);
        REQUIRE(testbench::is_memory_same(in_place.data, expected_product.data, gtl::x25519::key_size));
        in_place = point;
        gtl::x25519::scalar_multiply(scalar.data, in_place.data, in_place.data);
        REQUIRE(testbench::is_memory_same(in_place.data, expected_product.data, gtl::x25519::key_size));
    }
}

TEST(x25519, evaluate, clamping) {
    // The bits of the scalar that are cleared or set by clamping do not change the result.
    const key_type point = hex_2_key(references[0].point);
    key_type scalar = hex_2_key(references[0].scalar);
    key_type expected;
    gtl::x25519::scalar_multiply(scalar.data, point.data, expected.data);
    for (unsigned int bits = 0; bits < 32; ++bits) {
        key_type changed = scalar;
        changed.data[0] = static_cast<unsigned char>((changed.data[0] & 0xF8) | (bits & 0x07));
        changed.data[31] = static_cast<unsigned char>((changed.data[31] & 0x3F) | ((bits & 0x18) << 3));
        key_type output;
        gtl::x25519::scalar_multiply(changed.data, point.data, output.data);
        REQUIRE(testbench::is_memory_same(output.data, expected.data, gtl::x25519::key_size));
    }
    // Any other bit of the scalar does.
    for (unsigned int bit = 3; bit < 254; ++bit) {
        key_type changed = scalar;
        changed.data[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        key_type output;
        gtl::x25519::scalar_multiply(changed.data, point.data, output.data);
        REQUIRE(testbench::is_memory_same(output.data, expected.data, gtl::x25519::key_size) == false);
    }
}

TEST(x25519, evaluate, key_agreement) {
    // Every pair of the reference keys agree on a shared secret, and different pairs get different secrets.
    const unsigned int count = sizeof(references) / sizeof(references[0]);
    for (unsigned int i = 0; i < 6; ++i) {
        for (unsigned int j = i + 1; j < 6; ++j) {
            const key_type private_key_i = hex_2_key(references[i].scalar);
            const key_type public_key_i = hex_2_key(references[i].public_key);
            const key_type private_key_j = hex_2_key(references[j].scalar);
            const key_type public_key_j = hex_2_key(references[j].public_key);
            key_type shared_secret_i;
            key_type shared_secret_j;
            REQUIRE(gtl::x25519::generate_shared_secret(private_key_i.data, public_key_j.data, shared_secret_i.data) == true);
            REQUIRE(gtl::x25519::generate_shared_secret(private_key_j.data, public_key_i.data, shared_secret_j.data) == true);
            REQUIRE(testbench::is_memory_same(shared_secret_i.data, shared_secret_j.data, gtl::x25519::key_size));

            const key_type public_key_other = hex_2_key(references[(j + 1) % count].public_key);
            key_type shared_secret_other;
            REQUIRE(gtl::x25519::generate_shared_secret(private_key_i.data, public_key_other.data, shared_secret_other.data) == true);
            REQUIRE(testbench::is_memory_same(shared_secret_i.data, shared_secret_other.data, gtl::x25519::key_size) == false);
        }
    }
}

TEST(x25519, evaluate, small_order_points) {
    const key_type zeros;
    for (const char* small_order_point : small_order_points) {
        const key_type point = hex_2_key(small_order_point);
        for (unsigned int i = 0; i < 4; ++i) {
            const key_type private_key = hex_2_key(references[i].scalar);
            key_type shared_secret = private_key;
            REQUIRE(gtl::x25519::generate_shared_secret(private_key.data, point.data, shared_secret.data) == false);
            REQUIRE(testbench::is_memory_same(shared_secret.data, zeros.data, gtl::x25519::key_size));
        }
    }
}
