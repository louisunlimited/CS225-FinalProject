cmake_minimum_required(VERSION 3.10)

project(CS225-FinalProject)

# specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# setup flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function")

# check if catch2 is present, else download it
find_package(Catch2)

if(NOT Catch2_FOUND)
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.1 # or a later release
)
    FetchContent_MakeAvailable(Catch2)
endif()

# add subdirectories
# add_subdirectory(tests)

# configure testexecutables
add_executable(test_all tests/tests.cpp)
target_link_libraries(test_all Catch2::Catch2WithMain)
add_executable(main main.cpp)

# configure CI tests
include(Catch)
include(CTest)
catch_discover_tests(test_all)
