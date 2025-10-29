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

#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

#include <parser/generator>

TEST(parsers, constructor, empty) {
    gtl::generator<char> generator;
    testbench::do_not_optimise_away(generator);
}

TEST(parser, element, empty) {
    using parser = gtl::generator<char>;
    parser grammar = parser::empty();
    parser::parse_forest_type forest;
    grammar.parse('T', forest);
    REQUIRE(grammar.finalise(forest, nullptr, nullptr) == false);
}

TEST(parser, element, barrier) {
    constexpr static const size_t grammar_count = 6;

    std::string test_inputs[grammar_count] = {
        R"(0)", // 0
        R"(1)", // 1 or 2
        R"(4)", // 3 or 4
        R"(3)", // !5
        R"(5)", // !6 and !7
        R"(0)", // !8 and !9
    };

    using parser = gtl::generator<char>;

    parser grammars[grammar_count] = {
        parser::barrier_any('0') + parser::terminal_any(),
        parser::barrier_any('1', '2') + parser::terminal_any(),
        parser::barrier_any('3', '4') + parser::terminal_any(),
        parser::barrier_not('5') + parser::terminal_any(),
        parser::barrier_not('6', '7') + parser::terminal_any(),
        parser::barrier_not('8', '9') + parser::terminal_any()
    };

    parser::parse_forest_type forests[grammar_count];

    for (size_t index = 0; index < grammar_count; ++index) {
        const std::string& test_input = test_inputs[index];
        const parser& grammar = grammars[index];
        parser::parse_forest_type& forest = forests[index];
        for (char character : test_input) {
            grammar.parse(character, forest);
        }
        std::vector<char> tokens;
        std::string error;
        REQUIRE(grammar.finalise(forest, &tokens, &error));
        REQUIRE(error.empty());
    }
}

TEST(parser, element, terminal) {
    constexpr static const size_t grammar_count = 7;

    std::string test_inputs[grammar_count] = {
        R"(0)", // 0
        R"(1)", // 1 or 2
        R"(4)", // 3 or 4
        R"(3)", // !5
        R"(5)", // !6 and !7
        R"(0)", // !8 and !9
        R"(10)" // 10
    };

    using parser = gtl::generator<char>;

    parser grammars[grammar_count] = {
        parser::terminal_any('0'),
        parser::terminal_any('1', '2'),
        parser::terminal_any('3', '4'),
        parser::terminal_not('5'),
        parser::terminal_not('6', '7'),
        parser::terminal_not('8', '9'),
        parser::terminal("10"),
    };

    parser::parse_forest_type forests[grammar_count];

    for (size_t index = 0; index < grammar_count; ++index) {
        const std::string& test_input = test_inputs[index];
        const parser& grammar = grammars[index];
        parser::parse_forest_type& forest = forests[index];
        for (char character : test_input) {
            grammar.parse(character, forest);
        }
        std::vector<char> tokens;
        std::string error;
        REQUIRE(grammar.finalise(forest, &tokens, &error));
        REQUIRE(error.empty());
    }
}

TEST(parser, element, sequence) {
    std::string test_input = R"(Ab02)";

    using parser = gtl::generator<char>;

    parser grammar = parser::sequence(
        parser::empty(),
        parser::empty(),
        parser::terminal_any('A'),
        parser::empty(),
        parser::barrier_any('B', 'b'),
        parser::terminal_any(),
        parser::empty(),
        parser::terminal_any('0'),
        parser::terminal_any('1', '2')
    );

    parser::parse_forest_type forest;
    for (char character : test_input) {
        grammar.parse(character, forest);
    }
    std::vector<char> tokens;
    std::string error;
    REQUIRE(grammar.finalise(forest, &tokens, &error));
    REQUIRE(error.empty());
}

