/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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

#include <main.tests.hpp>
#include <benchmark.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <parser/pl0>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(pl0, traits, standard) {
    REQUIRE((std::is_pod<gtl::pl0>::value == true));

    REQUIRE((std::is_trivial<gtl::pl0>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::pl0>::value == true));

    REQUIRE((std::is_standard_layout<gtl::pl0>::value == true));
}

TEST(pl0, constructor, empty) {
    gtl::pl0 pl0;
    testbench::do_not_optimise_away(pl0);
}

TEST(pl0, function, parse) {
    gtl::pl0 pl0;
    std::vector<gtl::pl0::token_data> tokens = pl0.parse("");
}

TEST(pl0, function, interpret) {
    gtl::pl0 pl0;
    pl0.interpret({});
}

TEST(pl0, evaluate, square) {
    std::string code = R"(
CONST limit = 10;
VAR x, squ;

PROCEDURE square;
BEGIN
    squ:= x * x
END;

BEGIN
    x := 1;
    WHILE x <= limit DO
    BEGIN
        CALL square;
        ! squ;
        x := x + 1
    END
END
.
)";

    gtl::pl0 pl0;
    std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
    REQUIRE(!tokens.empty());
    pl0.interpret(tokens);
}

TEST(pl0, evaluate, prime) {
    std::string code = R"(
CONST max = 100;
VAR arg, ret;

PROCEDURE isprime;
VAR i;
BEGIN
    ret := 1;
    i := 2;
    WHILE i < arg DO
    BEGIN
        IF arg / i * i = arg THEN
        BEGIN
            ret := 0;
            i := arg
        END;
        i := i + 1
    END
END;

PROCEDURE primes;
BEGIN
    arg := 2;
    WHILE arg < max DO
    BEGIN
        CALL isprime;
        IF ret = 1 THEN ! arg;
        arg := arg + 1
    END
END;

CALL primes
.
)";

    gtl::pl0 pl0;
    std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
    REQUIRE(!tokens.empty());
    pl0.interpret(tokens);
}

TEST(pl0, benchmark, parse_empty) {
    PRINT("Empty code: %f\n", testbench::benchmark([](){
        std::string code = R"()";
        gtl::pl0 pl0;
        std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
        REQUIRE(tokens.empty());
    }));
}

TEST(pl0, benchmark, parse_simple) {
    PRINT("Parse simple code: %f\n", testbench::benchmark([](){
        std::string code = R"(VAR number; number := 42 .)";
        gtl::pl0 pl0;
        std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
        REQUIRE(!tokens.empty());
    }));
}

TEST(pl0, benchmark, interpret_simple) {
    std::string code = R"(VAR number; number := 42 .)";
    gtl::pl0 pl0;
    std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
    REQUIRE(!tokens.empty());
    PRINT("Interpret simple code: %f\n", testbench::benchmark([&](){
        pl0.interpret(tokens);
    }));
}

TEST(pl0, benchmark, parse_complex) {
    PRINT("Parse complex code: %f\n", testbench::benchmark([](){
        std::string code = R"(
CONST max = 100;
VAR arg, ret;

PROCEDURE isprime;
VAR i;
BEGIN
    ret := 1;
    i := 2;
    WHILE i < arg DO
    BEGIN
        IF arg / i * i = arg THEN
        BEGIN
            ret := 0;
            i := arg
        END;
        i := i + 1
    END
END;

PROCEDURE primes;
BEGIN
    arg := 2;
    WHILE arg < max DO
    BEGIN
        CALL isprime;
        arg := arg + 1
    END
END;

CALL primes
.
    )";
        gtl::pl0 pl0;
        std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
        REQUIRE(!tokens.empty());
    }));
}

TEST(pl0, benchmark, interpret_complex) {
    std::string code = R"(
CONST max = 100;
VAR arg, ret;

PROCEDURE isprime;
VAR i;
BEGIN
ret := 1;
i := 2;
WHILE i < arg DO
BEGIN
    IF arg / i * i = arg THEN
    BEGIN
        ret := 0;
        i := arg
    END;
    i := i + 1
END
END;

PROCEDURE primes;
BEGIN
arg := 2;
WHILE arg < max DO
BEGIN
    CALL isprime;
    arg := arg + 1
END
END;

CALL primes
.
)";
    gtl::pl0 pl0;
    std::vector<gtl::pl0::token_data> tokens = pl0.parse(code);
    PRINT("Interpret complex code: %f\n", testbench::benchmark([&](){
        REQUIRE(!tokens.empty());
        pl0.interpret(tokens);
    }));
}
