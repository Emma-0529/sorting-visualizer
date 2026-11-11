# sorting-visualizer

Steps to Run:

1. Ensure CMake is downloaded and a C++ compiler
2. Create own feature branch from staging
3. Build RayLib window for first time:
   * mkdir build
   * cd build
   * cmake ..
   * cmake --build .
   - MacOS:\
     ./sorting_visualizer
   - Windows:\
     .\Debug\sorting_visualizer.exe

Note - Whenever you add new .cpp file, must add it to CMakeLists.txt under src/main.cpp and delete old build folder:
   * rm -rf build
   * mkdir build
   * cd build
   * cmake ..
   * cmake --build .
   - MacOS:\
     ./sorting_visualizer
   - Windows:\
     .\Debug\sorting_visualizer.exe

Note - To test changes to files in visualization window must rebuild by
   * cmake --build .
and re-launch the window
