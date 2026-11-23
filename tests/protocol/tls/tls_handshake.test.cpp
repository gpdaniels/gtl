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

#include <protocol/tls/tls_handshake>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <functional>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {
    using handshake = gtl::tls_handshake;

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

    /// @brief  A random generator of reasonable quality with a fixed seed.
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

    // Values computed by another implementation: the authentication codes of test case two of RFC 4231, and the hashes of "abcdef".
    constexpr const char* reference_code_sha2_256 = "5BDCC146BF60754E6A042426089575C75A003F089D2739839DEC58B964EC3843";
    constexpr const char* reference_code_sha2_384 = "AF45D2E376484031617F78D2B58A6B1B9C7EF464F5A01B47E42EC3736322445E8E2240CA5E69E2C78B3239ECFAB21649";
    constexpr const char* reference_hash_sha2_256 = "BEF57EC7F53A6D40BEB640A780A639C83BC29AC8A9816F1FC6C5C6DCD93C4721";
    constexpr const char* reference_hash_sha2_384 = "C6A4C65B227E7387B9C3E839D44869C4CFCA3EF583DEA64117859B808C1E3D8AE689E1E314EEEF52A6FFE22681AA11F5";
    // The random value that marks a hello of a server as a request to retry, which is the sha2-256 hash of "HelloRetryRequest".
    constexpr const char* reference_retry_random = "CF21AD74E59A6111BE1D8C021E65B891C2A211167ABB8C5E079E09E2C8A8339C";

    /// @brief  Write a client hello message with the usual contents for a pair of versions.
    std::vector<unsigned char> make_client_hello(const bool offer_tls12, const bool offer_tls13, const char* host) {
        static const unsigned char random[handshake::random_size] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        static const unsigned char session_id[handshake::session_id_size_max] = { 9, 8, 7, 6 };
        static const unsigned char share_x25519[32] = { 0x11, 0x22 };
        static const unsigned char share_secp256r1[65] = { 0x04, 0x33 };
        const unsigned int groups[2] = { handshake::group_x25519, handshake::group_secp256r1 };
        const unsigned char* const shares[2] = { share_x25519, share_secp256r1 };
        const unsigned int lengths[2] = { 32, 65 };
        handshake::client_hello_options_type options;
        options.offer_tls12 = offer_tls12;
        options.offer_tls13 = offer_tls13;
        options.random = random;
        options.session_id = session_id;
        options.session_id_length = offer_tls13 ? handshake::session_id_size_max : 0u;
        options.host = host;
        options.share_groups = groups;
        options.shares = shares;
        options.share_lengths = lengths;
        options.shares_length = 2;
        std::vector<unsigned char> message;
        handshake::write_client_hello(options, message);
        return message;
    }
}

TEST(tls_handshake, traits, standard) {
    REQUIRE((std::is_pod<gtl::tls_handshake>::value == true));

    REQUIRE((std::is_trivial<gtl::tls_handshake>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::tls_handshake>::value == true));

    REQUIRE((std::is_standard_layout<gtl::tls_handshake>::value == true));
}

TEST(tls_handshake, constructor, empty) {
    gtl::tls_handshake tls_handshake;
    testbench::do_not_optimise_away(tls_handshake);
    handshake::reader reader;
    REQUIRE(reader.is_valid() == true);
    REQUIRE(reader.is_end() == true);
    REQUIRE(reader.get_length() == 0);
    handshake::settings_type settings;
    REQUIRE(settings.is_client == true);
    REQUIRE(settings.verify_certificate == true);
    handshake::sender_type sender;
    REQUIRE(sender.buffer.empty() == true);
    REQUIRE(sender.record.is_protected() == false);
}

