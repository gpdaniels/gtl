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

#include <execution/thread_pool>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(thread_pool, traits, standard) {

#if 0
    // 176, 168, 152, 144, or 112...
    REQUIRE(sizeof(gtl::thread_pool) == 176, "sizeof(gtl::thread_pool) = %ld, expected == %lld", sizeof(gtl::thread_pool), 176ull);
#endif

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
    do_not_optimise_away(thread_pool);
    thread_pool.join();
}

TEST(thread_pool, constructor, number) {
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        do_not_optimise_away(thread_pool);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        do_not_optimise_away(thread_pool);
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
            queue.push([&flags, index](){
                flags[index] = true;
            });
        }

        thread_pool.join();

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%d] == true", i);
        }
    }

    {
        gtl::thread_pool thread_pool = gtl::thread_pool();

        gtl::thread_pool::queue queue(thread_pool);

        constexpr static const unsigned int flag_count = 10;
        bool flags[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            auto temp = [&flags, index](){
                flags[index] = true;
            };
            queue.push(std::move(temp));
        }

        thread_pool.join();

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%d] == true", i);
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
            queue.push([&flags, index](){
                flags[index] = true;
            });
        }

        thread_pool.drain(queue);

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%d] == true", i);
        }
    }

    {
        bool flags[flag_count] = {};
        for (unsigned int i = 0; i < flag_count; ++i) {
            unsigned int index = i;
            queue.push([&flags, index](){
                flags[index] = true;
            });
        }

        thread_pool.join();

        for (unsigned int i = 0; i < flag_count; ++i) {
            REQUIRE(flags[i], "Expected flags[%d] == true", i);
        }
    }
}

TEST(thread_pool, function, joinable) {
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        do_not_optimise_away(thread_pool);
        REQUIRE(thread_pool.joinable() == false);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        do_not_optimise_away(thread_pool);
        REQUIRE(thread_pool.joinable() == true);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
        thread_pool.join();
        REQUIRE(thread_pool.joinable() == false);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        do_not_optimise_away(thread_pool);
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
        do_not_optimise_away(thread_pool);
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(0);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
        thread_pool.join();
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(1);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
        thread_pool.join();
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        do_not_optimise_away(thread_pool);
        thread_pool.join();
    }
    {
        gtl::thread_pool thread_pool = gtl::thread_pool(10);
        do_not_optimise_away(thread_pool);
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
        queue.push([&flags, index](){
            flags[index] = true;
        });
    }

    thread_pool.join();

    for (unsigned int i = 0; i < flag_count; ++i) {
        REQUIRE(flags[i], "Expected flags[%d] == true", i);
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
        queue0.push([&flags, index](){
            flags[index] = false;
        });
        queue1.push([&flags, index](){
            flags[index] = true;
        });
    }

    thread_pool.join();

    for (unsigned int i = 0; i < flag_count; ++i) {
        REQUIRE(flags[i], "Expected flags[%d] == true", i);
    }
}


TEST(thread_pool, evaluate, add_work_from_job) {

    gtl::thread_pool thread_pool = gtl::thread_pool();

    gtl::thread_pool::queue queue(thread_pool);

    constexpr static const unsigned int flag_count = 10;

    bool flags[flag_count] = {};
    for (unsigned int i = 0; i < flag_count; i += 2) {
        unsigned int index = i;
        queue.push([&queue, &flags, index](){
            flags[index] = true;
            unsigned int second_index = index + 1;
            queue.push([&flags, second_index](){
                flags[second_index] = true;
            });
        });
    }

    thread_pool.join();

    for (unsigned int i = 0; i < flag_count; ++i) {
        REQUIRE(flags[i], "Expected flags[%d] == true", i);
    }
}

#define LINKED_TO_LIBDISPATCH 0
#if LINKED_TO_LIBDISPATCH
#   include <dispatch/dispatch.h>
#endif

TEST(thread_pool, evaluate, benchmark) {

    constexpr static const unsigned int flag_count = 1000;
    constexpr static const unsigned int sum_count = 10000;

    static auto work = [](void* values, size_t index) {
        volatile unsigned long long int sum = 0;
        for (unsigned int i = 0; i < sum_count; ++i) {
            sum += 1;
        }
        unsigned long long int sum2 = sum;
        do_not_optimise_away(sum2);

        static_cast<unsigned long long int*>(values)[index] = sum2 - index;
    };

    static auto for_loop_test = []() {
        unsigned long long int* values = new unsigned long long int[flag_count]();

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for (unsigned int index = 0; index < flag_count; ++index) {
            work(values, index);
        }

        for (unsigned long long int i = 0; i < flag_count; ++i) {
            REQUIRE(values[i] == sum_count - i, "Expected values[%lld] == %lld not %lld", i, sum_count - i, values[i]);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] values;

        PRINT("For loop:   %7.3fms\n", (end - start).count() / 1000000.0);
    };

    static auto threadpool_test = []() {
        unsigned long long int* values = new unsigned long long int[flag_count]();

        gtl::thread_pool thread_pool = gtl::thread_pool();

        gtl::thread_pool::queue queue(thread_pool);

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for (unsigned int index = 0; index < flag_count; ++index) {
            queue.push([=](){
                work(values, index);
            });
        }

        thread_pool.drain(queue);

        for (unsigned long long int i = 0; i < flag_count; ++i) {
            REQUIRE(values[i] == sum_count - i, "Expected values[%lld] == %lld not %lld", i, sum_count - i, values[i]);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] values;

        PRINT("Threadpool: %7.3fms\n", (end - start).count() / 1000000.0);

        thread_pool.join();
    };

#if LINKED_TO_LIBDISPATCH
    static auto dispatch_test = [](){
        unsigned long long int* values = new unsigned long long int[flag_count]();

        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        dispatch_apply_f(flag_count, queue, values,
            [](void* values, size_t index) {
                work(values, index);
            }
        );


        for (unsigned long long int i = 0; i < flag_count; ++i) {
            REQUIRE(values[i] == sum_count - i, "Expected values[%lld] == %lld not %lld", i, sum_count - i, values[i]);
        }

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        delete[] values;

        PRINT("Dispatch:   %7.3fms\n", (end - start).count() / 1000000.0);
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