TEST(parser, element, disjunction) {
    std::string test_input = R"(Ab02)";

    using parser = gtl::generator<char>;

    parser grammar = parser::disjunction(
        parser::empty(),
        parser::terminal_any('A') + parser::terminal_any('b') + parser::terminal_any('0') + parser::terminal_any('2'),
        parser::terminal_any('b') + parser::terminal_any('A') + parser::terminal_any('0') + parser::terminal_any('2'),
        parser::terminal_any('A') + parser::terminal_any('b') + parser::terminal_any('0') + parser::terminal_any('1'),
        parser::barrier_any('A') + parser::barrier_any('b') + parser::barrier_any('0') + parser::barrier_any('2')
    );

    parser::parse_forest_type forest;
    for (char character : test_input) {
        grammar.parse(character, forest);
    }
    std::vector<char> tokens;
    std::string error;
    REQUIRE(grammar.finalise(forest, &tokens, &error));
    REQUIRE(error.empty());
}

TEST(parser, element, recurse) {
    constexpr static const size_t grammar_count = 3;

    std::string test_input = R"(TTTTTTTT)";

    using parser = gtl::generator<char>;

    parser grammars[grammar_count] = {
        parser::recurse(parser::terminal_any('T') | parser::empty()) + parser::terminal_any('T') + parser::terminal_any('T'),
        parser::terminal_any('T') + parser::recurse(parser::terminal_any('T') | parser::empty()) + parser::terminal_any('T'),
        parser::terminal_any('T') + parser::terminal_any('T') + parser::recurse(parser::terminal_any('T') | parser::empty()),
    };

    parser::parse_forest_type forests[grammar_count];

    for (size_t index = 0; index < grammar_count; ++index) {
        const parser& grammar = grammars[index];
        parser::parse_forest_type& forest = forests[index];
        for (char character : test_input) {
            grammar.parse(character, forest);
        }
        std::vector<char> tokens;
        std::string error;
        REQUIRE(grammar.finalise(forest, &tokens, &error));
        REQUIRE(error.empty());
    }
}

TEST(parser, element, repeat) {
    std::string test_input = R"(TTTTTTTT)";

    using parser = gtl::generator<char>;

    parser grammar = parser::repeat(parser::terminal_any('T'), 8);

    parser::parse_forest_type forest;
    for (char character : test_input) {
        grammar.parse(character, forest);
    }
    std::vector<char> tokens;
    std::string error;
    REQUIRE(grammar.finalise(forest, &tokens, &error));
    REQUIRE(error.empty());
}

TEST(parser, element, emit) {
    constexpr static const size_t grammar_count = 8;

    std::string test_inputs[grammar_count] = {
        R"()",  // No requirement.
        R"(0)", // Requires a 0.
        R"(1)", // Requires a 1 or 2.
        R"(4)", // Requires a 3 or 4.
        R"(3)", // Requires not a 5.
        R"(5)", // Requires not a 6 or a 7.
        R"(0)", // Requires not an 8 or a 9.
        R"(10)" // Requires a 10.
    };

    using parser = gtl::generator<char>;

    parser grammars[grammar_count] = {
        parser::emit('\0'),
        parser::emit(parser::terminal_any('0'), '0'),
        parser::emit(parser::terminal_any('1', '2'), '1'),
        parser::terminal_any('3', '4') >> '2',
        parser::emit(parser::terminal_not('5'), []() {
            return '3';
        }),
        parser::terminal_not('6', '7') >> []() {
            return '4';
        },
        parser::emit(parser::terminal_not('8', '9'), [](const std::vector<char>& terminals) {
            return terminals[0];
        }),
        parser::terminal("10") >> [](const std::vector<char>& terminals) {
            return terminals[0] + terminals[1];
        }
    };

    char expected[grammar_count] = {
        '\0',
        '0',
        '1',
        '2',
        '3',
        '4',
        '0',
        '1' + '0'
    };

    parser::parse_forest_type forests[grammar_count];

    for (size_t index = 0; index < grammar_count; ++index) {
        const std::string& test_input = test_inputs[index];
        const parser& grammar = grammars[index];
        parser::parse_forest_type& forest = forests[index];
        for (char character : test_input) {
            grammar.parse(character, forest);
        }
        std::vector<char> tokens;
        std::string error;
        REQUIRE(grammar.finalise(forest, &tokens, &error));
        REQUIRE(error.empty());
        REQUIRE(tokens.size() == 1);
        if (!tokens.empty()) {
            REQUIRE(tokens[0] == expected[index]);
        }
    }
}

