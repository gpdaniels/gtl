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

#include <vision/consensus/evaluate/least_median_of_squares>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(least_median_of_squares, constructor, threshold) {
    {
        gtl::least_median_of_squares least_median_of_squares(0);
        testbench::do_not_optimise_away(least_median_of_squares);
    }
    {
        gtl::least_median_of_squares least_median_of_squares(1);
        testbench::do_not_optimise_away(least_median_of_squares);
    }
    {
        gtl::least_median_of_squares least_median_of_squares(2);
        testbench::do_not_optimise_away(least_median_of_squares);
    }
    {
        gtl::least_median_of_squares least_median_of_squares(10);
        testbench::do_not_optimise_away(least_median_of_squares);
    }
    {
        gtl::least_median_of_squares least_median_of_squares(1000);
        testbench::do_not_optimise_away(least_median_of_squares);
    }
}

TEST(least_median_of_squares, function, evaluate) {
    gtl::least_median_of_squares least_median_of_squares(1);
    float residuals[5] = { 100.0f, 1.0f, 2.0f, 1.0f, -1.0f };
    std::size_t inliers[5] = { 1111, 2222, 3333, 4444, 5555 };
    std::size_t inliers_size = 0;
    float cost = least_median_of_squares.evaluate(residuals, 5, inliers, inliers_size);
    REQUIRE(testbench::is_value_equal(cost, 2.5f));
    REQUIRE(inliers_size == 4);
    REQUIRE(inliers[0] == 1);
    REQUIRE(inliers[1] == 2);
    REQUIRE(inliers[2] == 3);
    REQUIRE(inliers[3] == 4);
    REQUIRE(inliers[4] == 5555);
}
