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

#include <crypto/asymmetric/ed25519>

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

    /// @brief  Get a pointer to the data of a buffer that is valid to pass even when the buffer is empty.
    const unsigned char* data_of(const std::vector<unsigned char>& bytes) {
        static const unsigned char empty[1] = {};
        return bytes.empty() ? empty : bytes.data();
    }

    // The first three test vectors of RFC 8032 section 7.1.
    struct vector_type {
        const char* private_key;
        const char* public_key;
        const char* message;
        const char* signature;
    };

    constexpr const vector_type vectors[] = {
        { "9D61B19DEFFD5A60BA844AF492EC2CC44449C5697B326919703BAC031CAE7F60",
          "D75A980182B10AB7D54BFED3C964073A0EE172F3DAA62325AF021A68F707511A",
          "",
          "E5564300C360AC729086E2CC806E828A84877F1EB8E5D974D873E065224901555FB8821590A33BACC61E39701CF9B46BD25BF5F0595BBE24655141438E7A100B" },
        { "4CCD089B28FF96DA9DB6C346EC114E0F5B8A319F35ABA624DA8CF6ED4FB8A6FB",
          "3D4017C3E843895A92B70AA74D1B7EBC9C982CCF2EC4968CC0CD55F12AF4660C",
          "72",
          "92A009A9F0D4CAB8720E820B5F642540A2B27B5416503F8FB3762223EBDB69DA085AC1E43E15996E458F3613D0F11D8C387B2EAEB4302AEEB00D291612BB0C00" },
        { "C5AA8DF43F9F837BEDB7442F31DCB7B166D38535076F094B85CE3A2E0B4458F7",
          "FC51CD8E6218A1A38DA47ED00230F0580816ED13BA3303AC5DEB911548908025",
          "AF82",
          "6291D657DEEC24024827E69C3ABE01A30CE548A284743A445E3680D7DB5AC3AC18FF9B538D16F290AE67F760984DC6594A7C15E9716ED28DC027BECEEA1EC40A" },
    };

    // Signatures made by another implementation, of messages of lengths either side of the block size of the hash function.
    // Each message is its length in bytes, where byte i is (i * 13) + length.
    struct reference_type {
        const char* private_key;
        const char* public_key;
        unsigned int length;
        const char* signature;
    };

    constexpr const reference_type references[] = {
        { "B40EF35A48AA85F165157BB984BA44F100B2D651915221228C7AA5A861A23DD0",
          "B027CA8DA70760C37A4FC1BFB23AD0DB22E41282A7B59FB285B7A7A6F95F5D8E",
          0,
          "5391B8DB4589FBE75925B11D44F69B2407F3B71509ADA0ABC4E34BFA1A756C1245D8638A46CE44CF9B219C7725A561635EC1ECFD6A3A05F88EDEBBFB5DD4BF04" },
        { "320228DD84368541D0B669827A9A57F96A3B9AC2E74727D07EADA3FDCF9ACC0A",
          "5C2AF9F6A88EC37D1CF252DCF471AE4787B29496D698EB8A67DDF55223629A4B",
          1,
          "A2A239401E7895997BC89137372B66B9D7EB449A89A1D72CBF69A2D6D5BA2DB966DA783776C6A280E8EC5C90FD02C88674B413F33B7BB0A43DFDCC404A5DC404" },
        { "E55D08F063426FFE05F288B6444C20DFC244463F94DCE4844B29C759064BF337",
          "F96A510D178F1049E7FBB9BD82A69F4E059A2B39D63E4999790F68C2BC6D6030",
          2,
          "2A3E285399B2CB745C7A33DF0FCD8BB3E05187DE6A6CC4F4CDD375CD3CDA96BD862E00303CE8D5AAE1ACD2762DE872CA29F1A4CCD973F3C5D21DBB4EDC0B010E" },
        { "09AE8302B592702CCDBA3612063A7EBC3EE6F2FDB490893CA2E662796B1B8E64",
          "22C98831D85490D5D4C47B327C0C3E7B72330D10D9FBBEBE4FD4AC47A284E8B9",
          3,
          "540D8A201D31884D93739523B7F7AF46F3110630F64C50BC708634927EB98B2CCC403E2A794EEF5C47247A1DEC299C6877A2ED9259FC7243EC91E40BD1F56E0B" },
        { "D25220E7A8CF7B09C53D0A25DEE0403B0A60DA0F8162264A4CE49236BF9E0EF3",
          "43E7EBFB5D3582468C0E5A447F485BDD52C23E3C473F9A3DBA903B0A9DA3EE10",
          31,
          "121C3225B3762F64A8BC2B8064D8487CE07125B96906D1CBDE71A884B83BC68D3746E99D71B83728CDEA0431B3A2B7C6F943080339B2B3EA52F9FCE0892CB408" },
        { "590BBE2591AA8935401222014367A0C1E1DC1BEEE824C963B87317C7E450D292",
          "14FD081E48F833EAF92C176523CB4CF13EC264DA41FA87830F6374C89FE33F6C",
          32,
          "FBD851B9D4B92714ECCE02D8B51B68E5146766E00FE8BA528DD64F50DBC6ABBF9C2EA4FC868DCF842499C091F623E653DE563D1D7BCB03B17A92A4276F72F508" },
        { "43D5C368153E486729866297F91A19CB8296C3130C32DD71F24C410B0CCF12FD",
          "B10AAC592686180FA48943080D602E4DDEC72775372B92108B420C5867B08DDA",
          33,
          "65A2F49A7AC648809503ED5BE3F3B75D961729D0B86E5A428223D88A6C95D9D25305C2469851441B856394F125319E254CA84118378425F9E4A515256BFD5C0A" },
        { "FE7790AA9C4DF814B48B4C89F31F1237F4326D702E0A0D80026DF7F0DCCBC075",
          "7006174A0A6CA9DDD34155E90B5C1B44B4354689005725490BC7EC1801693248",
          63,
          "D19346879C5C8F1FACB24ED2AA80EF025995CD0FE8E75D2AD1DA89F14773112A8A504328FD5864AEDD257478B54FAB52EF277103C742F9A9DF14A4E57CC61D0F" },
        { "C69E67C51C30DB364EF42E5140FC73A03C0929E293650FE6D685F671FFAA8340",
          "952E715F37EE84097E1FE50D505BE7ECF1B825017FEC118DB2097B18B8767FD2",
          64,
          "A7BDF43BFE3A06D6FA03B0EA8E85947ECEDB7D9AD0CEAE90C2F4C73E5F99C141936A295B2CA5AFCF7A27AE868DC51B5E5BC641B91040201EC58111CC1DE6120A" },
        { "569988294051B13A28264F9DDEF0D8E5D1EC30E413EB77A1328F5DB18706140B",
          "BEEFEF3E6CB2DAF88AE92D6855B00CA41BBEC8BA4D7C623926A269FE1F991C38",
          65,
          "97D181D9F246C6F5561E98EDDB6BD3204A75C612A21830CECAE7934BCA52AB95285B70B67B6E84B4F58CF9A4DBE96B9CDFB772D2BFB330E99C2E471CC4BE160F" },
        { "CF517824B98CFD41DF1C830BE68B6A52A7A628658E75D755644C42F10EB9C484",
          "F180B37891321191B65A157DB8DD66A5EFCBC582CB489081757AFC60886E6A81",
          111,
          "35A6ABCF653A25631541BBE8354C67BD6D3DE848A2135311D19EF4BA31A8FBAC74D5AF3A4FE8FAD1BF942FA8E41DC64717920EE51E88926C8B75D6F6F223220D" },
        { "23966CB0000808DAECD86981CB1FC5E76A709BB1394F702FFF2E248FEBAF311B",
          "B6C5F25C54F6EB62EFF4AA98D4D107C152F1C87C0179E78B5E791D7C87E6AD69",
          112,
          "0633869FD89CDA79350A5FE0F235E56F35F772C3A1F2FB2E07E708A8F1ABCB523291807FE8963F391C1C1558566C3DC4DD14CEFA4F0901A82607090E482CB502" },
        { "8339BEF5C58002A0BBAC92F4A11B40E7BA88B3DCB8C660570CC8CF7A41236556",
          "729103DA2CE96F3F3FA27217130C1188C4406360A53451CEBDD2B8B8E389A56D",
          113,
          "4DA785C9773A2FD27C86E53F9FA93F4A7C9244F725C7F381E481F388C1FB60A7A32E96E480F951261934C9D411E7271E162E74F51BCA2C7684FAEBDA8FDE3A02" },
        { "5E99DF0214B6DCCBFD3A509F4C96EF9B7E01C1CC482D8FF4AC679F8C2AF2C3F4",
          "30AD74CF603A9278DB761575031DB59BFAE3C7DB5837B8FD1A32358749790C74",
          127,
          "DFE551F482AE0CB19C7D15B9201A264BEA9EB187BD54513F3FF5D63C1CCF1B24C5A766B21E46686BAF61B93B1CDED047B96E53C482FC77826DF2262D07BB2F05" },
        { "D36747C0929D39F9F8D568424E443AB162FBC183D2C6D0AD3057371119AD6EF7",
          "A1461335DBDC493C5CD9EF4CD809B76CF85126D43F859BD885CE8FA8474DA53A",
          128,
          "6CB2373691895B087F2D42BC0FAE6752A26A4606AA52BE25B8DB1D9D36CF0568CDF8CA211E29718AB9F6C404382CDE3131262BF566A0BDC16A8CDB6466EB9F00" },
        { "0AC433DE59939347DE71ADA537CD7D181F2B0EDDDB8C0A684DA51167663E160E",
          "5C087AD1B622203986F30A18705E49A62B98565B0274E7DE9E8A4251D755CFE5",
          129,
          "5D3DDCFAA2A8508448CADB3C56D773BFB4982F331C63677B56C3E16D3A874E47ADDC763F071FF8B696396588E0AF64D13B8AAA08976CA3893F2AE10C9BE97706" },
        { "7601D9F8D5CBC77763F67B3A61BDA754F443D88459870D09B02B788D80AB44B1",
          "618CCA7176E4B9AD836FC75FA678AD2F6D0BAE0F33E36060C250BD31E6A5CD0E",
          255,
          "413786F2002DA1FCD3B56AC012B82A6F3627BF335ED5AE5D5EF1034FAF2C82CEA9F1180CF92C9472277DEE3934D0C134FDA6F4B993445FFC2957D503333DC800" },
        { "1717DF9C2BCEC529248B4F553637C0014771DCF32A5A355F9565DA03DFB705A8",
          "5F96C8638734BF746A77E530F2453E6C7AE9AC58A76E8B0039385685DF8B1808",
          1000,
          "AB47A4EF1379AEA0C2C80CE17CC62A8AE7841C3BD2A6E249F39FA0BCE566450EB99008C9E43EAE0DEF7CF1D054CC84320D5B54B772DDE2110A0989585C37BD0E" },
    };

    /// @brief  Generate the message of a reference signature.
    std::vector<unsigned char> reference_message(const unsigned int length) {
        std::vector<unsigned char> message;
        for (unsigned int i = 0; i < length; ++i) {
            message.push_back(static_cast<unsigned char>(((i * 13) + length) & 0xFF));
        }
        return message;
    }

    // The order of the base point, little endian.
    constexpr const char* order = "EDD3F55C1A631258D69CF7A2DEF9DE1400000000000000000000000000000010";

    // Values of y, little endian, for which there is no point on the curve.
    constexpr const char* invalid_points[] = {
        "0200000000000000000000000000000000000000000000000000000000000000",
        "0700000000000000000000000000000000000000000000000000000000000000",
        "0800000000000000000000000000000000000000000000000000000000000000",
    };

    // The point with a y of one, and the same point with the modulus added to its y, which is not a canonical encoding.
    constexpr const char* point_one = "0100000000000000000000000000000000000000000000000000000000000000";
    constexpr const char* point_one_not_canonical = "EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F";
}

