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
#include <testbench/require.tests.hpp>
#include <testbench/template.tests.hpp>

#include <utility/event>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <functional>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

struct test_event {
    int value;
};

TEST(event, traits, standard) {
    REQUIRE(std::is_pod<gtl::event_manager<test_event>>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::event_manager<test_event>>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::event_manager<test_event>>::value == false, "Expected std::is_trivially_copyable to be false.");

    #if defined(__APPLE__)
        REQUIRE(std::is_standard_layout<gtl::event_manager<test_event>>::value == true, "Expected std::is_standard_layout to be true.");
    #else
        // This test seems to be unreliable between compilers and operating systems.
        //REQUIRE(std::is_standard_layout<gtl::event_manager<test_event>>::value == false, "Expected std::is_standard_layout to be false.");
    #endif
}

class test_class
    : private gtl::event_queue<test_event>
    , private gtl::event_queue<std::function<void(void)>>{
public:
    int value = 0;

private:
    void on_event(test_event& data) override;

    // This line is just to silence a clang warning.
    using gtl::event_queue<std::function<void(void)>>::on_event;

public:
    void process();
};

void test_class::on_event(test_event& data) {
    this->value = data.value;
}

void test_class::process() {
    gtl::event_queue<test_event>::process_events();
    gtl::event_queue<std::function<void(void)>>::process_events();
}

TEST(event, evaluate, emit_and_process_event) {
    test_class test;

    REQUIRE(test.value == 0, "Expected the value in the test_class to be initialised to zero.");

    gtl::event_manager<test_event>::emit(test_event{1});

    REQUIRE(test.value == 0, "Expected the value in the test_class to be zero before processing any events.");

    test.process();

    REQUIRE(test.value == 1, "Expected the value of the event to have been propadated to the test class.");
}

TEST(event, evaluate, emit_and_process_invokable_event) {
    test_class test;

    REQUIRE(test.value == 0, "Expected the value in the test_class to be initialised to zero.");

    gtl::event_manager<std::function<void(void)>>::emit([&test](){
        test.value = 1;
    });

    REQUIRE(test.value == 0, "Expected the value in the test_class to be zero before processing any events.");

    test.process();

    REQUIRE(test.value == 1, "Expected the value of the event to have been propadated to the test class.");
}
