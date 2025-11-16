#pragma once
#include <parlay/sequence.h>
#include <algorithm>

template<typename T>
size_t array_partition(parlay::sequence<T>& arr, size_t begin, size_t end) {
    T pivot = arr[begin + (end - begin) / 2];
    size_t i = begin - 1;
    size_t j = end;

    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);

        if (i >= j) return j + 1;
        std::swap(arr[i], arr[j]);
    }
}

template<typename T>
void sequential_quicksort(parlay::sequence<T>& arr, size_t begin, size_t end) {
    while (end - begin > 16) {
        auto pivot = array_partition(arr, begin, end);

        if (pivot - begin < end - pivot) {
            sequential_quicksort(arr, begin, pivot);
            begin = pivot;
        } else {
            sequential_quicksort(arr, pivot, end);
            end = pivot;
        }
    }

    for (size_t i = begin + 1; i < end; i++) {
        T key = arr[i];
        size_t j = i;
        while (j > begin && arr[j-1] > key) {
            arr[j] = arr[j-1];
            j--;
        }
        arr[j] = key;
    }
}

template<typename T>
void sequential_quicksort(parlay::sequence<T>& arr) {
    if (arr.size() <= 1) return;
    sequential_quicksort(arr, 0, arr.size());
}