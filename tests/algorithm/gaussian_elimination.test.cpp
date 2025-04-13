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
#include <testbench/comparison.tests.hpp>
#include <testbench/require.tests.hpp>

#include <algorithm/gaussian_elimination>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(gaussian_elimination, traits, standard) {
    // Different on windows.
    //REQUIRE((std::is_pod<gtl::gaussian_elimination<float, 1>>::value == true));

    // Different on windows.
    //REQUIRE((std::is_trivial<gtl::gaussian_elimination<float, 1>>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::gaussian_elimination<float, 1>>::value == true));

    REQUIRE((std::is_standard_layout<gtl::gaussian_elimination<float, 1>>::value == true));
}

TEST(gaussian_elimination, function, solve_1) {
    using ge = gtl::gaussian_elimination<float, 1>;
    ge::problem_type problem;
    problem.equations[0].coefficients[0] = 1.0f;
    problem.equations[0].result = 123.0f;
    ge::solution_type solution;
    REQUIRE(ge::solve(problem, solution));
    REQUIRE(testbench::is_value_equal(solution.variables[0], 123.0f));
}

TEST(gaussian_elimination, function, solve_2) {
    using ge = gtl::gaussian_elimination<float, 2>;
    ge::problem_type problem;
    problem.equations[0].coefficients[0] = 1.0f;
    problem.equations[0].coefficients[1] = 2.0f;
    problem.equations[0].result = 8.0f;
    problem.equations[1].coefficients[0] = -2.0f;
    problem.equations[1].coefficients[1] = 1.0f;
    problem.equations[1].result = -1.0f;
    ge::solution_type solution;
    REQUIRE(ge::solve(problem, solution));
    REQUIRE(testbench::is_value_approx(solution.variables[0], 2.0f, 1e-6f));
    REQUIRE(testbench::is_value_approx(solution.variables[1], 3.0f, 1e-6f));
}

TEST(gaussian_elimination, function, solve_3) {
    using ge = gtl::gaussian_elimination<float, 3>;
    ge::problem_type problem;
    problem.equations[0].coefficients[0] = 1.0f;
    problem.equations[0].coefficients[1] = 1.0f;
    problem.equations[0].coefficients[2] = -1.0f;
    problem.equations[0].result = 4.0f;
    problem.equations[1].coefficients[0] = 1.0f;
    problem.equations[1].coefficients[1] = -2.0f;
    problem.equations[1].coefficients[2] = 3.0f;
    problem.equations[1].result = -6.0f;
    problem.equations[2].coefficients[0] = 2.0f;
    problem.equations[2].coefficients[1] = 3.0f;
    problem.equations[2].coefficients[2] = 1.0f;
    problem.equations[2].result = 7.0f;
    ge::solution_type solution;
    REQUIRE(ge::solve(problem, solution));
    REQUIRE(testbench::is_value_approx(solution.variables[0], 1.0f, 1e-6f));
    REQUIRE(testbench::is_value_approx(solution.variables[1], 2.0f, 1e-6f));
    REQUIRE(testbench::is_value_approx(solution.variables[2], -1.0f, 1e-6f));
}

TEST(gaussian_elimination, function, solve_4) {
    using ge = gtl::gaussian_elimination<float, 4>;
    ge::problem_type problem;
    problem.equations[0].coefficients[0] = -0.0f;
    problem.equations[0].coefficients[1] = 1.0f;
    problem.equations[0].coefficients[2] = 1.0f;
    problem.equations[0].coefficients[3] = -0.0f;
    problem.equations[0].result = -1.0f;
    problem.equations[1].coefficients[0] = 2.0f;
    problem.equations[1].coefficients[1] = 2.0f;
    problem.equations[1].coefficients[2] = -2.0f;
    problem.equations[1].coefficients[3] = 3.0f;
    problem.equations[1].result = 10.0f;
    problem.equations[2].coefficients[0] = 1.0f;
    problem.equations[2].coefficients[1] = 2.0f;
    problem.equations[2].coefficients[2] = -3.0f;
    problem.equations[2].coefficients[3] = 4.0f;
    problem.equations[2].result = 12.0f;
    problem.equations[3].coefficients[0] = 1.0f;
    problem.equations[3].coefficients[1] = -1.0f;
    problem.equations[3].coefficients[2] = 1.0f;
    problem.equations[3].coefficients[3] = -2.0f;
    problem.equations[3].result = -4.0f;
    ge::solution_type solution;
    REQUIRE(ge::solve(problem, solution));
    REQUIRE(testbench::is_value_approx(solution.variables[0], 1.0f, 1e-6f));
    REQUIRE(testbench::is_value_approx(solution.variables[1], 0.0f, 1e-6f));
    REQUIRE(testbench::is_value_approx(solution.variables[2], -1.0f, 1e-6f));
    REQUIRE(testbench::is_value_approx(solution.variables[3], 2.0f, 1e-6f));
}

TEST(gaussian_elimination, function, fail_1) {
    using ge = gtl::gaussian_elimination<float, 1>;
    ge::problem_type problem;
    problem.equations[0].coefficients[0] = 0.0f;
    problem.equations[0].result = 123.0f;
    ge::solution_type solution;
    REQUIRE(ge::solve(problem, solution) == false);
}

TEST(gaussian_elimination, function, fail_2) {
    using ge = gtl::gaussian_elimination<float, 2>;
    ge::problem_type problem;
    problem.equations[0].coefficients[0] = 0.0f;
    problem.equations[0].coefficients[1] = 2.0f;
    problem.equations[0].result = 8.0f;
    problem.equations[1].coefficients[0] = 0.0f;
    problem.equations[1].coefficients[1] = 1.0f;
    problem.equations[1].result = 4.0f;
    ge::solution_type solution;
    REQUIRE(ge::solve(problem, solution) == false);
}

TEST(gaussian_elimination, function, should_solve_2) {
    {
        using ge = gtl::gaussian_elimination<float, 2>;
        ge::problem_type problem;
        problem.equations[0].coefficients[0] = 1.0f;
        problem.equations[0].coefficients[1] = 0.0f;
        problem.equations[0].result = 1.0f;
        problem.equations[1].coefficients[0] = 1.0f;
        problem.equations[1].coefficients[1] = 1.0f;
        problem.equations[1].result = 2.0f;
        ge::solution_type solution;
        REQUIRE(ge::solve(problem, solution) == true);
        REQUIRE(testbench::is_value_approx(solution.variables[0], 1.0f, 1e-6f));
        REQUIRE(testbench::is_value_approx(solution.variables[1], 1.0f, 1e-6f));
    }
    {
        using ge = gtl::gaussian_elimination<float, 2>;
        ge::problem_type problem;
        problem.equations[0].coefficients[0] = 1.0f;
        problem.equations[0].coefficients[1] = 1.0f;
        problem.equations[0].result = 2.0f;
        problem.equations[1].coefficients[0] = 1.0f;
        problem.equations[1].coefficients[1] = 0.0f;
        problem.equations[1].result = 1.0f;
        ge::solution_type solution;
        REQUIRE(ge::solve(problem, solution) == false);
        // NOTE: This fails because of the pivoting method.
    }
}