TEST(ed25519, traits, standard) {
    REQUIRE((std::is_pod<gtl::ed25519>::value == true));

    REQUIRE((std::is_trivial<gtl::ed25519>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::ed25519>::value == true));

    REQUIRE((std::is_standard_layout<gtl::ed25519>::value == true));
}

TEST(ed25519, constructor, empty) {
    gtl::ed25519 ed25519;
    testbench::do_not_optimise_away(ed25519);
}

TEST(ed25519, parameter, sizes) {
    REQUIRE(gtl::ed25519::private_key_size == 32);
    REQUIRE(gtl::ed25519::public_key_size == 32);
    REQUIRE(gtl::ed25519::signature_size == 64);
}

TEST(ed25519, function, generate_public_key) {
    const unsigned char private_key[gtl::ed25519::private_key_size] = { 1, 2, 3 };
    unsigned char public_key[gtl::ed25519::public_key_size] = {};
    gtl::ed25519::generate_public_key(private_key, public_key);
}

TEST(ed25519, function, sign_verify) {
    const unsigned char private_key[gtl::ed25519::private_key_size] = { 1, 2, 3 };
    unsigned char public_key[gtl::ed25519::public_key_size] = {};
    gtl::ed25519::generate_public_key(private_key, public_key);
    const unsigned char message[5] = { 'h', 'e', 'l', 'l', 'o' };
    unsigned char signature[gtl::ed25519::signature_size] = {};
    gtl::ed25519::sign(private_key, message, 5, signature);
    REQUIRE(gtl::ed25519::verify(public_key, message, 5, signature) == true);
    REQUIRE(gtl::ed25519::verify(public_key, message, 4, signature) == false);
}

