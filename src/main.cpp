#include "evaluator.h"
#include "postfix.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

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
