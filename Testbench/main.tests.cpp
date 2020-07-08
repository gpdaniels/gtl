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

#include "main.tests.hpp"
#include "comparison.tests.hpp"
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
                    PRINT("Running tests in this process.\n");
                }
                continue;
            }
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

        PRINT("Running [File: %s] [Group: %s] [Test: %s]...\n", current_test->get_file(), current_test->get_group(), current_test->get_name());
        ++TEST_COUNT;
        if (execute_in_process) {
            const char* test_arguments[] = { "--quiet", "--execute", "--file", current_test->get_file(), "--group", current_test->get_group(), "--name", current_test->get_name(), nullptr };
            int result = testbench::launch_process(testbench::get_executable(), test_arguments);
            if (result != 0) {
                ++TEST_FAILURE_COUNT;
            }
        }
        else {
            testbench::REQUIRE_FAILURE_COUNT = 0;
            current_test->get_function()();
            if (testbench::REQUIRE_FAILURE_COUNT > 0) {
                ++TEST_FAILURE_COUNT;
            }
        }
        PRINT("Finished [File: %s] [Group: %s] [Test: %s].\n", current_test->get_file(), current_test->get_group(), current_test->get_name());
    }

    if (!quiet) {
        PRINT("Finished '%lld' tests. Detected '%lld' failures.\n", TEST_COUNT, TEST_FAILURE_COUNT);
    }

    return (TEST_FAILURE_COUNT != 0);
}
