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
