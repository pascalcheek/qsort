#include <iostream>
#include <chrono>
#include <parlay/sequence.h>
#include <parlay/random.h>
#include <parlay/primitives.h>

#include "src/sequential_sort.h"
#include "src/parallel_sort.h"

bool verify_sorted(const parlay::sequence<long>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i-1]) return false;
    }
    return true;
}

void run_edge_case_tests() {
    std::cout << "RUNNING EDGE CASE TESTS\n";
    int passed = 0;
    int total = 0;

    {
        total++;
        parlay::sequence<long> empty;
        sequential_quicksort(empty);
        parallel_quicksort(empty);
        if (empty.empty()) passed++;
    }

    {
        total++;
        parlay::sequence<long> single = {42};
        sequential_quicksort(single);
        parallel_quicksort(single);
        if (single.size() == 1 && single[0] == 42) passed++;
    }

    {
        total++;
        parlay::sequence<long> sorted = {1, 2, 3, 4, 5};
        auto sorted_copy = sorted;
        sequential_quicksort(sorted);
        parallel_quicksort(sorted_copy);
        if (verify_sorted(sorted) && verify_sorted(sorted_copy)) passed++;
    }

    {
        total++;
        parlay::sequence<long> reversed = {5, 4, 3, 2, 1};
        auto reversed_copy = reversed;
        sequential_quicksort(reversed);
        parallel_quicksort(reversed_copy);
        if (verify_sorted(reversed) && verify_sorted(reversed_copy)) passed++;
    }

    {
        total++;
        parlay::sequence<long> same = {7, 7, 7, 7, 7};
        auto same_copy = same;
        sequential_quicksort(same);
        parallel_quicksort(same_copy);
        if (verify_sorted(same) && verify_sorted(same_copy)) passed++;
    }


    std::cout << "Edge cases: " << passed << "/" << total << " passed\n\n";
}

bool validate_sorting(const parlay::sequence<long>& original, const parlay::sequence<long>& sorted) {
    if (original.size() != sorted.size()) return false;
    return verify_sorted(sorted);
}

void run_correctness_tests() {
    std::cout << "RUNNING CORRECTNESS TESTS\n";

    parlay::random_generator rng(42);
    int passed_sequential = 0;
    int passed_parallel = 0;
    const int total_tests = 300;

    for (int test_num = 0; test_num < total_tests; test_num++) {
        auto size_rng = rng[test_num];
        size_t array_size = 10 + (size_rng() % 100000);

        int max_value = 100000;
        if (test_num % 4 == 0) max_value = 100;
        else if (test_num % 4 == 1) max_value = 1000;
        else if (test_num % 4 == 2) max_value = 10000;
        else max_value = array_size;

        auto test_array = parlay::tabulate(array_size, [&](long idx) {

            auto elem_rng = rng[test_num * 10000 + idx];
            return (long)(elem_rng() % max_value);
        });

        auto seq_copy = test_array;
        sequential_quicksort(seq_copy);
        bool seq_correct = validate_sorting(test_array, seq_copy);
        if (seq_correct) passed_sequential++;

        auto par_copy = test_array;
        parallel_quicksort(par_copy);
        bool par_correct = validate_sorting(test_array, par_copy);
        if (par_correct) passed_parallel++;

        if ((test_num + 1) % 50 == 0) {
            std::cout << "Completed " << (test_num + 1) << " tests" << std::endl;
        }
    }

    std::cout << "\nSequential sort: " << passed_sequential << "/" << total_tests << " passed\n";
    std::cout << "Parallel sort: " << passed_parallel << "/" << total_tests << " passed\n\n";
}

void run_performance_test() {
    const long array_size = 100000000;
    const int num_trials = 5;

    std::cout << "RUNNING PERFORMANCE TEST\n";
    parlay::random_generator rng(12345);
    auto test_dataset = parlay::tabulate(array_size, [&](long idx) {
        auto random_num = rng[idx];
        return (long)(random_num() % array_size);
    });

    std::cout << "Sequential execution:\n";
    double seq_total_time = 0.0;

    for (int trial = 0; trial < num_trials; trial++) {
        auto data_copy = test_dataset;
        auto start_moment = std::chrono::high_resolution_clock::now();
        sequential_quicksort(data_copy);
        auto end_moment = std::chrono::high_resolution_clock::now();

        double elapsed = std::chrono::duration<double>(end_moment - start_moment).count();
        seq_total_time += elapsed;

        std::cout << "Trial " << (trial + 1) << ": " << elapsed << "s" << "\n";
    }

    double seq_avg_time = seq_total_time / num_trials;
    std::cout << "Sequential average: " << seq_avg_time << "s\n";

    std::cout << "Parallel execution:\n";
    double par_total_time = 0.0;

    for (int trial = 0; trial < num_trials; trial++) {
        auto data_copy = test_dataset;
        auto start_moment = std::chrono::high_resolution_clock::now();
        parallel_quicksort(data_copy);
        auto end_moment = std::chrono::high_resolution_clock::now();

        double elapsed = std::chrono::duration<double>(end_moment - start_moment).count();
        par_total_time += elapsed;

        std::cout << "Trial " << (trial + 1) << ": " << elapsed << "s" << "\n";
    }

    double par_avg_time = par_total_time / num_trials;
    std::cout << "Parallel average: " << par_avg_time << "s\n";

    std::cout << "PERFORMANCE RESULTS\n";
    std::cout << "Seq: " << seq_avg_time << "s\n";
    std::cout << "Par: " << par_avg_time << "s\n";

    if (par_avg_time > 0) {
        double speed_ratio = seq_avg_time / par_avg_time;
        std::cout << "Speed ratio: " << speed_ratio << "x\n";
    }
}

int main() {
    run_edge_case_tests();
    run_correctness_tests();
    run_performance_test();

    return 0;
}