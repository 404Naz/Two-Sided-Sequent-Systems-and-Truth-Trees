cmake_minimum_required(VERSION 3.22)
project(SequentSystemsToTruthTrees)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(CTest)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(catch2)
# Make Catch2's CMake modules available
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# Create a library for project code, update with new cpp files
add_library(project_lib
        src/LogicalAtom.cpp
        src/LogicExpression.cpp
)

target_include_directories(project_lib PUBLIC include)

# Define the executable target and its source files
add_executable(SequentSystemsToTruthTrees
        src/main.cpp
)

# test executable, update with new files
add_executable(tests
        tests/LogicalAtomTests.cpp
)

target_link_libraries(SequentSystemsToTruthTrees PRIVATE project_lib)
target_link_libraries(tests PRIVATE project_lib Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE include)
catch_discover_tests(tests)

# Add the 'include' directory to the compiler's search path for project
target_include_directories(SequentSystemsToTruthTrees
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
