/*
The MIT License
Copyright (c) 2021 Geoffrey Daniels. http://gpdaniels.com/
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
        xy operator/(const float other) const {
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

    std::vector<int> clusters;

    clusters = gtl::kmeans<xy, float>::compute(data, 2, 100, 0.001, [](const xy& lhs, const xy& rhs){
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
