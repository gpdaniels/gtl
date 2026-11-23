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

#include <crypto/certificate/pem>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <string>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace {
    /// @brief  Generate a buffer where each byte is its index times thirty seven plus eleven.
    std::vector<unsigned char> pattern(const unsigned long long int length) {
        std::vector<unsigned char> bytes;
        for (unsigned long long int i = 0; i < length; ++i) {
            bytes.push_back(static_cast<unsigned char>(((i * 37) + 11) & 0xFF));
        }
        return bytes;
    }

    // Blocks with the label "TEST DATA" that hold pattern(length), encoded by another implementation.
    struct reference_type {
        unsigned long long int length;
        const char* text;
    };

    constexpr const reference_type references[] = {
        { 0,
          "-----BEGIN TEST DATA-----\n"
          "-----END TEST DATA-----\n" },
        { 1,
          "-----BEGIN TEST DATA-----\n"
          "Cw==\n"
          "-----END TEST DATA-----\n" },
        { 2,
          "-----BEGIN TEST DATA-----\n"
          "CzA=\n"
          "-----END TEST DATA-----\n" },
        { 3,
          "-----BEGIN TEST DATA-----\n"
          "CzBV\n"
          "-----END TEST DATA-----\n" },
        { 4,
          "-----BEGIN TEST DATA-----\n"
          "CzBVeg==\n"
          "-----END TEST DATA-----\n" },
        { 5,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep8=\n"
          "-----END TEST DATA-----\n" },
        { 6,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E\n"
          "-----END TEST DATA-----\n" },
        { 47,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLE=\n"
          "-----END TEST DATA-----\n" },
        { 48,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLHW\n"
          "-----END TEST DATA-----\n" },
        { 49,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLHW\n"
          "+w==\n"
          "-----END TEST DATA-----\n" },
        { 95,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLHW\n"
          "+yBFao+02f4jSG2St9wBJktwlbrfBClOc5i94gcsUXabwOUKL1R5nsPoDTJXfKE=\n"
          "-----END TEST DATA-----\n" },
        { 96,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLHW\n"
          "+yBFao+02f4jSG2St9wBJktwlbrfBClOc5i94gcsUXabwOUKL1R5nsPoDTJXfKHG\n"
          "-----END TEST DATA-----\n" },
        { 97,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLHW\n"
          "+yBFao+02f4jSG2St9wBJktwlbrfBClOc5i94gcsUXabwOUKL1R5nsPoDTJXfKHG\n"
          "6w==\n"
          "-----END TEST DATA-----\n" },
        { 200,
          "-----BEGIN TEST DATA-----\n"
          "CzBVep/E6Q4zWH2ix+wRNluApcrvFDleg6jN8hc8YYar0PUaP2SJrtP4HUJnjLHW\n"
          "+yBFao+02f4jSG2St9wBJktwlbrfBClOc5i94gcsUXabwOUKL1R5nsPoDTJXfKHG\n"
          "6xA1Wn+kye4TOF2Cp8zxFjtgharP9Bk+Y4it0vccQWaLsNX6H0RpjrPY/SJHbJG2\n"
          "2wAlSm+Uud4DKE1yl7zhBitQdZq/5AkuU3idwucMMVZ7oMXqDzRZfqPI7RI3XIGm\n"
          "y/AVOl+Eqc4=\n"
          "-----END TEST DATA-----\n" },
    };

    /// @brief  Decode the first block with a label from a string, returning whether it was decoded.
    bool decode(const std::string& text, const char* label, std::vector<unsigned char>& data) {
        std::vector<unsigned char> buffer(text.size() + 1, 0xA5);
        unsigned long long int length = 0;
        if (!gtl::pem::decode(text.data(), text.size(), label, buffer.data(), buffer.size(), length)) {
            return false;
        }
        data.assign(buffer.begin(), buffer.begin() + static_cast<long long int>(length));
        return true;
    }
}

TEST(pem, traits, standard) {
    REQUIRE((std::is_pod<gtl::pem>::value == true));

    REQUIRE((std::is_trivial<gtl::pem>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::pem>::value == true));

    REQUIRE((std::is_standard_layout<gtl::pem>::value == true));
}

TEST(pem, constructor, empty) {
    gtl::pem pem;
    testbench::do_not_optimise_away(pem);
}

