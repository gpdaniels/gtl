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

#include <testbench/main.tests.hpp>

#include <testbench/optimise.tests.hpp>
#include <testbench/require.tests.hpp>

#include <execution/thread_pool>

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <chrono>
#include <type_traits>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

TEST(thread_pool, traits, standard) {
    REQUIRE((std::is_pod<gtl::thread_pool>::value == false));

    REQUIRE((std::is_trivial<gtl::thread_pool>::value == false));

    REQUIRE((std::is_trivially_copyable<gtl::thread_pool>::value == false));

#if 0
    // Not reliable across compiler / os.
    REQUIRE((std::is_standard_layout<gtl::thread_pool>::value == true));
#endif
}

TEST(thread_pool, constructor, empty) {
    gtl::thread_pool thread_pool;
    testbench::do_not_optimise_away(thread_pool);
    thread_pool.join();
}

TEST(thread_pool, constructor, number) {
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
}

TEST(thread_pool, function, push_job) {
    {
        gtl::thread_pool thread_pool = gtl::thread_pool();

        gtl::thread_pool::queue queue(thread_pool);

        constexpr static const unsigned int flag_count = 10;
        bool flags[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            queue.push([&flags, index]() {
                flags[index] = true;
            });
        }

        thread_pool.join();

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%u] == true", i);
        }
    }

    {
        gtl::thread_pool thread_pool = gtl::thread_pool();

        gtl::thread_pool::queue queue(thread_pool);

        constexpr static const unsigned int flag_count = 10;
        bool flags[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            auto temp = [&flags, index]() {
                flags[index] = true;
            };
            queue.push(std::move(temp));
        }

        thread_pool.join();

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%u] == true", i);
        }
    }
}

TEST(thread_pool, function, drain) {
    gtl::thread_pool thread_pool = gtl::thread_pool();

    gtl::thread_pool::queue queue(thread_pool);

    constexpr static const unsigned int flag_count = 10;

    {
        bool flags[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            queue.push([&flags, index]() {
                flags[index] = true;
            });
        }

        thread_pool.drain(queue);

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%u] == true", i);
        }
    }

    {
        bool flags[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            queue.push([&flags, index]() {
                flags[index] = true;
            });
        }

        thread_pool.join();

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%u] == true", i);
        }
    }
}

TEST(thread_pool, function, joinable) {
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
        REQUIRE(thread_pool.joinable() == false);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        testbench::do_not_optimise_away(thread_pool);
        REQUIRE(thread_pool.joinable() == true);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        testbench::do_not_optimise_away(thread_pool);
        REQUIRE(thread_pool.joinable() == true);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
    }
}

TEST(thread_pool, function, join) {
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
        thread_pool.join();
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
        thread_pool.join();
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        testbench::do_not_optimise_away(thread_pool);
        thread_pool.join();
        thread_pool.join();
        thread_pool.join();
    }
}

TEST(thread_pool, evaluate, work) {
    gtl::thread_pool thread_pool = gtl::thread_pool();

    gtl::thread_pool::queue queue(thread_pool);

    constexpr static const unsigned int flag_count = 10;

    bool flags[flag_count] = {};
    for (unsigned int i = 0; i < flag_count; ++i) {
        unsigned int index = i;
        queue.push([&flags, index]() {
            flags[index] = true;
        });
    }

    thread_pool.join();

    for (unsigned int i = 0; i < flag_count; ++i) {
        REQUIRE(flags[i], "Expected flags[%u] == true", i);
    }
}

TEST(thread_pool, evaluate, priority) {
    gtl::thread_pool thread_pool = gtl::thread_pool(0);

    // Lower value is higher priority.
    gtl::thread_pool::queue queue0(thread_pool, 0);
    gtl::thread_pool::queue queue1(thread_pool, 1);

    constexpr static const unsigned int flag_count = 10;

    bool flags[flag_count] = {};
    for (unsigned int i = 0; i < flag_count; ++i) {
        unsigned int index = i;
        queue0.push([&flags, index]() {
            flags[index] = false;
        });
        queue1.push([&flags, index]() {
            flags[index] = true;
        });
    }

    thread_pool.join();

    for (unsigned int i = 0; i < flag_count; ++i) {
        REQUIRE(flags[i], "Expected flags[%u] == true", i);
    }
}

