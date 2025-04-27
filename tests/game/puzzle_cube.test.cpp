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

#include <game/puzzle_cube>

void print_cube(const gtl::puzzle_cube& cube);

void print_cube(const gtl::puzzle_cube& cube) {
    PRINT("Cube:\n");

    struct {
        int r, g, b;
    } colour_type[6] = {
        { 0, 255, 0 },     // gtl::puzzle_cube::cell_colour::green  == 0;
        { 255, 0, 0 },     // gtl::puzzle_cube::cell_colour::red    == 1;
        { 0, 0, 255 },     // gtl::puzzle_cube::cell_colour::blue   == 2;
        { 255, 128, 0 },   // gtl::puzzle_cube::cell_colour::orange == 3;
        { 255, 255, 255 }, // gtl::puzzle_cube::cell_colour::white  == 4;
        { 255, 255, 0 }    // gtl::puzzle_cube::cell_colour::yellow == 5;
    };

    for (int row = 0; row < 9; ++row) {
        for (int column = 0; column < 12; ++column) {
            if (((row < 3) || (row >= 6)) && ((column < 3) || (column >= 6))) {
                PRINT("   ");
                continue;
            }
            gtl::puzzle_cube::face_type face =
                (row < 3) ? gtl::puzzle_cube::face_type::up : (row < 6) && (column < 3) ? gtl::puzzle_cube::face_type::left
                                                          : (row < 6) && (column < 6)   ? gtl::puzzle_cube::face_type::front
                                                          : (row < 6) && (column < 9)   ? gtl::puzzle_cube::face_type::right
                                                          : (row < 6)                   ? gtl::puzzle_cube::face_type::back
                                                                                        : gtl::puzzle_cube::face_type::down;
            const int x = column % 3;
            const int y = row % 3;
            gtl::puzzle_cube::colour_type colour = cube.get_cubie(face, x, y);
            PRINT("\033[1;38;2;0;0;0m\033[4;48;2;%d;%d;%dm", colour_type[+colour].r, colour_type[+colour].g, colour_type[+colour].b);
            PRINT("__");
            PRINT("\033[0m");
            PRINT(" ");
        }
        PRINT("\n");
    }
}

void scramble_cube(gtl::puzzle_cube& cube, int moves);

void scramble_cube(gtl::puzzle_cube& cube, int moves) {
    class random_pcg final {
    private:
        unsigned long long int state = 0x853C49E6748FEA9Bull;
        unsigned long long int increment = 0xDA3E39CB94B95BDBull;

    public:
        unsigned int get_random_raw() {
            // Save current state for output calculation.
            unsigned long long int state_previous = this->state;
            // Advance internal state.
            this->state = state_previous * 0x5851F42D4C957F2Dull + this->increment;
            // Calculate output function.
            unsigned int state_shift_xor_shift = static_cast<unsigned int>(((state_previous >> 18u) ^ state_previous) >> 27u);
            int rotation = state_previous >> 59u;
            return (state_shift_xor_shift >> rotation) | (state_shift_xor_shift << ((-rotation) & 31));
        }
    };

    random_pcg rng;
    for (int i = 0; i < moves; ++i) {
        switch (rng.get_random_raw() % 12) {
            case 0:
                cube.apply_moves(gtl::puzzle_cube::move_type::up, 1);
                break;
            case 1:
                cube.apply_moves(gtl::puzzle_cube::move_type::down, 1);
                break;
            case 2:
                cube.apply_moves(gtl::puzzle_cube::move_type::front, 1);
                break;
            case 3:
                cube.apply_moves(gtl::puzzle_cube::move_type::back, 1);
                break;
            case 4:
                cube.apply_moves(gtl::puzzle_cube::move_type::left, 1);
                break;
            case 5:
                cube.apply_moves(gtl::puzzle_cube::move_type::right, 1);
                break;
            case 6:
                cube.apply_moves(gtl::puzzle_cube::move_type::equatial, 1);
                break;
            case 7:
                cube.apply_moves(gtl::puzzle_cube::move_type::middle, 1);
                break;
            case 8:
                cube.apply_moves(gtl::puzzle_cube::move_type::standing, 1);
                break;
            case 9:
                cube.apply_moves(gtl::puzzle_cube::move_type::x, 1);
                break;
            case 10:
                cube.apply_moves(gtl::puzzle_cube::move_type::y, 1);
                break;
            case 11:
                cube.apply_moves(gtl::puzzle_cube::move_type::z, 1);
                break;
        }
    }
}

TEST(puzzle_cube, constructor, empty) {
    gtl::puzzle_cube puzzle_cube;
    testbench::do_not_optimise_away(puzzle_cube);
}

