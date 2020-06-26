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
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <algorithm/quicksort>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <random>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(quicksort, traits, standard) {
    REQUIRE((std::is_pod<gtl::quicksort>::value == true));

    REQUIRE((std::is_trivial<gtl::quicksort>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::quicksort>::value == true));

    REQUIRE((std::is_standard_layout<gtl::quicksort>::value == true));
}

TEST(quicksort, function, sort_0) {
    std::array<int, 0> data_original = {};
    std::array<int, 0> data_sorted = data_original;
    std::sort(data_sorted.begin(), data_sorted.end());

    std::array<int, 0> data = data_original;
    gtl::quicksort::sort(data.data(), data.size());

    REQUIRE(data == data_sorted);
}

TEST(quicksort, function, sort_1) {
    {
        std::array<int, 1> data_original = { 0 };
        std::array<int, 1> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 1> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 1> data_original = { 10 };
        std::array<int, 1> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 1> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 1> data_original = { -10 };
        std::array<int, 1> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 1> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
}

TEST(quicksort, function, sort_2) {
    {
        std::array<int, 2> data_original = { 0, 0 };
        std::array<int, 2> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 2> data_original = { 1, 0 };
        std::array<int, 2> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 2> data_original = { 0, -1 };
        std::array<int, 2> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 2> data_original = { 1, 2 };
        std::array<int, 2> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
}

TEST(quicksort, function, sort_3) {
    std::array<int, 3> data_original = { 0, 1, -1 };
    std::array<int, 3> data_sorted = data_original;
    std::sort(data_sorted.begin(), data_sorted.end());

    std::array<int, 3> data = data_original;
    gtl::quicksort::sort(data.data(), data.size());

    REQUIRE(data == data_sorted);
}

TEST(quicksort, function, sort_10_sorted) {
    std::array<int, 10> data_original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::array<int, 10> data_sorted = data_original;
    std::sort(data_sorted.begin(), data_sorted.end());

    std::array<int, 10> data = data_original;
    gtl::quicksort::sort(data.data(), data.size());

    REQUIRE(data == data_sorted);
}

TEST(quicksort, function, sort_10_some_same) {
    {
        std::array<int, 10> data_original = { 0, 1, 2, 5, 5, 5, 5, 7, 8, 9 };
        std::array<int, 10> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 10> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 10> data_original = { 5, 0, 1, 2, 5, 5, 7, 8, 9, 5 };
        std::array<int, 10> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 10> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
    {
        std::array<int, 10> data_original = { 5, 5, 0, 1, 2, 7, 8, 9, 5, 5 };
        std::array<int, 10> data_sorted = data_original;
        std::sort(data_sorted.begin(), data_sorted.end());

        std::array<int, 10> data = data_original;
        gtl::quicksort::sort(data.data(), data.size());

        REQUIRE(data == data_sorted);
    }
}

TEST(quicksort, function, sort_10_random) {
    std::array<int, 10> data_original = { 0, 1, 3, 9, 4, 6, 7, 8, 5, 2 };
    std::array<int, 10> data_sorted = data_original;
    std::sort(data_sorted.begin(), data_sorted.end());

    std::array<int, 10> data = data_original;
    gtl::quicksort::sort(data.data(), data.size());

    REQUIRE(data == data_sorted);
}

TEST(quicksort, function, sort_10_random_decending) {
    std::array<int, 10> data_original = { 0, 1, 3, 9, 4, 6, 7, 8, 5, 2 };
    std::array<int, 10> data_sorted = data_original;
    std::sort(data_sorted.begin(), data_sorted.end(), [](const int& lhs, const int& rhs){ return lhs > rhs; });

    std::array<int, 10> data = data_original;
    gtl::quicksort::sort(data.data(), data.size(), gtl::quicksort::comparitor_descending<int>);

    REQUIRE(data == data_sorted);
}

TEST(quicksort, function, sort_10_random_indexed) {
    std::array<int, 10> data_original = { 0, 1, 3, 9, 4, 6, 7, 8, 5, 2 };
    std::array<int, 10> data_sorted = data_original;
    std::array<unsigned long long int, 10> data_sorted_indexes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::sort(data_sorted_indexes.begin(), data_sorted_indexes.end(), [data_sorted](const unsigned long long int& lhs, const unsigned long long int& rhs){ return data_sorted[lhs] < data_sorted[rhs]; });

    std::array<int, 10> data = data_original;
    std::array<unsigned long long int, 10> data_indexes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    gtl::quicksort::sort(data_indexes.data(), data_indexes.size(), [data](const unsigned long long int& lhs, const unsigned long long int& rhs){ return data[lhs] < data[rhs]; });

    REQUIRE(data_indexes == data_sorted_indexes);
}

TEST(quicksort, function, partial_sort_0) {
    unsigned int partial_length = 0;
    std::array<int, 0> data_original = {};
    std::array<int, 0> data_sorted = data_original;
    std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

    std::array<int, 0> data = data_original;
    gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

    REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
}

TEST(quicksort, function, partial_sort_1) {
    {
        unsigned int partial_length = 1;
        std::array<int, 1> data_original = { 0 };
        std::array<int, 1> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 1> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), 1);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 1;
        std::array<int, 1> data_original = { 10 };
        std::array<int, 1> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 1> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 1;
        std::array<int, 1> data_original = { -10 };
        std::array<int, 1> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 1> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
}

TEST(quicksort, function, partial_sort_2) {
    {
        unsigned int partial_length = 1;
        std::array<int, 2> data_original = { 0, 0 };
        std::array<int, 2> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 1;
        std::array<int, 2> data_original = { 1, 0 };
        std::array<int, 2> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 1;
        std::array<int, 2> data_original = { 0, -1 };
        std::array<int, 2> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 1;
        std::array<int, 2> data_original = { 1, 2 };
        std::array<int, 2> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 2> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
}

TEST(quicksort, function, partial_sort_3) {
    unsigned int partial_length = 2;
    std::array<int, 3> data_original = { 0, 1, -1 };
    std::array<int, 3> data_sorted = data_original;
    std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

    std::array<int, 3> data = data_original;
    gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

    REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
}

TEST(quicksort, function, partial_sort_10_sorted) {
    unsigned int partial_length = 5;
    std::array<int, 10> data_original = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::array<int, 10> data_sorted = data_original;
    std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

    std::array<int, 10> data = data_original;
    gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

    REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
}

TEST(quicksort, function, partial_sort_10_some_same) {
    {
        unsigned int partial_length = 5;
        std::array<int, 10> data_original = { 0, 1, 2, 5, 5, 5, 5, 7, 8, 9 };
        std::array<int, 10> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 10> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 5;
        std::array<int, 10> data_original = { 5, 0, 1, 2, 5, 5, 7, 8, 9, 5 };
        std::array<int, 10> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 10> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
    {
        unsigned int partial_length = 5;
        std::array<int, 10> data_original = { 5, 5, 0, 1, 2, 7, 8, 9, 5, 5 };
        std::array<int, 10> data_sorted = data_original;
        std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

        std::array<int, 10> data = data_original;
        gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

        REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
    }
}

TEST(quicksort, function, partial_sort_10_random) {
    unsigned int partial_length = 5;
    std::array<int, 10> data_original = { 0, 1, 3, 9, 4, 6, 7, 8, 5, 2 };
    std::array<int, 10> data_sorted = data_original;
    std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end());

    std::array<int, 10> data = data_original;
    gtl::quicksort::sort_partial(data.data(), data.size(), partial_length);

    REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
}

TEST(quicksort, function, partial_sort_10_random_decending) {
    unsigned int partial_length = 5;
    std::array<int, 10> data_original = { 0, 1, 3, 9, 4, 6, 7, 8, 5, 2 };
    std::array<int, 10> data_sorted = data_original;
    std::partial_sort(data_sorted.begin(), data_sorted.begin() + partial_length, data_sorted.end(), [](const int& lhs, const int& rhs){ return lhs > rhs; });

    std::array<int, 10> data = data_original;
    gtl::quicksort::sort_partial(data.data(), data.size(), partial_length, gtl::quicksort::comparitor_descending<int>);

    REQUIRE(testbench::is_memory_same(data.data(), data_sorted.data(), partial_length));
}

TEST(quicksort, function, partial_sort_10_random_indexed) {
    unsigned int partial_length = 5;
    std::array<int, 10> data_original = { 0, 1, 3, 9, 4, 6, 7, 8, 5, 2 };
    std::array<int, 10> data_sorted = data_original;
    std::array<unsigned long long int, 10> data_sorted_indexes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::partial_sort(data_sorted_indexes.begin(), data_sorted_indexes.begin() + partial_length, data_sorted_indexes.end(), [data_sorted](const unsigned long long int& lhs, const unsigned long long int& rhs){ return data_sorted[lhs] < data_sorted[rhs]; });

    std::array<int, 10> data = data_original;
    std::array<unsigned long long int, 10> data_indexes = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    gtl::quicksort::sort_partial(data_indexes.data(), data_indexes.size(), partial_length, [data](const unsigned long long int& lhs, const unsigned long long int& rhs){ return data[lhs] < data[rhs]; });

    REQUIRE(testbench::is_memory_same(data_indexes.data(), data_sorted_indexes.data(), partial_length));
}

TEST(quicksort, evaluate, benchmark_sort) {
    constexpr static const auto create_random_data = [](unsigned long long int size)->std::vector<int> {
        std::random_device entropy;
        std::seed_seq      seed{entropy(), entropy(), entropy(), entropy(), entropy(), entropy(), entropy(), entropy()};
        std::mt19937       engine(seed);
        std::uniform_int_distribution<int> distribution;
        std::vector<int> random_data(size);
        std::generate(random_data.begin(), random_data.end(), bind(distribution, engine));
        return random_data;
    };

    static const auto standard = [](const std::vector<int>& random_data){
        for (int i = 0; i < 5; ++i) {
            std::vector<int> data = random_data;
            std::sort(data.begin(), data.end());
        }
        std::vector<int> data = random_data;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::sort(data.begin(), data.end());
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        PRINT("Standard:    %7.3fms\n", (end - start).count() / 1000000.0);
    };

    static const auto quicksort = [](const std::vector<int>& random_data){
        for (int i = 0; i < 5; ++i) {
            std::vector<int> data = random_data;
            gtl::quicksort::sort(data.data(), data.size());
        }
        std::vector<int> data = random_data;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        gtl::quicksort::sort(data.data(), data.size());
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        PRINT("Quicksort:   %7.3fms\n", (end - start).count() / 1000000.0);
    };

    for (unsigned long long int size = 0; size < (1 << 18); size = (!size) + (size << 4)) {
        std::vector<int> random_data = create_random_data(size);
        PRINT("Array size: %llu\n", static_cast<unsigned long long int>(random_data.size()));
        standard(random_data);
        quicksort(random_data);
    }
}

TEST(quicksort, evaluate, benchmark_partial) {
    constexpr static const auto create_random_data = [](unsigned long long int size)->std::vector<int> {
        std::random_device entropy;
        std::seed_seq      seed{entropy(), entropy(), entropy(), entropy(), entropy(), entropy(), entropy(), entropy()};
        std::mt19937       engine(seed);
        std::uniform_int_distribution<int> distribution;
        std::vector<int> random_data(size);
        std::generate(random_data.begin(), random_data.end(), bind(distribution, engine));
        return random_data;
    };

    static const auto standard = [](const std::vector<int>& random_data){
        for (int i = 0; i < 5; ++i) {
            std::vector<int> data = random_data;
            std::sort(data.begin(), data.end());
        }
        std::vector<int> data = random_data;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        std::partial_sort(data.begin(), data.begin() + data.size() / 2, data.end());
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        PRINT("Partial Standard:    %7.3fms\n", (end - start).count() / 1000000.0);
    };

    static const auto quicksort = [](const std::vector<int>& random_data){
        for (int i = 0; i < 5; ++i) {
            std::vector<int> data = random_data;
            gtl::quicksort::sort(data.data(), data.size());
        }
        std::vector<int> data = random_data;
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        gtl::quicksort::sort_partial(data.data(), data.size(), data.size() / 2);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        PRINT("Partial Quicksort:   %7.3fms\n", (end - start).count() / 1000000.0);
    };

    for (unsigned long long int size = 0; size < (1 << 18); size = (!size) + (size << 4)) {
        std::vector<int> random_data = create_random_data(size);
        PRINT("Array size: %llu\n", static_cast<unsigned long long int>(random_data.size()));
        standard(random_data);
        quicksort(random_data);
    }
}
