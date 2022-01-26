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
#include <optimise.tests.hpp>
#include <require.tests.hpp>

#include <container/lambda>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(lambda, traits, standard) {
    REQUIRE((std::is_pod<gtl::lambda<void()>>::value == false));

    REQUIRE((std::is_trivial<gtl::lambda<void()>>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::lambda<void()>>::value == false));

    REQUIRE((std::is_standard_layout<gtl::lambda<void()>>::value == true));
}

TEST(lambda, constructor, empty) {
    gtl::lambda<void()> lambda;
    testbench::do_not_optimise_away(lambda);
}

TEST(lambda, constructor, function) {
    gtl::lambda<void()> lambda([](){});
    testbench::do_not_optimise_away(lambda);
}

TEST(lambda, constructor, capturing_function) {
    {
        int variable = 123;
        gtl::lambda<void()> lambda([=](){
            UNUSED(variable == 123);
        });
        testbench::do_not_optimise_away(lambda);
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        gtl::lambda<void()> lambda([&](){
            UNUSED(variable == 123);
            variable = 321;
            UNUSED(variable == 321);
        });
        testbench::do_not_optimise_away(lambda);
        REQUIRE(variable == 123);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        gtl::lambda<void()> lambda([variable1, variable2, variable3](){
            UNUSED(variable1 == 123);
            UNUSED(variable2 == 234);
            UNUSED(variable3 > 345.677f);
            UNUSED(variable3 < 345.679f);
        });
        testbench::do_not_optimise_away(lambda);
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
}

TEST(lambda, constructor, copying) {
    gtl::lambda<void()> static_lambda1([](){});
    testbench::do_not_optimise_away(static_lambda1);
    const gtl::lambda<void()> static_lambda2(static_lambda1);
    testbench::do_not_optimise_away(static_lambda2);
    const gtl::lambda<void()> static_lambda3(static_lambda2);
    testbench::do_not_optimise_away(static_lambda3);
}

TEST(lambda, constructor, moving) {
    gtl::lambda<void()> static_lambda1([](){});
    testbench::do_not_optimise_away(static_lambda1);
    gtl::lambda<void()> static_lambda2(static_cast<gtl::lambda<void()>&&>(static_lambda1));
    testbench::do_not_optimise_away(static_lambda2);
}

TEST(lambda, operator, copying) {
    auto function = [](){};
    gtl::lambda<void()> static_lambda1 = function;
    testbench::do_not_optimise_away(static_lambda1);
    gtl::lambda<void()> static_lambda2 = static_lambda1;
    testbench::do_not_optimise_away(static_lambda2);
}

TEST(lambda, operator, moving) {
    auto function = [](){};
    gtl::lambda<void()> static_lambda1 = static_cast<decltype(function)&&>(function);
    testbench::do_not_optimise_away(static_lambda1);
    gtl::lambda<void()> static_lambda2 = static_cast<gtl::lambda<void()>&&>(static_lambda1);
    testbench::do_not_optimise_away(static_lambda2);
}

TEST(lambda, operator, bool) {
    gtl::lambda<void()> lambda;
    REQUIRE(lambda == false);
    lambda = gtl::lambda<void()>([](){});
    REQUIRE(lambda == true);
}

TEST(lambda, operator, executing) {
    {
        gtl::lambda<void()> lambda([](){});
        lambda();
    }
    {
        auto function = [](){};
        gtl::lambda<void()> lambda(function);
        lambda();
    }
    {
        int variable = 123;
        gtl::lambda<void()> lambda([=](){
            REQUIRE(variable == 123);
        });
        lambda();
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        gtl::lambda<void()> lambda([&](){
            REQUIRE(variable == 123);
            variable = 321;
            REQUIRE(variable == 321);
        });
        lambda();
        REQUIRE(variable == 321);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        gtl::lambda<void()> lambda([variable1, variable2, variable3](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 234);
            REQUIRE(variable3 > 345.677f);
            REQUIRE(variable3 < 345.679f);
        });
        lambda();
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
    {
        int variable1 = 123;
        int variable2 = 234;
        gtl::lambda<void()> lambda([variable1, &variable2](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 567);
            PRINT("Print inside lambda with two variables %d and %d.\n", variable1, variable2);
            variable2 = 890;
        });
        variable1 = 456;
        variable2 = 567;
        lambda();
        REQUIRE(variable1 == 456);
        REQUIRE(variable2 == 890);
    }
}

