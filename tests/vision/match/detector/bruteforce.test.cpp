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

#include <vision/match/detector/bruteforce>

namespace {
    // The score is the distance between the descriptors, lower is better.
    float score_distance(const int& lhs, const int& rhs) {
        return static_cast<float>((lhs > rhs) ? (lhs - rhs) : (rhs - lhs));
    }

    // A score that is always negative, lower is better.
    float score_negative(const int& lhs, const int& rhs) {
        return score_distance(lhs, rhs) - 1000.0f;
    }

    // A score that is always very large, lower is better.
    float score_large(const int& lhs, const int& rhs) {
        return score_distance(lhs, rhs) * 1e36f;
    }

    constexpr const size_t invalid_index = 0xFFFF;

    void reset(gtl::match* matches, const size_t matches_size) {
        for (size_t index = 0; index < matches_size; ++index) {
            matches[index].lhs_index = invalid_index;
            matches[index].rhs_index = invalid_index;
            matches[index].score = -12345.0f;
        }
    }

    bool is_match(const gtl::match& match, const size_t lhs_index, const size_t rhs_index, const float score) {
        return (match.lhs_index == lhs_index) && (match.rhs_index == rhs_index) && (match.score >= score) && (match.score <= score);
    }

    bool is_untouched(const gtl::match& match) {
        return is_match(match, invalid_index, invalid_index, -12345.0f);
    }
}

TEST(bruteforce, constructor, empty) {
    gtl::bruteforce bruteforce;
    testbench::do_not_optimise_away(bruteforce);
}

