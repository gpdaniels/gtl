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
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <algorithm/maxn>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <functional>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

class state {
public:
    constexpr static const unsigned int entities = 2;

public:
    char next_player = 'X';
    char grid[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};

public:
    unsigned int get_entity_id() const {
        if (this->next_player == 'X') {
            return 0;
        }
        return 1;
    }

    std::vector<state> get_options() const {
        // Check if the game is already over.
        if (this->check_win()) {
            return {};
        }

        std::vector<state> moves;
        moves.reserve(3 * 3);

        // Every empty square is a potential move.
        for (unsigned int row = 0; row < 3; ++row) {
            for (unsigned int column = 0; column < 3; ++column) {
                if (this->grid[row][column] == ' ') {
                    moves.push_back(*this);
                    moves.back().grid[row][column] = moves.back().next_player;
                    moves.back().next_player = moves.back().next_player == 'X' ? 'O' : 'X';
                }
            }
        }

        return moves;
    }

    std::array<int, state::entities> get_scores() const {
        if (this->check_win()) {
            if (this->next_player == 'X') {
                return { -1, 1 };
            }
            return { 1, -1 };
        }
        return { 0, 0 };
    }

public:
    bool check_win() const {
        if ((this->grid[0][0] == 'X') && (this->grid[0][1] == 'X') && (this->grid[0][2] == 'X')) { return true; }
        if ((this->grid[1][0] == 'X') && (this->grid[1][1] == 'X') && (this->grid[1][2] == 'X')) { return true; }
        if ((this->grid[2][0] == 'X') && (this->grid[2][1] == 'X') && (this->grid[2][2] == 'X')) { return true; }
        if ((this->grid[0][0] == 'X') && (this->grid[1][0] == 'X') && (this->grid[2][0] == 'X')) { return true; }
        if ((this->grid[0][1] == 'X') && (this->grid[1][1] == 'X') && (this->grid[2][1] == 'X')) { return true; }
        if ((this->grid[0][2] == 'X') && (this->grid[1][2] == 'X') && (this->grid[2][2] == 'X')) { return true; }
        if ((this->grid[0][0] == 'X') && (this->grid[1][1] == 'X') && (this->grid[2][2] == 'X')) { return true; }
        if ((this->grid[0][2] == 'X') && (this->grid[1][1] == 'X') && (this->grid[2][0] == 'X')) { return true; }
        if ((this->grid[0][0] == 'O') && (this->grid[0][1] == 'O') && (this->grid[0][2] == 'O')) { return true; }
        if ((this->grid[1][0] == 'O') && (this->grid[1][1] == 'O') && (this->grid[1][2] == 'O')) { return true; }
        if ((this->grid[2][0] == 'O') && (this->grid[2][1] == 'O') && (this->grid[2][2] == 'O')) { return true; }
        if ((this->grid[0][0] == 'O') && (this->grid[1][0] == 'O') && (this->grid[2][0] == 'O')) { return true; }
        if ((this->grid[0][1] == 'O') && (this->grid[1][1] == 'O') && (this->grid[2][1] == 'O')) { return true; }
        if ((this->grid[0][2] == 'O') && (this->grid[1][2] == 'O') && (this->grid[2][2] == 'O')) { return true; }
        if ((this->grid[0][0] == 'O') && (this->grid[1][1] == 'O') && (this->grid[2][2] == 'O')) { return true; }
        if ((this->grid[0][2] == 'O') && (this->grid[1][1] == 'O') && (this->grid[2][0] == 'O')) { return true; }
        return false;
    }

    void print() const {
        for (unsigned int row = 0; row < 3; ++row) {
            PRINT(" ");
            for (unsigned int column = 0; column < 3; ++column) {
                switch (this->grid[row][column]) {
                case ' ':  PRINT(" "); break;
                case 'O': PRINT("O"); break;
                case 'X':  PRINT("X"); break;
                }
                if (column < 3 - 1) {
                    PRINT(" | ");
                }
            }
            PRINT("\n");
            if (row < 3 - 1) {
                for (unsigned int column = 0; column < 3 - 1; ++column) {
                    PRINT("---+");
                }
                PRINT("---\n");
            }
        }
        PRINT("\n");
    }
};

