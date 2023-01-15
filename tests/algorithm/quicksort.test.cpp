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
#include <comparison.tests.hpp>
#include <require.tests.hpp>

#include <algorithm/quicksort>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <random>
#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(quicksort, traits, standard) {
    // Different on windows.
    //REQUIRE((std::is_pod<gtl::quicksort>::value == true));

    // Different on windows.
    //REQUIRE((std::is_trivial<gtl::quicksort>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::quicksort>::value == true));

    REQUIRE((std::is_standard_layout<gtl::quicksort>::value == true));
}

TEST(quicksort, function, sort_0) {
    int* data = nullptr;
    gtl::quicksort::sort(data, 0);
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
    int* data = nullptr;
    gtl::quicksort::sort_partial(data, 0, partial_length);
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
        std::generate(random_data.begin(), random_data.end(), std::bind(distribution, engine));
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
        PRINT("Standard:    %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);
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
        PRINT("Quicksort:   %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);
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
        std::generate(random_data.begin(), random_data.end(), std::bind(distribution, engine));
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
        PRINT("Partial Standard:    %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);
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
        PRINT("Partial Quicksort:   %7.3fms\n", static_cast<double>((end - start).count()) / 1000000.0);
    };

    for (unsigned long long int size = 0; size < (1 << 18); size = (!size) + (size << 4)) {
        std::vector<int> random_data = create_random_data(size);
        PRINT("Array size: %llu\n", static_cast<unsigned long long int>(random_data.size()));
        standard(random_data);
        quicksort(random_data);
    }
}