TEST(thread_pool, evaluate, add_work_from_job) {
    gtl::thread_pool thread_pool = gtl::thread_pool();

    gtl::thread_pool::queue queue(thread_pool);

    constexpr static const unsigned int flag_count = 10;

    bool flags[flag_count] = {};
    for (unsigned int i = 0; i < flag_count; i += 2) {
        unsigned int index = i;
        queue.push([&queue, &flags, index]() {
            flags[index] = true;
            unsigned int second_index = index + 1;
            queue.push([&flags, second_index]() {
                flags[second_index] = true;
            });
        });
    }

    thread_pool.join();

    for (unsigned int i = 0; i < flag_count; ++i) {
        REQUIRE(flags[i], "Expected flags[%u] == true", i);
    }
}

TEST(thread_pool, evaluate, queues_with_same_priority) {
    constexpr static const unsigned int flag_count = 10;

    // Without threads, all the work is done by the join.
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);

        gtl::thread_pool::queue queue0(thread_pool);
        gtl::thread_pool::queue queue1(thread_pool);
        gtl::thread_pool::queue queue2(thread_pool, 0);

        bool flags0[flag_count] = {};
        bool flags1[flag_count] = {};
        bool flags2[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            queue0.push([&flags0, index]() {
                flags0[index] = true;
            });
            queue1.push([&flags1, index]() {
                flags1[index] = true;
            });
            queue2.push([&flags2, index]() {
                flags2[index] = true;
            });
        }

        thread_pool.join();

        REQUIRE(queue0.empty() && queue0.finished());
        REQUIRE(queue1.empty() && queue1.finished());
        REQUIRE(queue2.empty() && queue2.finished());

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags0[i], "Expected flags0[%u] == true", i);
            REQUIRE(flags1[i], "Expected flags1[%u] == true", i);
            REQUIRE(flags2[i], "Expected flags2[%u] == true", i);
        }
    }

    // With threads, the work is done by the threads or by draining.
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(2);

        gtl::thread_pool::queue queue0(thread_pool);
        gtl::thread_pool::queue queue1(thread_pool);

        std::atomic<unsigned int> count0 = 0;
        std::atomic<unsigned int> count1 = 0;
        for (unsigned int i = 0; i < flag_count; ++i) {
            queue0.push([&count0]() {
                ++count0;
            });
            queue1.push([&count1]() {
                ++count1;
            });
        }

        queue0.drain();
        queue1.drain();

        REQUIRE(count0 == flag_count, "Expected count0 == %u not %u", flag_count, count0.load());
        REQUIRE(count1 == flag_count, "Expected count1 == %u not %u", flag_count, count1.load());

        thread_pool.join();
    }
}