TEST(bruteforce, function, find_matches_best) {
    const int lhs[4] = { 10, 20, 30, 41 };
    const int rhs[5] = { 43, 31, 9, 100, 22 };

    gtl::match matches[5];
    reset(matches, 5);

    const size_t count = gtl::bruteforce::find_matches(lhs, 4, rhs, 5, score_distance, matches, 4);
    REQUIRE(count == 4, "Expected 4 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 1, 4, 2.0f));
    REQUIRE(is_match(matches[2], 2, 1, 1.0f));
    REQUIRE(is_match(matches[3], 3, 0, 2.0f));
    REQUIRE(is_untouched(matches[4]));
}

TEST(bruteforce, function, find_matches_best_score_ranges) {
    const int lhs[2] = { 10, 20 };
    const int rhs[3] = { 21, 12, 100 };

    gtl::match matches[2];

    // Scores that are all negative.
    reset(matches, 2);
    size_t count = gtl::bruteforce::find_matches(lhs, 2, rhs, 3, score_negative, matches, 2);
    REQUIRE(count == 2, "Expected 2 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 1, -998.0f));
    REQUIRE(is_match(matches[1], 1, 0, -999.0f));

    // Scores that are all very large.
    reset(matches, 2);
    count = gtl::bruteforce::find_matches(lhs, 2, rhs, 3, score_large, matches, 2);
    REQUIRE(count == 2, "Expected 2 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 1, 2.0f * 1e36f));
    REQUIRE(is_match(matches[1], 1, 0, 1.0f * 1e36f));
}

TEST(bruteforce, function, find_matches_best_ties) {
    // The first of the equally scored rhs descriptors is the match.
    const int lhs[1] = { 10 };
    const int rhs[4] = { 13, 12, 8, 12 };

    gtl::match matches[1];
    reset(matches, 1);

    const size_t count = gtl::bruteforce::find_matches(lhs, 1, rhs, 4, score_distance, matches, 1);
    REQUIRE(count == 1, "Expected 1 match, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 1, 2.0f));
}

TEST(bruteforce, function, find_matches_best_limits) {
    const int lhs[4] = { 10, 20, 30, 41 };
    const int rhs[5] = { 43, 31, 9, 100, 22 };

    gtl::match matches[4];

    // No lhs descriptors.
    reset(matches, 4);
    size_t count = gtl::bruteforce::find_matches(lhs, 0, rhs, 5, score_distance, matches, 4);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);
    REQUIRE(is_untouched(matches[0]));

    // No rhs descriptors.
    reset(matches, 4);
    count = gtl::bruteforce::find_matches(lhs, 4, rhs, 0, score_distance, matches, 4);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);

    // No space for matches.
    reset(matches, 4);
    count = gtl::bruteforce::find_matches(lhs, 4, rhs, 5, score_distance, matches, 0);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);
    REQUIRE(is_untouched(matches[0]));

    // Less space than lhs descriptors.
    reset(matches, 4);
    count = gtl::bruteforce::find_matches(lhs, 4, rhs, 5, score_distance, matches, 2);
    REQUIRE(count == 2, "Expected 2 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 1, 4, 2.0f));
    REQUIRE(is_untouched(matches[2]));
    REQUIRE(is_untouched(matches[3]));
}

TEST(bruteforce, function, find_matches_best_threshold) {
    const int lhs[4] = { 10, 20, 30, 41 };
    const int rhs[5] = { 43, 31, 9, 100, 22 };

    gtl::match matches[5];

    // Only scores below the threshold are matches.
    reset(matches, 5);
    size_t count = gtl::bruteforce::find_matches(lhs, 4, rhs, 5, score_distance, 2.0f, matches, 4);
    REQUIRE(count == 2, "Expected 2 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 2, 1, 1.0f));
    REQUIRE(is_untouched(matches[4]));

    // A threshold above every best score matches everything.
    reset(matches, 5);
    count = gtl::bruteforce::find_matches(lhs, 4, rhs, 5, score_distance, 2.5f, matches, 4);
    REQUIRE(count == 4, "Expected 4 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 1, 4, 2.0f));
    REQUIRE(is_match(matches[2], 2, 1, 1.0f));
    REQUIRE(is_match(matches[3], 3, 0, 2.0f));
    REQUIRE(is_untouched(matches[4]));

    // A threshold below every score matches nothing.
    reset(matches, 5);
    count = gtl::bruteforce::find_matches(lhs, 4, rhs, 5, score_distance, 1.0f, matches, 4);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);

    // No rhs descriptors.
    reset(matches, 5);
    count = gtl::bruteforce::find_matches(lhs, 4, rhs, 0, score_distance, 2.5f, matches, 4);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);
}

TEST(bruteforce, function, find_matches_count) {
    const int lhs[3] = { 10, 20, 30 };
    const int rhs[5] = { 43, 31, 9, 100, 22 };

    gtl::match matches[7];
    reset(matches, 7);

    // The matches for each lhs descriptor are sorted from best to worst, and the matches array is exactly large enough.
    const size_t count = gtl::bruteforce::find_matches(lhs, 3, rhs, 5, score_distance, static_cast<size_t>(2), matches, 6);
    REQUIRE(count == 6, "Expected 6 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 0, 4, 12.0f));
    REQUIRE(is_match(matches[2], 1, 4, 2.0f));
    REQUIRE(is_match(matches[3], 1, 1, 11.0f));
    REQUIRE(is_match(matches[4], 2, 1, 1.0f));
    REQUIRE(is_match(matches[5], 2, 4, 8.0f));
    REQUIRE(is_untouched(matches[6]));
}

TEST(bruteforce, function, find_matches_count_ties) {
    // Equally scored rhs descriptors are kept in order.
    const int lhs[1] = { 10 };
    const int rhs[5] = { 13, 12, 8, 12, 7 };

    gtl::match matches[3];
    reset(matches, 3);

    const size_t count = gtl::bruteforce::find_matches(lhs, 1, rhs, 5, score_distance, static_cast<size_t>(3), matches, 3);
    REQUIRE(count == 3, "Expected 3 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 1, 2.0f));
    REQUIRE(is_match(matches[1], 0, 2, 2.0f));
    REQUIRE(is_match(matches[2], 0, 3, 2.0f));
}

TEST(bruteforce, function, find_matches_count_limits) {
    const int lhs[3] = { 10, 20, 30 };
    const int rhs[5] = { 43, 31, 9, 100, 22 };

    gtl::match matches[12];

    // Fewer rhs descriptors than requested matches only returns the matches that exist.
    reset(matches, 12);
    size_t count = gtl::bruteforce::find_matches(lhs, 3, rhs, 2, score_distance, static_cast<size_t>(4), matches, 12);
    REQUIRE(count == 6, "Expected 6 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 1, 21.0f));
    REQUIRE(is_match(matches[1], 0, 0, 33.0f));
    REQUIRE(is_match(matches[2], 1, 1, 11.0f));
    REQUIRE(is_match(matches[3], 1, 0, 23.0f));
    REQUIRE(is_match(matches[4], 2, 1, 1.0f));
    REQUIRE(is_match(matches[5], 2, 0, 13.0f));

    // No rhs descriptors.
    reset(matches, 12);
    count = gtl::bruteforce::find_matches(lhs, 3, rhs, 0, score_distance, static_cast<size_t>(2), matches, 12);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);

    // Space for the matches of only two of the lhs descriptors.
    reset(matches, 12);
    count = gtl::bruteforce::find_matches(lhs, 3, rhs, 5, score_distance, static_cast<size_t>(2), matches, 5);
    REQUIRE(count == 4, "Expected 4 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 0, 4, 12.0f));
    REQUIRE(is_match(matches[2], 1, 4, 2.0f));
    REQUIRE(is_match(matches[3], 1, 1, 11.0f));
    REQUIRE(is_untouched(matches[4]));

    // Scores that are all negative.
    reset(matches, 12);
    count = gtl::bruteforce::find_matches(lhs, 1, rhs, 5, score_negative, static_cast<size_t>(2), matches, 2);
    REQUIRE(count == 2, "Expected 2 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, -999.0f));
    REQUIRE(is_match(matches[1], 0, 4, -988.0f));
    REQUIRE(is_untouched(matches[2]));
}

TEST(bruteforce, function, find_matches_count_threshold) {
    const int lhs[3] = { 10, 20, 30 };
    const int rhs[5] = { 43, 31, 9, 100, 22 };

    gtl::match matches[7];

    // Only scores below the threshold are matches, and the matches array is exactly large enough.
    reset(matches, 7);
    size_t count = gtl::bruteforce::find_matches(lhs, 3, rhs, 5, score_distance, 10.0f, static_cast<size_t>(2), matches, 6);
    REQUIRE(count == 4, "Expected 4 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 1, 4, 2.0f));
    REQUIRE(is_match(matches[2], 2, 1, 1.0f));
    REQUIRE(is_match(matches[3], 2, 4, 8.0f));
    REQUIRE(is_untouched(matches[6]));

    // A threshold above every score matches the requested count for each lhs descriptor.
    reset(matches, 7);
    count = gtl::bruteforce::find_matches(lhs, 3, rhs, 5, score_distance, 1000.0f, static_cast<size_t>(2), matches, 6);
    REQUIRE(count == 6, "Expected 6 matches, found %zu.", count);
    REQUIRE(is_match(matches[0], 0, 2, 1.0f));
    REQUIRE(is_match(matches[1], 0, 4, 12.0f));
    REQUIRE(is_match(matches[2], 1, 4, 2.0f));
    REQUIRE(is_match(matches[3], 1, 1, 11.0f));
    REQUIRE(is_match(matches[4], 2, 1, 1.0f));
    REQUIRE(is_match(matches[5], 2, 4, 8.0f));
    REQUIRE(is_untouched(matches[6]));

    // A threshold below every score matches nothing.
    reset(matches, 7);
    count = gtl::bruteforce::find_matches(lhs, 3, rhs, 5, score_distance, 1.0f, static_cast<size_t>(2), matches, 6);
    REQUIRE(count == 0, "Expected 0 matches, found %zu.", count);
}
