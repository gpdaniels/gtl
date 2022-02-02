/*
Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

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

#include <vision/feature/distributor/square_covering>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(square_covering, function, zero_points) {
    const gtl::feature features_detected_sorted[1] = {{ 2, 3, 16, 0.7f }};
    const int features_detected_sorted_size = 0;
    const int max_width = 80;
    const int max_height = 80;
    const int min_features = 0;
    const int max_features = 0;
    gtl::feature features_distributed[1] = {};
    const int features = gtl::square_covering(&features_detected_sorted[0], features_detected_sorted_size, max_width, max_height, min_features, max_features, &features_distributed[0]);
    REQUIRE(features == 0, "Number of features distributed %d of %d", features, max_features);
    REQUIRE(features_distributed[0].x == 0);
    REQUIRE(features_distributed[0].y == 0);
    REQUIRE(testbench::is_value_equal(features_distributed[0].response, 0.0f));
    REQUIRE(testbench::is_value_equal(features_distributed[0].angle, 0.0f));
}

TEST(square_covering, function, one_point) {
    const gtl::feature features_detected_sorted[1] = {{ 2, 3, 16, 0.7f }};
    const int features_detected_sorted_size = 1;
    const int max_width = 80;
    const int max_height = 80;
    const int min_features = 1;
    const int max_features = 1;
    gtl::feature features_distributed[1] = {};
    const int features = gtl::square_covering(&features_detected_sorted[0], features_detected_sorted_size, max_width, max_height, min_features, max_features, &features_distributed[0]);
    REQUIRE(features == 1, "Number of features distributed %d of %d", features, max_features);
    REQUIRE(features_distributed[0].x == 2);
    REQUIRE(features_distributed[0].y == 3);
    REQUIRE(testbench::is_value_equal(features_distributed[0].response, 16.0f));
    REQUIRE(testbench::is_value_equal(features_distributed[0].angle, 0.7f));
}

TEST(square_covering, function, square_ten_points) {
    const gtl::feature features_detected_sorted[10] = {
        {  1, 21, 89, 0.2f },
        { 42, 42, 65, 0.6f },
        { 26,  1, 35, 0.3f },
        { 21, 15, 21, 0.4f },
        { 48, 72, 10, 0.7f },
        { 72,  2,  3, 0.9f },
        {  7,  3,  2, 0.1f },
        { 63, 16,  2, 0.8f },
        { 79, 68,  2, 1.0f },
        { 24, 29,  1, 0.5f }
    };
    const int features_detected_sorted_size = 10;
    const int max_width = 80;
    const int max_height = 80;
    gtl::feature features_distributed[20] = {};

    int max_sizes[10] = {
        2, 3, 4, 5, 6, 7, 8, 9, 10, 20
    };
    int distributed_sizes[10] = {
        2, 3, 4, 5, 6, 7, 8, 9, 10, 10
    };
    int distributed_indexes[10][10] = {
        { 0, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 4, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 5, 8, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 4, 7, 8, 0, 0, 0, 0, 0 },
        { 0, 1, 2, 4, 5, 8, 0, 0, 0, 0 },
        { 0, 1, 2, 4, 5, 8, 9, 0, 0, 0 },
        { 0, 1, 2, 4, 5, 6, 8, 9, 0, 0 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
    };

    for (int i = 0; i < 10; ++i) {
        const int min_features = max_sizes[i];
        const int max_features = max_sizes[i];
        const int features = gtl::square_covering(&features_detected_sorted[0], features_detected_sorted_size, max_width, max_height, min_features, max_features, &features_distributed[0]);
        REQUIRE(features == distributed_sizes[i], "Number of features distributed %d of %d", features, max_features);

        for (int j = 0; j < features; ++j) {
            const int distributed_index = distributed_indexes[i][j];
            REQUIRE(features_distributed[j].x == features_detected_sorted[distributed_index].x);
            REQUIRE(features_distributed[j].y == features_detected_sorted[distributed_index].y);
            REQUIRE(testbench::is_value_equal(features_distributed[j].response, features_detected_sorted[distributed_index].response));
            REQUIRE(testbench::is_value_equal(features_distributed[j].angle, features_detected_sorted[distributed_index].angle));
        }
    }
}

TEST(square_covering, function, portait_seven_points) {
}

TEST(square_covering, function, landscape_seven_points) {
}

TEST(square_covering, function, bunch_points) {
}

#if 0

std::vector<cv::FeaturePoint> defineKeyPointsSquare()
{
    std::vector<cv::FeaturePoint> keyPoints = {cv::FeaturePoint(7, 3, 2),
                                                 cv::FeaturePoint(1, 21, 89),
                                                 cv::FeaturePoint(26, 1, 35),
                                                 cv::FeaturePoint(21, 15, 21),
                                                 cv::FeaturePoint(24, 29, 1),
                                                 cv::FeaturePoint(42, 42, 65),
                                                 cv::FeaturePoint(48, 72, 10),
                                                 cv::FeaturePoint(63, 16, 2),
                                                 cv::FeaturePoint(72, 2, 3),
                                                 cv::FeaturePoint(79, 68, 2)};
    return keyPoints;
}


std::vector<cv::FeaturePoint> defineKeyPointsPortrait()
{
    std::vector<cv::FeaturePoint> keyPoints = {cv::FeaturePoint(15, 12, 56),
                                                 cv::FeaturePoint(0, 20, 10),
                                                 cv::FeaturePoint(6, 25, 15),
                                                 cv::FeaturePoint(7, 51, 1),
                                                 cv::FeaturePoint(9, 54, 3),
                                                 cv::FeaturePoint(27, 55, 15),
                                                 cv::FeaturePoint(18, 63, 13)};
    return keyPoints;
}


// Transpose the portrait keypoints.
std::vector<cv::FeaturePoint> defineKeyPointsLandscape()
{
    std::vector<cv::FeaturePoint> keyPointsPortrait = defineKeyPointsPortrait();
    std::vector<cv::FeaturePoint> keyPoints;
    keyPoints.reserve(keyPointsPortrait.size());
    for (const cv::FeaturePoint& fp : keyPointsPortrait)
    {
        keyPoints.push_back(cv::FeaturePoint(fp.y, fp.x, fp.score));
    }
    return keyPoints;
}


void case0KeyPointsSquareCovering()
{
    cv::SquareCoveringDistributor distributor(100);
    // A tree made from no keypoints should give zero keypoints:
    std::vector<cv::FeaturePoint> inputFeaturePoints;
    std::vector<cv::FeaturePoint> result;
    distributor.Distribute(inputFeaturePoints, 80, 80, result);

    THEN("The output vector should be empty") { REQUIRE(result.size() == 0); }
}


void case1KeyPointSquareCovering()
{
    cv::SquareCoveringDistributor distributor(100);
    // A tree made from just one keypoint.
    std::vector<cv::FeaturePoint> inputFeaturePoints = {cv::FeaturePoint(2, 3, 16)};
    std::vector<cv::FeaturePoint> result;
    distributor.Distribute(inputFeaturePoints, 80, 80, result);

    THEN("The output vector should contain the input keypoint")
    {
        REQUIRE(result.size() == 1);
        REQUIRE(inputFeaturePoints[0].x == static_cast<int>(result[0].x));
        REQUIRE(inputFeaturePoints[0].y == static_cast<int>(result[0].y));
    }
}


/**
 @brief Helper function to distribute the keypoints with the SquareCoveringDistributor,
 and then check the result is as expected.
 @param inputFeaturePoints The input feature points.
 @param numDesiredFeatures The number of desired features.
 @param expectedIndices The indices of the points in inputFeaturePoints that are expected to be
 returned by the distributor.
 @param width The maximum X coordinate for a point.
 @param height The maximum Y coordinate for a point.
*/
void distributeAndCheckWithSquareCovering(
    std::vector<cv::FeaturePoint>& inputFeaturePoints,
    int numDesiredFeatures,
    const std::vector<int>& expectedIndices,
    int width = 80,
    int height = 80)
{
    REQUIRE(std::all_of(
        inputFeaturePoints.cbegin(),
        inputFeaturePoints.cend(),
        [width, height](const cv::FeaturePoint& fp) {
            return fp.x >= 0 && fp.x < width && fp.y >= 0 && fp.y < height;
        }));

    WHEN("The number of desired features is " + std::to_string(numDesiredFeatures))
    {
        cv::SquareCoveringDistributor distributor(numDesiredFeatures);
        std::vector<cv::FeaturePoint> result;
        distributor.Distribute(inputFeaturePoints, width, height, result);

        THEN(
            "The output vector should contain " + std::to_string(expectedIndices.size()) +
            " keypoint")
        {
            REQUIRE(result.size() == expectedIndices.size());
        }

        THEN("All the expected points should be returned")
        {
            bool allFound = true;
            for (int i : expectedIndices)
            {
                bool found = false;
                for (const cv::FeaturePoint& fp : result)
                {
                    if (static_cast<int>(fp.x) == inputFeaturePoints[i].x &&
                        static_cast<int>(fp.y) == inputFeaturePoints[i].y)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    allFound = false;
                    break;
                }
            }
            REQUIRE(allFound);
        }
    }
}


