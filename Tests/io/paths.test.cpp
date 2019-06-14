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
#include <require.tests.hpp>

#include <io/paths>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(traits, standard) {
    REQUIRE(sizeof(gtl::paths) > 0);

    #if defined(__clang__)
        REQUIRE((std::is_pod<gtl::paths>::value == false));
        REQUIRE((std::is_trivial<gtl::paths>::value == false));
        REQUIRE((std::is_trivially_copyable<gtl::paths>::value == false));
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__INTEL_COMPILER))
        REQUIRE((std::is_pod<gtl::paths>::value == true));
        REQUIRE((std::is_trivial<gtl::paths>::value == true));
        REQUIRE((std::is_trivially_copyable<gtl::paths>::value == true));
    #elif defined(_MSC_VER)
        REQUIRE((std::is_pod<gtl::paths>::value == false));
        REQUIRE((std::is_trivial<gtl::paths>::value == false));
        REQUIRE((std::is_trivially_copyable<gtl::paths>::value == false));
    #endif

    REQUIRE((std::is_standard_layout<gtl::paths>::value == true));
}

TEST(function, get_executable_path) {
    std::string executable_path;
    REQUIRE(gtl::paths::get_executable_path(executable_path));
    PRINT("executable_path: %s\n", executable_path.c_str());
}

TEST(function, get_executable_name) {
    std::string executable_name;
    REQUIRE(gtl::paths::get_executable_name(executable_name));
    PRINT("executable_name: %s\n", executable_name.c_str());
}

TEST(function, get_executable_directory) {
    std::string executable_directory;
    REQUIRE(gtl::paths::get_executable_directory(executable_directory));
    PRINT("executable_directory: %s\n", executable_directory.c_str());
}

TEST(function, get_directory_contents) {
    std::vector<std::string> directory_contents = gtl::paths::get_directory_contents(".");
    PRINT("directory_contents size: %zu\n", directory_contents.size());
}

TEST(function, get_current_working_directory) {
    std::string current_working_directory;
    REQUIRE(gtl::paths::get_current_working_directory(current_working_directory));
    PRINT("current_working_directory: %s\n", current_working_directory.c_str());
}

TEST(function, get_home_directory) {
    std::string home_directory;
    REQUIRE(gtl::paths::get_home_directory(home_directory));
    PRINT("home_directory: %s\n", home_directory.c_str());
}

TEST(function, get_application_data_directory) {
    std::string application_data_directory;
    REQUIRE(gtl::paths::get_application_data_directory(application_data_directory));
    PRINT("application_data_directory: %s\n", application_data_directory.c_str());
}

TEST(evaluation, print_all) {
    std::string executable_path;
    REQUIRE(gtl::paths::get_executable_path(executable_path));
    PRINT("executable_path:             %s\n", executable_path.c_str());

    std::string executable_name;
    REQUIRE(gtl::paths::get_executable_name(executable_name));
    PRINT("executable_name:             %s\n", executable_name.c_str());

    std::string executable_directory;
    REQUIRE(gtl::paths::get_executable_directory(executable_directory));
    PRINT("executable_directory:        %s\n", executable_directory.c_str());

    std::vector<std::string> directory_contents = gtl::paths::get_directory_contents(".");
    PRINT("directory_contents size:     %zu\n", directory_contents.size());

    std::string current_working_directory;
    REQUIRE(gtl::paths::get_current_working_directory(current_working_directory));
    PRINT("current_working_directory:   %s\n", current_working_directory.c_str());

    std::string home_directory;
    REQUIRE(gtl::paths::get_home_directory(home_directory));
    PRINT("home_directory:              %s\n", home_directory.c_str());

    std::string application_data_directory;
    REQUIRE(gtl::paths::get_application_data_directory(application_data_directory));
    PRINT("application_data_directory:  %s\n", application_data_directory.c_str());
}
