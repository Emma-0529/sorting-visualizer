#pragma once
#include <vector>
#include <string>

using namespace std;

// different data set types for sorting
enum class DatasetType {
    RANDOM,
    ALREADY_SORTED,
    NEARLY_SORTED,
    REVERSED
};

struct SortState {
    vector<int> array;
    DatasetType currentDataset = DatasetType::RANDOM;
    int activeIndex1 = -1;
    int activeIndex2 = -1;
    bool isSorting = false;
    bool isSorted = false;
    bool skipAnimation = false;
    bool fastMode = false;
};

// data generator functions
vector<int> generateDataset(DatasetType type, int size, int minVal, int maxVal);
string getDatasetName(DatasetType type);

// visualizer functions
void drawBars(const SortState& state, int screenWidth, int screenHeight);
void renderStep(SortState& state, int idx1, int idx2, float delaySeconds);
void animateSwap(SortState& state, int idx1, int idx2, float durationSeconds);