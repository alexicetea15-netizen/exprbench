#include "evaluator.h"
#include "operators.h"

#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

double evaluatePostfix(const std::vector<std::string>& postfix) {
    std::vector<double> values;

    for (const std::string& token : postfix) {
        if (!token.empty() &&
            std::isdigit(
                static_cast<unsigned char>(token[0])
            )) {
            values.push_back(std::stod(token));
            continue;
        }

        if (token.size() != 1 || !isOperator(token[0])) {
            throw std::runtime_error(
                "Invalid token in postfix expression"
            );
        }

        if (values.size() < 2) {
            throw std::runtime_error("Not enough operands");
        }

        double right = values.back();
        values.pop_back();

        double left = values.back();
        values.pop_back();

        double result = 0;
        char operation = normalizeOperator(token[0]);

        switch (operation) {
            case '+':
                result = left + right;
                break;

            case '-':
                result = left - right;
                break;

            case '*':
                result = left * right;
                break;

            case '/':
                if (right == 0) {
                    throw std::runtime_error("Division by zero");
                }

                result = left / right;
                break;

            default:
                throw std::runtime_error("Unknown operator");
        }

        values.push_back(result);
    }

    if (values.size() != 1) {
        throw std::runtime_error(
            "Invalid postfix expression: expected one result"
        );
    }

    return values.back();
}
