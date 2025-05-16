#include <iostream>
#include <utility>

void print() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
void print(T&& first, Args&&... args) {
    std::cout << std::forward<T>(first);
    if constexpr (sizeof...(args) > 0) {
        std::cout << " ";
        print(std::forward<Args>(args)...);
    } else {
        std::cout << std::endl;
    }
}

int main() {
    print("Enter values to print:");
    print(1, 2.5, "hello", 'a');
    
    std::cout << "Enter your own values (type 'done' to finish):\n";
    std::string input;
    while (true) {
        std::cin >> input;
        if (input == "done") break;
        print(input);
    }
    
    return 0;
}