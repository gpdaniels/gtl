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

#include "main.tests.hpp"

#include "comparison.tests.hpp"
#include "debugger.tests.hpp"
#include "print.tests.hpp"
#include "process.tests.hpp"
#include "require.tests.hpp"
#include "sanitizers.tests.hpp"

int main(int argument_count, char* arguments[]) {
    // Disable output buffering.
    testbench::disable_output_buffering(testbench::output_stream::output);
    testbench::disable_output_buffering(testbench::output_stream::error);

    bool help = false;
    bool quiet = false;
    bool list = false;
    const char* filter_file = nullptr;
    const char* filter_group = nullptr;
    const char* filter_name = nullptr;
    bool execute_in_process = true;

    // Process the command line arguments:
    for (int argument_index = 0; argument_index < argument_count; ++argument_index) {
        const int arguments_remaining = argument_count - argument_index;

        // Extract a help flag from the command line arguments.
        if (arguments_remaining >= 1) {
            if (testbench::is_string_same("-?", arguments[argument_index]) || testbench::is_string_same("--help", arguments[argument_index])) {
                help = true;
                continue;
            }
        }

        // Extract a quiet flag from the command line arguments.
        if (arguments_remaining >= 1) {
            if (testbench::is_string_same("-q", arguments[argument_index]) || testbench::is_string_same("--quiet", arguments[argument_index])) {
                quiet = true;
                continue;
            }
        }

        // Extract a list flag from the command line arguments.
        if (arguments_remaining >= 1) {
            if (testbench::is_string_same("-l", arguments[argument_index]) || testbench::is_string_same("--list", arguments[argument_index])) {
                list = true;
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

        // Extract an execute flag from the command line arguments.
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

    if (help) {
        PRINT("Command line options:\n");
        PRINT("  -? --help    : Print this help message (Overrides all other arguments).\n");
        PRINT("  -q --quiet   : Reduce logging output.\n");
        PRINT("  -l --list    : List tests, respects -fgn filter arguments.\n");
        PRINT("  -f --file    : Filter tests by name.\n");
        PRINT("  -g --group   : Filter tests by group.\n");
        PRINT("  -n --name    : Filter tests by name.\n");
        PRINT("  -x --execute : Execute tests in this process rather than isolated (Enabled if a debugger is detected).\n");
        return 0;
    }

    if (list) {
        PRINT("All tests:\n");
        const testbench::test_node* last_file = nullptr;
        const testbench::test_node* last_group = nullptr;
        const testbench::test_node* last_name = nullptr;
        for (const testbench::test_node* current_test = testbench::test_node::root; current_test != nullptr; current_test = current_test->next) {
            if (filter_file && !testbench::is_string_same(filter_file, current_test->file)) {
                continue;
            }
            if (filter_group && !testbench::is_string_same(filter_group, current_test->group)) {
                continue;
            }
            if (filter_name && !testbench::is_string_same(filter_name, current_test->name)) {
                continue;
            }
            if (!last_file || !testbench::is_string_same(last_file->file, current_test->file)) {
                last_file = current_test;
                PRINT("  File: %s:\n", current_test->file);
            }
            if (!last_group || !testbench::is_string_same(last_group->group, current_test->group)) {
                last_group = current_test;
                PRINT("    Group: %s:\n", current_test->group);
            }
            if (!last_name || !testbench::is_string_same(last_name->name, current_test->group)) {
                last_name = current_test;
                PRINT("      Name: %s:\n", current_test->name);
            }
        }
        return 0;
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

    for (const testbench::test_node* current_test = testbench::test_node::root; current_test != nullptr; current_test = current_test->next) {
        if (filter_file && !testbench::is_string_same(filter_file, current_test->file)) {
            continue;
        }
        if (filter_group && !testbench::is_string_same(filter_group, current_test->group)) {
            continue;
        }
        if (filter_name && !testbench::is_string_same(filter_name, current_test->name)) {
            continue;
        }

        if (!quiet) {
            PRINT("Launching [File: %s] [Group: %s] [Test: %s]...\n", current_test->file, current_test->group, current_test->name);
        }

        ++TEST_COUNT;
        if (execute_in_process) {
            const char* test_arguments[] = { "--quiet", "--execute", "--file", current_test->file, "--group", current_test->group, "--name", current_test->name, nullptr };
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
            current_test->function();
            if (testbench::REQUIRE_FAILURE_COUNT > 0) {
                ++TEST_FAILURE_COUNT;
            }
            if (!quiet) {
                PRINT("REQUIRES: [TOTAL: %llu] [PASSED: %llu] [FAILED: %llu].\n", testbench::REQUIRE_COUNT, testbench::REQUIRE_COUNT - testbench::REQUIRE_FAILURE_COUNT, testbench::REQUIRE_FAILURE_COUNT);
            }
        }
        if (!quiet) {
            PRINT("Finishing [File: %s] [Group: %s] [Test: %s].\n", current_test->file, current_test->group, current_test->name);
        }
    }

    if (!quiet) {
        PRINT("Finishing '%llu' tests. Detected '%llu' failures.\n", TEST_COUNT, TEST_FAILURE_COUNT);
    }

    return (TEST_FAILURE_COUNT != 0);
}