TEST(parser, element, capture) {
    constexpr static const size_t grammar_count = 7;

    std::string test_inputs[grammar_count] = {
        R"(0)", // Requires a 0.
        R"(1)", // Requires a 1 or 2.
        R"(4)", // Requires a 3 or 4.
        R"(3)", // Requires not a 5.
        R"(5)", // Requires not a 6 or a 7.
        R"(0)", // Requires not an 8 or a 9.
        R"(10)" // Requires a 10.
    };

    using parser = gtl::generator<char>;

    parser grammars[grammar_count] = {
        parser::reemit(parser::terminal_any('0') >> '0', [](const std::vector<char>& tokens) {
            return tokens[0];
        }),
        parser::reemit(parser::terminal_any('1', '2') >> '1', [](const std::vector<char>& tokens) {
            return tokens[0];
        }),
        (parser::terminal_any('3', '4') >> '2')->*[](const std::vector<char>& tokens) {
            return tokens[0];
        },
        parser::reemit(parser::terminal_not('5') >> '3', [](const std::vector<char>& tokens) {
            return tokens[0];
        }),
        (parser::terminal_not('6', '7') >> '4')->*[](const std::vector<char>& tokens) {
            return tokens[0];
        },
        parser::reemit(parser::terminal_not('8', '9') >> '0', [](const std::vector<char>& tokens) {
            return tokens[0];
        }),
        (parser::terminal("10") >> ('1' + '0'))->*[](const std::vector<char>& tokens) {
            return tokens[0];
        }
    };

    char expected[grammar_count] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '0',
        '1' + '0'
    };

    parser::parse_forest_type forests[grammar_count];

    for (size_t index = 0; index < grammar_count; ++index) {
        const std::string& test_input = test_inputs[index];
        const parser& grammar = grammars[index];
        parser::parse_forest_type& forest = forests[index];
        for (char character : test_input) {
            grammar.parse(character, forest);
        }
        std::vector<char> tokens;
        std::string error;
        REQUIRE(grammar.finalise(forest, &tokens, &error));
        REQUIRE(error.empty());
        REQUIRE(tokens.size() == 1);
        if (!tokens.empty()) {
            REQUIRE(tokens[0] == expected[index]);
        }
    }
}

TEST(parser, element, reference) {
    std::string test_input = R"(TTTTTTTT)";

    using parser = gtl::generator<char>;

    // Right recursive.
    {
        parser grammar;
        grammar = parser::terminal_any('T') + (parser::empty() | parser::reference(grammar));

        parser::parse_forest_type forest;
        for (char character : test_input) {
            grammar.parse(character, forest);
        }
        std::vector<char> tokens;
        std::string error;
        REQUIRE(grammar.finalise(forest, &tokens, &error));
        REQUIRE(error.empty());
    }

    // TODO: This currently will enter an infinite loop.
    // Left recursive.
    // {
    //     parser grammar;
    //     grammar = (parser::empty() | parser::reference(grammar)) + parser::terminal_any('T');
    //
    //     parser::parse_forest_type forest;
    //     for (char character : test_input) {
    //         grammar.parse(character, forest);
    //     }
    //     REQUIRE(grammar.finalise(forest, nullptr, nullptr));
    //     REQUIRE(forest.branches.size() == 1, "%zu", forest.branches.size());
    // }
}

TEST(parser, element, custom) {
    std::string test_input = R"(abc)";

    using parser = gtl::generator<char>;

    std::vector<char> tracer;

    parser grammar =
        parser(true, false, [&tracer](char input, parser& self, parser::parse_branch_type& branch, parser::parse_forest_type& forest) -> void {
            static_cast<void>(input);
            static_cast<void>(self);
            static_cast<void>(branch);
            static_cast<void>(forest);
            tracer.push_back(input);
        }) +
        parser::terminal_any() + // first test_input char.
        parser::terminal_any() + // second test_input char.
        parser(true, false, [&tracer](char input, parser& self, parser::parse_branch_type& branch, parser::parse_forest_type& forest) -> void {
            static_cast<void>(input);
            static_cast<void>(self);
            static_cast<void>(branch);
            static_cast<void>(forest);
            tracer.push_back(input);
        }) +
        parser::terminal_any(); // third test_input char.

    std::vector<char> expected = {
        'a',
        'c'
    };

    parser::parse_forest_type forest;
    for (char character : test_input) {
        grammar.parse(character, forest);
    }
    std::vector<char> tokens;
    std::string error;
    REQUIRE(grammar.finalise(forest, &tokens, &error));
    REQUIRE(error.empty());
    REQUIRE(tracer == expected);
}

