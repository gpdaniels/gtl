/*
The MIT License
Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/
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

#include <protection/virtual_machine>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(virtual_machine, traits, standard) {
    REQUIRE((std::is_pod<gtl::virtual_machine>::value == false));

    REQUIRE((std::is_trivial<gtl::virtual_machine>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::virtual_machine>::value == true));

    REQUIRE((std::is_standard_layout<gtl::virtual_machine>::value == true));
}

TEST(virtual_machine, constructor, empty) {
    gtl::virtual_machine virtual_machine;
    testbench::do_not_optimise_away(virtual_machine);
}

TEST(virtual_machine, constructor, program) {
    const unsigned char program[1] = {};
    {
        gtl::virtual_machine virtual_machine(program, 0);
        testbench::do_not_optimise_away(virtual_machine);
    }
    {
        gtl::virtual_machine virtual_machine(program, 1);
        testbench::do_not_optimise_away(virtual_machine);
    }
}

TEST(virtual_machine, function, register_opcode) {
    gtl::virtual_machine virtual_machine;
    virtual_machine.register_opcode<0x00, &gtl::vmo::nop>();

    virtual_machine.register_opcode<0x01, &gtl::vmo::copy<gtl::vm::reg<gtl::vm::rn::general_a>, gtl::vm::reg<gtl::vm::rn::general_b>>>();

    virtual_machine.register_opcode<0x02, &gtl::vmo::add<gtl::vm::reg<gtl::vm::rn::general_a>, gtl::vm::reg<gtl::vm::rn::general_b>, gtl::vm::reg<gtl::vm::rn::general_c>>>();
    virtual_machine.register_opcode<0x03, &gtl::vmo::sub<gtl::vm::reg<gtl::vm::rn::general_a>, gtl::vm::reg<gtl::vm::rn::general_b>, gtl::vm::reg<gtl::vm::rn::general_c>>>();

    virtual_machine.register_opcode<0x04, &gtl::vmo::pop<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x05, &gtl::vmo::push<gtl::vm::reg<gtl::vm::rn::general_a>>>();

    virtual_machine.register_opcode<0x06, &gtl::vmo::jump<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x07, &gtl::vmo::jump_if_zero<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x08, &gtl::vmo::jump_if_not_zero<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x09, &gtl::vmo::jump_if_less_than_zero<gtl::vm::reg<gtl::vm::rn::general_a>>>();

    virtual_machine.register_opcode<0x0A, &gtl::vmo::call<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x0B, &gtl::vmo::call_if_zero<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x0C, &gtl::vmo::call_if_not_zero<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x0D, &gtl::vmo::call_if_less_than_zero<gtl::vm::reg<gtl::vm::rn::general_a>>>();
    virtual_machine.register_opcode<0x0E, &gtl::vmo::call_return>();
    virtual_machine.register_opcode<0x0F, &gtl::vmo::call_return_if_zero>();
    virtual_machine.register_opcode<0x10, &gtl::vmo::call_return_if_not_zero>();
    virtual_machine.register_opcode<0x11, &gtl::vmo::call_return_if_less_than_zero>();

    testbench::do_not_optimise_away(virtual_machine);
}

TEST(virtual_machine, function, reset) {
    gtl::virtual_machine virtual_machine;
    testbench::do_not_optimise_away(virtual_machine);
    virtual_machine.reset();
}

TEST(virtual_machine, function, tick) {
    gtl::virtual_machine virtual_machine;
    REQUIRE(virtual_machine.tick() == false);
}

TEST(virtual_machine, evaluate, simple) {
    unsigned char program[] = {
        0x01
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::nop>();

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }

    REQUIRE(successful_ticks == 1);
}


TEST(virtual_machine, evaluate, complex_copy) {
    unsigned char program[] = {
        0x01, 0xBB
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::copy<gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::reg<gtl::vm::rn::general_a>>>();

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 1);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_a>::get(virtual_machine);
    REQUIRE(result == 0xBB);
}

TEST(virtual_machine, evaluate, complex_add) {
    unsigned char program[] = {
        0x01, 34, 12
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::add<gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::reg<gtl::vm::rn::general_a>>>();

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 1);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_a>::get(virtual_machine);
    REQUIRE(result == 34 + 12);
}

TEST(virtual_machine, evaluate, complex_sub) {
    unsigned char program[] = {
        0x01, 34, 12
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::sub<gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::reg<gtl::vm::rn::general_a>>>();

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 1);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_a>::get(virtual_machine);
    REQUIRE(result == 34 - 12);
}

TEST(virtual_machine, evaluate, complex_push_and_pop) {
    unsigned char program[] = {
        0x01, 0xBB,
        0x02,
        0x01, 0x45,
        0x01, 0x96,
        0x02,
        0x02
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::push<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x02, &gtl::vmo::pop<gtl::vm::reg<gtl::vm::rn::general_a>>>();

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;

        if (successful_ticks == 2) {
            const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_a>::get(virtual_machine);
            REQUIRE(result == 0xBB);
        }
        else if (successful_ticks == 5) {
            const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_a>::get(virtual_machine);
            REQUIRE(result == 0x96);
        }
    }
    REQUIRE(successful_ticks == 6);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_a>::get(virtual_machine);
    REQUIRE(result == 0x45);
}

TEST(virtual_machine, evaluate, complex_jumps) {
    unsigned char program[255] = {};
    program[0] = 0x01;
    program[1] = 5;

    program[5] = 0xFF;
    program[6] = 123;
    program[7] = 123;
    program[8] = 0x02;
    program[9] = 15;

    program[15] = 0xFF;
    program[16] = 123;
    program[17] = 12;
    program[18] = 0x03;
    program[19] = 25;

    program[25] = 0xFF;
    program[26] = 12;
    program[27] = 123;
    program[28] = 0x04;
    program[29] = 35;

    program[35] = 0x00;

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::jump<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x02, &gtl::vmo::jump_if_zero<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x03, &gtl::vmo::jump_if_not_zero<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x04, &gtl::vmo::jump_if_less_than_zero<gtl::vm::mem<gtl::vm::rn::program_counter>>>();

    virtual_machine.register_opcode<0xFF, &gtl::vmo::sub<gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::reg<gtl::vm::rn::general_a>>>();


    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 7, "%d\n", successful_ticks);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::program_counter>::get(virtual_machine);
    REQUIRE(result == 35, "%d\n", result);
}

TEST(virtual_machine, evaluate, complex_calls_and_returns) {
    unsigned char program[255] = {};

    program[0] = 0x01;
    program[1] = 50;

    program[2] = 0xFF;
    program[3] = 123;
    program[4] = 123;
    program[5] = 0x02;
    program[6] = 60;

    program[7] = 0xFF;
    program[8] = 123;
    program[9] = 12;
    program[10] = 0x03;
    program[11] = 70;

    program[12] = 0xFF;
    program[13] = 12;
    program[14] = 123;
    program[15] = 0x04;
    program[16] = 80;

    program[17] = 0x00;

    // Functions:
    program[50] = 0x10;

    program[60] = 0xFF;
    program[61] = 123;
    program[62] = 123;
    program[63] = 0x20;

    program[70] = 0xFF;
    program[71] = 123;
    program[72] = 12;
    program[73] = 0x30;

    program[80] = 0xFF;
    program[81] = 12;
    program[82] = 123;
    program[83] = 0x40;

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::call<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x02, &gtl::vmo::call_if_zero<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x03, &gtl::vmo::call_if_not_zero<gtl::vm::mem<gtl::vm::rn::program_counter>>>();
    virtual_machine.register_opcode<0x04, &gtl::vmo::call_if_less_than_zero<gtl::vm::mem<gtl::vm::rn::program_counter>>>();

    virtual_machine.register_opcode<0x10, &gtl::vmo::call_return>();
    virtual_machine.register_opcode<0x20, &gtl::vmo::call_return_if_zero>();
    virtual_machine.register_opcode<0x30, &gtl::vmo::call_return_if_not_zero>();
    virtual_machine.register_opcode<0x40, &gtl::vmo::call_return_if_less_than_zero>();

    virtual_machine.register_opcode<0xFF, &gtl::vmo::sub<gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::reg<gtl::vm::rn::general_a>>>();


    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 14, "%d\n", successful_ticks);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::program_counter>::get(virtual_machine);
    REQUIRE(result == 17, "%d\n", result);
}

TEST(virtual_machine, evaluate, complex_syscall) {
    unsigned char program[] = {
        0x01, 1, 'h',
        0x01, 1, 'e',
        0x01, 1, 'l',
        0x01, 1, 'l',
        0x01, 1, 'o',
        0x01, 1, ' ',
        0x01, 1, 'v',
        0x01, 1, 'm',
        0x01, 1, ' ',
        0x01, 1, 'w',
        0x01, 1, 'o',
        0x01, 1, 'r',
        0x01, 1, 'l',
        0x01, 1, 'd',
        0x01, 1, '\n'
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    virtual_machine.register_opcode<0x01, &gtl::vmo::syscall<gtl::vm::mem<gtl::vm::rn::program_counter>, gtl::vm::mem<gtl::vm::rn::program_counter>>>();

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 15);
}

TEST(virtual_machine, evaluate, custom_opcode) {
    unsigned char program[] = {
        0x01,
        0x01,
        0x01
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));

    constexpr static const gtl::virtual_machine::function_type custom_opcode = [](gtl::virtual_machine& vm)->void {
        static_cast<void>(vm);
        unsigned char register_e = gtl::vm::reg<gtl::vm::rn::general_e>::get(vm);
        register_e += 1;
        PRINT("Hello world %d\n", static_cast<int>(register_e));
        gtl::vm::reg<gtl::vm::rn::general_e>::set(vm, register_e);
    };

    virtual_machine.register_opcode(0x01, custom_opcode);

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 3);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_e>::get(virtual_machine);
    REQUIRE(result == 3, "%d\n", result);
}

TEST(virtual_machine, evaluate, custom_opcode_loop) {
    unsigned char program[] = {
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01
    };

    gtl::virtual_machine virtual_machine(program, sizeof(program));
    constexpr static const gtl::virtual_machine::function_type custom_opcode_1 = [](gtl::virtual_machine& vm_1)->void {
        static_cast<void>(vm_1);
        unsigned char register_e_1 = gtl::vm::reg<gtl::vm::rn::general_e>::get(vm_1);
        register_e_1 += 1;
        PRINT("Level 1, sum: %d\n", static_cast<int>(register_e_1));
        gtl::vm::reg<gtl::vm::rn::general_e>::set(vm_1, register_e_1);

        constexpr static const gtl::virtual_machine::function_type custom_opcode_10 = [](gtl::virtual_machine& vm_10)->void {
            static_cast<void>(vm_10);
            unsigned char register_e_10 = gtl::vm::reg<gtl::vm::rn::general_e>::get(vm_10);
            register_e_10 += 10;
            PRINT("Level 2, sum: %d\n", static_cast<int>(register_e_10));
            gtl::vm::reg<gtl::vm::rn::general_e>::set(vm_10, register_e_10);

            constexpr static const gtl::virtual_machine::function_type custom_opcode_100 = [](gtl::virtual_machine& vm_100)->void {
                static_cast<void>(vm_100);
                unsigned char register_e_100 = gtl::vm::reg<gtl::vm::rn::general_e>::get(vm_100);
                register_e_100 += 100;
                PRINT("Level 3, sum: %d\n", static_cast<int>(register_e_100));
                gtl::vm::reg<gtl::vm::rn::general_e>::set(vm_100, register_e_100);

                vm_100.register_opcode(0x01, custom_opcode_1);
            };

            vm_10.register_opcode(0x01, custom_opcode_100);
        };

        vm_1.register_opcode(0x01, custom_opcode_10);
    };

    virtual_machine.register_opcode(0x01, custom_opcode_1);

    unsigned int successful_ticks = 0;
    while (virtual_machine.tick()) {
        ++successful_ticks;
    }
    REQUIRE(successful_ticks == 6);

    const unsigned char result = gtl::vm::reg<gtl::vm::rn::general_e>::get(virtual_machine);
    REQUIRE(result == 222, "%d\n", result);
}
