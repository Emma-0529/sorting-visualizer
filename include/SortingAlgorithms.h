#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <cstddef>
#include <vector>
#include "visualizer.h"

// Statistics are reset at the start of every sorting function.
// A movement means one write of an array element (including temporary arrays).
struct SortStats {
    unsigned long long comparisons = 0;
    unsigned long long swaps = 0;
    unsigned long long movements = 0;
};

// Sorts the vector in ascending order, modifying it in place.
void bubbleSort(SortState& state, SortStats& stats);
void mergeSort(SortState& state, SortStats& stats);
void quickSort(SortState& state, SortStats& stats);

#endif