TEST(maxn, evaluate, tic_tac_toe) {
    // Known values for the search space of tic tac toe.
    constexpr static const unsigned int games_potential = 19683;
    constexpr static const unsigned int games_valid     = 8533 + 2423;
    constexpr static const unsigned int games_ended     = 1916;
    constexpr static const unsigned int games_won       = 1884;
    constexpr static const unsigned int games_filled    = 156;
    constexpr static const unsigned int games_tied      = 32;

    // Generate all games:
    char option[3] = { ' ', 'X', 'O' };
    std::vector<state> games;
    games.reserve(8533);
    for (unsigned int index = 0; index < games_potential; ++index) {
        for (unsigned int player = 1; player < 3; ++player) {
            state game;
            game.next_player = option[player];
            // Generate game.
            unsigned int code = index;
            for (unsigned int row = 0; row < 3; ++row) {
                for (unsigned int column = 0; column < 3; ++column) {
                    game.grid[row][column] = option[code % 3];
                    code /= 3;
                }
            }
            // Validate game, at most a difference in 1 between number of 'X' and 'O' characters, and at most one winning line.
            int count_X = 0;
            int count_O = 0;
            for (unsigned int row = 0; row < 3; ++row) {
                for (unsigned int column = 0; column < 3; ++column) {
                    count_X += (game.grid[row][column] == 'X');
                    count_O += (game.grid[row][column] == 'O');
                }
            }
            bool valid_difference = (std::abs(count_X - count_O) <= 1);
            if (!valid_difference) {
                continue;
            }

            bool valid_player_difference = false;
            if (game.next_player == 'X') {
                valid_player_difference = (count_O >= count_X);
            }
            else  {
                valid_player_difference = (count_X >= count_O);
            }
            if (!valid_player_difference) {
                continue;
            }

            bool valid_win = true;
            if (game.check_win()) {
                state game_copy = game;
                valid_win = false;
                // Remove the win.
                char winner = [](state& g){
                    if ((g.grid[0][0] == 'X') && (g.grid[0][1] == 'X') && (g.grid[0][2] == 'X')) { g.grid[0][0] = ' '; g.grid[0][1] = ' '; g.grid[0][2] = ' '; return 'X'; }
                    if ((g.grid[1][0] == 'X') && (g.grid[1][1] == 'X') && (g.grid[1][2] == 'X')) { g.grid[1][0] = ' '; g.grid[1][1] = ' '; g.grid[1][2] = ' '; return 'X'; }
                    if ((g.grid[2][0] == 'X') && (g.grid[2][1] == 'X') && (g.grid[2][2] == 'X')) { g.grid[2][0] = ' '; g.grid[2][1] = ' '; g.grid[2][2] = ' '; return 'X'; }
                    if ((g.grid[0][0] == 'X') && (g.grid[1][0] == 'X') && (g.grid[2][0] == 'X')) { g.grid[0][0] = ' '; g.grid[1][0] = ' '; g.grid[2][0] = ' '; return 'X'; }
                    if ((g.grid[0][1] == 'X') && (g.grid[1][1] == 'X') && (g.grid[2][1] == 'X')) { g.grid[0][1] = ' '; g.grid[1][1] = ' '; g.grid[2][1] = ' '; return 'X'; }
                    if ((g.grid[0][2] == 'X') && (g.grid[1][2] == 'X') && (g.grid[2][2] == 'X')) { g.grid[0][2] = ' '; g.grid[1][2] = ' '; g.grid[2][2] = ' '; return 'X'; }
                    if ((g.grid[0][0] == 'X') && (g.grid[1][1] == 'X') && (g.grid[2][2] == 'X')) { g.grid[0][0] = ' '; g.grid[1][1] = ' '; g.grid[2][2] = ' '; return 'X'; }
                    if ((g.grid[0][2] == 'X') && (g.grid[1][1] == 'X') && (g.grid[2][0] == 'X')) { g.grid[0][2] = ' '; g.grid[1][1] = ' '; g.grid[2][0] = ' '; return 'X'; }
                    if ((g.grid[0][0] == 'O') && (g.grid[0][1] == 'O') && (g.grid[0][2] == 'O')) { g.grid[0][0] = ' '; g.grid[0][1] = ' '; g.grid[0][2] = ' '; return 'O'; }
                    if ((g.grid[1][0] == 'O') && (g.grid[1][1] == 'O') && (g.grid[1][2] == 'O')) { g.grid[1][0] = ' '; g.grid[1][1] = ' '; g.grid[1][2] = ' '; return 'O'; }
                    if ((g.grid[2][0] == 'O') && (g.grid[2][1] == 'O') && (g.grid[2][2] == 'O')) { g.grid[2][0] = ' '; g.grid[2][1] = ' '; g.grid[2][2] = ' '; return 'O'; }
                    if ((g.grid[0][0] == 'O') && (g.grid[1][0] == 'O') && (g.grid[2][0] == 'O')) { g.grid[0][0] = ' '; g.grid[1][0] = ' '; g.grid[2][0] = ' '; return 'O'; }
                    if ((g.grid[0][1] == 'O') && (g.grid[1][1] == 'O') && (g.grid[2][1] == 'O')) { g.grid[0][1] = ' '; g.grid[1][1] = ' '; g.grid[2][1] = ' '; return 'O'; }
                    if ((g.grid[0][2] == 'O') && (g.grid[1][2] == 'O') && (g.grid[2][2] == 'O')) { g.grid[0][2] = ' '; g.grid[1][2] = ' '; g.grid[2][2] = ' '; return 'O'; }
                    if ((g.grid[0][0] == 'O') && (g.grid[1][1] == 'O') && (g.grid[2][2] == 'O')) { g.grid[0][0] = ' '; g.grid[1][1] = ' '; g.grid[2][2] = ' '; return 'O'; }
                    if ((g.grid[0][2] == 'O') && (g.grid[1][1] == 'O') && (g.grid[2][0] == 'O')) { g.grid[0][2] = ' '; g.grid[1][1] = ' '; g.grid[2][0] = ' '; return 'O'; }
                    return ' ';
                }(game_copy);

                if (winner != game.next_player) {
                    if (!game_copy.check_win()) {
                        if (winner == 'X') {
                            valid_win = (count_X >= count_O);
                        }
                        else  {
                            valid_win = (count_O >= count_X);
                        }
                    }
                }
            }
            if (!valid_win) {
                continue;
            }

            games.push_back(game);
        }
    }

    // Check generated game count.
    REQUIRE(games.size() == games_valid);

    // Double check the some stats on the generated games.
    unsigned int count_wins = 0;
    unsigned int count_ties = 0;
    unsigned int count_filled = 0;
    for (const state& game : games) {
        unsigned int count_full = 0;
        for (unsigned int row = 0; row < 3; ++row) {
            for (unsigned int column = 0; column < 3; ++column) {
                count_full += (game.grid[row][column] == 'X');
                count_full += (game.grid[row][column] == 'O');
            }
        }
        if (count_full == 9) {
            ++count_filled;
        }
        if (game.check_win()) {
            ++count_wins;
        }
        else if (count_full == 9) {
            ++count_ties;
        }
    }

    // Check generated game stats.
    REQUIRE(count_wins == games_won);
    REQUIRE(count_ties == games_tied);
    REQUIRE(count_filled == games_filled);
    REQUIRE(count_wins + count_ties == games_ended);

    // Extract all non finished games.
    std::vector<state> games_to_evaluate;
    games_to_evaluate.reserve(games_valid - games_ended);

    for (const state& game : games) {
        if (game.get_options().empty()) {
            continue;
        }
        if (game.check_win()) {
            continue;
        }
        games_to_evaluate.push_back(game);
    }

    // Check list of games to evaluate.
    REQUIRE(games_to_evaluate.size() == games_valid - games_ended);

    std::function<int(const state& game, bool isMax, unsigned int player)> evaluate_minimax;
    evaluate_minimax = [&evaluate_minimax](const state& g, bool isMax, unsigned int player)->int{
        if (g.check_win()) {
            if (g.get_entity_id() != player) {
                return 1;
            }
            else {
                return -1;
            }
        }
        std::vector<state> c = g.get_options();
        if (c.empty()) {
            return 0;
        }
        if (isMax) {
            int bestValue = -(1 + 1);
            for (const state& m : c) {
                bestValue = std::max(bestValue, evaluate_minimax(m, false, player));
            }
            return bestValue;
        }
        else {
            int bestValue = (1 + 1);
            for (const state& m : c) {
                bestValue = std::min(bestValue, evaluate_minimax(m, true, player));
            }
            return bestValue;
        }
    };

    // Evaluate games:
    unsigned int evaluate_start_X   = 0;
    unsigned int evaluate_start_O   = 0;
    unsigned int evaluate_wins_X    = 0;
    unsigned int evaluate_wins_O    = 0;
    unsigned int evaluate_ties      = 0;
    unsigned int evaluate_moves_X   = 0;
    unsigned int evaluate_moves_O   = 0;
    unsigned int required_wins_X    = 0;
    unsigned int required_wins_O    = 0;
    unsigned int required_ties      = 0;

    for (const state& game : games_to_evaluate) {

        evaluate_start_X += (game.next_player == 'X');
        evaluate_start_O += (game.next_player == 'O');

        gtl::maxn<state> maxn;
        state current_game = game;
        state move;
        unsigned int moves = 0;
        while (maxn.search(current_game, 9, 1, move)) {
            ++moves;
            current_game = move;
        }

        if (game.next_player == 'X') { evaluate_moves_X += moves; }
        if (game.next_player == 'O') { evaluate_moves_O += moves; }

        if (current_game.check_win()) {
            if (current_game.next_player == 'X') { ++evaluate_wins_O; }
            if (current_game.next_player == 'O') { ++evaluate_wins_X; }
        }
        else {
            ++evaluate_ties;
        }

        // Solve using recursive minimax
        const int score = evaluate_minimax(game, true, game.get_entity_id());
        if ((score < 0) && (game.next_player == 'X')) { ++required_wins_O; }
        if ((score < 0) && (game.next_player == 'O')) { ++required_wins_X; }
        if (score == 0) { ++required_ties; }
        if ((score > 0) && (game.next_player == 'X')) { ++required_wins_X; }
        if ((score > 0) && (game.next_player == 'O')) { ++required_wins_O; }

        static bool print_first_failure = true;
        if (print_first_failure) {
            if ((evaluate_wins_X != required_wins_X) || (evaluate_wins_O != required_wins_O) || (evaluate_ties != required_ties)) {
                print_first_failure = false;
                REQUIRE(false, "Failed to correctly solve this game.");
                game.print();
                if (evaluate_wins_X != required_wins_X) { PRINT("Wins for X do not match. EVAL:%d REQ:%d\n", evaluate_wins_X, required_wins_X); }
                if (evaluate_wins_O != required_wins_O) { PRINT("Wins for O do not match. EVAL:%d REQ:%d\n", evaluate_wins_O, required_wins_O); }
                if (evaluate_ties != required_ties)     { PRINT("Ties do not match. EVAL:%d REQ:%d\n", evaluate_ties, required_ties);       }
                current_game.print();
            }
        }
    }

    PRINT("TEST# X: %u[%u] moves: %u\n", evaluate_start_X, evaluate_wins_X, evaluate_moves_X);
    PRINT("TEST# O: %u[%u] moves: %u\n", evaluate_start_O, evaluate_wins_O, evaluate_moves_O);
    PRINT("TEST# TIES: %u\n", evaluate_ties);

    PRINT("CHECK X: %u[%u]\n", evaluate_start_X, required_wins_X);
    PRINT("CHECK O: %u[%u]\n", evaluate_start_O, required_wins_O);
    PRINT("CHECK TIES: %u\n", required_ties);

    REQUIRE(evaluate_wins_X == required_wins_O);
    REQUIRE(evaluate_wins_O == required_wins_X);
    REQUIRE(evaluate_ties == required_ties);
}