TEST(tls_handshake, function, reader) {
    const unsigned char data[12] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x02, 0xAA, 0xBB, 0x00, 0x01, 0xCC };
    handshake::reader reader(data, sizeof(data));
    REQUIRE(reader.get_data() == data);
    REQUIRE(reader.get_length() == 12);
    REQUIRE(reader.read_integer(1) == 0x01);
    REQUIRE(reader.read_integer(2) == 0x0203);
    REQUIRE(reader.read_integer(3) == 0x040506);
    REQUIRE(reader.is_end() == false);
    // A vector with a one byte length, and one with a two byte length.
    handshake::reader vector = reader.read_vector(1);
    REQUIRE(vector.is_valid() == true);
    REQUIRE(vector.get_length() == 2);
    REQUIRE(vector.read_integer(2) == 0xAABB);
    REQUIRE(vector.is_end() == true);
    handshake::reader vector_wide = reader.read_vector(2);
    REQUIRE(vector_wide.get_length() == 1);
    const unsigned char* bytes = vector_wide.read_bytes(1);
    REQUIRE(bytes == &data[11]);
    REQUIRE(reader.is_end() == true);
    REQUIRE(reader.is_valid() == true);

    // Reading past the end returns zeros and makes the reader invalid from then on.
    REQUIRE(reader.read_integer(1) == 0);
    REQUIRE(reader.is_valid() == false);
    REQUIRE(reader.is_end() == false);
    REQUIRE(reader.read_bytes(0) == nullptr);
    handshake::reader short_integer(data, 1);
    REQUIRE(short_integer.read_integer(2) == 0);
    REQUIRE(short_integer.is_valid() == false);
    REQUIRE(short_integer.get_length() == 0);
    // A vector that is longer than the data, and the contents of a vector read from an invalid reader, are invalid.
    const unsigned char long_vector[3] = { 0x05, 0x01, 0x02 };
    handshake::reader reader_long(long_vector, sizeof(long_vector));
    handshake::reader contents = reader_long.read_vector(1);
    REQUIRE(reader_long.is_valid() == false);
    REQUIRE(contents.is_valid() == false);
    REQUIRE(contents.is_end() == false);
    REQUIRE(contents.read_integer(1) == 0);
    // A null buffer has nothing to read.
    handshake::reader reader_null(nullptr, 10);
    REQUIRE(reader_null.is_end() == true);
}

TEST(tls_handshake, function, writer) {
    std::vector<unsigned char> buffer;
    handshake::writer writer(buffer);
    writer.write_integer(0x01, 1);
    writer.write_integer(0x0203, 2);
    writer.write_integer(0x040506, 3);
    writer.write_integer(0x0708090A, 4);
    const unsigned char bytes[2] = { 0xAA, 0xBB };
    writer.write_bytes(bytes, 2);
    writer.write_bytes(nullptr, 0);
    const unsigned long long int outer = writer.begin_vector(2);
    const unsigned long long int inner = writer.begin_vector(1);
    writer.write_bytes(bytes, 2);
    writer.end_vector(inner, 1);
    const unsigned long long int empty = writer.begin_vector(3);
    writer.end_vector(empty, 3);
    writer.end_vector(outer, 2);
    const unsigned long long int message = writer.begin_message(handshake::message_finished);
    writer.write_integer(0xCC, 1);
    writer.end_message(message);
    const std::vector<unsigned char> expected = hex_2_bytes("0102030405060708090AAABB000602AABB00000014000001CC");
    REQUIRE(buffer == expected);

    // What was written can be read back.
    handshake::reader reader(buffer.data(), buffer.size());
    reader.read_bytes(12);
    handshake::reader reader_outer = reader.read_vector(2);
    handshake::reader reader_inner = reader_outer.read_vector(1);
    REQUIRE(reader_inner.read_integer(2) == 0xAABB);
    REQUIRE(reader_outer.read_vector(3).is_end() == true);
    REQUIRE(reader_outer.is_end() == true);
    REQUIRE(reader.read_integer(1) == handshake::message_finished);
    REQUIRE(reader.read_vector(3).read_integer(1) == 0xCC);
    REQUIRE(reader.is_end() == true);
}

TEST(tls_handshake, function, helpers) {
    const unsigned char lhs[4] = { 1, 2, 3, 4 };
    const unsigned char rhs[4] = { 1, 2, 3, 5 };
    REQUIRE(handshake::is_equal(lhs, lhs, 4) == true);
    REQUIRE(handshake::is_equal(lhs, rhs, 4) == false);
    REQUIRE(handshake::is_equal(lhs, rhs, 3) == true);
    REQUIRE(handshake::is_equal(lhs, rhs, 0) == true);

    unsigned char buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    handshake::wipe(buffer, 6);
    const unsigned char wiped[8] = { 0, 0, 0, 0, 0, 0, 7, 8 };
    REQUIRE(testbench::is_memory_same(buffer, wiped, 8) == true);

    // Random bytes fill exactly the buffer, and are the same for the same generator.
    unsigned char random_0[11];
    unsigned char random_1[11];
    for (unsigned int i = 0; i < 11; ++i) {
        random_0[i] = 0xA5;
        random_1[i] = 0xA5;
    }
    handshake::fill_random(pcg_generator(1), random_0, 10);
    handshake::fill_random(pcg_generator(1), random_1, 10);
    REQUIRE(testbench::is_memory_same(random_0, random_1, 11) == true);
    REQUIRE(random_0[10] == 0xA5);
    handshake::fill_random(pcg_generator(2), random_1, 10);
    REQUIRE(testbench::is_memory_same(random_0, random_1, 10) == false);

    const unsigned char list[6] = { 0x13, 0x01, 0x13, 0x03, 0xC0, 0x2B };
    REQUIRE(handshake::has_value(list, 6, 0x1301) == true);
    REQUIRE(handshake::has_value(list, 6, 0xC02B) == true);
    REQUIRE(handshake::has_value(list, 6, 0x1302) == false);
    REQUIRE(handshake::has_value(list, 6, 0x0113) == false);
    REQUIRE(handshake::has_value(list, 5, 0xC02B) == false);
    REQUIRE(handshake::has_value(nullptr, 0, 0x1301) == false);

    REQUIRE(handshake::get_hash_size(handshake::hash_type::sha2_256) == 32);
    REQUIRE(handshake::get_hash_size(handshake::hash_type::sha2_384) == 48);
    REQUIRE(handshake::get_hash_size(handshake::hash_type::sha2_384) == handshake::hash_size_max);

    using status = gtl::x509::status;
    REQUIRE(handshake::get_alert(status::expired) == handshake::alert_certificate_expired);
    REQUIRE(handshake::get_alert(status::not_yet_valid) == handshake::alert_certificate_expired);
    REQUIRE(handshake::get_alert(status::unknown_issuer) == handshake::alert_unknown_authority);
    REQUIRE(handshake::get_alert(status::host_mismatch) == handshake::alert_bad_certificate);
    REQUIRE(handshake::get_alert(status::invalid_signature) == handshake::alert_bad_certificate);
    REQUIRE(handshake::get_alert(status::invalid_purpose) == handshake::alert_unsupported_certificate);
}

