#include <iostream>
#include <memory>
#include <stdexcept>
#include <limits>
#include <string>

template <size_t Rows, size_t Cols, typename T = double>
class Matrix {
public:
    Matrix() : data(std::make_unique<T[]>(Rows * Cols)) {
        for (size_t i = 0; i < Rows * Cols; ++i) {
            data[i] = T{};
        }
    }

    ~Matrix() = default;

    Matrix(const Matrix& other) : Matrix() {
        for (size_t i = 0; i < Rows * Cols; ++i) {
            data[i] = other.data[i];
        }
    }

    Matrix(Matrix&& other) noexcept : data(std::move(other.data)) {}

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            for (size_t i = 0; i < Rows * Cols; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    T& operator()(size_t row, size_t col) {
        if (row >= Rows || col >= Cols) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row * Cols + col];
    }

    const T& operator()(size_t row, size_t col) const {
        if (row >= Rows || col >= Cols) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row * Cols + col];
    }

    void print() const {
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Cols; ++j) {
                std::cout << (*this)(i, j) << ' ';
            }
            std::cout << '\n';
        }
    }

    Matrix operator+(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        Matrix result;
        for (size_t i = 0; i < Rows * Cols; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

private:
    std::unique_ptr<T[]> data;
};

template <size_t R1, size_t C1, size_t C2, typename T>
Matrix<R1, C2, T> operator*(const Matrix<R1, C1, T>& lhs, const Matrix<C1, C2, T>& rhs) {
    Matrix<R1, C2, T> result;
    for (size_t i = 0; i < R1; ++i) {
        for (size_t j = 0; j < C2; ++j) {
            T sum{};
            for (size_t k = 0; k < C1; ++k) {
                sum += lhs(i, k) * rhs(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

template <size_t Rows, size_t Cols, typename T>
auto transpose(Matrix<Rows, Cols, T>&& mat) {
    Matrix<Cols, Rows, T> result;
    for (size_t i = 0; i < Rows; ++i) {
        for (size_t j = 0; j < Cols; ++j) {
            result(j, i) = std::forward<Matrix<Rows, Cols, T>>(mat)(i, j);
        }
    }
    return result;
}

// Replacing lambda with a function template for trace
template <size_t N, typename T>
T trace(const Matrix<N, N, T>& mat) {
    T trace{};
    for (size_t i = 0; i < N; ++i) {
        trace += mat(i, i);
    }
    return trace;
}

Matrix<2, 2> createSquareMatrix(const std::string& name) {
    Matrix<2, 2> mat;
    std::cout << "Enter values for 2x2 matrix " << name << " (row-wise):\n";
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            std::cout << "Enter value for [" << i << "][" << j << "]: ";
            while (!(std::cin >> mat(i, j))) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
            }
        }
    }
    return mat;
}

Matrix<2, 3> create2x3Matrix(const std::string& name) {
    Matrix<2, 3> mat;
    std::cout << "Enter values for 2x3 matrix " << name << " (row-wise):\n";
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            std::cout << "Enter value for [" << i << "][" << j << "]: ";
            while (!(std::cin >> mat(i, j))) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
            }
        }
    }
    return mat;
}

Matrix<3, 2> create3x2Matrix(const std::string& name) {
    Matrix<3, 2> mat;
    std::cout << "Enter values for 3x2 matrix " << name << " (row-wise):\n";
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            std::cout << "Enter value for [" << i << "][" << j << "]: ";
            while (!(std::cin >> mat(i, j))) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
            }
        }
    }
    return mat;
}

int main() {
    int choice;
    do {
        std::cout << "\nMatrix Operations Menu:\n";
        std::cout << "1. Add two 2x3 matrices\n";
        std::cout << "2. Subtract two 2x3 matrices\n";
        std::cout << "3. Multiply 2x3 and 3x2 matrices\n";
        std::cout << "4. Transpose a 2x3 matrix\n";
        std::cout << "5. Calculate trace of 2x2 matrix\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        while (!(std::cin >> choice) || choice < 1 || choice > 6) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter 1-6: ";
        }

        switch (choice) {
            case 1: {
                auto m1 = create2x3Matrix("A");
                auto m2 = create2x3Matrix("B");
                auto sum = m1 + m2;
                std::cout << "\nResult of A + B:\n";
                sum.print();
                break;
            }
            case 2: {
                auto m1 = create2x3Matrix("A");
                auto m2 = create2x3Matrix("B");
                auto diff = m1 - m2;
                std::cout << "\nResult of A - B:\n";
                diff.print();
                break;
            }
            case 3: {
                auto m1 = create2x3Matrix("A");
                auto m2 = create3x2Matrix("B");
                auto product = m1 * m2;
                std::cout << "\nResult of A * B:\n";
                product.print();
                break;
            }
            case 4: {
                auto m1 = create2x3Matrix("A");
                auto transposed = transpose(std::move(m1));
                std::cout << "\nTranspose of A:\n";
                transposed.print();
                break;
            }
            case 5: {
                auto m1 = createSquareMatrix("A");
                std::cout << "\nTrace of A: " << trace(m1) << "\n";
                break;
            }
            case 6:
                std::cout << "Exiting...\n";
                break;
        }
    } while (choice != 6);

    return 0;
}
