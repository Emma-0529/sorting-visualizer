// SortingAlgorithms.cpp
// Implements Bubble Sort, Merge Sort, and Quick Sort for the project.
// Each function sorts a vector in ascending order and records statistics.

#include "SortingAlgorithms.h"

#include <algorithm> // std::swap
#include <vector>

// Bubble Sort: compare neighboring values and swap any out-of-order pair.
// Stop early if a full pass makes no swaps.
void bubbleSort(std::vector<int>& data, SortStats& stats) {
    stats = {}; // reset counts for this sorting run
    for (std::size_t end = data.size(); end > 1; --end) {
        bool swapped = false;
        for (std::size_t j = 1; j < end; ++j) {
            ++stats.comparisons;
            if (data[j - 1] > data[j]) {
                std::swap(data[j - 1], data[j]);
                ++stats.swaps;
                stats.movements += 2; // two array positions changed
                swapped = true;
            }
        }
        if (!swapped) break; // already sorted
    }
}

// Helper functions are kept private to this .cpp file.
namespace {
// Merge two sorted halves: [left, mid) and [mid, right).
// Count comparisons and copies into/out of the temporary array.
void mergeRange(std::vector<int>& data, std::vector<int>& temp,
                std::size_t left, std::size_t mid, std::size_t right,
                SortStats& stats) {
    std::size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        ++stats.comparisons;
        if (data[i] <= data[j]) temp[k++] = data[i++];
        else temp[k++] = data[j++];
        ++stats.movements;
    }
    while (i < mid) { temp[k++] = data[i++]; ++stats.movements; }
    while (j < right) { temp[k++] = data[j++]; ++stats.movements; }
    for (std::size_t p = left; p < right; ++p) {
        data[p] = temp[p];
        ++stats.movements;
    }
}

// Split the array until each part has at most one element,
// then merge the parts back together in sorted order.
void mergeRecursive(std::vector<int>& data, std::vector<int>& temp,
                    std::size_t left, std::size_t right, SortStats& stats) {
    if (right - left <= 1) return;
    const std::size_t mid = left + (right - left) / 2;
    mergeRecursive(data, temp, left, mid, stats);
    mergeRecursive(data, temp, mid, right, stats);
    mergeRange(data, temp, left, mid, right, stats);
}

// Quick Sort partition (Lomuto method): use the last value as pivot.
// Put values <= pivot on its left and return its final index.
std::size_t partition(std::vector<int>& data, std::size_t low,
                      std::size_t high, SortStats& stats) {
    const int pivot = data[high];
    std::size_t i = low;
    for (std::size_t j = low; j < high; ++j) {
        ++stats.comparisons;
        if (data[j] <= pivot) {
            if (i != j) {
                std::swap(data[i], data[j]);
                ++stats.swaps;
                stats.movements += 2;
            }
            ++i;
        }
    }
    if (i != high) {
        std::swap(data[i], data[high]);
        ++stats.swaps;
        stats.movements += 2;
    }
    return i;
}

// Sort the partitions around the pivot.
// Recursing on the smaller side helps keep stack usage low.
void quickRecursive(std::vector<int>& data, std::size_t low,
                    std::size_t high, SortStats& stats) {
    // Iterate over the larger partition to limit recursion depth.
    while (low < high) {
        const std::size_t pivot = partition(data, low, high, stats);
        const std::size_t leftSize = pivot - low;
        const std::size_t rightSize = high - pivot;
        if (leftSize < rightSize) {
            if (pivot > low) quickRecursive(data, low, pivot - 1, stats);
            low = pivot + 1;
        } else {
            if (pivot < high) quickRecursive(data, pivot + 1, high, stats);
            if (pivot == 0) break;
            high = pivot - 1;
        }
    }
}
} // namespace

// Public Merge Sort function: create temporary storage and start recursion.
void mergeSort(std::vector<int>& data, SortStats& stats) {
    stats = {};
    std::vector<int> temp(data.size());
    mergeRecursive(data, temp, 0, data.size(), stats);
}

// Public Quick Sort function: skip recursion for empty or single-item arrays.
void quickSort(std::vector<int>& data, SortStats& stats) {
    stats = {};
    if (data.size() > 1) quickRecursive(data, 0, data.size() - 1, stats);
}