TEST(tls_handshake, function, authenticate) {
    const unsigned char key[4] = { 'J', 'e', 'f', 'e' };
    const char* message = "what do ya want for nothing?";
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(message);
    const handshake::hash_type hashes[2] = { handshake::hash_type::sha2_256, handshake::hash_type::sha2_384 };
    const char* references[2] = { reference_code_sha2_256, reference_code_sha2_384 };
    for (unsigned int i = 0; i < 2; ++i) {
        const std::vector<unsigned char> expected = hex_2_bytes(references[i]);
        REQUIRE(expected.size() == handshake::get_hash_size(hashes[i]));
        unsigned char output[handshake::hash_size_max + 1];
        output[expected.size()] = 0xA5;
        // The message in one part, and split into parts with an empty part between them.
        const unsigned char* const parts_whole[1] = { bytes };
        const unsigned long long int lengths_whole[1] = { 28 };
        handshake::authenticate(hashes[i], key, 4, parts_whole, lengths_whole, 1, output);
        REQUIRE(testbench::is_memory_same(output, expected.data(), expected.size()) == true);
        REQUIRE(output[expected.size()] == 0xA5);
        const unsigned char* const parts_split[4] = { bytes, nullptr, bytes + 5, bytes + 27 };
        const unsigned long long int lengths_split[4] = { 5, 0, 22, 1 };
        handshake::authenticate(hashes[i], key, 4, parts_split, lengths_split, 4, output);
        REQUIRE(testbench::is_memory_same(output, expected.data(), expected.size()) == true);
        // A different key gives a different code.
        const unsigned char other_key[4] = { 'J', 'e', 'f', 'f' };
        handshake::authenticate(hashes[i], other_key, 4, parts_whole, lengths_whole, 1, output);
        REQUIRE(testbench::is_memory_same(output, expected.data(), expected.size()) == false);
    }
}

TEST(tls_handshake, function, transcript) {
    const std::vector<unsigned char> expected_sha2_256 = hex_2_bytes(reference_hash_sha2_256);
    const std::vector<unsigned char> expected_sha2_384 = hex_2_bytes(reference_hash_sha2_384);
    const unsigned char data[6] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    handshake::transcript transcript;
    transcript.update(data, 2);
    transcript.update(nullptr, 0);
    // Getting the hash does not end the transcript.
    unsigned char partial[handshake::hash_size_max] = {};
    transcript.get_hash(handshake::hash_type::sha2_256, partial);
    REQUIRE(testbench::is_memory_same(partial, expected_sha2_256.data(), 32) == false);
    transcript.update(data + 2, 4);
    unsigned char output[handshake::hash_size_max] = {};
    for (unsigned int repeat = 0; repeat < 2; ++repeat) {
        transcript.get_hash(handshake::hash_type::sha2_256, output);
        REQUIRE(testbench::is_memory_same(output, expected_sha2_256.data(), 32) == true);
        transcript.get_hash(handshake::hash_type::sha2_384, output);
        REQUIRE(testbench::is_memory_same(output, expected_sha2_384.data(), 48) == true);
    }
    // A copy continues separately, and a reset starts again.
    handshake::transcript copy = transcript;
    copy.update(data, 1);
    copy.get_hash(handshake::hash_type::sha2_256, output);
    REQUIRE(testbench::is_memory_same(output, expected_sha2_256.data(), 32) == false);
    transcript.reset();
    transcript.update(data, 6);
    transcript.get_hash(handshake::hash_type::sha2_256, output);
    REQUIRE(testbench::is_memory_same(output, expected_sha2_256.data(), 32) == true);
}