TEST(parser, evaluate, ambiguous) {
    std::string test_input = R"(AB)";

    using parser = gtl::generator<std::string>;

    parser grammar = ((parser::terminal("A") >> "A") + (parser::terminal("B") >> "B")) | (parser::terminal("AB") >> "AB");

    parser::parse_forest_type forest;
    for (char character : test_input) {
        grammar.parse(character, forest);
    }
    std::vector<std::string> tokens;
    std::string error;
    REQUIRE(grammar.finalise(forest, &tokens, &error) == false);
    REQUIRE(error.empty() == false);
}

TEST(parser, evaluate, csv) {
    // [1] csv       ::= header record+
    // [2] header    ::= record
    // [3] record    ::= fields ("\r\n" | "\n")
    // [4] fields    ::= field ("," fields)*
    // [5] field     ::= WS* rawfield WS*
    // [6] rawfield  ::= '"' QCHAR* '"' | SCHAR*
    // [6] QCHAR     ::= [^"] | '""'
    // [7] SCHAR     ::= [^",\r\n]
    // [8] WS        ::= [ \t]

    enum class token_type {
        cell,
        line
    };

    using parser = gtl::generator<std::pair<token_type, std::vector<char>>>;

    // Note: The parser defined here has been slightly altered from the specification above to avoid the ambiguity of whether ws was inside or outside the field.
    parser ws = parser::terminal_any(' ', '\t');
    parser schar = parser::terminal_not('"', ',', '\r', '\n');
    parser qchar = parser::terminal_not('"') | parser::terminal("\"\"");
    parser rowfield = ((parser::terminal_any('"') + parser::recurse(qchar | parser::empty()) + parser::terminal_any('"')) | (parser::barrier_not(' ', '\t') + parser::recurse(schar | parser::empty()) + parser::terminal_not('"', ',', '\r', '\n', ' ', '\t')) | parser::empty()) >> [](const std::vector<char>& terminals) {
        return std::pair<token_type, std::vector<char>>{ token_type::cell, terminals };
    };
    parser field = parser::recurse(ws | parser::empty()) + rowfield + parser::recurse(ws | parser::empty());
    parser fields = (field + parser::recurse((parser::terminal_any(',') + field) | parser::empty())) >> std::pair<token_type, std::vector<char>>{ token_type::line, {} };
    parser record = fields + (parser::terminal("\r\n") | parser::terminal_any('\n'));
    parser header = record;
    parser csv = header + record + parser::recurse(record | parser::empty());

    std::string test_input = R"(header cell
text,000, 123.456   ,"space "" space", "
line
"
)";

    parser::parse_forest_type forest;
    for (char character : test_input) {
        csv.parse(character, forest);
    }
    std::vector<std::pair<token_type, std::vector<char>>> tokens;
    std::string error;
    REQUIRE(csv.finalise(forest, &tokens, &error));
    REQUIRE(error.empty());

    std::vector<std::pair<token_type, std::vector<char>>> expected = {
        { token_type::cell, { 'h', 'e', 'a', 'd', 'e', 'r', ' ', 'c', 'e', 'l', 'l' } },
        { token_type::line, {} },
        { token_type::cell, { 't', 'e', 'x', 't' } },
        { token_type::cell, { '0', '0', '0' } },
        { token_type::cell, { '1', '2', '3', '.', '4', '5', '6' } },
        { token_type::cell, { '"', 's', 'p', 'a', 'c', 'e', ' ', '"', '"', ' ', 's', 'p', 'a', 'c', 'e', '"' } },
        { token_type::cell, { '"', '\n', 'l', 'i', 'n', 'e', '\n', '"' } },
        { token_type::line, {} },
    };
    REQUIRE(tokens == expected);
}
