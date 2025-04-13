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
#include <testbench/print.tests.hpp>
#include <testbench/require.tests.hpp>

#include <game/mastermind>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <random>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(mastermind, constructor, empty) {
    constexpr static const unsigned int code_length = 4;
    constexpr static const unsigned int code_base = 6;
    gtl::mastermind<code_length, code_base> mastermind;
    testbench::do_not_optimise_away(mastermind);
}

TEST(mastermind, function, solve) {
    constexpr static const unsigned int code_length = 4;
    constexpr static const unsigned int code_base = 6;
    {
        unsigned int turns = gtl::mastermind<code_length, code_base>::solve({1,2,3,4});
        REQUIRE(turns <= 5);
    }
    {
        unsigned int turns = gtl::mastermind<code_length, code_base>::solve({5,4,3,2}, [](unsigned int turn, const std::array<unsigned int, 4>& guess, unsigned int correct, unsigned int close){
            PRINT("GUESS %d: %d %d %d %d ==> %d %d\n", turn, guess[0], guess[1], guess[2], guess[3], correct, close);
        });
        REQUIRE(turns <= 5);
    }
}

TEST(mastermind, evaluate, all) {
    constexpr static const unsigned int code_length = 4;
    constexpr static const unsigned int code_base = 6;
    // Generate all possible codes.
    std::vector<std::array<unsigned int, code_length>> all_codes;
    all_codes.push_back(std::array<unsigned int, code_length>());
    do {
        all_codes.push_back(all_codes.back());
        for (unsigned int i = 0; i < code_length; ++i) {
            all_codes.back()[code_length - 1 - i] = (all_codes.back()[code_length - 1 - i] + 1) % code_base;
            if (all_codes.back()[code_length - 1 - i] != 0) {
                break;
            }
        }
    } while (all_codes.back() != all_codes.front());
    all_codes.pop_back();

#if !defined(NDEBUG)
    // Sample the full set as running the full set is slow.
    std::vector<std::array<unsigned int, code_length>> test_codes;
    std::sample(all_codes.begin(), all_codes.end(), std::back_inserter(test_codes), 10, std::mt19937{std::random_device{}()});
#else
    std::vector<std::array<unsigned int, code_length>>& test_codes = all_codes;
#endif

    // Solve codes.
    for (unsigned int i = 0; i < test_codes.size(); ++i) {
        unsigned int turns = gtl::mastermind<code_length, code_base>::solve(test_codes[i]);
        REQUIRE(turns <= 5, "Code %d %d %d %d took %d turns.", test_codes[i][0], test_codes[i][1], test_codes[i][2], test_codes[i][3], turns);
    }
}


#if 0
TEST(mastermind, interactive, game) {
    enum colours : unsigned int {
        yellow  = 0,
        green   = 1,
        blue    = 2,
        white   = 3,
        orange  = 4,
        purple  = 5,
        pink    = 6,
        red     = 7,
    };
    constexpr static const auto to_string = [](unsigned int colour){
        switch (colour) {
            case yellow:    return "yellow";
            case green:     return " green";
            case blue:      return "  blue";
            case white:     return " white";
            case orange:    return "orange";
            case purple:    return "purple";
            case pink:      return "  pink";
            case red:       return "   red";
        }
            return "unknown";
    };
    unsigned int turns = gtl::mastermind<4, 8>::solve([](const std::array<unsigned int, 4>& guess){
        std::pair<unsigned int, unsigned int> score;
        PRINT("GUESS: %s %s %s %s\n", to_string(guess[0]), to_string(guess[1]), to_string(guess[2]), to_string(guess[3]));
        PRINT("Enter [correct colour in correct place] and [correct colour in incorrect place]:\n");
        std::scanf("%d %d", &score.first, &score.second);
        return score;
    }, [](unsigned int turn, const std::array<unsigned int, 4>& guess, unsigned int correct, unsigned int close){
        PRINT("SOLUTION %d: %s %s %s %s ==> %d %d\n", turn, to_string(guess[0]), to_string(guess[1]), to_string(guess[2]), to_string(guess[3]), correct, close);
    });
    PRINT("Turns: %u\n", turns);
}
#endif