TEST(tls_handshake, function, exchange) {
    const unsigned int groups[3] = { handshake::group_x25519, handshake::group_secp256r1, handshake::group_secp384r1 };
    const unsigned int public_lengths[3] = { 32, 65, 97 };
    const unsigned int secret_lengths[3] = { 32, 32, 48 };
    REQUIRE(handshake::is_supported_group(0) == false);
    REQUIRE(handshake::is_supported_group(25) == false);
    for (unsigned int i = 0; i < 3; ++i) {
        REQUIRE(handshake::is_supported_group(groups[i]) == true);
        unsigned char private_values[2][handshake::exchange_private_size_max] = {};
        unsigned char public_values[2][handshake::exchange_public_size_max] = {};
        unsigned int lengths[2] = {};
        REQUIRE(handshake::generate_exchange(groups[i], pcg_generator(10 + i), private_values[0], public_values[0], lengths[0]) == true);
        REQUIRE(handshake::generate_exchange(groups[i], pcg_generator(20 + i), private_values[1], public_values[1], lengths[1]) == true);
        REQUIRE(lengths[0] == public_lengths[i]);
        REQUIRE(lengths[1] == public_lengths[i]);
        REQUIRE(testbench::is_memory_same(public_values[0], public_values[1], lengths[0]) == false);
        // Both ends compute the same secret.
        unsigned char secrets[2][handshake::exchange_secret_size_max] = {};
        unsigned int secret_length[2] = {};
        REQUIRE(handshake::compute_exchange(groups[i], private_values[0], public_values[1], lengths[1], secrets[0], secret_length[0]) == true);
        REQUIRE(handshake::compute_exchange(groups[i], private_values[1], public_values[0], lengths[0], secrets[1], secret_length[1]) == true);
        REQUIRE(secret_length[0] == secret_lengths[i]);
        REQUIRE(secret_length[1] == secret_lengths[i]);
        REQUIRE(testbench::is_memory_same(secrets[0], secrets[1], secret_length[0]) == true);

        // Public values of the wrong length, of the wrong form, or that are not valid, are refused.
        unsigned int length = 1234;
        REQUIRE(handshake::compute_exchange(groups[i], private_values[0], public_values[1], lengths[1] - 1, secrets[0], length) == false);
        REQUIRE(handshake::compute_exchange(groups[i], private_values[0], public_values[1], lengths[1] + 1, secrets[0], length) == false);
        REQUIRE(handshake::compute_exchange(groups[i], private_values[0], nullptr, lengths[1], secrets[0], length) == false);
        unsigned char zeros[handshake::exchange_public_size_max] = {};
        REQUIRE(handshake::compute_exchange(groups[i], private_values[0], zeros, lengths[1], secrets[0], length) == false);
        if (groups[i] != handshake::group_x25519) {
            unsigned char changed[handshake::exchange_public_size_max] = {};
            for (unsigned int j = 0; j < lengths[1]; ++j) {
                changed[j] = public_values[1][j];
            }
            changed[lengths[1] - 1] ^= 0x01;
            REQUIRE(handshake::compute_exchange(groups[i], private_values[0], changed, lengths[1], secrets[0], length) == false);
            changed[lengths[1] - 1] ^= 0x01;
            changed[0] = 0x02;
            REQUIRE(handshake::compute_exchange(groups[i], private_values[0], changed, lengths[1], secrets[0], length) == false);
        }
        REQUIRE(length == 1234);
        // A random generator is required.
        REQUIRE(handshake::generate_exchange(groups[i], std::function<unsigned int()>(), private_values[0], public_values[0], lengths[0]) == false);
    }
    unsigned char private_value[handshake::exchange_private_size_max] = {};
    unsigned char public_value[handshake::exchange_public_size_max] = {};
    unsigned int length = 0;
    REQUIRE(handshake::generate_exchange(25, pcg_generator(1), private_value, public_value, length) == false);
    REQUIRE(handshake::compute_exchange(25, private_value, public_value, 32, private_value, length) == false);
}

