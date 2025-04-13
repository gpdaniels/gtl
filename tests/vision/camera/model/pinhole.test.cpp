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

#include <vision/camera/model/pinhole>

TEST(pinhole, constructor, empty) {
    gtl::pinhole<float> pinhole;
    testbench::do_not_optimise_away(pinhole);
}

TEST(pinhole, function, get_parameters) {
    gtl::pinhole<float> pinhole;
    float parameters[4]{};
    pinhole.get_parameters(&parameters[0], 4);
    REQUIRE(testbench::is_value_equal(parameters[0], 1.0f));
    REQUIRE(testbench::is_value_equal(parameters[1], 1.0f));
    REQUIRE(testbench::is_value_equal(parameters[2], 0.5f));
    REQUIRE(testbench::is_value_equal(parameters[3], 0.5f));
    const float parameters_new[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    pinhole.set_parameters(&parameters_new[0], 4);
    pinhole.get_parameters(&parameters[0], 4);
    REQUIRE(testbench::is_value_equal(parameters_new[0], parameters[0]));
    REQUIRE(testbench::is_value_equal(parameters_new[1], parameters[1]));
    REQUIRE(testbench::is_value_equal(parameters_new[2], parameters[2]));
    REQUIRE(testbench::is_value_equal(parameters_new[3], parameters[3]));
}

TEST(pinhole, function, set_parameters) {
    gtl::pinhole<float> pinhole;
    const float parameters_new[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    pinhole.set_parameters(&parameters_new[0], 4);
    float parameters[4];
    pinhole.get_parameters(&parameters[0], 4);
    REQUIRE(testbench::is_value_equal(parameters_new[0], parameters[0]));
    REQUIRE(testbench::is_value_equal(parameters_new[1], parameters[1]));
    REQUIRE(testbench::is_value_equal(parameters_new[2], parameters[2]));
    REQUIRE(testbench::is_value_equal(parameters_new[3], parameters[3]));
}

TEST(pinhole, function, project) {
    {
        gtl::pinhole<float> pinhole;
        const float world_point[3] = { 0, 0, 1 };
        const float image_point_expected[2] = { 0.5f, 0.5f };
        float image_point[2]{};
        REQUIRE(pinhole.project(&world_point[0], &image_point[0]));
        REQUIRE(testbench::is_value_equal(image_point[0], image_point_expected[0]));
        REQUIRE(testbench::is_value_equal(image_point[1], image_point_expected[1]));
    }
    {
        gtl::pinhole<float> pinhole;
        const float world_point[3] = { 0, 0, 1 };
        const float image_point_expected[2] = { 0.5f, 0.5f };
        const float jacobian_projection_expected[2][3] = { { 1, 0, 0 }, { 0, 1, 0 } };
        float image_point[2]{};
        float jacobian_projection[2 * 3]{};
        REQUIRE(pinhole.project(&world_point[0], &image_point[0], &jacobian_projection[0]));
        REQUIRE(testbench::is_value_equal(image_point[0], image_point_expected[0]));
        REQUIRE(testbench::is_value_equal(image_point[1], image_point_expected[1]));
        for (size_t j = 0; j < 2; ++j) {
            for (size_t k = 0; k < 3; ++k) {
                REQUIRE(testbench::is_value_equal(jacobian_projection[j * 3 + k], jacobian_projection_expected[j][k]));
            }
        }
    }
    {
        gtl::pinhole<float> pinhole;
        const float world_point[3] = { 0, 0, 1 };
        const float image_point_expected[2] = { 0.5f, 0.5f };
        const float jacobian_parameter_expected[2][pinhole.parameter_count] = { { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
        float image_point[2]{};
        float jacobian_parameter[2 * pinhole.parameter_count]{};
        REQUIRE(pinhole.project(&world_point[0], &image_point[0], nullptr, &jacobian_parameter[0]));
        REQUIRE(testbench::is_value_equal(image_point[0], image_point_expected[0]));
        REQUIRE(testbench::is_value_equal(image_point[1], image_point_expected[1]));
        for (size_t j = 0; j < 2; ++j) {
            for (size_t k = 0; k < pinhole.parameter_count; ++k) {
                REQUIRE(testbench::is_value_equal(jacobian_parameter[j * pinhole.parameter_count + k], jacobian_parameter_expected[j][k]));
            }
        }
    }
    {
        gtl::pinhole<float> pinhole;
        const float world_point[3] = { -1, 1, 2 };
        const float image_point_expected[2] = { 0.0f, 1.0f };
        const float jacobian_projection_expected[2][3] = { { 0.5, 0, 0.25 }, { 0, 0.5, -0.25 } };
        const float jacobian_parameter_expected[2][pinhole.parameter_count] = { { -0.5f, 0, 1, 0 }, { 0, 0.5f, 0, 1 } };
        float image_point[2]{};
        float jacobian_projection[2 * 3]{};
        float jacobian_parameter[2 * pinhole.parameter_count]{};
        REQUIRE(pinhole.project(&world_point[0], &image_point[0], &jacobian_projection[0], &jacobian_parameter[0]));
        REQUIRE(testbench::is_value_equal(image_point[0], image_point_expected[0]));
        REQUIRE(testbench::is_value_equal(image_point[1], image_point_expected[1]));
        for (size_t j = 0; j < 2; ++j) {
            for (size_t k = 0; k < 3; ++k) {
                REQUIRE(testbench::is_value_equal(jacobian_projection[j * 3 + k], jacobian_projection_expected[j][k]));
            }
            for (size_t k = 0; k < pinhole.parameter_count; ++k) {
                REQUIRE(testbench::is_value_equal(jacobian_parameter[j * pinhole.parameter_count + k], jacobian_parameter_expected[j][k]));
            }
        }
    }
}

TEST(pinhole, function, unproject) {
    {
        gtl::pinhole<float> pinhole;
        const float image_point[2] = { 0.5, 0.5 };
        const float world_ray_expected[3] = { 0, 0, 1 };
        float world_ray[3]{};
        REQUIRE(pinhole.unproject(&image_point[0], &world_ray[0]));
        REQUIRE(testbench::is_value_equal(world_ray[0], world_ray_expected[0]));
        REQUIRE(testbench::is_value_equal(world_ray[1], world_ray_expected[1]));
        REQUIRE(testbench::is_value_equal(world_ray[2], world_ray_expected[2]));
    }
    {
        gtl::pinhole<float> pinhole;
        const float image_point[2] = { 0.5, 0.5 };
        const float world_ray_expected[3] = { 0, 0, 1 };
        float jacobian_unprojection_expected[3][2] = { { 1, 0 }, { 0, 1 }, { 0, 0 } };
        float world_ray[3]{};
        float jacobian_unprojection[3 * 2]{};
        REQUIRE(pinhole.unproject(&image_point[0], &world_ray[0], &jacobian_unprojection[0]));
        REQUIRE(testbench::is_value_equal(world_ray[0], world_ray_expected[0]));
        REQUIRE(testbench::is_value_equal(world_ray[1], world_ray_expected[1]));
        REQUIRE(testbench::is_value_equal(world_ray[2], world_ray_expected[2]));
        for (size_t j = 0; j < 3; ++j) {
            for (size_t k = 0; k < 2; ++k) {
                REQUIRE(testbench::is_value_equal(jacobian_unprojection[j * 2 + k], jacobian_unprojection_expected[j][k]));
            }
        }
    }
}
