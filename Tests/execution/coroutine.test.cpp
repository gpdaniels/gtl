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
#include <data.tests.hpp>
#include <require.tests.hpp>
#include <template.tests.hpp>

#include <execution/coroutine>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(coroutine, traits, standard) {

    #if defined(__APPLE__)
        REQUIRE(sizeof(gtl::coroutine) == 352, "sizeof(gtl::coroutine) = %ld, expected == %lld", sizeof(gtl::coroutine), 352ull);
    #elif defined(__linux__)
        REQUIRE(sizeof(gtl::coroutine) == 576 || sizeof(gtl::coroutine) == 584, "sizeof(gtl::coroutine) = %ld, expected == %lld", sizeof(gtl::coroutine), 576ull);
    #elif defined(_WIN32)
        REQUIRE(sizeof(gtl::coroutine) == 28, "sizeof(gtl::coroutine) = %ld, expected == %lld", sizeof(gtl::coroutine), 28ull);
    #endif

    REQUIRE(std::is_pod<gtl::coroutine>::value == false, "Expected std::is_pod to be false.");

    REQUIRE(std::is_trivial<gtl::coroutine>::value == false, "Expected std::is_trivial to be false.");

    REQUIRE(std::is_trivially_copyable<gtl::coroutine>::value == false, "Expected std::is_trivially_copyable to be false.");

    REQUIRE(std::is_standard_layout<gtl::coroutine>::value == true, "Expected std::is_standard_layout to be true.");
}

TEST(coroutine, constructor, empty) {
    gtl::coroutine coroutine;
    testbench::do_not_optimise_away(coroutine);
}


TEST(coroutine, constructor, lambda) {
    gtl::coroutine coroutine([](){});
    // Ensure coroutine has run.
    coroutine.join();
}

TEST(coroutine, constructor, lamda_argument) {
    testbench::test_template<testbench::test_types>(
        [](auto test_type)->void {
            using type = typename decltype(test_type)::type;
            for (const type& value : testbench::test_data<type>()) {
                gtl::coroutine coroutine([](type){}, value);
                // Ensure coroutine has run.
                coroutine.join();
            }
        }
    );
}

TEST(coroutine, constructor, move) {
    gtl::coroutine coroutine1([](){});
    gtl::coroutine coroutine2(std::move(coroutine1));
    // Ensure coroutine2 has run.
    coroutine2.join();
}

TEST(coroutine, operator, move_assignment) {
    gtl::coroutine coroutine1([](){});
    gtl::coroutine coroutine2;
    coroutine2 = std::move(coroutine1);
    // Ensure coroutine2 has run.
    coroutine2.join();
}

TEST(coroutine, function, get_id) {
    gtl::coroutine coroutine1;
    REQUIRE(coroutine1.get_id() == gtl::coroutine::id{}, "Extected get_id() to be null for an empty coroutine.");
    gtl::coroutine coroutine2 = gtl::coroutine([](){});
    REQUIRE(coroutine2.get_id() != gtl::coroutine::id{}, "Extected get_id() to not be null for a coroutine with an uncalled function.");
    // Ensure coroutine2 has run.
    coroutine2.join();
}

TEST(coroutine, function, joinable) {
    gtl::coroutine coroutine1;
    REQUIRE(coroutine1.joinable() == false, "Extected joinable() to be false for an empty coroutine.");
    gtl::coroutine coroutine2([](){});
    REQUIRE(coroutine2.joinable() == true, "Extected joinable() to be true for a coroutine with an uncalled function.");
    // Ensure coroutine2 has run.
    coroutine2.join();
}

TEST(coroutine, function, join) {
    bool result = false;
    gtl::coroutine coroutine([&result](){
        result = true;
    });
    REQUIRE(result == false, "Expected result to be set to false before coroutine run.");
    coroutine.join();
    REQUIRE(result == true, "Expected result to be set to true after coroutine run.");
}

TEST(coroutine, function, current_get_id) {
    gtl::coroutine::id id;
    gtl::coroutine coroutine([&id](){
        id = gtl::this_coroutine::get_id();
    });
    REQUIRE(coroutine.get_id() != gtl::coroutine::id{}, "Extected get_id() to not be null for a coroutine with an uncalled function.");
    coroutine.join();
    REQUIRE(coroutine.get_id() == id, "Extected get_id() be the same as the global call.");
}

TEST(coroutine, function, current_yield) {
    int result = 0;
    gtl::coroutine coroutine([&result](){
        result = 1;
        gtl::this_coroutine::yield();
        result = 2;
    });
    REQUIRE(result == 0, "Expected result to be set to 0 not '%d' before coroutine run.", result);
    coroutine.join();
    REQUIRE(result == 1, "Expected result to be set to 1 not '%d' after coroutine yield.", result);
    coroutine.join();
    REQUIRE(result == 2, "Expected result to be set to 2 not '%d' after coroutine run.", result);
}

TEST(coroutine, function, current_sleep_for) {
    gtl::coroutine coroutine([](){
        gtl::this_coroutine::sleep_for(std::chrono::milliseconds(100));
    });
    coroutine.join();
}

TEST(coroutine, function, current_sleep_until) {
    gtl::coroutine coroutine([](){
        gtl::this_coroutine::sleep_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(100));
    });
    coroutine.join();
}