TEST(pem, function, encode) {
    for (const reference_type& reference : references) {
        const std::vector<unsigned char> data = pattern(reference.length);
        const std::string expected = reference.text;
        REQUIRE(gtl::pem::get_encoded_length(reference.length, "TEST DATA") == expected.size(), "Encoded length mismatch for %llu bytes.", reference.length);

        std::vector<char> output(expected.size() + 1, '#');
        unsigned long long int output_length = 0;
        REQUIRE(gtl::pem::encode(data.empty() ? nullptr : data.data(), data.size(), "TEST DATA", output.data(), expected.size(), output_length) == true);
        REQUIRE(output_length == expected.size());
        REQUIRE(std::string(output.data(), output_length) == expected, "Encoded text mismatch for %llu bytes.", reference.length);
        // Nothing is written past the end of the text.
        REQUIRE(output[expected.size()] == '#');

        // A buffer that is one character too small is not written to.
        std::vector<char> small(expected.size(), '#');
        output_length = 1234;
        REQUIRE(gtl::pem::encode(data.empty() ? nullptr : data.data(), data.size(), "TEST DATA", small.data(), expected.size() - 1, output_length) == false);
        REQUIRE(output_length == 1234);
        REQUIRE(small[0] == '#');
    }

    // Null pointers are rejected.
    const unsigned char data[3] = { 1, 2, 3 };
    char output[128] = {};
    unsigned long long int output_length = 0;
    REQUIRE(gtl::pem::encode(nullptr, 3, "TEST DATA", output, sizeof(output), output_length) == false);
    REQUIRE(gtl::pem::encode(data, 3, nullptr, output, sizeof(output), output_length) == false);
    REQUIRE(gtl::pem::encode(data, 3, "TEST DATA", nullptr, sizeof(output), output_length) == false);
}

TEST(pem, function, decode) {
    for (const reference_type& reference : references) {
        const std::vector<unsigned char> expected = pattern(reference.length);
        const std::string text = reference.text;

        std::vector<unsigned char> output(reference.length + 1, 0xA5);
        unsigned long long int output_length = 1234;
        REQUIRE(gtl::pem::decode(text.data(), text.size(), "TEST DATA", output.data(), reference.length, output_length) == true, "Failed to decode %llu bytes.", reference.length);
        REQUIRE(output_length == reference.length);
        REQUIRE(testbench::is_memory_same(output.data(), expected.data(), reference.length));
        REQUIRE(output[reference.length] == 0xA5);

        // A buffer that is one byte too small.
        if (reference.length > 0) {
            output_length = 1234;
            REQUIRE(gtl::pem::decode(text.data(), text.size(), "TEST DATA", output.data(), reference.length - 1, output_length) == false);
            REQUIRE(output_length == 1234);
            REQUIRE(output[reference.length] == 0xA5);
        }

        // A different label is not found.
        REQUIRE(gtl::pem::decode(text.data(), text.size(), "TEST", output.data(), output.size(), output_length) == false);
        REQUIRE(gtl::pem::decode(text.data(), text.size(), "DATA", output.data(), output.size(), output_length) == false);
        REQUIRE(gtl::pem::decode(text.data(), text.size(), "TEST DATA ", output.data(), output.size(), output_length) == false);
        REQUIRE(gtl::pem::decode(text.data(), text.size(), "", output.data(), output.size(), output_length) == false);

        // Text that is cut short is not decoded.
        for (unsigned long long int length = 0; length < (text.size() - 1); ++length) {
            const std::string truncated = text.substr(0, length);
            REQUIRE_SILENT(gtl::pem::decode(truncated.data(), truncated.size(), "TEST DATA", output.data(), output.size(), output_length) == false);
        }
    }

    // Null pointers are rejected.
    unsigned char output[16] = {};
    unsigned long long int output_length = 0;
    REQUIRE(gtl::pem::decode(nullptr, 10, "TEST DATA", output, sizeof(output), output_length) == false);
    REQUIRE(gtl::pem::decode(references[0].text, 10, nullptr, output, sizeof(output), output_length) == false);
    REQUIRE(gtl::pem::decode(references[0].text, 10, "TEST DATA", nullptr, sizeof(output), output_length) == false);
}