TEST(tls_handshake, function, signatures) {
    using scheme = gtl::x509::signature_scheme;
    using key = gtl::x509::key_algorithm;
    REQUIRE(handshake::get_signature_scheme(handshake::signature_rsa_pkcs1_sha256) == scheme::rsa_pkcs1_sha2_256);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_rsa_pkcs1_sha384) == scheme::rsa_pkcs1_sha2_384);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_rsa_pkcs1_sha512) == scheme::rsa_pkcs1_sha2_512);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_rsa_pss_sha256) == scheme::rsa_pss_sha2_256);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_rsa_pss_sha384) == scheme::rsa_pss_sha2_384);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_rsa_pss_sha512) == scheme::rsa_pss_sha2_512);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_ecdsa_sha256) == scheme::ecdsa_sha2_256);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_ecdsa_sha384) == scheme::ecdsa_sha2_384);
    REQUIRE(handshake::get_signature_scheme(handshake::signature_ecdsa_sha512) == scheme::ecdsa_sha2_512);
    // The schemes that use sha1, and one that does not exist.
    REQUIRE(handshake::get_signature_scheme(0x0201) == scheme::unknown);
    REQUIRE(handshake::get_signature_scheme(0x0203) == scheme::unknown);
    REQUIRE(handshake::get_signature_scheme(0x0000) == scheme::unknown);

    // In TLS 1.3 the older rsa padding is not allowed, and each elliptic curve scheme is tied to one curve.
    REQUIRE(handshake::is_signature_allowed(handshake::signature_rsa_pss_sha256, key::rsa, true) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_rsa_pss_sha256, key::rsa, false) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_rsa_pkcs1_sha256, key::rsa, true) == false);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_rsa_pkcs1_sha256, key::rsa, false) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_rsa_pss_sha256, key::secp256r1, false) == false);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha256, key::rsa, false) == false);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha256, key::secp256r1, true) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha256, key::secp384r1, true) == false);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha256, key::secp384r1, false) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha384, key::secp384r1, true) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha384, key::secp256r1, true) == false);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha384, key::secp256r1, false) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha512, key::secp256r1, true) == false);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha512, key::secp256r1, false) == true);
    REQUIRE(handshake::is_signature_allowed(handshake::signature_ecdsa_sha256, key::unknown, false) == false);
    REQUIRE(handshake::is_signature_allowed(0x0201, key::rsa, false) == false);

    // The scheme that is chosen depends on the key, the version, and what the peer supports, not on the order the peer lists them in.
    const unsigned char all[18] = {
        0x02,
        0x01,
        0x06,
        0x03,
        0x06,
        0x01,
        0x05,
        0x03,
        0x04,
        0x03,
        0x04,
        0x01,
        0x05,
        0x01,
        0x08,
        0x06,
        0x08,
        0x05,
    };
    const unsigned char all_and_pss[20] = { 0x02, 0x01, 0x06, 0x03, 0x06, 0x01, 0x05, 0x03, 0x04, 0x03, 0x04, 0x01, 0x05, 0x01, 0x08, 0x06, 0x08, 0x05, 0x08, 0x04 };
    REQUIRE(handshake::choose_signature(key::rsa, all_and_pss, 20, true) == handshake::signature_rsa_pss_sha256);
    REQUIRE(handshake::choose_signature(key::rsa, all, 18, true) == handshake::signature_rsa_pss_sha384);
    REQUIRE(handshake::choose_signature(key::rsa, all, 14, true) == 0);
    REQUIRE(handshake::choose_signature(key::rsa, all, 14, false) == handshake::signature_rsa_pkcs1_sha256);
    REQUIRE(handshake::choose_signature(key::rsa, all, 6, false) == handshake::signature_rsa_pkcs1_sha512);
    REQUIRE(handshake::choose_signature(key::rsa, all, 4, false) == 0);
    REQUIRE(handshake::choose_signature(key::secp256r1, all, 18, true) == handshake::signature_ecdsa_sha256);
    REQUIRE(handshake::choose_signature(key::secp384r1, all, 18, true) == handshake::signature_ecdsa_sha384);
    REQUIRE(handshake::choose_signature(key::secp256r1, all, 8, true) == 0);
    REQUIRE(handshake::choose_signature(key::secp256r1, all, 8, false) == handshake::signature_ecdsa_sha384);
    REQUIRE(handshake::choose_signature(key::secp256r1, all, 4, false) == handshake::signature_ecdsa_sha512);
    REQUIRE(handshake::choose_signature(key::secp384r1, all, 10, false) == handshake::signature_ecdsa_sha384);
    REQUIRE(handshake::choose_signature(key::secp256r1, all, 2, false) == 0);
    REQUIRE(handshake::choose_signature(key::unknown, all, 18, false) == 0);
    REQUIRE(handshake::choose_signature(key::rsa, nullptr, 0, false) == 0);
}

