#include <cctype>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

char normalizeOperator(char symbol) {
    switch (symbol) {
        case '+':
        case '@':
            return '+';

        case '-':
            return '-';

        case '*':
        case '#':
            return '*';

        case '/':
            return '/';

        default:
            return '\0';
    }
}

bool isOperator(char symbol) {
    return normalizeOperator(symbol) != '\0';
}

int precedence(char operation) {
    char normalized = normalizeOperator(operation);

    if (normalized == '+' || normalized == '-') {
        return 1;
    }

    if (normalized == '*' || normalized == '/') {
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

double evaluatePostfix(const std::vector<std::string>& postfix) {
    std::vector<double> values;

    for (const std::string& token : postfix) {
        if (!token.empty() &&
            std::isdigit(static_cast<unsigned char>(token[0]))) {
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

        double result = evaluatePostfix(postfix);

        std::cout << std::setprecision(15);
        std::cout << "Result: " << result << '\n';

    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}