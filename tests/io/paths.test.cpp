/*
Copyright (C) 2018-2023 Geoffrey Daniels. https://gpdaniels.com/

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

#include <io/paths>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(paths, traits, standard) {
    #if defined(__clang__)
        REQUIRE((std::is_pod<gtl::paths>::value == false));
        REQUIRE((std::is_trivial<gtl::paths>::value == false));
        REQUIRE((std::is_trivially_copyable<gtl::paths>::value == false));
    #elif (defined(__GNUC__) || defined(__GNUG__)) && (!defined(__clang__) && (!defined(__INTEL_COMPILER)))
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

TEST(paths, function, get_executable_path) {
    std::string executable_path;
    REQUIRE(gtl::paths::get_executable_path(executable_path));
    PRINT("executable_path: %s\n", executable_path.c_str());
}

TEST(paths, function, get_executable_name) {
    std::string executable_name;
    REQUIRE(gtl::paths::get_executable_name(executable_name));
    PRINT("executable_name: %s\n", executable_name.c_str());
}

TEST(paths, function, get_executable_directory) {
    std::string executable_directory;
    REQUIRE(gtl::paths::get_executable_directory(executable_directory));
    PRINT("executable_directory: %s\n", executable_directory.c_str());
}

TEST(paths, function, get_directory_contents) {
    std::vector<std::string> directory_contents = gtl::paths::get_directory_contents(".");
    PRINT("directory_contents size: %zu\n", directory_contents.size());
}

TEST(paths, function, get_current_working_directory) {
    std::string current_working_directory;
    REQUIRE(gtl::paths::get_current_working_directory(current_working_directory));
    PRINT("current_working_directory: %s\n", current_working_directory.c_str());
}

TEST(paths, function, get_home_directory) {
    std::string home_directory;
    REQUIRE(gtl::paths::get_home_directory(home_directory));
    PRINT("home_directory: %s\n", home_directory.c_str());
}

TEST(paths, function, get_application_data_directory) {
    std::string application_data_directory;
    REQUIRE(gtl::paths::get_application_data_directory(application_data_directory));
    PRINT("application_data_directory: %s\n", application_data_directory.c_str());
}

TEST(paths, evaluate, print_all) {
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