TEST(ed25519, evaluate, rfc_8032_vectors) {
    for (const vector_type& vector : vectors) {
        const std::vector<unsigned char> private_key = hex_2_bytes(vector.private_key);
        const std::vector<unsigned char> public_key = hex_2_bytes(vector.public_key);
        const std::vector<unsigned char> message = hex_2_bytes(vector.message);
        const std::vector<unsigned char> signature = hex_2_bytes(vector.signature);
        REQUIRE(private_key.size() == gtl::ed25519::private_key_size);
        REQUIRE(public_key.size() == gtl::ed25519::public_key_size);
        REQUIRE(signature.size() == gtl::ed25519::signature_size);

        unsigned char test_public_key[gtl::ed25519::public_key_size] = {};
        gtl::ed25519::generate_public_key(private_key.data(), test_public_key);
        REQUIRE(testbench::is_memory_same(test_public_key, public_key.data(), gtl::ed25519::public_key_size));

        unsigned char test_signature[gtl::ed25519::signature_size] = {};
        gtl::ed25519::sign(private_key.data(), data_of(message), message.size(), test_signature);
        REQUIRE(testbench::is_memory_same(test_signature, signature.data(), gtl::ed25519::signature_size));

        REQUIRE(gtl::ed25519::verify(public_key.data(), data_of(message), message.size(), signature.data()) == true);
        // An empty message may be passed as a null pointer.
        if (message.empty()) {
            gtl::ed25519::sign(private_key.data(), nullptr, 0, test_signature);
            REQUIRE(testbench::is_memory_same(test_signature, signature.data(), gtl::ed25519::signature_size));
            REQUIRE(gtl::ed25519::verify(public_key.data(), nullptr, 0, signature.data()) == true);
        }
    }
}

