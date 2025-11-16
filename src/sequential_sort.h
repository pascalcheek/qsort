#pragma once
#include <parlay/sequence.h>
#include <algorithm>

template<typename T>
size_t array_partition(parlay::sequence<T>& arr, size_t begin, size_t end) {
    T pivot = arr[end - 1];
    size_t i = begin;

    for (size_t j = begin; j < end - 1; j++) {
        if (arr[j] <= pivot) {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    std::swap(arr[i], arr[end - 1]);
    return i;
}

template<typename T>
void sequential_quicksort(parlay::sequence<T>& arr, size_t begin, size_t end) {
    if (end - begin <= 1) return;

    auto pivot = array_partition(arr, begin, end);
    sequential_quicksort(arr, begin, pivot);
    sequential_quicksort(arr, pivot + 1, end);
}

template<typename T>
void sequential_quicksort(parlay::sequence<T>& arr) {
    if (arr.size() <= 1) return;
    sequential_quicksort(arr, 0, arr.size());
}