/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
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
