/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <vision/consensus/evaluate/inlier_support>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(inlier_support, constructor, threshold) {
    {
        gtl::inlier_support inlier_support(0);
        testbench::do_not_optimise_away(inlier_support);
    }
    {
        gtl::inlier_support inlier_support(0.0f);
        testbench::do_not_optimise_away(inlier_support);
    }
    {
        gtl::inlier_support inlier_support(-1.0f);
        testbench::do_not_optimise_away(inlier_support);
    }
    {
        gtl::inlier_support inlier_support(1.0f);
        testbench::do_not_optimise_away(inlier_support);
    }
    {
        gtl::inlier_support inlier_support(10000.0f);
        testbench::do_not_optimise_away(inlier_support);
    }
}

TEST(inlier_support, function, evaluate) {
    gtl::inlier_support inlier_support(1.5f);
    float residuals[5] = { 0.0f, 1.0f, 2.0f, 1.0f, -1.0f };
    size_t inliers[5] = { 1111, 2222, 3333, 4444, 5555 };
    size_t inliers_size = 0;
    float cost = inlier_support.evaluate(residuals, 5, inliers, inliers_size);
    REQUIRE(testbench::is_value_equal(cost, 1.0f));
    REQUIRE(inliers_size == 4);
    REQUIRE(inliers[0] == 0);
    REQUIRE(inliers[1] == 1);
    REQUIRE(inliers[2] == 3);
    REQUIRE(inliers[3] == 4);
    REQUIRE(inliers[4] == 5555);
}
