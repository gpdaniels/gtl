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
#include <comparison.tests.hpp>
#include <require.tests.hpp>

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
    problem.equations[0].coefficients[0] = 0.0f;
    problem.equations[0].coefficients[1] = 1.0f;
    problem.equations[0].coefficients[2] = 1.0f;
    problem.equations[0].coefficients[3] = 0.0f;
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
