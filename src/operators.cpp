#include "operators.h"

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
