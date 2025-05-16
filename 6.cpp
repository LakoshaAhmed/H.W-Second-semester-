#include <iostream>
#include <cstring>

class String {
    static constexpr size_t SSO_LIMIT = 15;
    size_t len_;
    char* data_;
    bool heap_;
    char buf_[SSO_LIMIT + 1];  

    void init_from_cstr(const char* str) {
        len_ = strlen(str);
        if (len_ <= SSO_LIMIT) {
            memcpy(buf_, str, len_ + 1);
            data_ = buf_;
            heap_ = false;
        } else {
            data_ = new char[len_ + 1];
            memcpy(data_, str, len_ + 1);
            heap_ = true;
        }
    }

public:
    size_t capacity() const { 
        return heap_ ? len_ : SSO_LIMIT;
    }

    // NEW: reserve() method
    void reserve(size_t new_cap) {
        if (new_cap <= capacity()) return;
        
        char* new_data = new char[new_cap + 1];
        memcpy(new_data, data_, len_ + 1);
        if (heap_) delete[] data_;
        data_ = new_data;
        heap_ = true;
    }

    char& operator[](size_t pos) {
        return data_[pos];
    }
    
    const char& operator[](size_t pos) const {
        return data_[pos];
    }
    String() : len_(0), data_(buf_), heap_(false) {
        buf_[0] = '\0';
    }

    String(const char* str) {
        init_from_cstr(str);
    }

    String(const String& other) {
        init_from_cstr(other.c_str());
    }

    String& operator=(const String& rhs) {
        if (this == &rhs) return *this;
        if (heap_) delete[] data_;
        init_from_cstr(rhs.c_str());
        return *this;
    }

    ~String() {
        if (heap_) delete[] data_;
    }

    void append(char c) {
        if (len_ + 1 > capacity()) {
            reserve(capacity() * 2); 
        }
        
        data_[len_] = c;
        data_[len_ + 1] = '\0';
        ++len_;
    }

    const char* c_str() const { return data_; }
    size_t size() const { return len_; }
    bool using_heap() const { return heap_; }
};

int main() {
    char temp[256];

    std::cout << "Give me a word: ";
    std::cin.getline(temp, sizeof(temp));
    String a(temp);

    std::cout << "Another one: ";
    std::cin.getline(temp, sizeof(temp));
    String b(temp);

    std::cout << "Stored:\n";
    std::cout << "A: [" << a.c_str() << "] size=" << a.size() 
              << " capacity=" << a.capacity() << "\n";
    std::cout << "B: [" << b.c_str() << "] size=" << b.size() 
              << " capacity=" << b.capacity() << "\n";

    std::cout << "Type chars to add to A ('.' ends): ";
    char ch;
    while (std::cin >> ch && ch != '.') {
        a.append(ch);
    }

    std::cout << "Modified A: " << a.c_str() << " (" << a.size() << " chars)\n";
    std::cout << "First character: " << a[0] << "\n"; 
    std::cout << "Current capacity: " << a.capacity() << "\n";
    a.reserve(50);
    std::cout << "After reserve(50): " << a.capacity() << "\n";

    return 0;
}