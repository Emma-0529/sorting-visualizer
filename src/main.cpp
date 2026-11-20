#include "raylib.h"
#include "visualizer.h"
#include "SortingAlgorithms.h"
#include <utility>
#include <string>
#include <random>
#include <vector>

using namespace std;

// available algorithms to choose from
enum class AlgorithmType {
    BUBBLE,
    MERGE,
    QUICK
};

// returns algorithm names for display
string getAlgorithmName(AlgorithmType algo) {
    switch (algo) {
        case AlgorithmType::BUBBLE: return "Bubble Sort";
        case AlgorithmType::MERGE:  return "Merge Sort";
        case AlgorithmType::QUICK:  return "Quick Sort";
        default:                    return "Bubble Sort";
    }
}

// resets array with new dataset type, number of elements, and clears states
void resetDataset(SortState& state, DatasetType type, vector<int>& baselineArray) {
    // random number generator for the dataset size
    random_device rd;
    mt19937 gen(rd());
    // number of elements randomized in range 12-25
    uniform_int_distribution<> sizeDist(12, 25);
    
    int dynamicSize = sizeDist(gen);

    state.currentDataset = type;
    baselineArray = generateDataset(type, dynamicSize, 15, 500);
    state.array = baselineArray;
    state.isSorted = false;
    state.skipAnimation = false;
    state.activeIndex1 = -1;
    state.activeIndex2 = -1;
}

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 600;

    // initialize raylib window
    InitWindow(screenWidth, screenHeight, "Sorting Visualizer");
    SetTargetFPS(60);

    SortState state;
    SortStats stats;
    vector<int> baselineArray;
    AlgorithmType currentAlgo = AlgorithmType::BUBBLE;
    resetDataset(state, DatasetType::RANDOM, baselineArray);

    while (!WindowShouldClose()) {
        if (!state.isSorting) {
            // 1. select dataset type (random, already-sorted, nearly-sorted, or reversed)
            if (IsKeyPressed(KEY_ONE))   resetDataset(state, DatasetType::RANDOM, baselineArray);
            if (IsKeyPressed(KEY_TWO))   resetDataset(state, DatasetType::ALREADY_SORTED, baselineArray);
            if (IsKeyPressed(KEY_THREE)) resetDataset(state, DatasetType::NEARLY_SORTED, baselineArray);
            if (IsKeyPressed(KEY_FOUR))  resetDataset(state, DatasetType::REVERSED, baselineArray);

            // 2. select algorithm (bubble, merge, or quick sort)
            if (IsKeyPressed(KEY_B)) currentAlgo = AlgorithmType::BUBBLE;
            if (IsKeyPressed(KEY_M)) currentAlgo = AlgorithmType::MERGE;
            if (IsKeyPressed(KEY_Q)) currentAlgo = AlgorithmType::QUICK;

            // 3. reset back to unsorted data with same elements or change speed
            if (IsKeyPressed(KEY_R)) {
                state.array = baselineArray;
                state.isSorted = false;
                state.skipAnimation = false;
                state.activeIndex1 = -1;
                state.activeIndex2 = -1;
            }
            if (IsKeyPressed(KEY_F))     state.fastMode = !state.fastMode;

            // 4. start the sort using spacebar
            if (IsKeyPressed(KEY_SPACE) && !state.isSorted) {
                if (currentAlgo == AlgorithmType::BUBBLE) {
                    bubbleSort(state, stats);
                } else if (currentAlgo == AlgorithmType::MERGE) {
                    mergeSort(state, stats);
                } else if (currentAlgo == AlgorithmType::QUICK) {
                    quickSort(state, stats);
                }
            }
        }

        // begin raylib drawing frame
        BeginDrawing();
        ClearBackground(WHITE);

        // headers and text displayed to user - algorithm, dataset, speed, and stats
        string speedTag = state.fastMode ? " [2x SPEED]" : " [1x SPEED]";
        string statusText = "Algo: " + getAlgorithmName(currentAlgo) + "  |  Data: " + getDatasetName(state.currentDataset) + speedTag + "  |  ";
        
        if (state.isSorted) {
            statusText += "SORTED! [R] Reset  (Comp: " + to_string(stats.comparisons) + ", Swaps: " + to_string(stats.swaps) + ")";
            DrawText(statusText.c_str(), 20, 16, 18, DARKGREEN);
        } else {
            statusText += "[SPACE] Start Sort";
            DrawText(statusText.c_str(), 20, 16, 18, BLACK);
        }

        DrawText("Algos: [B] Bubble  [M] Merge  [Q] Quick   ||   Data: [1] Random  [2] Sorted  [3] Nearly  [4] Reversed   ||   [F] 2x", 20, 46, 15, DARKGRAY);

        // draws sorting bars across the width of the screen
        drawBars(state, screenWidth, screenHeight);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}