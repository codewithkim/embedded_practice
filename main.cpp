#include <iostream>
#include <string_view> // C++17 기능

int main() {
    constexpr std::string_view message = "Embedded C++17 Environment Ready!";
    std::cout << message << std::endl;
    return 0;
}