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
#include <macro.tests.hpp>

#include <cpu>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::cpu) >= 24, "sizeof(gtl::cpu) = %ld, expected >= %lld", sizeof(gtl::cpu), 24ull);

    REQUIRE(std::is_pod<gtl::cpu>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::cpu>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::cpu>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::cpu>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(constructor, empty) {
    gtl::cpu cpu;
    do_not_optimise_away(cpu);
}

TEST(function, get_max_leaf_id) {
    gtl::cpu cpu;
    cpu.get_max_leaf_id();
}

TEST(function, get_manufacturer_id) {
    gtl::cpu cpu;
    cpu.get_manufacturer_id();
}

TEST(function, has_xxx) {
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
}

TEST(evaluation, print_flags) {
    gtl::cpu cpu;
    PRINT("get_max_leaf_id:     %d", cpu.get_max_leaf_id());

    PRINT("get_manufacturer_id: %s", cpu.get_manufacturer_id().c_str());

    PRINT("has_mmx:             %d", cpu.has_mmx());

    PRINT("has_sse:             %d", cpu.has_sse());
    PRINT("has_sse2:            %d", cpu.has_sse2());
    PRINT("has_sse3:            %d", cpu.has_sse3());
    PRINT("has_ssse3:           %d", cpu.has_ssse3());
    PRINT("has_sse4.1:          %d", cpu.has_sse4_1());
    PRINT("has_sse4.2:          %d", cpu.has_sse4_2());

    PRINT("has_popcnt:          %d", cpu.has_popcnt());

    PRINT("has_avx:             %d", cpu.has_avx());
    PRINT("has_avx2:            %d", cpu.has_avx2());
}
