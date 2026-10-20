#include "evaluator.h"
#include "postfix.h"

#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

int failures = 0;
int totalTests = 0;

void check(const std::string& name, bool passed) {
    ++totalTests;

    if (passed) {
        std::cout << "[PASS] " << name << '\n';
    } else {
        std::cout << "[FAIL] " << name << '\n';
        ++failures;
    }
}

void checkThrows(
    const std::string& name,
    const std::function<void()>& function
) {
    bool threw = false;

    try {
        function();
    } catch (const std::exception&) {
        threw = true;
    }

    check(name, threw);
}

double evaluate(const std::string& expression) {
    return evaluatePostfix(toPostfix(expression));
}

int main() {
    check(
        "simple addition",
        toPostfix("2 + 3") ==
        std::vector<std::string>{"2", "3", "+"}
    );

    check(
        "operator precedence",
        toPostfix("2 + 3 * 4") ==
        std::vector<std::string>{"2", "3", "4", "*", "+"}
    );

    check(
        "parentheses",
        toPostfix("(2 + 3) * 4") ==
        std::vector<std::string>{"2", "3", "+", "4", "*"}
    );

    check(
        "left associativity",
        toPostfix("10 - 3 - 2") ==
        std::vector<std::string>{"10", "3", "-", "2", "-"}
    );

    check(
        "custom operators",
        evaluate("8 @ 2 # (3 - 1)") == 12.0
    );

    check(
        "simple evaluation",
        evaluate("2 + 3") == 5.0
    );

    check(
        "operator precedence evaluation",
        evaluate("2 + 3 * 4") == 14.0
    );

    check(
        "parentheses evaluation",
        evaluate("(2 + 3) * 4") == 20.0
    );

    check(
        "subtraction order",
        evaluate("10 - 3") == 7.0
    );

    check(
        "fractional division",
        std::fabs(evaluate("7 / 2") - 3.5) < 1e-9
    );

    checkThrows(
        "invalid expression",
        [] {
            toPostfix("2 + * 3");
        }
    );

    checkThrows(
        "missing closing parenthesis",
        [] {
            toPostfix("(2 + 3");
        }
    );

    checkThrows(
        "extra closing parenthesis",
        [] {
            toPostfix("2 + 3)");
        }
    );

    checkThrows(
        "empty expression",
        [] {
            toPostfix("");
        }
    );

    checkThrows(
        "division by zero",
        [] {
            evaluate("5 / (3 - 3)");
        }
    );

    checkThrows(
        "insufficient postfix operands",
        [] {
            evaluatePostfix({"2", "+"});
        }
    );

    std::cout << "\nTests passed: "
              << totalTests - failures
              << "/" << totalTests << '\n';

    return failures == 0 ? 0 : 1;
}
