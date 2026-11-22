# Project Title: Sorting Algorithm Visualization Program

- **Team members:** Emma Tripp, Jayme Nguyen, Krishala Panta
- **Project summary:** We built a C++ Sorting Algorithm Visualization Program using Raylib to animate Bubble Sort, Merge Sort, and Quick Sort on randomly generated arrays of 12 to 25 elements across random, nearly sorted, already sorted, and reverse sorted orderings. Performance metrics are outputted to the terminal for comparison.
- **Dependencies:** C++ Compiler, CMake, Raylib, and g++ (for manual logic tests)

## Installation/Setup Steps and Execution Instructions:

1. Ensure CMake is downloaded and a C++ compiler
2. Create own feature branch from staging
3. Build RayLib window for first time:
      - mkdir build
      - cd build
      - cmake ..
      - cmake --build .
      - MacOS: ./sorting_visualizer
      - Windows: .\Debug\sorting_visualizer.exe

**Note** - Whenever you add new .cpp file, must add it to CMakeLists.txt under src/main.cpp and delete old build folder:
  - rm -rf build
  - mkdir build
  - cd build
  - cmake ..
  - cmake --build .
  - MacOS: ./sorting_visualizer
  - Windows: .\Debug\sorting_visualizer.exe

**Note** - To test changes to .cpp files in visualization window must rebuild by
  - cmake --build .
and re-launch the window

**Note** - To simply test logic without the visualization window do not use cmake commands, use standard compiler (g++) commands manually.

## Example Usage

1. Launch the visualizer and select a dataset ordering (random, nearly sorted, already sorted, or reverse sorted) to generate an array of 12 to 25 elements with random values.
2. Run Bubble Sort, Merge Sort, or Quick Sort and watch the Raylib window highlight comparisons and swaps as the array sorts while metrics print to the terminal.
3. Reset the dataset back to its starting state and run another sorting algorithm on the same dataset to compare how each algorithm handles the identical input.
