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

#include <vision/feature/refinement/exhaustive>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#include <vision/image_processing/sub_pixel_patch>
#include <vision/match_distance/ncc>
#include <vision/match_distance/ssd>
#include <vision/match_distance/zncc>

TEST(exhaustive, function, search_2d) {
    constexpr static const unsigned int data_width = 8;
    constexpr static const unsigned int data_height = 8;
    constexpr static const unsigned char data_lhs[data_height][data_width] = {
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0, 254, 127,   0,   0,   0,   0,   0 },
        {   0, 127,  64,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 }
    };
    constexpr static const unsigned int start_lhs_x = 1;
    constexpr static const unsigned int start_lhs_y = 1;

    constexpr static const unsigned char data_rhs[data_height][data_width] = {
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0, 254, 254,   0,   0,   0,   0 },
        {   0,   0, 254, 254,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 },
        {   0,   0,   0,   0,   0,   0,   0,   0 }
    };

    constexpr static const unsigned int start_rhs_x = 2;
    constexpr static const unsigned int start_rhs_y = 2;

    constexpr static const unsigned int patch_width = 3;
    constexpr static const unsigned int patch_height = 3;

    // Note: In the worse case the search in the range (-1 -> +1) can offset the start pixel by a max of -1 or +2 rows and columns.
    // Note: Therefore for a start index of 2, and a size of 3, we need to be able to accomodate indices (2 + 0 - 1) = 1 and (2 + 3 + 2 = 7).

    float offset_rhs_x;
    float offset_rhs_y;

    const float best_score_nnc = gtl::exhaustive_2d<patch_width, patch_height, &gtl::sub_pixel_patch<patch_width, patch_height, unsigned char>, &gtl::ncc<patch_width, patch_height, unsigned char, float>>(
        &data_lhs[start_lhs_y][start_lhs_x],
        data_width,
        &data_rhs[start_rhs_y][start_rhs_x],
        data_width,
        offset_rhs_x,
        offset_rhs_y
    );

    // The rhs should shift by +0.5 in both x and y.
    // As patch at rhs[2.5][2.5] is equal to the lhs[1][1]
    REQUIRE(testbench::is_value_approx(offset_rhs_x, 0.5f, 1E-6f));
    REQUIRE(testbench::is_value_approx(offset_rhs_y, 0.5f, 1E-6f));
    REQUIRE(testbench::is_value_approx(best_score_nnc, -1.0f, 1E-5f));

    const float best_score_ssd = gtl::exhaustive_2d<patch_width, patch_height, &gtl::sub_pixel_patch<patch_width, patch_height, unsigned char>, &gtl::ssd<patch_width, patch_height, unsigned char, float>>(
        &data_lhs[start_lhs_y][start_lhs_x],
        data_width,
        &data_rhs[start_rhs_y][start_rhs_x],
        data_width,
        offset_rhs_x,
        offset_rhs_y
    );

    // The rhs should shift by +0.5 in both x and y.
    // As patch at rhs[2.5][2.5] is equal to the lhs[1][1]
    REQUIRE(testbench::is_value_approx(offset_rhs_x, 0.5f, 1E-6f));
    REQUIRE(testbench::is_value_approx(offset_rhs_y, 0.5f, 1E-6f));
    REQUIRE(testbench::is_value_approx(best_score_ssd, 0.25f, 1E-6f));

    const float best_score_zncc = gtl::exhaustive_2d<patch_width, patch_height, &gtl::sub_pixel_patch<patch_width, patch_height, unsigned char>, &gtl::zncc<patch_width, patch_height, unsigned char, float>>(
        &data_lhs[start_lhs_y][start_lhs_x],
        data_width,
        &data_rhs[start_rhs_y][start_rhs_x],
        data_width,
        offset_rhs_x,
        offset_rhs_y
    );

    // The rhs should shift by +0.5 in both x and y.
    // As patch at rhs[2.5][2.5] is equal to the lhs[1][1]
    REQUIRE(testbench::is_value_approx(offset_rhs_x, 0.5f, 1E-6f));
    REQUIRE(testbench::is_value_approx(offset_rhs_y, 0.5f, 1E-6f));
    REQUIRE(testbench::is_value_approx(best_score_zncc, -1.0f, 1E-5f));
}
