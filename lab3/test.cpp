#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <chrono>
using namespace std;

const int FIELD_SIZE = 359;
const int POLYNOMIAL_DEGREES[5] = { 0, 2, 4, 18, 359 }; // x^359 + x^18 + x^4 + x^2 + 1

int PolynomialDegree(uint64_t poly[]);
class FieldElement; // forward declaration
FieldElement ReducePolynomial(uint64_t poly[]);

class FieldElement {
public:
    uint64_t coefficient[(FIELD_SIZE / 64) + 1] = {0};

    FieldElement() = default;

    FieldElement(const FieldElement& other) {
        for (int i = 0; i <= FIELD_SIZE / 64; i++) {
            coefficient[i] = other.coefficient[i];
        }
    }

    FieldElement(string binary) {
        for (int i = binary.length() - 1; i >= 0 ; i--) {
            if (binary.at(binary.length() - 1 - i) == '1') {
                coefficient[i / 64] += static_cast<unsigned long long>(1) << (i % 64);
            }
        }
    }

    FieldElement operator+(const FieldElement& other) const {
        FieldElement result;
        for (int i = 0; i <= FIELD_SIZE / 64; i++) {
            result.coefficient[i] = coefficient[i] ^ other.coefficient[i];
        }
        return result;
    }

    FieldElement operator*(const FieldElement& other) const {
        uint64_t result[(2 * FIELD_SIZE - 1) / 64 + 1] = {0};
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                result[(i + j) / 64] ^= (((coefficient[i / 64] >> (i % 64)) & (other.coefficient[j / 64] >> (j % 64))) & 1ULL) << ((i + j) % 64);
            }
        }
        return ReducePolynomial(result);
    }

    FieldElement square() const {
        uint64_t result[(2 * FIELD_SIZE - 1) / 64 + 1] = { 0 };
        for (int i = 0; i < FIELD_SIZE; i++) {
            uint64_t bit = (coefficient[i / 64] >> (i % 64)) & 1;
            result[2*i / 64] += bit << (2*i % 64);
        }
        return ReducePolynomial(result);
    }

    FieldElement power(const FieldElement& exponent) const {
        FieldElement result("1");
        FieldElement base = *this;
        for (int i = FIELD_SIZE - 1; i >= 0; i--) {
            if ((exponent.coefficient[i / 64] >> (i % 64)) & 1) {
                result = base * result;
            }
            if (i > 0) result = result.square();
        }
        return result;
    }

    FieldElement trace() const {
        FieldElement result = *this;
        FieldElement temp = *this;
        for (int i = 1; i < FIELD_SIZE; i++) {
            temp = temp.square();
            result = result + temp;
        }
        return result;
    }

    FieldElement inv() const {
        FieldElement temp = *this;
        FieldElement result("1");
        for (int i = 0; i < FIELD_SIZE - 1; i++) {
            temp = temp.square();
            result = result * temp;
        }
        return result;
    }

    string to_string() const {
        string s;
        for (int i = FIELD_SIZE - 1; i >= 0; i--) {
            s += ((coefficient[i / 64] >> (i % 64)) & 1) ? '1' : '0';
        }
        return s;
    }
};

int PolynomialDegree(uint64_t poly[]) {
    for (int i = (2 * FIELD_SIZE - 1); i >= 0; i--) {
        if ((poly[i / 64] >> (i % 64)) & 1) return i;
    }
    return 0;
}

FieldElement ReducePolynomial(uint64_t poly[]) {
    int d = PolynomialDegree(poly);
    while (d >= FIELD_SIZE) {
        for (int i : POLYNOMIAL_DEGREES) {
            int index = d + i - FIELD_SIZE;
            poly[index / 64] ^= 1ULL << (index % 64);
        }
        d = PolynomialDegree(poly);
    }
    FieldElement result;
    for (int i = 0; i <= FIELD_SIZE / 64; i++) {
        result.coefficient[i] = poly[i];
    }
    return result;
}

int main() {
    FieldElement a("11001010001111101010011001110001000111100110001010010011010010000111101010101011001001010111100000110100000111101001011001111100111100101011111001101001100000001110100000101100010101011000110010001111110101110110110110100001001010011001101100100111110000111100111100011010001101001011100101011100111100011110100000100011011110001111010011101000010011101011110"); // заміни на повноцінні бінарні рядки
    FieldElement b("00110111010000110011000101001011100001001110001110001100101100011000010101010110100011010101011111000010010011111001011011100101111100000010100001010011001100001001010011100011101100110010110100110001100001001010000100110000100100111011110110101011000110000001010001111011010101000111011100101110111101010011111011100001100100011101001011100011001000001111110");
    FieldElement n("00100011110000110100111011001001110101000001110011111010001111110010111101001000011101001001101011110110100010011101101011000100000000110000001011011111110011000000100101111101101001010101010000010110011101011001110000100011111100110000101110110010111111001101011101010101100110111111011000010101011001000001001110110110111100010010011111000011001001101100101");

    cout << "A = " << a.to_ ing() << "\n";
    cout << "B = " << b.to_string() << "\n";
    cout << "A+B = " << (a + b).to_string() << "\n";
    cout << "A*B = " << (a * b).to_string() << "\n";
    cout << "A^2 = " << a.square().to_string() << "\n";
    cout << "A^N = " << a.power(n).to_string() << "\n";
    cout << "A^-1 = " << a.inv().to_string() << "\n";
    cout << "Tr(A) = " << a.trace().to_string() << "\n";

    return 0;
}