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
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <utility/error>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(error, traits, standard) {
    REQUIRE((std::is_pod<gtl::error>::value == false));

    REQUIRE((std::is_trivial<gtl::error>::value == false));

    #if defined(_MSC_VER)
        #if 0
            // Not reliable across compiler / os.
            REQUIRE((std::is_trivially_copyable<gtl::error>::value == false));
        #endif
    #else
        REQUIRE((std::is_trivially_copyable<gtl::error>::value == true));
    #endif

    REQUIRE((std::is_standard_layout<gtl::error>::value == true));
}

TEST(error, constructor, empty) {
    gtl::error error;
    REQUIRE(error);
}


TEST(error, constructor, enum_class) {
    enum class error_code {
        pass = 0,
        fail = 1
    };
    {
        gtl::error error(error_code::pass);
        REQUIRE(error);
    }
    {
        gtl::error error(error_code::fail);
        REQUIRE(!error);
    }
}

TEST(error, parameter, type) {
    enum class error_code1 {
        pass = 0,
        fail = 1
    };
    enum class error_code2 {
        pass = 0,
        fail = 1
    };
    gtl::error error1(error_code1::pass);
    REQUIRE(error1);
    gtl::error error2(error_code1::fail);
    REQUIRE(!error2);
    gtl::error error3(error_code2::pass);
    REQUIRE(error3);
    gtl::error error4(error_code2::fail);
    REQUIRE(!error4);

    REQUIRE(error1.type == error2.type);
    REQUIRE(error2.type != error3.type);
    REQUIRE(error3.type == error4.type);
}

TEST(error, parameter, value) {
    enum class error_code1 {
        pass = 0,
        fail = 1
    };
    enum class error_code2 {
        pass = 0,
        fail = 1
    };
    gtl::error error1(error_code1::pass);
    REQUIRE(error1);
    gtl::error error2(error_code1::fail);
    REQUIRE(!error2);
    gtl::error error3(error_code2::pass);
    REQUIRE(error3);
    gtl::error error4(error_code2::fail);
    REQUIRE(!error4);

    REQUIRE(error1.value != error2.value);
    REQUIRE(error2.value != error3.value);
    REQUIRE(error3.value != error4.value);

    REQUIRE(error1.value == error3.value);
    REQUIRE(error2.value == error4.value);
}

TEST(error, parameter, name) {
    enum class error_code1 {
        pass = 0,
        fail = 1
    };
    enum class error_code2 {
        pass = 0,
        fail = 1
    };
    gtl::error error1(error_code1::pass);
    REQUIRE(error1);
    gtl::error error2(error_code1::fail);
    REQUIRE(!error2);
    gtl::error error3(error_code2::pass);
    REQUIRE(error3);
    gtl::error error4(error_code2::fail);
    REQUIRE(!error4);

    #if defined(__clang__)
        REQUIRE(testbench::is_string_same(error1.name, "error_code1") == true, "gtl::error.name = '%s', expected '%s'", error1.name, "error_code1");
        REQUIRE(testbench::is_string_same(error2.name, "error_code1") == true, "gtl::error.name = '%s', expected '%s'", error2.name, "error_code1");
        REQUIRE(testbench::is_string_same(error3.name, "error_code2") == true, "gtl::error.name = '%s', expected '%s'", error3.name, "error_code2");
        REQUIRE(testbench::is_string_same(error4.name, "error_code2") == true, "gtl::error.name = '%s', expected '%s'", error4.name, "error_code2");
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
        REQUIRE(testbench::is_string_same(error1.name, "test_error_parameter_name()::error_code1") == true, "gtl::error.name = '%s', expected '%s'", error1.name, "test_error_parameter_name()::error_code1");
        REQUIRE(testbench::is_string_same(error2.name, "test_error_parameter_name()::error_code1") == true, "gtl::error.name = '%s', expected '%s'", error2.name, "test_error_parameter_name()::error_code1");
        REQUIRE(testbench::is_string_same(error3.name, "test_error_parameter_name()::error_code2") == true, "gtl::error.name = '%s', expected '%s'", error3.name, "test_error_parameter_name()::error_code2");
        REQUIRE(testbench::is_string_same(error4.name, "test_error_parameter_name()::error_code2") == true, "gtl::error.name = '%s', expected '%s'", error4.name, "test_error_parameter_name()::error_code2");
    #elif defined(_MSC_VER)
        REQUIRE(testbench::is_string_same(error1.name, "test_error_parameter_name::error_code1") == true, "gtl::error.name = '%s', expected '%s'", error1.name, "test_error_parameter_name::error_code1");
        REQUIRE(testbench::is_string_same(error2.name, "test_error_parameter_name::error_code1") == true, "gtl::error.name = '%s', expected '%s'", error2.name, "test_error_parameter_name::error_code1");
        REQUIRE(testbench::is_string_same(error3.name, "test_error_parameter_name::error_code2") == true, "gtl::error.name = '%s', expected '%s'", error3.name, "test_error_parameter_name::error_code2");
        REQUIRE(testbench::is_string_same(error4.name, "test_error_parameter_name::error_code2") == true, "gtl::error.name = '%s', expected '%s'", error4.name, "test_error_parameter_name::error_code2");
    #endif
}

TEST(error, evaluate, call_stack) {

    enum class error_code1 {
        pass = 0,
        fail = 1
    };

    enum class error_code2 {
        pass = 0,
        fail = 1
    };

    enum class error_code3 {
        pass = 0,
        fail = 1
    };

    static auto function1 = [](bool pass1) -> gtl::error {
        if (pass1) {
            return error_code1::pass;
        }
        return error_code1::fail;
    };

    static auto function2 = [](bool pass1, bool pass2) -> gtl::error {
        if (pass2) {
            return function1(pass1);
        }
        return error_code2::fail;
    };

    static auto function3 = [](bool pass1, bool pass2, bool pass3) -> gtl::error {
        if (pass3) {
            return function2(pass1, pass2);
        }
        return error_code3::fail;
    };

    REQUIRE(function3(false, false, false) == gtl::error(error_code3::fail));
    REQUIRE(function3(false, false,  true) == gtl::error(error_code3::fail));
    REQUIRE(function3(false,  true, false) == gtl::error(error_code3::fail));
    REQUIRE(function3(false,  true,  true) == gtl::error(error_code3::fail));
    REQUIRE(function3( true, false, false) == gtl::error(error_code2::fail));
    REQUIRE(function3( true, false,  true) == gtl::error(error_code2::fail));
    REQUIRE(function3( true,  true, false) == gtl::error(error_code1::fail));
    REQUIRE(function3( true,  true,  true) == gtl::error(error_code1::pass));
}
