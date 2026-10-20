cmake_minimum_required(VERSION 3.16)

project(ExprBench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(exprbench
    src/main.cpp
    src/operators.cpp
    src/postfix.cpp
    src/evaluator.cpp
)

target_include_directories(exprbench PRIVATE include)

add_executable(exprbench_benchmark
    benchmarks/benchmark.cpp
    src/operators.cpp
    src/evaluator.cpp
)

target_include_directories(exprbench_benchmark PRIVATE include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(exprbench PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

    target_compile_options(exprbench_benchmark PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