TEST(puzzle_cube, function, is_valid) {
    {
        gtl::puzzle_cube puzzle_cube;
        REQUIRE(puzzle_cube.is_valid());
    }
    {
        gtl::puzzle_cube puzzle_cube;
        puzzle_cube.set_cubie(gtl::puzzle_cube::face_type::up, 1, 1, gtl::puzzle_cube::colour_type::blue);
        REQUIRE(!puzzle_cube.is_valid());
    }
    {
        gtl::puzzle_cube puzzle_cube;
        puzzle_cube.set_cubie(gtl::puzzle_cube::face_type::up, 1, 2, gtl::puzzle_cube::colour_type::green);
        puzzle_cube.set_cubie(gtl::puzzle_cube::face_type::front, 1, 2, gtl::puzzle_cube::colour_type::white);
        REQUIRE(!puzzle_cube.is_valid());
    }
    {
        gtl::puzzle_cube puzzle_cube;
        puzzle_cube.set_cubie(gtl::puzzle_cube::face_type::up, 2, 2, gtl::puzzle_cube::colour_type::green);
        puzzle_cube.set_cubie(gtl::puzzle_cube::face_type::front, 2, 0, gtl::puzzle_cube::colour_type::white);
        REQUIRE(!puzzle_cube.is_valid());
    }
}

TEST(puzzle_cube, function, is_solved) {
    gtl::puzzle_cube puzzle_cube;
    REQUIRE(puzzle_cube.is_solved());
    puzzle_cube.apply_moves(gtl::puzzle_cube::move_type::x, 1);
    puzzle_cube.apply_moves(gtl::puzzle_cube::move_type::y, 2);
    puzzle_cube.apply_moves(gtl::puzzle_cube::move_type::z, 3);
    REQUIRE(puzzle_cube.is_solved());
}

TEST(puzzle_cube, function, solve_first_layer_cross) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
    }
}

TEST(puzzle_cube, function, solve_first_layer_corners) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
        REQUIRE(puzzle_cube.solve_first_layer_corners());
    }
}

TEST(puzzle_cube, function, solve_middle_edges) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
        REQUIRE(puzzle_cube.solve_first_layer_corners());
        REQUIRE(puzzle_cube.solve_middle_edges());
    }
}

TEST(puzzle_cube, function, solve_last_layer_cross) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
        REQUIRE(puzzle_cube.solve_first_layer_corners());
        REQUIRE(puzzle_cube.solve_middle_edges());
        REQUIRE(puzzle_cube.solve_last_layer_cross());
    }
}

TEST(puzzle_cube, function, solve_last_layer_edges) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
        REQUIRE(puzzle_cube.solve_first_layer_corners());
        REQUIRE(puzzle_cube.solve_middle_edges());
        REQUIRE(puzzle_cube.solve_last_layer_cross());
        REQUIRE(puzzle_cube.solve_last_layer_edges());
    }
}

TEST(puzzle_cube, function, solve_last_layer_corner_positions) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
        REQUIRE(puzzle_cube.solve_first_layer_corners());
        REQUIRE(puzzle_cube.solve_middle_edges());
        REQUIRE(puzzle_cube.solve_last_layer_cross());
        REQUIRE(puzzle_cube.solve_last_layer_edges());
        REQUIRE(puzzle_cube.solve_last_layer_corner_positions());
    }
}

TEST(puzzle_cube, function, solve_last_layer_corner_orientations) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i);
        REQUIRE(puzzle_cube.solve_first_layer_cross());
        REQUIRE(puzzle_cube.solve_first_layer_corners());
        REQUIRE(puzzle_cube.solve_middle_edges());
        REQUIRE(puzzle_cube.solve_last_layer_cross());
        REQUIRE(puzzle_cube.solve_last_layer_edges());
        REQUIRE(puzzle_cube.solve_last_layer_corner_positions());
        REQUIRE(puzzle_cube.solve_last_layer_corner_orientations());
    }
}

TEST(puzzle_cube, evaluate, solve) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube;
        scramble_cube(puzzle_cube, i + 2000);
        REQUIRE(puzzle_cube.solve());
    }
}

TEST(puzzle_cube, evaluate, solve_with_solution) {
    for (int i = 0; i < 2000; ++i) {
        gtl::puzzle_cube puzzle_cube_1;
        scramble_cube(puzzle_cube_1, i + 4000);
        gtl::puzzle_cube puzzle_cube_2 = puzzle_cube_1;
        gtl::puzzle_cube puzzle_cube_3 = puzzle_cube_1;
        std::string solution_1;
        REQUIRE(puzzle_cube_1.solve(&solution_1));
        REQUIRE(puzzle_cube_2.apply_moves(solution_1.c_str()));
        REQUIRE(puzzle_cube_2.is_solved());
        std::string solution_2 = solution_1;
        gtl::puzzle_cube::optimise(solution_2);
        REQUIRE(solution_2.size() <= solution_1.size());
        REQUIRE(puzzle_cube_3.apply_moves(solution_2.c_str()));
        REQUIRE(puzzle_cube_3.is_solved());
    }
}

TEST(puzzle_cube, evaluate, print_solving) {
    gtl::puzzle_cube puzzle_cube;
    print_cube(puzzle_cube);
    scramble_cube(puzzle_cube, 12);
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_first_layer_cross());
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_first_layer_corners());
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_middle_edges());
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_last_layer_cross());
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_last_layer_edges());
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_last_layer_corner_positions());
    print_cube(puzzle_cube);
    REQUIRE(puzzle_cube.solve_last_layer_corner_orientations());
    print_cube(puzzle_cube);
}
