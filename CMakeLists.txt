cmake_minimum_required(VERSION 3.14)
project(BEVBoxEvaluator LANGUAGES CXX)

# Enforce modern C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimize for speed (-O3) when compiling release builds
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Define the library containing the component scripts
add_library(box_eval_comp
    src/geometry.cpp
    src/data_reader.cpp
    src/evaluator.cpp
)
target_include_directories(box_eval_comp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create the final command-line app executable
add_executable(box_eval_app src/main.cpp)

# Link the executable to our library
target_link_libraries(box_eval_app PRIVATE box_eval_comp)