#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>  
template<typename T>
class Deque {
private:
    static const int BLOCK_SIZE = 64;
    T** map;
    int mapSize;
    int startBlock, startOffset;
    int endBlock, endOffset;
    int count;

    void reallocateMap(int newMapSize) {
        int newCenter = newMapSize / 2;
        T** newMap = new T*[newMapSize];
        memset(newMap, 0, sizeof(T*) * newMapSize);

        int offset = newCenter - (endBlock - startBlock) / 2;
        for (int i = startBlock; i <= endBlock; ++i) {
            newMap[offset + (i - startBlock)] = map[i];
        }

        delete[] map;
        map = newMap;
        endBlock = offset + (endBlock - startBlock);
        startBlock = offset;
        mapSize = newMapSize;
    }

    void ensureFrontCapacity() {
        if (startOffset == 0) {
            if (startBlock == 0) {
                reallocateMap(mapSize * 2);
            }
            map[--startBlock] = new T[BLOCK_SIZE];
            startOffset = BLOCK_SIZE;
        }
    }

    void ensureBackCapacity() {
        if (endOffset == BLOCK_SIZE) {
            if (endBlock == mapSize - 1) {
                reallocateMap(mapSize * 2);
            }
            map[++endBlock] = new T[BLOCK_SIZE];
            endOffset = 0;
        }
    }

    void cleanup() {
        for (int i = startBlock; i <= endBlock; ++i) {
            delete[] map[i];
        }
        delete[] map;
    }

public:
    Deque()
        : mapSize(8), startBlock(4), endBlock(4), startOffset(BLOCK_SIZE / 2), endOffset(BLOCK_SIZE / 2), count(0) {
        map = new T*[mapSize];
        memset(map, 0, sizeof(T*) * mapSize);
        map[startBlock] = new T[BLOCK_SIZE];
    }

    Deque(const Deque& other)
        : mapSize(other.mapSize), startBlock(other.startBlock), endBlock(other.endBlock),
          startOffset(other.startOffset), endOffset(other.endOffset), count(other.count) {
        map = new T*[mapSize];
        for (int i = 0; i < mapSize; ++i) {
            if (other.map[i]) {
                map[i] = new T[BLOCK_SIZE];
                std::copy(other.map[i], other.map[i] + BLOCK_SIZE, map[i]);
            } else {
                map[i] = nullptr;
            }
        }
    }

    Deque& operator=(const Deque& other) {
        if (this == &other) return *this;

        cleanup();
        mapSize = other.mapSize;
        startBlock = other.startBlock;
        endBlock = other.endBlock;
        startOffset = other.startOffset;
        endOffset = other.endOffset;
        count = other.count;

        map = new T*[mapSize];
        for (int i = 0; i < mapSize; ++i) {
            if (other.map[i]) {
                map[i] = new T[BLOCK_SIZE];
                std::copy(other.map[i], other.map[i] + BLOCK_SIZE, map[i]);
            } else {
                map[i] = nullptr;
            }
        }

        return *this;
    }

    ~Deque() {
        cleanup();
    }

    void push_back(const T& value) {
        ensureBackCapacity();
        map[endBlock][endOffset++] = value;
        count++;
    }

    void push_front(const T& value) {
        ensureFrontCapacity();
        map[startBlock][--startOffset] = value;
        count++;
    }

    void pop_back() {
        if (count == 0) throw std::underflow_error("Deque is empty");
        if (--endOffset < 0) {
            delete[] map[endBlock--];
            endOffset = BLOCK_SIZE - 1;
        }
        count--;
    }

    void pop_front() {
        if (count == 0) throw std::underflow_error("Deque is empty");
        if (++startOffset == BLOCK_SIZE) {
            delete[] map[startBlock++];
            startOffset = 0;
        }
        count--;
    }

    T& front() {
        if (count == 0) throw std::underflow_error("Deque is empty");
        return map[startBlock][startOffset];
    }

    T& back() {
        if (count == 0) throw std::underflow_error("Deque is empty");
        return map[endBlock][endOffset - 1];
    }

    int size() const {
        return count;
    }

    int capacity() const {
        return (endBlock - startBlock + 1) * BLOCK_SIZE;
    }
};

template<typename T, typename Container = Deque<T>>
class Stack {
private:
    Container c;

public:
    void push(const T& value) { c.push_back(value); }
    void pop() { 
        if (empty()) throw std::underflow_error("Stack is empty");
        c.pop_back(); 
    }
    T& top() { 
        if (empty()) throw std::underflow_error("Stack is empty");
        return c.back(); 
    }
    const T& top() const { 
        if (empty()) throw std::underflow_error("Stack is empty");
        return c.back(); 
    }
    bool empty() const { return c.size() == 0; }
    size_t size() const { return c.size(); }
};

int main() {
    Stack<int> stack;
    std::string command;
    int value;

    std::cout << "Stack Adapter (LIFO) - Available Commands:\n";
    std::cout << "  push <value>  - Push a value onto the stack\n";
    std::cout << "  pop           - Remove the top value\n";
    std::cout << "  top           - Peek at the top value\n";
    std::cout << "  size          - Get the current stack size\n";
    std::cout << "  empty         - Check if the stack is empty\n";
    std::cout << "  exit          - Quit the program\n";

    while (true) {
        std::cout << "> ";
        std::cin >> command;

        try {
            if (command == "push") {
                std::cin >> value;
                stack.push(value);
                std::cout << "Pushed " << value << "\n";
            } 
            else if (command == "pop") {
                std::cout << "Popped " << stack.top() << "\n";
                stack.pop();
            } 
            else if (command == "top") {
                std::cout << "Top: " << stack.top() << "\n";
            } 
            else if (command == "size") {
                std::cout << "Size: " << stack.size() << "\n";
            } 
            else if (command == "empty") {
                std::cout << "Stack is " << (stack.empty() ? "empty" : "not empty") << "\n";
            } 
            else if (command == "exit") {
                break;
            } 
            else {
                std::cout << "Unknown command. Try again.\n";
            }
        } 
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
