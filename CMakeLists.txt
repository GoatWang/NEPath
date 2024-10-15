# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(NEPathProject VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable all compiler warnings and optimization levels
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Add the source directory to the include directories
include_directories(NEPath-master)

# List all source files
set(SOURCE_FILES
    main.cpp
    demos.cpp
    NEPath-master/Basic.cpp
    NEPath-master/clipper.cpp
    NEPath-master/ContourParallel.cpp
    NEPath-master/Curve.cpp
    NEPath-master/DirectionParallel.cpp
    NEPath-master/FileAgent.cpp
    NEPath-master/NonEquidistant.cpp
    NEPath-master/path.cpp
    NEPath-master/NEPathPlanner.cpp
    NEPath-master/PlanningOptions.cpp
)

# Specify the target executable
add_executable(nepath ${SOURCE_FILES})

# Clean-up target (optional)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "nepath")
