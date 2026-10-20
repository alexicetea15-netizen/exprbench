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

add_executable(exprbench_benchmark
    benchmarks/benchmark.cpp
    src/operators.cpp
    src/evaluator.cpp
)

add_executable(exprbench_tests
    tests/tests.cpp
    src/operators.cpp
    src/postfix.cpp
    src/evaluator.cpp
)

target_include_directories(exprbench PRIVATE include)

target_include_directories(exprbench_benchmark PRIVATE include)

target_include_directories(exprbench_tests PRIVATE include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    foreach(target exprbench exprbench_benchmark exprbench_tests)
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endforeach()
endif()

enable_testing()

add_test(
    NAME exprbench_tests
    COMMAND exprbench_tests
)
