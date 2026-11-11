# sorting-visualizer

Steps to Run:

1. Ensure CMake is downloaded and a C++ compiler
2. Create own feature branch from staging
3. Build RayLib window for first time:
        1. mkdir build
        2. cd build
        3. cmake ..
        4. cmake --build .
    * MacOS:
      ./sorting_visualizer
    * Windows:
      .\Debug\sorting_visualizer.exe

Note - Whenever you add new .cpp file, must add it to CMakeLists.txt under src/main.cpp and delete old build folder:
        1. rm -rf build
        2. mkdir build
        3. cd build
        4. cmake ..
        5. cmake --build .
    * MacOS:
        ./sorting_visualizer
    * Windows:
        .\Debug\sorting_visualizer.exe


Note - To test changes to files in visualization window must rebuild by
        * cmake --build .
and re-launch the window