TEST_CASE("SquareCoveringDistributor", "[SquareCoveringDistributor][FeatureDistribution]")
{
    GIVEN("0 points for the uadTreeDistributor") { case0KeyPointsSquareCovering(); }

    GIVEN("1 point for the SquareCoveringDistributor") { case1KeyPointSquareCovering(); }

    GIVEN("10 points for the SquareCoveringDistributor")
    {
        std::vector<cv::FeaturePoint> inputFeaturePoints = defineKeyPointsSquare();

        // For any number of desired features bigger than the total number of points, the result
        // should always be the same: obtaining all the points.
        std::vector<int> allIndices(inputFeaturePoints.size());
        for (size_t i = 0; i < inputFeaturePoints.size(); i++)
        {
            allIndices[i] = static_cast<int>(i);
        }
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 10, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 11, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 12, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 13, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 14, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 15, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 20, allIndices);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 100, allIndices);

        // For desired numbers of features below the total amount of input points, different
        // behaviours can be obtained.
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 2, {1, 9});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 3, {1, 5, 8});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 4, {1, 5, 8, 9});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 5, {1, 5, 6, 8, 9});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 6, {1, 5, 6, 8, 9});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 7, {0, 1, 2, 4, 5, 6, 8, 9});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 8, {0, 1, 2, 3, 5, 6, 7, 8, 9});
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 9, {0, 1, 2, 3, 5, 6, 7, 8, 9});
    }

    GIVEN("7 points in portrait format")
    {
        int width = 30;
        int height = 80;
        std::vector<cv::FeaturePoint> inputFeaturePoints = defineKeyPointsPortrait();

        // For any number of desired features bigger than the total number of points, the result
        // should always be the same: obtaining all the points.
        std::vector<int> allIndices(inputFeaturePoints.size());
        for (size_t i = 0; i < inputFeaturePoints.size(); i++)
        {
            allIndices[i] = static_cast<int>(i);
        }
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 7, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 8, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 9, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 10, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 20, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 100, allIndices, width, height);

        // For desired numbers of features below the total amount of input points, different
        // behaviours can be obtained.
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 2, {0, 5}, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 3, {0, 3, 5}, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 4, {0, 2, 4, 5}, width, height);
        distributeAndCheckWithSquareCovering(
            inputFeaturePoints, 5, {0, 1, 2, 4, 5, 6}, width, height);
        distributeAndCheckWithSquareCovering(
            inputFeaturePoints, 6, {0, 1, 2, 4, 5, 6}, width, height);
    }

    GIVEN("7 points in landscape format")
    {
        int width = 80;
        int height = 30;
        std::vector<cv::FeaturePoint> inputFeaturePoints = defineKeyPointsLandscape();

        // For any number of desired features bigger than the total number of points, the result
        // should always be the same: obtaining all the points.
        std::vector<int> allIndices(inputFeaturePoints.size());
        for (size_t i = 0; i < inputFeaturePoints.size(); i++)
        {
            allIndices[i] = static_cast<int>(i);
        }
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 7, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 8, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 9, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 10, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 20, allIndices, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 100, allIndices, width, height);

        // For desired numbers of features below the total amount of input points, different
        // behaviours can be obtained.
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 2, {0, 5}, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 3, {0, 3, 5}, width, height);
        distributeAndCheckWithSquareCovering(inputFeaturePoints, 4, {0, 2, 4, 5}, width, height);
        distributeAndCheckWithSquareCovering(
            inputFeaturePoints, 5, {0, 1, 2, 4, 5, 6}, width, height);
        distributeAndCheckWithSquareCovering(
            inputFeaturePoints, 6, {0, 1, 2, 4, 5, 6}, width, height);
    }

    GIVEN("A bunch of keypoints with scores between 20 and 50")
    {
        int width = 80;
        int height = 80;
        std::vector<cv::FeaturePoint> inputFeaturePoints = {cv::FeaturePoint(12, 28, 34),
                                                              cv::FeaturePoint(64, 16, 20),
                                                              cv::FeaturePoint(1, 15, 50),
                                                              cv::FeaturePoint(74, 72, 27),
                                                              cv::FeaturePoint(26, 69, 24),
                                                              cv::FeaturePoint(31, 18, 48),
                                                              cv::FeaturePoint(50, 9, 39)};

        WHEN("We add a point with a score below 20")
        {
            int newX = 30;
            int newY = 30;
            int newScore = 15;
            inputFeaturePoints.push_back(cv::FeaturePoint(newX, newY, newScore));

            AND_WHEN("We distribute requiring a number of points lower than the total amount")
            {
                THEN("The last point added shouldn't be selected")
                {
                    cv::SquareCoveringDistributor distributor(5);
                    std::vector<cv::FeaturePoint> result;
                    distributor.Distribute(inputFeaturePoints, width, height, result);
                    bool found = false;
                    for (const cv::FeaturePoint& fp : result)
                    {
                        if (fp.x == newX && fp.y == newY && fp.score == newScore)
                        {
                            found = true;
                            break;
                        }
                    }
                    REQUIRE(!found);
                }
            }
        }

        WHEN("We add a point with a score above 50")
        {
            int newX = 30;
            int newY = 30;
            int newScore = 55;
            inputFeaturePoints.push_back(cv::FeaturePoint(newX, newY, newScore));

            AND_WHEN("We distribute requiring a number of points lower than the total amount")
            {
                THEN("The last point added should be selected")
                {
                    cv::SquareCoveringDistributor distributor(5);
                    std::vector<cv::FeaturePoint> result;
                    distributor.Distribute(inputFeaturePoints, width, height, result);
                    bool found = false;
                    for (const cv::FeaturePoint& fp : result)
                    {
                        if (fp.x == newX && fp.y == newY && fp.score == newScore)
                        {
                            found = true;
                            break;
                        }
                    }
                    REQUIRE(found);
                }
            }
        }
    }
}

#endif
