# exprbench

a small c++17 project for converting arithmetic expressions into reverse polish notation (rpn), evaluating them, and measuring execution performance.

the project was built to practice algorithms, stack-based evaluation, c++ development, and basic performance analysis.

## features

- converts infix expressions into reverse polish notation;
- evaluates rpn expressions using a stack;
- supports addition, subtraction, multiplication, and division;
- supports alternative operators: `@` for addition and `#` for multiplication;
- respects operator precedence and parentheses;
- reports malformed expressions and division by zero;
- includes automated tests through ctest;
- measures evaluator performance using `<ctime>` and `std::clock()`;
- exports benchmark measurements to csv;
- generates summary statistics and an svg chart.

## requirements

- a c++17-compatible compiler, such as gcc or clang;
- cmake 3.16 or newer;
- python 3 for benchmark analysis.

## build

run these commands from the project root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

:)




exit
