/*
The MIT License
Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/
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
#include <benchmark.tests.hpp>
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

    #if defined(_WIN32) && !defined(_WIN64)
        REQUIRE(sizeof(gtl::cpu) == 8, "sizeof(gtl::cpu) = %ld, expected == %lld", sizeof(gtl::cpu), 8ull);
    #else
        REQUIRE(sizeof(gtl::cpu) == 16, "sizeof(gtl::cpu) = %ld, expected == %lld", sizeof(gtl::cpu), 16ull);
    #endif

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
    cpu.get_max_leaf_id();
}

TEST(cpu, function, get_manufacturer_id) {
    gtl::cpu cpu;
    cpu.get_manufacturer_id();
}

TEST(cpu, function, get_brand_string) {
    gtl::cpu cpu;
    cpu.get_brand_string();
}

TEST(cpu, function, has_xxx) {
    gtl::cpu cpu;

    cpu.has_mmx();

    cpu.has_sse();
    cpu.has_sse2();
    cpu.has_sse3();
    cpu.has_ssse3();
    cpu.has_sse4_1();
    cpu.has_sse4_2();

    cpu.has_popcnt();

    cpu.has_avx();
    cpu.has_avx2();

    cpu.has_avx512_foundation();

    cpu.has_bmi();
    cpu.has_bmi2();
}

TEST(cpu, evaluation, print_flags) {
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