TEST(lambda, operator, executing_with_arguments) {
    {
        bool flag = false;
        gtl::lambda<void(bool&)> lambda([](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
        });
        lambda(flag);
        REQUIRE(flag);
    }
    {
        auto function = [](bool& flag){
            REQUIRE(!flag);
            flag = true;
            REQUIRE(flag);
        };
        bool flag = false;
        gtl::lambda<void(bool&)> lambda(function);
        lambda(flag);
        REQUIRE(flag);
    }
    {
        int variable = 123;
        bool flag = false;
        gtl::lambda<void(bool&)> lambda([=](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable == 123);
        });
        lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        bool flag = false;
        gtl::lambda<void(bool&)> lambda([&](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable == 123);
            variable = 321;
            REQUIRE(variable == 321);
        });
        lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable == 321);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        bool flag = false;
        gtl::lambda<void(bool&)> lambda([variable1, variable2, variable3](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 234);
            REQUIRE(variable3 > 345.677f);
            REQUIRE(variable3 < 345.679f);
        });
        lambda(flag);
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
        gtl::lambda<void(bool&)> lambda([variable1, &variable2](bool& flag_argument){
            REQUIRE(!flag_argument);
            flag_argument = true;
            REQUIRE(flag_argument);
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 567);
            PRINT("Print inside lambda with two variables %d and %d.\n", variable1, variable2);
            variable2 = 890;
        });
        variable1 = 456;
        variable2 = 567;
        lambda(flag);
        REQUIRE(flag);
        REQUIRE(variable1 == 456);
        REQUIRE(variable2 == 890);
    }
}

TEST(lambda, operator, executing_with_return) {
    {
        gtl::lambda<bool()> lambda([](){
            return true;
        });
        REQUIRE(lambda());
    }
    {
        auto function = []()->bool{
            return true;
        };
        gtl::lambda<bool()> lambda(function);
        REQUIRE(lambda());
    }
    {
        int variable = 123;
        gtl::lambda<bool()> lambda([=]()->bool{
            REQUIRE(variable == 123);
            return true;
        });
        REQUIRE(lambda());
        REQUIRE(variable == 123);
    }

    {
        int variable = 123;
        gtl::lambda<bool()> lambda([&](){
            REQUIRE(variable == 123);
            variable = 321;
            REQUIRE(variable == 321);
            return true;
        });
        REQUIRE(lambda());
        REQUIRE(variable == 321);
    }

    {
        int variable1 = 123;
        unsigned char variable2 = 234;
        float variable3 = 345.678f;
        gtl::lambda<bool()> lambda([variable1, variable2, variable3](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 234);
            REQUIRE(variable3 > 345.677f);
            REQUIRE(variable3 < 345.679f);
            return true;
        });
        REQUIRE(lambda());
        REQUIRE(variable1 == 123);
        REQUIRE(variable2 == 234);
        REQUIRE(variable3 > 345.677f);
        REQUIRE(variable3 < 345.679f);
    }
    {
        int variable1 = 123;
        int variable2 = 234;
        gtl::lambda<bool()> lambda([variable1, &variable2](){
            REQUIRE(variable1 == 123);
            REQUIRE(variable2 == 567);
            PRINT("Print inside lambda with two variables %d and %d.\n", variable1, variable2);
            variable2 = 890;
            return true;
        });
        variable1 = 456;
        variable2 = 567;
        REQUIRE(lambda());
        REQUIRE(variable1 == 456);
        REQUIRE(variable2 == 890);
    }
}

TEST(lambda, evaluate, construction_and_destruction) {
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

            gtl::lambda<void()> lambda(function);

            REQUIRE(constructed == 1);
            REQUIRE(copied == 1);
            REQUIRE(moved == 0);
            REQUIRE(operated == 0);
            REQUIRE(destructed == 0);

            lambda();

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

            gtl::lambda<void()> lambda(static_cast<function_type&&>(function));

            REQUIRE(constructed == 1);
            REQUIRE(copied == 0);
            REQUIRE(moved == 1);
            REQUIRE(operated == 0);
            REQUIRE(destructed == 0);

            lambda();

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
