cmake_minimum_required(VERSION 3.22)
project(SequentSystemsToTruthTrees)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the executable target and its source files
add_executable(SequentSystemsToTruthTrees
        src/main.cpp
)

# Add the 'include' directory to the compiler's search path for project
target_include_directories(SequentSystemsToTruthTrees
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
