# DataCollector

A cross-platform C++ application for data collection.

## Building

### Prerequisites
- CMake 3.10+
- C++17 compatible compiler (MSVC, GCC, or Clang)

### Linux
```bash
mkdir build
cd build
cmake ..
cmake --build .
./datacollector
```

### Windows
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\Release\datacollector.exe
```

## Project Structure
- `src/` - Source files
- `CMakeLists.txt` - Build configuration
