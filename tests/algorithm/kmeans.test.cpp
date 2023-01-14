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
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <algorithm/kmeans>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <cmath>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(kmeans, evaluate, data) {

    struct xy {
        float x, y;

        xy operator+(const xy& other) const {
            return { x + other.x, y + other.y };
        }
        xy operator/(const int other) const {
            return { x / other, y / other };
        }
    };

    std::vector<xy> data = {
        {-10.0f + 0.5f, -10.0f + 0.5f},
        {-10.0f + 0.5f, -10.0f + 0.0f},
        {-10.0f + 0.5f, -10.0f - 0.5f},
        {-10.0f + 0.0f, -10.0f + 0.5f},
        {-10.0f + 0.0f, -10.0f + 0.0f},
        {-10.0f + 0.0f, -10.0f - 0.5f},
        {-10.0f - 0.5f, -10.0f + 0.5f},
        {-10.0f - 0.5f, -10.0f + 0.0f},
        {-10.0f - 0.5f, -10.0f - 0.5f},

        {+10.0f + 0.5f, +10.0f + 0.5f},
        {+10.0f + 0.5f, +10.0f + 0.0f},
        {+10.0f + 0.5f, +10.0f - 0.5f},
        {+10.0f + 0.0f, +10.0f + 0.5f},
        {+10.0f + 0.0f, +10.0f + 0.0f},
        {+10.0f + 0.0f, +10.0f - 0.5f},
        {+10.0f - 0.5f, +10.0f + 0.5f},
        {+10.0f - 0.5f, +10.0f + 0.0f},
        {+10.0f - 0.5f, +10.0f - 0.5f}
    };

    std::vector<std::size_t> clusters;

    clusters = gtl::kmeans<xy, float>::compute(data, 2, 100, 0.001f, [](const xy& lhs, const xy& rhs){
        return std::sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + ((lhs.y - rhs.y) * (lhs.y - rhs.y)));
    });

    REQUIRE(clusters[0] == clusters[1]);
    REQUIRE(clusters[1] == clusters[2]);
    REQUIRE(clusters[2] == clusters[3]);
    REQUIRE(clusters[3] == clusters[4]);
    REQUIRE(clusters[4] == clusters[5]);
    REQUIRE(clusters[5] == clusters[6]);
    REQUIRE(clusters[6] == clusters[7]);
    REQUIRE(clusters[7] == clusters[8]);
    REQUIRE(clusters[8] == clusters[0]);

    REQUIRE(clusters[9] == clusters[10]);
    REQUIRE(clusters[10] == clusters[11]);
    REQUIRE(clusters[11] == clusters[12]);
    REQUIRE(clusters[12] == clusters[13]);
    REQUIRE(clusters[13] == clusters[14]);
    REQUIRE(clusters[14] == clusters[15]);
    REQUIRE(clusters[15] == clusters[16]);
    REQUIRE(clusters[16] == clusters[17]);
    REQUIRE(clusters[17] == clusters[10]);
}
