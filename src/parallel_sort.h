#pragma once
#include <parlay/parallel.h>
#include "sequential_sort.h"

template<typename T>
void parallel_quicksort(parlay::sequence<T>& arr, size_t begin, size_t end, int depth = 0) {
    if (end - begin <= 50000 || depth > 8) {
        sequential_quicksort(arr, begin, end);
        return;
    }

    auto pivot = array_partition(arr, begin, end);

    parlay::par_do(
        [&] { parallel_quicksort(arr, begin, pivot, depth + 1); },
        [&] { parallel_quicksort(arr, pivot, end, depth + 1); }
    );
}

template<typename T>
void parallel_quicksort(parlay::sequence<T>& arr) {
    if (arr.size() <= 1) return;
    parallel_quicksort(arr, 0, arr.size(), 0);
}