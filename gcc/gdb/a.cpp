#include <iostream>

int divide(int a, int b) {
    return a / b;  // 当 b 为 0 时会出现除零错误
}

int main() {
    int num1 = 10;
    int num2 = 0;
    int result = divide(num1, num2);
    std::cout << "The result is: " << result << std::endl;
    return 0;
}