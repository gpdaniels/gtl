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

#include <game/tic_tac_toe>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#undef REQUIRE
#define REQUIRE(X)

TEST(tic_tac_toe, traits, standard) {
    REQUIRE((std::is_pod<gtl::tic_tac_toe>::value == true));

    REQUIRE((std::is_trivial<gtl::tic_tac_toe>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::tic_tac_toe>::value == true));

    REQUIRE((std::is_standard_layout<gtl::tic_tac_toe>::value == true));
}

TEST(tic_tac_toe, constructor, empty) {
    gtl::tic_tac_toe tic_tac_toe;
    testbench::do_not_optimise_away(tic_tac_toe);
}

TEST(tic_tac_toe, evaluate, always_tie) {
    // Check a normal game is a tie.
    {
        gtl::tic_tac_toe::state_type game = gtl::tic_tac_toe::new_game();
        while (!gtl::tic_tac_toe::check_win(game)) {
            if (!gtl::tic_tac_toe::try_move(game)) {
                break;
            }
        }
        REQUIRE(!gtl::tic_tac_toe::check_win(game));
    }

    // Check that for any starting position it is always a tie.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gtl::tic_tac_toe::state_type game = gtl::tic_tac_toe::new_game();
            game.grid[i][j] = gtl::tic_tac_toe::symbol_type::nought;
            while (!gtl::tic_tac_toe::check_win(game)) {
                if (!gtl::tic_tac_toe::try_move(game)) {
                    break;
                }
            }
            REQUIRE(!gtl::tic_tac_toe::check_win(game));
        }
    }

    // Check that for any starting position it is always a tie.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gtl::tic_tac_toe::state_type game = gtl::tic_tac_toe::new_game();
            game.grid[i][j] = gtl::tic_tac_toe::symbol_type::cross;
            game.next_symbol = gtl::tic_tac_toe::symbol_type::nought;
            while (!gtl::tic_tac_toe::check_win(game)) {
                if (!gtl::tic_tac_toe::try_move(game)) {
                    break;
                }
            }
            REQUIRE(!gtl::tic_tac_toe::check_win(game));
        }
    }
}

TEST(tic_tac_toe, evaluate, speed) {
    const double seconds = testbench::benchmark([](){
        gtl::tic_tac_toe::state_type game = gtl::tic_tac_toe::new_game();
        while (!gtl::tic_tac_toe::check_win(game)) {
            if (!gtl::tic_tac_toe::try_move(game)) {
                break;
            }
        }
        REQUIRE(!gtl::tic_tac_toe::check_win(game));
    }, 10, 1.0);
    PRINT("Game speed: %0.10f seconds\n", seconds);
}

TEST(tic_tac_toe, evaluate, game) {
    constexpr static auto print = [](const gtl::tic_tac_toe::state_type& game) {
        for (unsigned int row = 0; row < gtl::tic_tac_toe::grid_height; ++row) {
            PRINT(" ");
            for (unsigned int column = 0; column < gtl::tic_tac_toe::grid_width; ++column) {
                switch (game.grid[row][column]) {
                    case gtl::tic_tac_toe::symbol_type::empty:  PRINT(" "); break;
                    case gtl::tic_tac_toe::symbol_type::nought: PRINT("O"); break;
                    case gtl::tic_tac_toe::symbol_type::cross:  PRINT("X"); break;
                }
                if (column < gtl::tic_tac_toe::grid_width - 1) {
                    PRINT(" | ");
                }
            }
            PRINT("\n");
            if (row < gtl::tic_tac_toe::grid_height - 1) {
                for (unsigned int column = 0; column < gtl::tic_tac_toe::grid_width - 1; ++column) {
                    PRINT("---+");
                }
                PRINT("---\n");
            }
        }
    };

    // Check a normal game is a tie.
    {
        gtl::tic_tac_toe::state_type game = gtl::tic_tac_toe::new_game();
        print(game);
        while (!gtl::tic_tac_toe::check_win(game)) {
            if (!gtl::tic_tac_toe::try_move(game)) {
                break;
            }
            print(game);
            PRINT("\n");
        }
        print(game);
        PRINT("\n");
        REQUIRE(!gtl::tic_tac_toe::check_win(game));
    }

    // Check that for any starting position it is always a tie.
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gtl::tic_tac_toe::state_type game = gtl::tic_tac_toe::new_game();
            game.grid[i][j] = gtl::tic_tac_toe::symbol_type::nought;
            print(game);
            while (!gtl::tic_tac_toe::check_win(game)) {
                if (!gtl::tic_tac_toe::try_move(game)) {
                    break;
                }
                //print(game);
                //PRINT("\n");
            }
            print(game);
            PRINT("\n");
            REQUIRE(!gtl::tic_tac_toe::check_win(game));
        }
    }
}
