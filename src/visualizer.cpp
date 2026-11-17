#include "visualizer.h"
#include "raylib.h"
#include <algorithm>
#include <cmath>

using namespace std;

// displays array as vertical bars across the window
void drawBars(const SortState& state, int screenWidth, int screenHeight) {
    int n = static_cast<int>(state.array.size());
    if (n == 0) return;

    // finds max value in array to scale bar heights
    int maxVal = *max_element(state.array.begin(), state.array.end());
    float barWidth = static_cast<float>(screenWidth) / n;
    int topPadding = 85;

    for (int i = 0; i < n; ++i) {
        float normalizedHeight = static_cast<float>(state.array[i]) / maxVal;
        int barHeight = static_cast<int>(normalizedHeight * (screenHeight - topPadding));

        int x = static_cast<int>(i * barWidth);
        int y = screenHeight - barHeight;

        // color code bars: green = sorted, red = comparing, blue = default
        Color barColor = BLUE;
        if (state.isSorted) {
            barColor = GREEN;
        } else if (i == state.activeIndex1 || i == state.activeIndex2) {
            barColor = RED;
        }

        int drawWidth = (barWidth > 4.0f) ? static_cast<int>(barWidth - 2) : static_cast<int>(barWidth);
        DrawRectangle(x, y, drawWidth, barHeight, barColor);
    }
}

// pauses sorting to check user key inputs (S for skip or F for speed) and refreshes frame
void renderStep(SortState& state, int idx1, int idx2, float delaySeconds) {
    if (WindowShouldClose() || state.skipAnimation) return;

    if (IsKeyPressed(KEY_S)) {
        state.skipAnimation = true;
        return;
    }
    if (IsKeyPressed(KEY_F)) {
        state.fastMode = !state.fastMode;
    }

    // halve delay time if sped up
    float effectiveDelay = state.fastMode ? (delaySeconds * 0.5f) : delaySeconds;
    string speedTag = state.fastMode ? " [2x SPEED]" : " [1x SPEED]";

    state.activeIndex1 = idx1;
    state.activeIndex2 = idx2;

    BeginDrawing();
    ClearBackground(WHITE);

    // draws header text and user controls
    string header = "Dataset: " + getDatasetName(state.currentDataset) + " | Comparing..." + speedTag;
    DrawText(header.c_str(), 20, 18, 20, DARKGRAY);
    DrawText("Controls: [F] Toggle 2x Speed   |   [S] Skip to End", 20, 48, 17, GRAY);

    drawBars(state, GetScreenWidth(), GetScreenHeight());
    EndDrawing();

    WaitTime(effectiveDelay);
}

// animates sliding motion for swapping elements
void animateSwap(SortState& state, int idx1, int idx2, float durationSeconds) {
    if (WindowShouldClose() || state.skipAnimation || idx1 == idx2) return;

    if (IsKeyPressed(KEY_S)) {
        state.skipAnimation = true;
        return;
    }
    if (IsKeyPressed(KEY_F)) {
        state.fastMode = !state.fastMode;
    }

    int n = static_cast<int>(state.array.size());
    if (n == 0) return;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int maxVal = *max_element(state.array.begin(), state.array.end());
    float barWidth = static_cast<float>(screenWidth) / n;
    int topPadding = 85;
    int drawWidth = (barWidth > 4.0f) ? static_cast<int>(barWidth - 2) : static_cast<int>(barWidth);

    // use fewer sub-frames in 2x mode so the swap stays smooth
    int steps = state.fastMode ? 9 : 18;
    float effectiveDuration = state.fastMode ? (durationSeconds * 0.5f) : durationSeconds;
    float stepDelay = effectiveDuration / steps;

    float startX1 = idx1 * barWidth;
    float startX2 = idx2 * barWidth;

    // loop through each step of slide animations
    for (int s = 1; s <= steps; ++s) {
        if (WindowShouldClose()) return;

        if (IsKeyPressed(KEY_S)) {
            state.skipAnimation = true;
            return;
        }
        if (IsKeyPressed(KEY_F)) {
            state.fastMode = !state.fastMode;
            // adjust remaining delay if changed mid-swap
            stepDelay = (state.fastMode ? (durationSeconds * 0.5f) : durationSeconds) / steps;
        }

        // applies smooth acceleration/deceleration movement for speed changes
        float t = static_cast<float>(s) / steps;
        float smoothT = t * t * (3.0f - 2.0f * t);

        BeginDrawing();
        ClearBackground(WHITE);

        string speedTag = state.fastMode ? " [2x SPEED]" : " [1x SPEED]";
        string header = "Dataset: " + getDatasetName(state.currentDataset) + " | Swapping bars..." + speedTag;
        DrawText(header.c_str(), 20, 18, 20, ORANGE);
        DrawText("Controls: [F] Toggle 2x Speed   |   [S] Skip to End", 20, 48, 17, GRAY);

        // draws bars not moving or currently selected
        for (int i = 0; i < n; ++i) {
            if (i == idx1 || i == idx2) continue;

            float normalizedHeight = static_cast<float>(state.array[i]) / maxVal;
            int barHeight = static_cast<int>(normalizedHeight * (screenHeight - topPadding));
            int x = static_cast<int>(i * barWidth);
            int y = screenHeight - barHeight;

            DrawRectangle(x, y, drawWidth, barHeight, BLUE);
        }

        // calculates sliding positions for the swapping bars
        int indices[2] = { idx1, idx2 };
        float currentXs[2] = {
            startX1 + (startX2 - startX1) * smoothT,
            startX2 + (startX1 - startX2) * smoothT
        };

        // draws two actively moving bars in orange during swaps
        for (int k = 0; k < 2; ++k) {
            int idx = indices[k];
            float normalizedHeight = static_cast<float>(state.array[idx]) / maxVal;
            int barHeight = static_cast<int>(normalizedHeight * (screenHeight - topPadding));
            int x = static_cast<int>(currentXs[k]);
            int y = screenHeight - barHeight;

            DrawRectangle(x, y, drawWidth, barHeight, ORANGE);
        }

        EndDrawing();
        WaitTime(stepDelay);
    }
}