TEST(ed25519, evaluate, reference) {
    for (const reference_type& reference : references) {
        const std::vector<unsigned char> private_key = hex_2_bytes(reference.private_key);
        const std::vector<unsigned char> public_key = hex_2_bytes(reference.public_key);
        const std::vector<unsigned char> message = reference_message(reference.length);
        const std::vector<unsigned char> signature = hex_2_bytes(reference.signature);

        unsigned char test_public_key[gtl::ed25519::public_key_size] = {};
        gtl::ed25519::generate_public_key(private_key.data(), test_public_key);
        REQUIRE(testbench::is_memory_same(test_public_key, public_key.data(), gtl::ed25519::public_key_size), "Public key mismatch for length %u.", reference.length);

        unsigned char test_signature[gtl::ed25519::signature_size + 1];
        test_signature[gtl::ed25519::signature_size] = 0xA5;
        gtl::ed25519::sign(private_key.data(), data_of(message), message.size(), test_signature);
        REQUIRE(testbench::is_memory_same(test_signature, signature.data(), gtl::ed25519::signature_size), "Signature mismatch for length %u.", reference.length);
        REQUIRE(test_signature[gtl::ed25519::signature_size] == 0xA5);

        REQUIRE(gtl::ed25519::verify(public_key.data(), data_of(message), message.size(), signature.data()) == true, "Reference signature for length %u did not verify.", reference.length);
    }
}

