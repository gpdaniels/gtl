/*
Copyright (C) 2018-2022 Geoffrey Daniels. https://gpdaniels.com/

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

    #if 0
        // Not reliable across compiler / os.
        REQUIRE((std::is_standard_layout<gtl::signal<SIGUSR1>>::value == true));
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
