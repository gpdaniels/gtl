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

#include "main.tests.hpp"
#include "comparison.tests.hpp"
#include "debugger.tests.hpp"
#include "print.tests.hpp"
#include "process.tests.hpp"
#include "require.tests.hpp"
#include "sanitizers.tests.hpp"
#include "unused.tests.hpp"

int main(int argument_count, char* arguments[]) {

    // Disable output buffering.
    testbench::disable_output_buffering(testbench::output_stream::output);
    testbench::disable_output_buffering(testbench::output_stream::error);

    bool quiet = false;
    const char* filter_file = nullptr;
    const char* filter_group = nullptr;
    const char* filter_name = nullptr;
    bool execute_in_process = true;

    // Process the command line arguments:
    for (int argument_index = 0; argument_index < argument_count; ++argument_index) {
        const int arguments_remaining = argument_count - argument_index;

        // Extract a quiet flag from the command line arguments.
        if (arguments_remaining >= 1) {
            if (testbench::is_string_same("-q", arguments[argument_index]) || testbench::is_string_same("--quiet", arguments[argument_index])) {
                quiet = true;
                continue;
            }
        }

        // Extract a test group filter from the command line arguments.
        if (arguments_remaining >= 2) {
            if (testbench::is_string_same("-f", arguments[argument_index]) || testbench::is_string_same("--file", arguments[argument_index])) {
                filter_file = arguments[++argument_index];
                if (!quiet) {
                    PRINT("Filtering tests by file: %s\n", filter_file);
                }
                continue;
            }
        }

        // Extract a test group filter from the command line arguments.
        if (arguments_remaining >= 2) {
            if (testbench::is_string_same("-g", arguments[argument_index]) || testbench::is_string_same("--group", arguments[argument_index])) {
                filter_group = arguments[++argument_index];
                if (!quiet) {
                    PRINT("Filtering tests by group: %s\n", filter_group);
                }
                continue;
            }
        }

        // Extract a test name filter from the command line arguments.
        if (arguments_remaining >= 2) {
            if (testbench::is_string_same("-n", arguments[argument_index]) || testbench::is_string_same("--name", arguments[argument_index])) {
                filter_name = arguments[++argument_index];
                if (!quiet) {
                    PRINT("Filtering tests by name: %s\n", filter_name);
                }
                continue;
            }
        }

        // Extract a test name filter from the command line arguments.
        if (arguments_remaining >= 1) {
            if (testbench::is_string_same("-x", arguments[argument_index]) || testbench::is_string_same("--execute", arguments[argument_index])) {
                execute_in_process = false;
                if (!quiet) {
                    PRINT("Launching tests in this process.\n");
                }
                continue;
            }
        }
    }

    if (testbench::is_debugger_attached()) {
        execute_in_process = false;
        if (!quiet) {
            PRINT("Debugger detected: Launching tests in this process.\n");
        }
    }

    // Notifiy if any sanitizers are running.
    #if GTL_HAS_SANITIZER_ADDRESS
        if (!quiet) {
            PRINT("Testing under address sanitizer.\n");
        }
    #endif

    #if GTL_HAS_SANITIZER_MEMORY
        if (!quiet) {
            PRINT("Testing under memory sanitizer.\n");
        }
    #endif

    #if GTL_HAS_SANITIZER_THREAD
        if (!quiet) {
            PRINT("Testing under thread sanitizer.\n");
        }
    #endif

    #if GTL_HAS_SANITIZER_UNDEFINED_BEHAVIOR
        if (!quiet) {
            PRINT("Testing under undefined behavior sanitizer.\n");
        }
    #endif

    if (!quiet) {
        PRINT("Starting tests...\n");
    }
    
    unsigned long long TEST_COUNT = 0;
    unsigned long long TEST_FAILURE_COUNT = 0;

    testbench::test_node::reverse();

    for (const testbench::test_node* current_test = testbench::test_node::get_root(); current_test != nullptr; current_test = current_test->get_next()) {
        if (filter_file && !testbench::is_string_same(filter_file, current_test->get_file())) {
            continue;
        }
        if (filter_group && !testbench::is_string_same(filter_group, current_test->get_group())) {
            continue;
        }
        if (filter_name && !testbench::is_string_same(filter_name, current_test->get_name())) {
            continue;
        }

        if (!quiet) {
            PRINT("Launching [File: %s] [Group: %s] [Test: %s]...\n", current_test->get_file(), current_test->get_group(), current_test->get_name());
        }

        ++TEST_COUNT;
        if (execute_in_process) {
            const char* test_arguments[] = { "--quiet", "--execute", "--file", current_test->get_file(), "--group", current_test->get_group(), "--name", current_test->get_name(), nullptr };
            int result = testbench::launch_process(testbench::get_executable(), test_arguments);
            if (result != 0) {
                ++TEST_FAILURE_COUNT;
                if (!quiet) {
                    PRINT("FAILURE.\n");
                }
            }
        }
        else {
            testbench::REQUIRE_COUNT = 0;
            testbench::REQUIRE_FAILURE_COUNT = 0;
            current_test->get_function()();
            if (testbench::REQUIRE_FAILURE_COUNT > 0) {
                ++TEST_FAILURE_COUNT;
            }
            if (!quiet) {
                PRINT("REQUIRES: [TOTAL: %lld] [PASSED: %lld] [FAILED: %lld].\n", testbench::REQUIRE_COUNT, testbench::REQUIRE_COUNT - testbench::REQUIRE_FAILURE_COUNT, testbench::REQUIRE_FAILURE_COUNT);
            }
        }
        if (!quiet) {
            PRINT("Finishing [File: %s] [Group: %s] [Test: %s].\n", current_test->get_file(), current_test->get_group(), current_test->get_name());
        }
    }

    if (!quiet) {
        PRINT("Finishing '%lld' tests. Detected '%lld' failures.\n", TEST_COUNT, TEST_FAILURE_COUNT);
    }

    return (TEST_FAILURE_COUNT != 0);
}
