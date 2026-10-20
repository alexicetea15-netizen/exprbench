#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

bool isOperator(char symbol) {
    return symbol == '+' ||
           symbol == '-' ||
           symbol == '*' ||
           symbol == '/';
}

int precedence(char operation) {
    if (operation == '+' || operation == '-') {
        return 1;
    }

    if (operation == '*' || operation == '/') {
        return 2;
    }

    return 0;
}

std::vector<std::string> toPostfix(const std::string& expression) {
    std::vector<std::string> output;
    std::vector<char> operators;

    bool expectOperand = true;

    for (std::size_t i = 0; i < expression.size();) {
        unsigned char current =
            static_cast<unsigned char>(expression[i]);

        if (std::isspace(current)) {
            ++i;
            continue;
        }

        if (std::isdigit(current)) {
            if (!expectOperand) {
                throw std::runtime_error(
                    "Missing operator between operands"
                );
            }

            std::size_t start = i;

            while (i < expression.size() &&
                   std::isdigit(
                       static_cast<unsigned char>(expression[i])
                   )) {
                ++i;
            }

            output.push_back(expression.substr(start, i - start));

            expectOperand = false;
            continue;
        }

        char symbol = expression[i];

        if (symbol == '(') {
            if (!expectOperand) {
                throw std::runtime_error(
                    "Missing operator before '('"
                );
            }

            operators.push_back(symbol);
            ++i;
            expectOperand = true;
            continue;
        }

        if (symbol == ')') {
            if (expectOperand) {
                throw std::runtime_error(
                    "Unexpected ')' or missing operand"
                );
            }

            bool foundOpening = false;

            while (!operators.empty()) {
                if (operators.back() == '(') {
                    operators.pop_back();
                    foundOpening = true;
                    break;
                }

                output.emplace_back(1, operators.back());
                operators.pop_back();
            }

            if (!foundOpening) {
                throw std::runtime_error(
                    "Mismatched parentheses: missing '('"
                );
            }

            ++i;
            expectOperand = false;
            continue;
        }

        if (isOperator(symbol)) {
            if (expectOperand) {
                throw std::runtime_error(
                    "Operator without a left operand"
                );
            }

            while (!operators.empty() &&
                   operators.back() != '(' &&
                   precedence(operators.back()) >= precedence(symbol)) {
                output.emplace_back(1, operators.back());
                operators.pop_back();
            }

            operators.push_back(symbol);
            ++i;
            expectOperand = true;
            continue;
        }

        throw std::runtime_error("Invalid character in expression");
    }

    if (output.empty() && operators.empty()) {
        throw std::runtime_error("Expression is empty");
    }

    if (expectOperand) {
        throw std::runtime_error(
            "Incomplete expression: expected an operand"
        );
    }

    while (!operators.empty()) {
        if (operators.back() == '(') {
            throw std::runtime_error(
                "Mismatched parentheses: missing ')'"
            );
        }

        output.emplace_back(1, operators.back());
        operators.pop_back();
    }

    return output;
}

int main() {
    std::string expression;

    std::cout << "Enter expression: ";

    if (!std::getline(std::cin, expression)) {
        std::cerr << "Error: failed to read expression\n";
        return 1;
    }

    try {
        std::vector<std::string> postfix = toPostfix(expression);

        std::cout << "Infix: " << expression << '\n';
        std::cout << "Postfix: ";

        for (std::size_t i = 0; i < postfix.size(); ++i) {
            if (i > 0) {
                std::cout << ' ';
            }

            std::cout << postfix[i];
        }

        std::cout << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}