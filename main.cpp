#include <iostream>
#include <string>

int main(){
    std::string expression;
    std::cout << "enter expression: ";
    std::getline(std::cin, expression);
    std::cout << "expression: " << expression << '\n';

    return 0;

}