TEST(tls_handshake, function, sender) {
    handshake::sender_type sender;
    // Data that fits in one record, data that needs several, and no data at all which is sent as an empty record.
    std::vector<unsigned char> data(40000, 0);
    for (unsigned long long int i = 0; i < data.size(); ++i) {
        data[i] = static_cast<unsigned char>(i * 3);
    }
    REQUIRE(sender.send(gtl::tls_record::content_handshake, data.data(), 100) == true);
    REQUIRE(sender.buffer.size() == 105);
    REQUIRE(sender.send(gtl::tls_record::content_application_data, data.data(), 40000) == true);
    REQUIRE(sender.buffer.size() == (105 + 40000 + (3 * 5)));
    REQUIRE(sender.send(gtl::tls_record::content_application_data, nullptr, 0) == true);
    REQUIRE(sender.buffer.size() == (105 + 40000 + (3 * 5) + 5));
    // Walk the records, putting the fragments back together.
    std::vector<unsigned char> joined;
    unsigned int records = 0;
    for (unsigned long long int offset = 0; offset < sender.buffer.size(); ++records) {
        unsigned char content_type = 0;
        unsigned int body_length = 0;
        REQUIRE(gtl::tls_record::parse_header(&sender.buffer[offset], content_type, body_length) == true);
        REQUIRE(content_type == ((records == 0) ? gtl::tls_record::content_handshake : gtl::tls_record::content_application_data));
        REQUIRE(body_length <= gtl::tls_record::fragment_size_max);
        if (records != 0) {
            joined.insert(joined.end(), &sender.buffer[offset + 5], &sender.buffer[offset + 5] + body_length);
        }
        offset += 5 + body_length;
    }
    REQUIRE(records == 5);
    REQUIRE(joined == data);
}