TEST(ed25519, evaluate, invalid_signatures) {
    // A message long enough to need more than one block of the hash function.
    const reference_type& reference = references[13];
    REQUIRE(reference.length == 127);
    const std::vector<unsigned char> public_key = hex_2_bytes(reference.public_key);
    const std::vector<unsigned char> message = reference_message(reference.length);
    const std::vector<unsigned char> signature = hex_2_bytes(reference.signature);
    REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), signature.data()) == true);

    // Changing any bit of the signature, the public key, or the message makes the signature invalid.
    for (unsigned int bit = 0; bit < (gtl::ed25519::signature_size * 8); bit += 5) {
        std::vector<unsigned char> changed = signature;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE_SILENT(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), changed.data()) == false);
    }
    for (unsigned int bit = 0; bit < (gtl::ed25519::public_key_size * 8); bit += 5) {
        std::vector<unsigned char> changed = public_key;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE_SILENT(gtl::ed25519::verify(changed.data(), message.data(), message.size(), signature.data()) == false);
    }
    for (unsigned int bit = 0; bit < (reference.length * 8); bit += 37) {
        std::vector<unsigned char> changed = message;
        changed[bit / 8] ^= static_cast<unsigned char>(1 << (bit % 8));
        REQUIRE_SILENT(gtl::ed25519::verify(public_key.data(), changed.data(), changed.size(), signature.data()) == false);
    }
    REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size() - 1, signature.data()) == false);
    REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), 0, signature.data()) == false);

    // A valid signature made with another key, and the two halves of the signature swapped.
    const std::vector<unsigned char> other_public_key = hex_2_bytes(references[12].public_key);
    REQUIRE(gtl::ed25519::verify(other_public_key.data(), message.data(), message.size(), signature.data()) == false);
    std::vector<unsigned char> swapped(signature.begin() + 32, signature.end());
    swapped.insert(swapped.end(), signature.begin(), signature.begin() + 32);
    REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), swapped.data()) == false);

    // Adding the order of the base point to the second half gives the same number modulo the order, but is not a valid encoding.
    {
        const std::vector<unsigned char> order_bytes = hex_2_bytes(order);
        std::vector<unsigned char> malleable = signature;
        unsigned int carry = 0;
        for (unsigned int i = 0; i < 32; ++i) {
            const unsigned int sum = static_cast<unsigned int>(signature[32 + i]) + order_bytes[i] + carry;
            malleable[32 + i] = static_cast<unsigned char>(sum & 0xFF);
            carry = sum >> 8;
        }
        REQUIRE(carry == 0);
        REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), malleable.data()) == false);
        // The order itself, and all ones, as the second half.
        for (unsigned int i = 0; i < 32; ++i) {
            malleable[32 + i] = order_bytes[i];
        }
        REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), malleable.data()) == false);
        for (unsigned int i = 0; i < 32; ++i) {
            malleable[32 + i] = 0xFF;
        }
        REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), malleable.data()) == false);
    }

    // All zeros.
    const std::vector<unsigned char> zeros(gtl::ed25519::signature_size, 0);
    REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), zeros.data()) == false);
    REQUIRE(gtl::ed25519::verify(zeros.data(), message.data(), message.size(), signature.data()) == false);

    // Null pointers are rejected.
    REQUIRE(gtl::ed25519::verify(nullptr, message.data(), message.size(), signature.data()) == false);
    REQUIRE(gtl::ed25519::verify(public_key.data(), nullptr, message.size(), signature.data()) == false);
    REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), nullptr) == false);
}

