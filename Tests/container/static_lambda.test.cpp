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

#include <container/static_lambda>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(static_lambda, traits, standard) {
    REQUIRE((std::is_pod<gtl::static_lambda<void(), 128>>::value == false));

    REQUIRE((std::is_trivial<gtl::static_lambda<void(), 128>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::static_lambda<void(), 128>>::value == false));

    REQUIRE((std::is_standard_layout<gtl::static_lambda<void(), 128>>::value == true));
}

TEST(static_lambda, constructor, empty) {
    gtl::static_lambda<void(), 128> static_lambda;
    testbench::do_not_optimise_away(static_lambda);
}

TEST(static_lambda, constructor, function)
{
    gtl::static_lambda<void(), 128> static_lambda([](){});
    testbench::do_not_optimise_away(static_lambda);
}

TEST(static_lambda, constructor, capturing_function)
{
    {
        int variable = 123;
        gtl::static_lambda<void(), 128> static_lambda([=](){
            UNUSED(variable == 123);
        });
        testbench::do_not_optimise_away(static_lambda);
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        gtl::static_lambda<void(), 128> static_lambda([&](){
            UNUSED(variable == 123);
            variable = 321;
            UNUSED(variable == 321);
        });
        testbench::do_not_optimise_away(static_lambda);
        REQUIRE(variable == 123);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        gtl::static_lambda<void(), 128> static_lambda([variable1, variable2, variable3](){
            UNUSED(variable1 == 123);
            UNUSED(variable2 == 234);
            UNUSED(variable3 > 345.677f);
            UNUSED(variable3 < 345.679f);
        });
        testbench::do_not_optimise_away(static_lambda);
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
}

TEST(static_lambda, constructor, copying)
{
    gtl::static_lambda<void(), 128> static_lambda1([](){});
    testbench::do_not_optimise_away(static_lambda1);
    const gtl::static_lambda<void(), 128> static_lambda2(static_lambda1);
    testbench::do_not_optimise_away(static_lambda2);
    const gtl::static_lambda<void(), 128> static_lambda3(static_lambda2);
    testbench::do_not_optimise_away(static_lambda3);
}

TEST(static_lambda, constructor, moving)
{
    gtl::static_lambda<void(), 128> static_lambda1([](){});
    testbench::do_not_optimise_away(static_lambda1);
    gtl::static_lambda<void(), 128> static_lambda2(std::move(static_lambda1));
    testbench::do_not_optimise_away(static_lambda2);
}

TEST(static_lambda, operator, copying)
{
    gtl::static_lambda<void(), 128> static_lambda1([](){});
    testbench::do_not_optimise_away(static_lambda1);
    gtl::static_lambda<void(), 128> static_lambda2 = static_lambda1;
    testbench::do_not_optimise_away(static_lambda2);
}

TEST(static_lambda, operator, moving)
{
    gtl::static_lambda<void(), 128> static_lambda1([](){});
    testbench::do_not_optimise_away(static_lambda1);
    gtl::static_lambda<void(), 128> static_lambda2 = std::move(static_lambda1);
    testbench::do_not_optimise_away(static_lambda2);
}

TEST(static_lambda, operator, bool)
{
    gtl::static_lambda<void(), 128> static_lambda;
    REQUIRE(static_lambda == false);
    static_lambda = gtl::static_lambda<void(), 128>([](){});
    REQUIRE(static_lambda == true);
}

TEST(static_lambda, operator, executing)
{
    {
        gtl::static_lambda<void(), 128> static_lambda([](){});
        static_lambda();
    }
    {
        auto lambda = [](){};
        gtl::static_lambda<void(), 128> static_lambda(lambda);
        static_lambda();
    }
    {
        int variable = 123;
        gtl::static_lambda<void(), 128> static_lambda([=](){
            REQUIRE(variable == 123);
        });
        static_lambda();
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        gtl::static_lambda<void(), 128> static_lambda([&](){
            REQUIRE(variable == 123);
            variable = 321;
            REQUIRE(variable == 321);
        });
        static_lambda();
        REQUIRE(variable == 321);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        gtl::static_lambda<void(), 128> static_lambda([variable1, variable2, variable3](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 234);
            REQUIRE(variable3 > 345.677f);
            REQUIRE(variable3 < 345.679f);
        });
        static_lambda();
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
    {
        int variable1 = 123;
        int variable2 = 234;
        gtl::static_lambda<void(), 128> static_lambda([variable1, &variable2](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 567);
            PRINT("Print inside static_lambda with two variables %d and %d.\n", variable1, variable2);
            variable2 = 890;
        });
        variable1 = 456;
        variable2 = 567;
        static_lambda();
        REQUIRE(variable1 == 456);
        REQUIRE(variable2 == 890);
    }
}

