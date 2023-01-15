/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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

#include <platform/cpu>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(cpu, traits, standard) {
    REQUIRE(std::is_pod<gtl::cpu>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::cpu>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::cpu>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::cpu>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(cpu, constructor, empty) {
    gtl::cpu cpu;
    testbench::do_not_optimise_away(cpu);
}

TEST(cpu, function, is_build_compatible) {
    gtl::cpu cpu;
    REQUIRE(cpu.is_build_compatible());
}

TEST(cpu, function, get_max_leaf_id) {
    gtl::cpu cpu;
    testbench::do_not_optimise_away(cpu.get_max_leaf_id());
}

TEST(cpu, function, get_manufacturer_id) {
    gtl::cpu cpu;
    testbench::do_not_optimise_away(cpu.get_manufacturer_id());
}

TEST(cpu, function, get_brand_string) {
    gtl::cpu cpu;
    testbench::do_not_optimise_away(cpu.get_brand_string());
}

TEST(cpu, function, has_xxx) {
    gtl::cpu cpu;

    testbench::do_not_optimise_away(cpu.has_mmx());

    testbench::do_not_optimise_away(cpu.has_sse());
    testbench::do_not_optimise_away(cpu.has_sse2());
    testbench::do_not_optimise_away(cpu.has_sse3());
    testbench::do_not_optimise_away(cpu.has_ssse3());
    testbench::do_not_optimise_away(cpu.has_sse4_1());
    testbench::do_not_optimise_away(cpu.has_sse4_2());

    testbench::do_not_optimise_away(cpu.has_popcnt());

    testbench::do_not_optimise_away(cpu.has_avx());
    testbench::do_not_optimise_away(cpu.has_avx2());

    testbench::do_not_optimise_away(cpu.has_avx512_foundation());

    testbench::do_not_optimise_away(cpu.has_bmi());
    testbench::do_not_optimise_away(cpu.has_bmi2());
}

TEST(cpu, evaluate, print_flags) {
    gtl::cpu cpu;
    PRINT("get_max_leaf_id:             %d\n", cpu.get_max_leaf_id());
    PRINT("get_max_extended_leaf_id:    %d\n", cpu.get_max_extended_leaf_id());

    PRINT("get_manufacturer_id:         %s\n", cpu.get_manufacturer_id().data);
    PRINT("get_brand_string:            %s\n", cpu.get_brand_string().data);

    PRINT("has_mmx:                     %d\n", cpu.has_mmx());

    PRINT("has_fma:                     %d\n", cpu.has_fma());

    PRINT("has_sse:                     %d\n", cpu.has_sse());
    PRINT("has_sse2:                    %d\n", cpu.has_sse2());
    PRINT("has_sse3:                    %d\n", cpu.has_sse3());
    PRINT("has_ssse3:                   %d\n", cpu.has_ssse3());
    PRINT("has_sse4.1:                  %d\n", cpu.has_sse4_1());
    PRINT("has_sse4.2:                  %d\n", cpu.has_sse4_2());

    PRINT("has_popcnt:                  %d\n", cpu.has_popcnt());

    PRINT("has_avx:                     %d\n", cpu.has_avx());
    PRINT("has_avx2:                    %d\n", cpu.has_avx2());

    PRINT("has_avx512f:                 %d\n", cpu.has_avx512_foundation());

    PRINT("has_bmi:                     %d\n", cpu.has_bmi());
    PRINT("has_bmi2:                    %d\n", cpu.has_bmi2());

}
