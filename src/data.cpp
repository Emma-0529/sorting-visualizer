#include "visualizer.h"
#include <random>
#include <algorithm>
#include <utility>

using namespace std;

// array of integers based on dataset type, array size, and value range
vector<int> generateDataset(DatasetType type, int size, int minVal, int maxVal) {
    vector<int> arr(size);

    // random number gen for elements in the array
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(minVal, maxVal);

    // fill array with random values first
    for (int i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }

    // modifies order of data depending on selected dataset type
    if (type == DatasetType::ALREADY_SORTED) {
        sort(arr.begin(), arr.end());       // ascending
    } 
    else if (type == DatasetType::REVERSED) {
        sort(arr.rbegin(), arr.rend());     // descending
    } 
    else if (type == DatasetType::NEARLY_SORTED) {
        sort(arr.begin(), arr.end());
        // perform a few small adjacent swaps (~10% of array size)
        int swapCount = max(2, size / 10);
        uniform_int_distribution<> indexDist(0, size - 2);
        for (int i = 0; i < swapCount; ++i) {
            int idx = indexDist(gen);
            swap(arr[idx], arr[idx + 1]);
        }
    }

    return arr;
}

// returns names for dataset types
string getDatasetName(DatasetType type) {
    switch (type) {
        case DatasetType::RANDOM:         return "Random";
        case DatasetType::ALREADY_SORTED: return "Already Sorted";
        case DatasetType::NEARLY_SORTED:  return "Nearly Sorted";
        case DatasetType::REVERSED:       return "Reverse-Sorted";
        default:                          return "Random";
    }
}