TEST(thread_pool, evaluate, queue_destructed_before_thread_pool) {
    constexpr static const unsigned int queue_count = 100;
    constexpr static const unsigned int task_count = 10;

    // Without threads, queues are emptied by draining.
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);

        unsigned int count = 0;
        for (unsigned int i = 0; i < queue_count; ++i) {
            gtl::thread_pool::queue queue(thread_pool);
            for (unsigned int j = 0; j < task_count; ++j) {
                queue.push([&count]() {
                    ++count;
                });
            }
            queue.drain();
        }

        // Use the thread_pool after the queues have been destructed.
        gtl::thread_pool::queue queue(thread_pool);
        queue.push([&count]() {
            ++count;
        });

        thread_pool.join();

        REQUIRE(count == (queue_count * task_count) + 1, "Expected count == %u not %u", (queue_count * task_count) + 1, count);
    }

    // With threads, queues are emptied by draining.
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(3);

        std::atomic<unsigned int> count = 0;
        for (unsigned int i = 0; i < queue_count; ++i) {
            gtl::thread_pool::queue queue(thread_pool);
            for (unsigned int j = 0; j < task_count; ++j) {
                queue.push([&count]() {
                    ++count;
                });
            }
            queue.drain();
        }

        // Use the thread_pool after the queues have been destructed.
        gtl::thread_pool::queue queue(thread_pool);
        queue.push([&count]() {
            ++count;
        });

        thread_pool.join();

        REQUIRE(count == (queue_count * task_count) + 1, "Expected count == %u not %u", (queue_count * task_count) + 1, count.load());
    }

    // With threads, queues are emptied by the threads.
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(3);

        std::atomic<unsigned int> count = 0;
        for (unsigned int i = 0; i < queue_count; ++i) {
            gtl::thread_pool::queue queue(thread_pool);
            for (unsigned int j = 0; j < task_count; ++j) {
                queue.push([&count]() {
                    ++count;
                });
            }
            while (!queue.finished()) {
                std::this_thread::yield();
            }
        }

        // Use the thread_pool after the queues have been destructed.
        gtl::thread_pool::queue queue(thread_pool);
        queue.push([&count]() {
            ++count;
        });

        thread_pool.join();

        REQUIRE(count == (queue_count * task_count) + 1, "Expected count == %u not %u", (queue_count * task_count) + 1, count.load());
    }
}

#define LINKED_TO_LIBDISPATCH 0
#if LINKED_TO_LIBDISPATCH
#include <dispatch/dispatch.h>
#endif

TEST(thread_pool, evaluate, benchmark) {
    constexpr static const unsigned int flag_count = 1000;
    constexpr static const unsigned int sum_count = 10000;

    static auto work = [](void* values, std::size_t index) {
        volatile unsigned long long int sum = 0;
        for (unsigned int i = 0; i < sum_count; ++i) {
            sum += 1;
        }
        unsigned long long int sum2 = sum;
        testbench::do_not_optimise_away(sum2);

        static_cast<unsigned long long int*>(values)[index] = sum2 - index;
    };

    static auto for_loop_test = []() {
        unsigned long long int* values = new unsigned long long int[flag_count]();

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for (unsigned int index = 0; index < flag_count; ++index) {
            work(values, index);
        }

        for (unsigned long long int i = 0; i < flag_count; ++i) {
            REQUIRE(values[i] == sum_count - i, "Expected values[%llu] == %llu not %llu", i, sum_count - i, values[i]);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] values;

        PRINT("For loop:   %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);
    };

    static auto threadpool_test = []() {
        unsigned long long int* values = new unsigned long long int[flag_count]();

        gtl::thread_pool thread_pool = gtl::thread_pool();

        gtl::thread_pool::queue queue(thread_pool);

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for (unsigned int index = 0; index < flag_count; ++index) {
            queue.push([=]() {
                work(values, index);
            });
        }

        thread_pool.drain(queue);

        for (unsigned long long int i = 0; i < flag_count; ++i) {
            REQUIRE(values[i] == sum_count - i, "Expected values[%llu] == %llu not %llu", i, sum_count - i, values[i]);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] values;

        PRINT("Threadpool: %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);

        thread_pool.join();
    };

#if LINKED_TO_LIBDISPATCH
    static auto dispatch_test = []() {
        unsigned long long int* values = new unsigned long long int[flag_count]();

        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        dispatch_apply_f(flag_count, queue, values, [](void* values, std::size_t index) {
            work(values, index);
        });

        for (unsigned long long int i = 0; i < flag_count; ++i) {
            REQUIRE(values[i] == sum_count - i, "Expected values[%lld] == %lld not %lld", i, sum_count - i, values[i]);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] values;

        PRINT("Dispatch:   %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);
    };
#endif

    for_loop_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    for_loop_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    for_loop_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    for_loop_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    for_loop_test();

    std::this_thread::sleep_for(std::chrono::microseconds(10));

    threadpool_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    threadpool_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    threadpool_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    threadpool_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    threadpool_test();

    std::this_thread::sleep_for(std::chrono::microseconds(10));

#if LINKED_TO_LIBDISPATCH
    dispatch_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    dispatch_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    dispatch_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    dispatch_test();
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    dispatch_test();
#endif
}
