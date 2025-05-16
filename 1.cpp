#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>

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
        std::memset(newMap, 0, sizeof(T*) * newMapSize);

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
        std::memset(map, 0, sizeof(T*) * mapSize);
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

int main() {
    Deque<int> d;
    std::string command;
    int value;

    std::cout << "Available commands:\n";
    std::cout << "  push_front <value>\n";
    std::cout << "  push_back <value>\n";
    std::cout << "  pop_front\n";
    std::cout << "  pop_back\n";
    std::cout << "  front\n";
    std::cout << "  back\n";
    std::cout << "  size\n";
    std::cout << "  capacity\n";
    std::cout << "  exit\n";

    while (true) {
        std::cout << "> ";
        std::cin >> command;

        try {
            if (command == "push_front") {
                std::cin >> value;
                d.push_front(value);
            } else if (command == "push_back") {
                std::cin >> value;
                d.push_back(value);
            } else if (command == "pop_front") {
                d.pop_front();
            } else if (command == "pop_back") {
                d.pop_back();
            } else if (command == "front") {
                std::cout << "Front: " << d.front() << "\n";
            } else if (command == "back") {
                std::cout << "Back: " << d.back() << "\n";
            } else if (command == "size") {
                std::cout << "Size: " << d.size() << "\n";
            } else if (command == "capacity") {
                std::cout << "Capacity: " << d.capacity() << "\n";
            } else if (command == "exit") {
                break;
            } else {
                std::cout << "Unknown command.\n";
            }
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