TEST(tls_handshake, function, client_hello) {
    // A hello that offers both versions.
    {
        const std::vector<unsigned char> message = make_client_hello(true, true, "example.test");
        REQUIRE(message[0] == handshake::message_client_hello);
        REQUIRE(message.size() == (4u + (static_cast<unsigned int>(message[1]) << 16) + (static_cast<unsigned int>(message[2]) << 8) + message[3]));
        handshake::client_hello_type hello;
        REQUIRE(handshake::parse_client_hello(message.data() + 4, message.size() - 4, hello) == true);
        REQUIRE(hello.legacy_version == handshake::version_tls12);
        REQUIRE(hello.random[0] == 1);
        REQUIRE(hello.random[7] == 8);
        REQUIRE(hello.session_id_length == 32);
        REQUIRE(hello.session_id[0] == 9);
        REQUIRE(hello.suites_length == 20);
        REQUIRE(handshake::has_value(hello.suites, hello.suites_length, handshake::suite_aes_128_gcm_sha256) == true);
        REQUIRE(handshake::has_value(hello.suites, hello.suites_length, handshake::suite_aes_256_gcm_sha384) == true);
        REQUIRE(handshake::has_value(hello.suites, hello.suites_length, handshake::suite_chacha20_poly1305_sha256) == true);
        REQUIRE(handshake::has_value(hello.suites, hello.suites_length, handshake::suite_ecdhe_ecdsa_aes_128_gcm_sha256) == true);
        REQUIRE(handshake::has_value(hello.suites, hello.suites_length, handshake::suite_ecdhe_rsa_chacha20_poly1305_sha256) == true);
        REQUIRE(hello.versions_length == 4);
        REQUIRE(handshake::has_value(hello.versions, hello.versions_length, handshake::version_tls13) == true);
        REQUIRE(handshake::has_value(hello.versions, hello.versions_length, handshake::version_tls12) == true);
        REQUIRE(hello.groups_length == 6);
        REQUIRE(handshake::has_value(hello.groups, hello.groups_length, handshake::group_x25519) == true);
        REQUIRE(handshake::has_value(hello.groups, hello.groups_length, handshake::group_secp384r1) == true);
        REQUIRE(hello.signatures_length == 18);
        REQUIRE(handshake::has_value(hello.signatures, hello.signatures_length, handshake::signature_rsa_pss_sha256) == true);
        REQUIRE(hello.host_length == 12);
        REQUIRE(testbench::is_memory_same(hello.host, "example.test", 12) == true);
        REQUIRE(hello.has_extended_master_secret == true);
        REQUIRE(hello.has_renegotiation_info == true);
        // The two key shares, each a group and a public value.
        REQUIRE(hello.shares != nullptr);
        REQUIRE(hello.shares_length == (4 + 32 + 4 + 65));
        handshake::reader shares(hello.shares, hello.shares_length);
        REQUIRE(shares.read_integer(2) == handshake::group_x25519);
        handshake::reader share = shares.read_vector(2);
        REQUIRE(share.get_length() == 32);
        REQUIRE(share.read_integer(2) == 0x1122);
        REQUIRE(shares.read_integer(2) == handshake::group_secp256r1);
        REQUIRE(shares.read_vector(2).get_length() == 65);
        REQUIRE(shares.is_end() == true);

        // Every truncation of the message is refused, other than the one that removes exactly the extensions, which are optional.
        // A failed parse does not change the result.
        const unsigned long long int extensions_offset = 2 + 32 + 1 + 32 + 2 + 20 + 2;
        handshake::client_hello_type unchanged;
        unchanged.legacy_version = 77;
        for (unsigned long long int length = 0; length < (message.size() - 4); ++length) {
            const std::vector<unsigned char> truncated(message.begin() + 4, message.begin() + 4 + static_cast<long long int>(length));
            if (length == extensions_offset) {
                handshake::client_hello_type without_extensions;
                REQUIRE(handshake::parse_client_hello(truncated.data(), truncated.size(), without_extensions) == true);
                REQUIRE(without_extensions.suites_length == 20);
                REQUIRE(without_extensions.versions == nullptr);
                REQUIRE(without_extensions.groups == nullptr);
            }
            else {
                REQUIRE_SILENT(handshake::parse_client_hello(truncated.data(), truncated.size(), unchanged) == false);
            }
        }
        REQUIRE(unchanged.legacy_version == 77);
        // Extra data after the extensions is refused.
        std::vector<unsigned char> extended(message.begin() + 4, message.end());
        extended.push_back(0x00);
        REQUIRE(handshake::parse_client_hello(extended.data(), extended.size(), unchanged) == false);
    }
    // A hello that only offers TLS 1.2 has no session identifier, versions, or key shares.
    {
        const std::vector<unsigned char> message = make_client_hello(true, false, nullptr);
        handshake::client_hello_type hello;
        REQUIRE(handshake::parse_client_hello(message.data() + 4, message.size() - 4, hello) == true);
        REQUIRE(hello.session_id_length == 0);
        REQUIRE(hello.suites_length == 14);
        REQUIRE(handshake::has_value(hello.suites, hello.suites_length, handshake::suite_aes_128_gcm_sha256) == false);
        REQUIRE(hello.versions == nullptr);
        REQUIRE(hello.shares == nullptr);
        REQUIRE(hello.host == nullptr);
        REQUIRE(hello.groups_length == 6);
        REQUIRE(hello.has_extended_master_secret == true);
        REQUIRE(hello.has_renegotiation_info == true);
    }
    // A hello that only offers TLS 1.3 has none of what only the older version uses.
    {
        const std::vector<unsigned char> message = make_client_hello(false, true, "a");
        handshake::client_hello_type hello;
        REQUIRE(handshake::parse_client_hello(message.data() + 4, message.size() - 4, hello) == true);
        REQUIRE(hello.suites_length == 6);
        REQUIRE(hello.versions_length == 2);
        REQUIRE(hello.host_length == 1);
        REQUIRE(hello.has_extended_master_secret == false);
        REQUIRE(hello.has_renegotiation_info == false);
        REQUIRE(hello.shares_length == (4 + 32 + 4 + 65));
    }
    // The smallest valid hello, of the kind an old client sends with no extensions at all.
    {
        const std::vector<unsigned char> minimal = hex_2_bytes(
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
        );
        handshake::client_hello_type hello;
        REQUIRE(handshake::parse_client_hello(minimal.data(), minimal.size(), hello) == true);
        REQUIRE(hello.session_id_length == 0);
        REQUIRE(hello.suites_length == 2);
        REQUIRE(hello.groups == nullptr);
        REQUIRE(hello.signatures == nullptr);
        // No cipher suites, an odd length of cipher suites, and no way to not compress.
        const char* invalid[] = {
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0000"
            "0100",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0003C02F00"
            "0100",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0101",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "00",
            // An extension that appears twice, and extensions that are not well formed.
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "0010"
            "000A00040002001D"
            "000A00040002001D",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "0007"
            "000A000300021D",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "0005"
            "0017000100",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "0006"
            "FF0100020100",
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "0008"
            "003300040002001D",
        };
        for (const char* hex : invalid) {
            const std::vector<unsigned char> bytes = hex_2_bytes(hex);
            REQUIRE(handshake::parse_client_hello(bytes.data(), bytes.size(), hello) == false, "Parsed the invalid hello '%s'.", hex);
        }
        // Extensions that are not known are ignored, and an empty list of key shares is not the same as none.
        const std::vector<unsigned char> unknown = hex_2_bytes(
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "000F"
            "FFFF0003AABBCC"
            "003300020000"
            "0000"
            "0000"
        );
        REQUIRE(handshake::parse_client_hello(unknown.data(), unknown.size(), hello) == false);
        const std::vector<unsigned char> empty_shares = hex_2_bytes(
            "0303"
            "0000000000000000000000000000000000000000000000000000000000000000"
            "00"
            "0002C02F"
            "0100"
            "000D"
            "FFFF0003AABBCC"
            "003300020000"
        );
        REQUIRE(handshake::parse_client_hello(empty_shares.data(), empty_shares.size(), hello) == true);
        REQUIRE(hello.shares != nullptr);
        REQUIRE(hello.shares_length == 0);
    }
}