TEST(pem, evaluate, layout) {
    const std::vector<unsigned char> expected = pattern(5);
    std::vector<unsigned char> data;

    // Other line endings, extra white space, missing line breaks, and surrounding text are all accepted.
    REQUIRE(decode("-----BEGIN TEST DATA-----\r\nCzBVep8=\r\n-----END TEST DATA-----\r\n", "TEST DATA", data) == true);
    REQUIRE(data == expected);
    REQUIRE(decode("-----BEGIN TEST DATA-----CzBVep8=-----END TEST DATA-----", "TEST DATA", data) == true);
    REQUIRE(data == expected);
    REQUIRE(decode("-----BEGIN TEST DATA-----\n  Cz BV\n\n\tep 8 =\n-----END TEST DATA-----", "TEST DATA", data) == true);
    REQUIRE(data == expected);
    REQUIRE(decode("Some text before\n-----BEGIN TEST DATA-----\nCzBVep8=\n-----END TEST DATA-----\nSome text after", "TEST DATA", data) == true);
    REQUIRE(data == expected);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBV\nep8=\n-----END TEST DATA-----\n", "TEST DATA", data) == true);
    REQUIRE(data == expected);

    // Characters that are not base64, including the url safe alphabet.
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBV*p8=\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBV_p8=\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nHeader: value\n\nCzBVep8=\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    // Groups that are not complete, and padding that is missing, misplaced, or too long.
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVe\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8==\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVe===\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzB=ep8=\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8=CzBV\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\n=zBVep8=\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    // Unused bits that are not zero.
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep9=\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVeh==\n-----END TEST DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVeg==\n-----END TEST DATA-----\n", "TEST DATA", data) == true);
    // The end of the block is missing or has another label.
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8=\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8=\n-----END OTHER DATA-----\n", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8=\n-----END TEST DATA----", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA-----\nCzBVep8=\n-----BEGIN TEST DATA-----\n", "TEST DATA", data) == false);
    // Not a block at all.
    REQUIRE(decode("", "TEST DATA", data) == false);
    REQUIRE(decode("CzBVep8=", "TEST DATA", data) == false);
    REQUIRE(decode("-----BEGIN TEST DATA----", "TEST DATA", data) == false);
}

TEST(pem, evaluate, multiple) {
    // Blocks are found by label, and consecutive blocks with the same label are decoded by passing the offset back in.
    const std::string text =
        "-----BEGIN FIRST-----\nAQID\n-----END FIRST-----\n"
        "-----BEGIN SECOND-----\nBAUG\n-----END SECOND-----\n"
        "-----BEGIN FIRST-----\nBwgJ\n-----END FIRST-----\n"
        "-----BEGIN FIRST-----\nCgsM\n-----END FIRST-----\n";
    unsigned char output[8] = {};
    unsigned long long int output_length = 0;
    unsigned long long int offset = 0;
    REQUIRE(gtl::pem::decode(text.data(), text.size(), "SECOND", output, sizeof(output), output_length, offset) == true);
    REQUIRE(output_length == 3);
    REQUIRE(output[0] == 4);
    REQUIRE(gtl::pem::decode(text.data(), text.size(), "SECOND", output, sizeof(output), output_length, offset) == false);

    offset = 0;
    for (unsigned char block = 0; block < 3; ++block) {
        REQUIRE(gtl::pem::decode(text.data(), text.size(), "FIRST", output, sizeof(output), output_length, offset) == true);
        REQUIRE(output_length == 3);
        const unsigned char first = static_cast<unsigned char>((block == 0) ? 1 : (4 + (block * 3)));
        REQUIRE(output[0] == first);
        REQUIRE(output[1] == first + 1);
        REQUIRE(output[2] == first + 2);
    }
    REQUIRE(offset == text.size() - 1);
    REQUIRE(gtl::pem::decode(text.data(), text.size(), "FIRST", output, sizeof(output), output_length, offset) == false);
    REQUIRE(offset == text.size() - 1);

    // An offset past the end of the text finds nothing.
    offset = text.size() + 10;
    REQUIRE(gtl::pem::decode(text.data(), text.size(), "FIRST", output, sizeof(output), output_length, offset) == false);
}

TEST(pem, evaluate, round_trip) {
    for (unsigned long long int length = 0; length < 300; ++length) {
        const std::vector<unsigned char> data = pattern(length);
        std::vector<char> text(gtl::pem::get_encoded_length(length, "ROUND TRIP"), 0);
        unsigned long long int text_length = 0;
        REQUIRE_SILENT(gtl::pem::encode(data.empty() ? nullptr : data.data(), length, "ROUND TRIP", text.data(), text.size(), text_length) == true);
        REQUIRE_SILENT(text_length == text.size());
        std::vector<unsigned char> decoded(length + 1, 0);
        unsigned long long int decoded_length = 0;
        REQUIRE_SILENT(gtl::pem::decode(text.data(), text_length, "ROUND TRIP", decoded.data(), length, decoded_length) == true);
        REQUIRE_SILENT(decoded_length == length);
        REQUIRE_SILENT(testbench::is_memory_same(decoded.data(), data.data(), length));
    }
}
