// SortingAlgorithms.cpp
// Implements Bubble Sort, Merge Sort, and Quick Sort for the project.
// Each function sorts a vector in ascending order and records statistics.

#include "SortingAlgorithms.h"

#include <algorithm> // std::swap, std::sort
#include <vector>
#include "raylib.h"

// Bubble Sort: compare neighboring values and swap any out-of-order pair.
// Stop early if a full pass makes no swaps.
void bubbleSort(SortState& state, SortStats& stats) {
    state.isSorting = true;
    state.skipAnimation = false;
    stats = {}; // reset counts for this sorting run
    auto& data = state.array;
    for (std::size_t end = data.size(); end > 1; --end) {
        bool swapped = false;
        for (std::size_t j = 1; j < end; ++j) {
            if (WindowShouldClose() || state.skipAnimation) break;
            ++stats.comparisons;
            renderStep(state, static_cast<int>(j - 1), static_cast<int>(j), 0.10f);
            if (state.skipAnimation) break;
            if (data[j - 1] > data[j]) {
                animateSwap(state, static_cast<int>(j - 1), static_cast<int>(j), 0.40f);
                if (state.skipAnimation) break;
                std::swap(data[j - 1], data[j]);
                ++stats.swaps;
                stats.movements += 2; // two array positions changed
                swapped = true;
            }
        }
        if (state.skipAnimation) {
            std::sort(data.begin(), data.end()); // Instantly finish sorting on skip
            break;
        }
        if (!swapped) break; // already sorted
    }
    state.activeIndex1 = -1;
    state.activeIndex2 = -1;
    state.isSorting = false;
    state.skipAnimation = false;
    state.isSorted = true;
}

// Helper functions are kept private to this .cpp file.
namespace {
// Merge two sorted halves: [left, mid) and [mid, right).
// Count comparisons and copies into/out of the temporary array.
void mergeRange(SortState& state, std::vector<int>& temp,
                std::size_t left, std::size_t mid, std::size_t right,
                SortStats& stats) {
    auto& data = state.array;
    std::size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (WindowShouldClose() || state.skipAnimation) return;
        ++stats.comparisons;
        renderStep(state, static_cast<int>(i), static_cast<int>(j), 0.10f);
        if (state.skipAnimation) return;
        if (data[i] <= data[j]) temp[k++] = data[i++];
        else temp[k++] = data[j++];
        ++stats.movements;
    }
    while (i < mid) { 
        if (WindowShouldClose() || state.skipAnimation) return;
        temp[k++] = data[i++]; 
        ++stats.movements; 
    }
    while (j < right) { 
        if (WindowShouldClose() || state.skipAnimation) return;
        temp[k++] = data[j++]; 
        ++stats.movements; 
    }
    for (std::size_t p = left; p < right; ++p) {
        if (WindowShouldClose() || state.skipAnimation) return;
        data[p] = temp[p];
        ++stats.movements;

        renderStep(state, static_cast<int>(p), -1, 0.08f);
        if (state.skipAnimation) return;
    }
}

// Split the array until each part has at most one element,
// then merge the parts back together in sorted order.
void mergeRecursive(SortState& state, std::vector<int>& temp,
                    std::size_t left, std::size_t right, SortStats& stats) {
    if (state.skipAnimation) {
        std::sort(state.array.begin(), state.array.end());
        return;
    }
    if (right - left <= 1 || WindowShouldClose()) return;
    const std::size_t mid = left + (right - left) / 2;
    mergeRecursive(state, temp, left, mid, stats);
    if (state.skipAnimation) return;
    mergeRecursive(state, temp, mid, right, stats);
    if (state.skipAnimation) return;
    mergeRange(state, temp, left, mid, right, stats);
}

// Quick Sort partition (Lomuto method): use the last value as pivot.
// Put values <= pivot on its left and return its final index.
std::size_t partition(SortState& state, std::size_t low,
                      std::size_t high, SortStats& stats) {
    auto& data = state.array;
    const int pivot = data[high];
    std::size_t i = low;
    for (std::size_t j = low; j < high; ++j) {
        if (WindowShouldClose() || state.skipAnimation) break;
        ++stats.comparisons;
        renderStep(state, static_cast<int>(j), static_cast<int>(high), 0.10f);
        if (state.skipAnimation) break;
        if (data[j] <= pivot) {
            if (i != j) {
                animateSwap(state, static_cast<int>(i), static_cast<int>(j), 0.45f);
                if (state.skipAnimation) break;
                std::swap(data[i], data[j]);
                ++stats.swaps;
                stats.movements += 2;
            }
            ++i;
        }
    }
    if (state.skipAnimation) {
        std::sort(data.begin(), data.end());
        return high;
    }
    if (i != high) {
        animateSwap(state, static_cast<int>(i), static_cast<int>(high), 0.40f);
        if (!state.skipAnimation) {
            std::swap(data[i], data[high]);
            ++stats.swaps;
            stats.movements += 2;
        }
    }
    return i;
}

// Sort the partitions around the pivot.
// Recursing on the smaller side helps keep stack usage low.
void quickRecursive(SortState& state, std::size_t low,
                    std::size_t high, SortStats& stats) {
    // Iterate over the larger partition to limit recursion depth.
    while (low < high && !WindowShouldClose()) {
        if (state.skipAnimation) {
            std::sort(state.array.begin(), state.array.end());
            break;
        }
        const std::size_t pivot = partition(state, low, high, stats);
        if (state.skipAnimation) {
            std::sort(state.array.begin(), state.array.end());
            break;
        }
        const std::size_t leftSize = pivot - low;
        const std::size_t rightSize = high - pivot;
        if (leftSize < rightSize) {
            if (pivot > low) quickRecursive(state, low, pivot - 1, stats);
            low = pivot + 1;
        } else {
            if (pivot < high) quickRecursive(state, pivot + 1, high, stats);
            if (pivot == 0) break;
            high = pivot - 1;
        }
    }
}
} // namespace

// Public Merge Sort function: create temporary storage and start recursion.
void mergeSort(SortState& state, SortStats& stats) {
    state.isSorting = true;
    state.skipAnimation = false;
    stats = {};
    std::vector<int> temp(state.array.size());
    mergeRecursive(state, temp, 0, state.array.size(), stats);
    if (state.skipAnimation) {
        std::sort(state.array.begin(), state.array.end());
    }
    state.activeIndex1 = -1;
    state.activeIndex2 = -1;
    state.isSorting = false;
    state.skipAnimation = false;
    state.isSorted = true;
}

// Public Quick Sort function: skip recursion for empty or single-item arrays.
void quickSort(SortState& state, SortStats& stats) {
    state.isSorting = true;
    state.skipAnimation = false;
    stats = {};
    if (state.array.size() > 1) {
        quickRecursive(state, 0, state.array.size() - 1, stats);
    }
    if (state.skipAnimation) {
        std::sort(state.array.begin(), state.array.end());
    }
    state.activeIndex1 = -1;
    state.activeIndex2 = -1;
    state.isSorting = false;
    state.skipAnimation = false;
    state.isSorted = true;
}