TEST(static_lambda, operator, executing_with_arguments)
{
    {
        bool flag = false;
        gtl::static_lambda<void(bool&), 128> static_lambda([](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
        });
        static_lambda(flag);
        REQUIRE(flag);
    }
    {
        auto lambda = [](bool& flag){
            REQUIRE(!flag);
            flag = true;
            REQUIRE(flag);
        };
        bool flag = false;
        gtl::static_lambda<void(bool&), 128> static_lambda(lambda);
        static_lambda(flag);
        REQUIRE(flag);
    }
    {
        int variable = 123;
        bool flag = false;
        gtl::static_lambda<void(bool&), 128> static_lambda([=](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable == 123);
        });
        static_lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        bool flag = false;
        gtl::static_lambda<void(bool&), 128> static_lambda([&](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable == 123);
            variable = 321;
            REQUIRE(variable == 321);
        });
        static_lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable == 321);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        bool flag = false;
        gtl::static_lambda<void(bool&), 128> static_lambda([variable1, variable2, variable3](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 234);
            REQUIRE(variable3 > 345.677f);
            REQUIRE(variable3 < 345.679f);
        });
        static_lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
    {
        int variable1 = 123;
        int variable2 = 234;
        bool flag = false;
        gtl::static_lambda<void(bool&), 128> static_lambda([variable1, &variable2](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 567);
            PRINT("Print inside static_lambda with two variables %d and %d.\n", variable1, variable2);
            variable2 = 890;
        });
        variable1 = 456;
        variable2 = 567;
        static_lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable1 == 456);
        REQUIRE(variable2 == 890);
    }
}

TEST(static_lambda, operator, executing_with_return)
{
    {
        gtl::static_lambda<bool(), 128> static_lambda([](){
            return true;
        });
        REQUIRE(static_lambda());
    }
    {
        auto lambda = []()->bool{
            return true;
        };
        gtl::static_lambda<bool(), 128> static_lambda(lambda);
        REQUIRE(static_lambda());
    }
    {
        int variable = 123;
        gtl::static_lambda<bool(), 128> static_lambda([=]()->bool{
            REQUIRE(variable == 123);
            return true;
        });
        REQUIRE(static_lambda());
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        gtl::static_lambda<bool(), 128> static_lambda([&](){
            REQUIRE(variable == 123);
            variable = 321;
            REQUIRE(variable == 321);
            return true;
        });
        REQUIRE(static_lambda());
        REQUIRE(variable == 321);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        gtl::static_lambda<bool(), 128> static_lambda([variable1, variable2, variable3](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 234);
            REQUIRE(variable3 > 345.677f);
            REQUIRE(variable3 < 345.679f);
            return true;
        });
        REQUIRE(static_lambda());
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
    {
        int variable1 = 123;
        int variable2 = 234;
        gtl::static_lambda<bool(), 128> static_lambda([variable1, &variable2](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 567);
            PRINT("Print inside static_lambda with two variables %d and %d.\n", variable1, variable2);
            variable2 = 890;
            return true;
        });
        variable1 = 456;
        variable2 = 567;
        REQUIRE(static_lambda());
        REQUIRE(variable1 == 456);
        REQUIRE(variable2 == 890);
    }
}

TEST(static_lambda, evaluation, construction_and_destruction)
{
    static int constructed = 0;
    static int copied = 0;
    static int moved = 0;
    static int operated = 0;
    static int destructed = 0;

    struct function_type {
        function_type() {
            ++constructed;
        }
        function_type(const function_type&) {
            ++copied;
        }
        function_type(function_type&&) {
            ++moved;
        }
        function_type& operator=(const function_type&) = delete;
        function_type& operator=(function_type&&) = delete;
        void operator()() const {
            ++operated;
        }
        ~function_type() {
            ++destructed;
        }
    };

    REQUIRE(constructed == 0);
    {
        REQUIRE(constructed == 0);
        function_type function;
        REQUIRE(constructed == 1);
        {
            REQUIRE(constructed == 1);
            REQUIRE(copied == 0);
            REQUIRE(moved == 0);
            REQUIRE(operated == 0);
            REQUIRE(destructed == 0);

            gtl::static_lambda<void(), 128> static_lambda(function);

            REQUIRE(constructed == 1);
            REQUIRE(copied == 1);
            REQUIRE(moved == 0);
            REQUIRE(operated == 0);
            REQUIRE(destructed == 0);

            static_lambda();

            REQUIRE(constructed == 1);
            REQUIRE(copied == 1);
            REQUIRE(moved == 0);
            REQUIRE(operated == 1);
            REQUIRE(destructed == 0);
        }
        REQUIRE(destructed == 1);
    }
    REQUIRE(destructed == 2);

    constructed = 0;
    copied = 0;
    moved = 0;
    operated = 0;
    destructed = 0;

    REQUIRE(constructed == 0);
    {
        REQUIRE(constructed == 0);
        function_type function;
        REQUIRE(constructed == 1);
        {
            REQUIRE(constructed == 1);
            REQUIRE(copied == 0);
            REQUIRE(moved == 0);
            REQUIRE(operated == 0);
            REQUIRE(destructed == 0);

            gtl::static_lambda<void(), 128> static_lambda(std::move(function));

            REQUIRE(constructed == 1);
            REQUIRE(copied == 0);
            REQUIRE(moved == 1);
            REQUIRE(operated == 0);
            REQUIRE(destructed == 0);

            static_lambda();

            REQUIRE(constructed == 1);
            REQUIRE(copied == 0);
            REQUIRE(moved == 1);
            REQUIRE(operated == 1);
            REQUIRE(destructed == 0);
        }
        REQUIRE(destructed == 1);
    }
    REQUIRE(destructed == 2);
}
