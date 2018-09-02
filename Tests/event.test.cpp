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
#include <macro.tests.hpp>
#include <template.tests.hpp>

#include <event>

#include <functional>
#include <type_traits>

struct test_event {
    int value;
};

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::event_manager<test_event>) >= 4, "sizeof(gtl::event_manager) = %ld, expected >= %lld", sizeof(gtl::event_manager<test_event>), 1ull);

    REQUIRE(std::is_pod<gtl::event_manager<test_event>>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::event_manager<test_event>>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::event_manager<test_event>>::value == false, "Expected std::is_trivially_copyable to be false.");

#if defined(__clang__)
    REQUIRE(std::is_standard_layout<gtl::event_manager<test_event>>::value == true, "Expected std::is_standard_layout to be true.");
#else
    REQUIRE(std::is_standard_layout<gtl::event_manager<test_event>>::value == false, "Expected std::is_standard_layout to be false.");
#endif
}

class test_class
    : private gtl::event_queue<test_event>
    , private gtl::event_queue<std::function<void(void)>>{
public:
    int value;

public:
    test_class(void);

private:
    virtual void on_event(test_event& data) override;

    // This line is just to silence a clang warning.
    using gtl::event_queue<std::function<void(void)>>::on_event;

public:
    void process();
};

test_class::test_class(void)
    : value(0) {
}

void test_class::on_event(test_event& data) {
    this->value = data.value;
}

void test_class::process() {
    gtl::event_queue<test_event>::process_events();
    gtl::event_queue<std::function<void(void)>>::process_events();
}

TEST(evaluation, emit_and_process_event) {
    test_class test;

    REQUIRE(test.value == 0, "Expected the value in the test_class to be initialised to zero.");

    gtl::event_manager<test_event>::emit(test_event{1});

    REQUIRE(test.value == 0, "Expected the value in the test_class to be zero before processing any events.");

    test.process();

    REQUIRE(test.value == 1, "Expected the value of the event to have been propadated to the test class.");
}

TEST(evaluation, emit_and_process_invokable_event) {
    test_class test;

    REQUIRE(test.value == 0, "Expected the value in the test_class to be initialised to zero.");

    gtl::event_manager<std::function<void(void)>>::emit([&test](){
        test.value = 1;
    });

    REQUIRE(test.value == 0, "Expected the value in the test_class to be zero before processing any events.");

    test.process();

    REQUIRE(test.value == 1, "Expected the value of the event to have been propadated to the test class.");
}
