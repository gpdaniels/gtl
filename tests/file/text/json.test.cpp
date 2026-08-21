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

#include <file/text/json>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(json, traits, standard) {
    REQUIRE((std::is_pod<gtl::json>::value == false));

    REQUIRE((std::is_trivial<gtl::json>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::json>::value == false));

#if 0
    // Unreliable.
    REQUIRE((std::is_standard_layout<gtl::json>::value == false));
#endif
}

TEST(json, constructor, empty) {
    gtl::json json;
    testbench::do_not_optimise_away(json);
}

constexpr static const char* valid_strings[] = {
    R"(null)",
    R"(true)",
    R"(false)",
    R"(0)",
    R"(-0)",
    R"(-1e+12)",
    R"(1.2e-34)",
    R"("hello world")",
    R"({})",
    R"({"false":false,"null":null,"number":-1.2e+34,"string":"string","true":true})",
    R"([])",
    R"([null,true,false,-1.2e+34,"string"])",
};

TEST(json, function, parse) {
    gtl::json json;
    for (const char* string : valid_strings) {
        REQUIRE(json.parse(string), "Failed to parse json: %s", string);
    }
}

TEST(json, function, compose) {
    gtl::json json;
    for (const char* string : valid_strings) {
        REQUIRE(json.parse(string), "Failed to parse json: %s", string);
        REQUIRE(testbench::is_string_same(json.compose().c_str(), string), "Failed to compose json: parse(%s) != %s", string, json.compose().c_str());
    }
}

TEST(json, function, parse_whitespace) {
    gtl::json json;

    struct test_data {
        const char* string;
        const char* composed;
    };

    constexpr static const test_data whitespace_strings[] = {
        { " {}", "{}" },
        { "{ }", "{}" },
        { "[] ", "[]" },
        { " [ ] ", "[]" },
        { "\r\n\t null \n", "null" },
        { "{ \"a\" : true , \"b\" : [ 1 , 2 ] }\n", "{\"a\":true,\"b\":[1,2]}" },
    };
    for (const test_data& data : whitespace_strings) {
        REQUIRE(json.parse(data.string), "Failed to parse json: %s", data.string);
        REQUIRE(testbench::is_string_same(json.compose().c_str(), data.composed), "Failed to compose json: parse(%s) != %s", data.string, json.compose().c_str());
    }
}

TEST(json, function, parse_rejects_trailing_comma) {
    gtl::json json;
    constexpr static const char* invalid_strings[] = {
        R"([1,])",
        R"([1, ])",
        R"({"a":1,})",
        R"({"a":1, })",
    };
    for (const char* string : invalid_strings) {
        REQUIRE(!json.parse(string), "Unexpectedly parsed json: %s", string);
    }
}

TEST(json, function, escape_compose) {
    gtl::json json;
    json.document() = gtl::json::value(std::string("quote:\" slash:\\ controls:\b\f\n\r\t other:\x01"));
    REQUIRE(testbench::is_string_same(json.compose().c_str(), R"("quote:\" slash:\\ controls:\b\f\n\r\t other:\u0001")"), "Failed to compose escaped json: %s", json.compose().c_str());
}

TEST(json, function, escape_parse) {
    gtl::json json;

    struct test_data {
        const char* string;
        const char* decoded;
    };

    constexpr static const test_data escape_strings[] = {
        { R"("")", "" },
        { R"("\/")", "/" },
        { R"("\"")", "\"" },
        { R"("\\")", "\\" },
        { R"("a\\")", "a\\" },
        { R"("A\u0041B")", "AAB" },
        { R"("\ud83d\ude00")", "\xF0\x9F\x98\x80" },
    };
    for (const test_data& data : escape_strings) {
        REQUIRE(json.parse(data.string), "Failed to parse json: %s", data.string);
        REQUIRE(testbench::is_string_same(json.document().as<gtl::json::value::string_type>().c_str(), data.decoded), "Failed to decode json: parse(%s) != %s", data.string, data.decoded);
    }
}

TEST(json, function, escape_rejects_invalid) {
    gtl::json json;
    constexpr static const char* invalid_strings[] = {
        R"("\q")",
        R"("\u12g4")",
        R"("\u123")",
        R"("\ud800")",
        R"("\ude00")",
        R"("\ud800X")",
        R"("\ud800\ud800")",
        "\"raw\ttab\"",
        R"("unterminated)",
        R"("trailing\)",
    };
    for (const char* string : invalid_strings) {
        REQUIRE(!json.parse(string), "Unexpectedly parsed json: %s", string);
    }
}

TEST(json, function, escape_round_trip) {
    gtl::json json;
    const std::string text = std::string("quote:\" slash:\\ controls:\b\f\n\r\t\x01 unicode:\xC3\xA9 emoji:\xF0\x9F\x98\x80");
    json.document() = gtl::json::value(text);
    const std::string composed = json.compose();
    REQUIRE(json.parse(composed), "Failed to parse composed json: %s", composed.c_str());
    REQUIRE(testbench::is_string_same(json.document().as<gtl::json::value::string_type>().c_str(), text.c_str()), "Failed to round trip json: %s", composed.c_str());

    gtl::json::value::object_type object;
    object[std::string("key\"\\\n")] = gtl::json::value(std::string("\t value \x01"));
    json.document() = gtl::json::value(object);
    const std::string composed_object = json.compose();
    REQUIRE(json.parse(composed_object), "Failed to parse composed json: %s", composed_object.c_str());
    REQUIRE(json.compose() == composed_object, "Failed to round trip json: %s", composed_object.c_str());
}
