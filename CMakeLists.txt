cmake_minimum_required(VERSION 3.15)
project(quicksort_evaluation)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")


find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(parlaylib
        GIT_REPOSITORY https://github.com/cmuparlay/parlaylib.git
        GIT_TAG master
)
FetchContent_MakeAvailable(parlaylib)

add_executable(quicksort_evaluation main.cpp)
target_include_directories(quicksort_evaluation PRIVATE src)
target_link_libraries(quicksort_evaluation PRIVATE parlay Threads::Threads)
set_target_properties(quicksort_evaluation PROPERTIES
        ENVIRONMENT "PARLAY_NUM_THREADS=4"
)