TEST(tls_handshake, function, server_hello) {
    const char* const prefix =
        "0303"
        "0102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F20"
        "04AABBCCDD";
    // The parsed hello is a view of the bytes, so the bytes are kept until the next hello is parsed.
    std::vector<unsigned char> bytes;
    const auto parse = [&](const char* rest, handshake::server_hello_type& parsed) -> bool {
        bytes = hex_2_bytes(prefix);
        const std::vector<unsigned char> tail = hex_2_bytes(rest);
        bytes.insert(bytes.end(), tail.begin(), tail.end());
        return handshake::parse_server_hello(bytes.data(), bytes.size(), parsed);
    };
    handshake::server_hello_type hello;
    // A hello of TLS 1.3: a cipher suite, the version, and a key share.
    REQUIRE(parse("130100"
                  "002E"
                  "002B00020304"
                  "00330024001D0020"
                  "1111111111111111111111111111111111111111111111111111111111111111",
                  hello) == true);
    REQUIRE(hello.legacy_version == handshake::version_tls12);
    REQUIRE(hello.random[0] == 0x01);
    REQUIRE(hello.random[31] == 0x20);
    REQUIRE(hello.session_id_length == 4);
    REQUIRE(hello.session_id[0] == 0xAA);
    REQUIRE(hello.suite == handshake::suite_aes_128_gcm_sha256);
    REQUIRE(hello.selected_version == handshake::version_tls13);
    REQUIRE(hello.share_group == handshake::group_x25519);
    REQUIRE(hello.share_length == 32);
    REQUIRE(hello.share[0] == 0x11);
    REQUIRE(hello.has_extended_master_secret == false);
    REQUIRE(hello.has_renegotiation_info == false);
    REQUIRE(hello.is_retry_request == false);
    REQUIRE(hello.has_unexpected_extension == false);

    // A hello of TLS 1.2: with no extensions, and with the extensions that version uses.
    REQUIRE(parse("C02F00", hello) == true);
    REQUIRE(hello.suite == handshake::suite_ecdhe_rsa_aes_128_gcm_sha256);
    REQUIRE(hello.selected_version == 0);
    REQUIRE(hello.share_group == 0);
    REQUIRE(hello.share == nullptr);
    REQUIRE(parse("C02F00"
                  "0013"
                  "FF01000100"
                  "00170000"
                  "000B00020100"
                  "00000000",
                  hello) == true);
    REQUIRE(hello.has_extended_master_secret == true);
    REQUIRE(hello.has_renegotiation_info == true);
    REQUIRE(hello.has_unexpected_extension == false);

    // An extension that was not asked for is noted, for the handshake to refuse.
    REQUIRE(parse("C02F00"
                  "0008"
                  "00230000"
                  "00170000",
                  hello) == true);
    REQUIRE(hello.has_unexpected_extension == true);
    REQUIRE(hello.has_extended_master_secret == true);

    // Compression, extensions that appear twice or are not well formed, and extra data.
    hello.suite = 77;
    REQUIRE(parse("C02F01", hello) == false);
    REQUIRE(parse("C02F", hello) == false);
    REQUIRE(parse("C02F00"
                  "0008"
                  "00170000"
                  "00170000",
                  hello) == false);
    REQUIRE(parse("C02F00"
                  "000C"
                  "002B00020304"
                  "002B00020304",
                  hello) == false);
    REQUIRE(parse("C02F00"
                  "0007"
                  "002B0003030400",
                  hello) == false);
    REQUIRE(parse("C02F00"
                  "0006"
                  "FF0100020100",
                  hello) == false);
    REQUIRE(parse("C02F00"
                  "0004"
                  "00170000"
                  "00",
                  hello) == false);
    REQUIRE(parse("C02F00"
                  "0005"
                  "00170000",
                  hello) == false);
    REQUIRE(parse("130100"
                  "000A"
                  "00330006001D0003AABB",
                  hello) == false);
    REQUIRE(hello.suite == 77);

    // A request to retry is marked by its random value, and names a group without a public value.
    {
        bytes = hex_2_bytes("0303");
        const std::vector<unsigned char> retry_random = hex_2_bytes(reference_retry_random);
        REQUIRE(retry_random.size() == handshake::random_size);
        bytes.insert(bytes.end(), retry_random.begin(), retry_random.end());
        const std::vector<unsigned char> tail = hex_2_bytes(
            "00"
            "130100"
            "000C"
            "002B00020304"
            "003300020017"
        );
        bytes.insert(bytes.end(), tail.begin(), tail.end());
        REQUIRE(handshake::parse_server_hello(bytes.data(), bytes.size(), hello) == true);
        REQUIRE(hello.is_retry_request == true);
        REQUIRE(hello.share_group == handshake::group_secp256r1);
        REQUIRE(hello.share == nullptr);
    }
}
