#include <iostream>
#include <functional>

template <typename T, typename Deleter = std::function<void(T*)>>
class shared_ptr {
private:
    T* ptr = nullptr;
    size_t* ref_count = nullptr;
    Deleter deleter;

    void cleanup() {
        if (ref_count && --(*ref_count) == 0) {
            if (ptr) deleter(ptr);
            delete ref_count;
        }
        ptr = nullptr;
        ref_count = nullptr;
    }

public:
    explicit shared_ptr(T* p = nullptr, Deleter d = [](T* p) { delete p; })
        : ptr(p), ref_count(new size_t(1)), deleter(d) {}

    shared_ptr(const shared_ptr& other)
        : ptr(other.ptr), ref_count(other.ref_count), deleter(other.deleter) {
        if (ref_count) (*ref_count)++;
    }

    shared_ptr(shared_ptr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count), deleter(std::move(other.deleter)) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    ~shared_ptr() { cleanup(); }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            ref_count = other.ref_count;
            deleter = other.deleter;
            if (ref_count) (*ref_count)++;
        }
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            ref_count = other.ref_count;
            deleter = std::move(other.deleter);
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
    size_t use_count() const { return ref_count ? *ref_count : 0; }

    void reset(T* p = nullptr, Deleter d = [](T* p) { delete p; }) {
        cleanup();
        ptr = p;
        ref_count = new size_t(1);
        deleter = d;
    }
};

int main() {
    int choice;
    std::cout << "1. Default deleter\n2. Array deleter\n3. File deleter\n4. Custom lambda\nEnter choice: ";
    std::cin >> choice;

    if (choice == 1) {
        shared_ptr<int> p1(new int);
        std::cout << "Created int with default deleter\n";
    }
    else if (choice == 2) {
        shared_ptr<int, void(*)(int*)> p2(new int[5], [](int* p) { delete[] p; });
        std::cout << "Created array with array deleter\n";
    }
    else if (choice == 3) {
        shared_ptr<FILE, std::function<void(FILE*)>> p3(fopen("test.txt", "w"), [](FILE* fp) { if(fp) fclose(fp); });
        std::cout << "Created FILE with file deleter\n";
    }
    else if (choice == 4) {
        std::cout << "Enter value to manage: ";
        double val;
        std::cin >> val;
        shared_ptr<double> p4(new double(val), [](double* p) { std::cout << "Deleting value: " << *p << "\n"; delete p; });
        std::cout << "Created double with custom lambda deleter\n";
    }

    return 0;
}