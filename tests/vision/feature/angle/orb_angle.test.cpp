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

#include <main.tests.hpp>
#include <comparison.tests.hpp>
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <vision/feature/angle/orb_angle>

#if defined(_MSC_VER)
#   pragma warning(push)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(orb_angle, function, gradient_with_inverted_square) {
     // Create some data.
     constexpr static const unsigned int data_width = 128;
     constexpr static const unsigned int data_height = 64;
     unsigned char data[data_height][data_width] = {};
     for (unsigned int y = 0; y < data_height; ++y) {
         for (unsigned int x = 0; x < data_width; ++x) {
             data[y][x] = static_cast<unsigned char>(x + y);
             if ((x > data_width / 4) && (x < (data_width * 3) / 4) && (y > data_height / 4) && (y < (data_height * 3) / 4)) {
                 data[y][x] = 0;
             }
         }
     }

     constexpr static const unsigned int features_count = 12;
     constexpr static const unsigned int features[features_count][2] = {
         {  93,  17 }, {  94,  17 }, {  95,  17 }, {  94,  18 }, {  95,  18 }, {  95,  19 },
         {  95,  45 }, {  94,  46 }, {  95,  46 }, {  93,  47 }, {  94,  47 }, {  95,  47 }
     };
     constexpr static const float angles_radians[features_count] = {
         -0.640070f, -0.576437f, -0.514927f, -0.543202f, -0.483588f, -0.448683f,
          0.698690f,  0.785398f,  0.743421f,  0.872107f,  0.827376f,  0.785398f
     };

     for (unsigned int i = 0; i < features_count; ++i) {
         REQUIRE(testbench::is_value_approx(gtl::orb_angle(&data[features[i][1]][features[i][0]], data_width), angles_radians[i], 1E-6f), "%f != %f", static_cast<double>(gtl::orb_angle(&data[features[i][1]][features[i][0]], data_width)), static_cast<double>(angles_radians[i]));
     }
 }
