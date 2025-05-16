#include <iostream>
#include <deque>
#include <vector>
#include <stdexcept>
#include <string>

template <typename T, typename Container = std::deque<T>>
class Stack {
private:
    Container c;
public:
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    Stack() = default;
    explicit Stack(const Container& cont) : c(cont) {}
    explicit Stack(Container&& cont) : c(std::move(cont)) {}

    reference top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return c.back();
    }
    
    const_reference top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return c.back();
    }
    
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    
    void push(const value_type& value) {
        c.push_back(value);
    }
    
    void push(value_type&& value) {
        c.push_back(std::move(value));
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        c.emplace_back(std::forward<Args>(args)...);
    }
    
    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        c.pop_back();
    }
    
    void swap(Stack& other) noexcept {
        using std::swap;
        swap(c, other.c);
    }

    template <typename U, typename C>
    friend bool operator==(const Stack<U, C>& lhs, const Stack<U, C>& rhs);

    template <typename U, typename C>
    friend bool operator!=(const Stack<U, C>& lhs, const Stack<U, C>& rhs);
};

template <typename T, typename Container>
void swap(Stack<T, Container>& lhs, Stack<T, Container>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename T, typename Container>
bool operator==(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
    return lhs.c == rhs.c;
}

template <typename T, typename Container>
bool operator!=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
    return !(lhs == rhs);
}

void displayMenu() {
    std::cout << "\nStack Operations Menu:\n";
    std::cout << "1. Push an element\n";
    std::cout << "2. Emplace an element\n";
    std::cout << "3. Pop an element\n";
    std::cout << "4. View top element\n";
    std::cout << "5. Check stack size\n";
    std::cout << "6. Check if stack is empty\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    Stack<int> stack;
    int choice;
    int value;
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1:
                    std::cout << "Enter value to push: ";
                    std::cin >> value;
                    stack.push(value);
                    std::cout << "Pushed " << value << " onto the stack.\n";
                    break;
                case 2:
                    std::cout << "Enter value to emplace: ";
                    std::cin >> value;
                    stack.emplace(value);
                    std::cout << "Emplaced " << value << " onto the stack.\n";
                    break;
                case 3:
                    value = stack.top();
                    stack.pop();
                    std::cout << "Popped " << value << " from the stack.\n";
                    break;
                case 4:
                    std::cout << "Top element: " << stack.top() << std::endl;
                    break;
                case 5:
                    std::cout << "Stack size: " << stack.size() << std::endl;
                    break;
                case 6:
                    std::cout << "Stack is " << (stack.empty() ? "empty" : "not empty") << std::endl;
                    break;
                case 7:
                    std::cout << "Exiting program.\n";
                    return 0;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
    }
    
    return 0;
}