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

#include <debug/signal>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#if !defined(SIGUSR1)
    #define SIGUSR1 10
#endif
#if !defined(SIGUSR2)
    #define SIGUSR2 12
#endif

TEST(signal, traits, standard) {
    REQUIRE((std::is_pod<gtl::signal<SIGUSR1>>::value == false));

    REQUIRE((std::is_trivial<gtl::signal<SIGUSR1>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::signal<SIGUSR1>>::value == false));

    #if defined(__clang__) || defined(_WIN32)
        REQUIRE((std::is_standard_layout<gtl::signal<SIGUSR1>>::value == true));
    #else
        REQUIRE((std::is_standard_layout<gtl::signal<SIGUSR1>>::value == false));
    #endif
}

TEST(signal, constructor, empty) {
    gtl::signal<SIGUSR1> signal([](int){});
    testbench::do_not_optimise_away(signal);
}

TEST(signal, evaluate, raise) {
    bool caught = false;

    gtl::signal<SIGTERM> handler1([&caught](int signal_number){
        if (signal_number == SIGTERM) {
            caught = true;
        }
    });

    REQUIRE(!caught);

    std::raise(SIGTERM);

    REQUIRE(caught);
}

TEST(signal, evaluate, raise_two) {
    #if !defined(_WIN32)
        bool caught_usr1 = false;
        bool caught_usr2 = false;

        gtl::signal<SIGUSR1> handler1([&caught_usr1, &caught_usr2](int signal_number){
            if (signal_number == SIGUSR1) {
                caught_usr1 = true;
            }
            if (signal_number == SIGUSR2) {
                caught_usr2 = true;
            }
        });

        REQUIRE(!caught_usr1);
        REQUIRE(!caught_usr2);

        gtl::signal<SIGUSR2> handler2([&caught_usr1, &caught_usr2](int signal_number){
            if (signal_number == SIGUSR1) {
                caught_usr1 = true;
            }
            if (signal_number == SIGUSR2) {
                caught_usr2 = true;
            }
        });

        REQUIRE(!caught_usr1);
        REQUIRE(!caught_usr2);

        std::raise(SIGUSR1);

        REQUIRE(caught_usr1);
        REQUIRE(!caught_usr2);

        std::raise(SIGUSR2);

        REQUIRE(caught_usr1);
        REQUIRE(caught_usr2);
    #endif
}

TEST(signal, evaluate, chain) {
    #if !defined(_WIN32)
        bool caught_handler1 = false;
        bool caught_handler2 = false;

        gtl::signal<SIGUSR1> handler1([&caught_handler1](int signal_number){
            if (signal_number == SIGUSR1) {
                caught_handler1 = true;
            }
        });

        REQUIRE(!caught_handler1);
        REQUIRE(!caught_handler2);

        std::raise(SIGUSR1);

        REQUIRE(caught_handler1);
        REQUIRE(!caught_handler2);

        {
            gtl::signal<SIGUSR1> handler2([&caught_handler2](int signal_number){
                if (signal_number == SIGUSR1) {
                    caught_handler2 = true;
                }
            });

            REQUIRE(caught_handler1);
            REQUIRE(!caught_handler2);

            std::raise(SIGUSR1);

            REQUIRE(caught_handler1);
            REQUIRE(caught_handler2);
        }

        caught_handler1 = false;
        caught_handler2 = false;

        std::raise(SIGUSR1);

        REQUIRE(caught_handler1);
        REQUIRE(!caught_handler2);
    #endif
}