TEST(ed25519, evaluate, invalid_public_keys) {
    const std::vector<unsigned char> message = reference_message(10);
    const std::vector<unsigned char> signature = hex_2_bytes(references[0].signature);
    // Public keys that are not points on the curve, with either sign.
    for (const char* invalid_point : invalid_points) {
        std::vector<unsigned char> public_key = hex_2_bytes(invalid_point);
        REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), signature.data()) == false);
        public_key[31] |= 0x80;
        REQUIRE(gtl::ed25519::verify(public_key.data(), message.data(), message.size(), signature.data()) == false);
    }

    // The point with a y of one is the neutral point, for which a signature of zero and the neutral point is valid for any message.
    // The same point written with a y that is not fully reduced is not a valid public key, so the same signature is refused.
    const std::vector<unsigned char> neutral = hex_2_bytes(point_one);
    const std::vector<unsigned char> not_canonical = hex_2_bytes(point_one_not_canonical);
    std::vector<unsigned char> neutral_signature(gtl::ed25519::signature_size, 0);
    neutral_signature[0] = 0x01;
    REQUIRE(gtl::ed25519::verify(neutral.data(), message.data(), message.size(), neutral_signature.data()) == true);
    REQUIRE(gtl::ed25519::verify(not_canonical.data(), message.data(), message.size(), neutral_signature.data()) == false);
    // The first half of a signature must be canonical too.
    for (unsigned int i = 0; i < 32; ++i) {
        neutral_signature[i] = not_canonical[i];
    }
    REQUIRE(gtl::ed25519::verify(neutral.data(), message.data(), message.size(), neutral_signature.data()) == false);
}

TEST(ed25519, evaluate, keys) {
    // Different private keys give different public keys and signatures, and each signature is only valid for its own key.
    const std::vector<unsigned char> message = reference_message(50);
    unsigned char public_keys[4][gtl::ed25519::public_key_size] = {};
    unsigned char signatures[4][gtl::ed25519::signature_size] = {};
    for (unsigned int i = 0; i < 4; ++i) {
        const std::vector<unsigned char> private_key = hex_2_bytes(references[i].private_key);
        gtl::ed25519::generate_public_key(private_key.data(), public_keys[i]);
        gtl::ed25519::sign(private_key.data(), message.data(), message.size(), signatures[i]);
        // Signing again gives the same signature.
        unsigned char again[gtl::ed25519::signature_size] = {};
        gtl::ed25519::sign(private_key.data(), message.data(), message.size(), again);
        REQUIRE(testbench::is_memory_same(signatures[i], again, gtl::ed25519::signature_size) == true);
    }
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 4; ++j) {
            REQUIRE(gtl::ed25519::verify(public_keys[i], message.data(), message.size(), signatures[j]) == (i == j));
            if (i != j) {
                REQUIRE(testbench::is_memory_same(public_keys[i], public_keys[j], gtl::ed25519::public_key_size) == false);
                REQUIRE(testbench::is_memory_same(signatures[i], signatures[j], gtl::ed25519::signature_size) == false);
            }
        }
    }
}
