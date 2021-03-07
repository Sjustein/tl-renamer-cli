# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_minimum_required (VERSION 3.8)

project ("tl-renamer-cli")

# Include sub-projects.
add_subdirectory ("tl-renamer-cli")