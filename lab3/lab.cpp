#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <bitset>

using namespace std;

const int M = 359;
const int WORDS = (M + 63) / 64;
using Element = uint64_t[WORDS];
const int polynomial_degrees[5] = {0, 2, 4, 18, 359};

void from_string(const string& bits, Element& result) {
    memset(result, 0, sizeof(uint64_t) * WORDS);
    for (int i = 0; i < M; ++i) {
        if (bits[M - 1 - i] == '1') {
            int word = i / 64;
            int bit = i % 64;
            result[word] |= (1ULL << bit);
        }
    }
}

string to_string(const Element& a) {
    string res;
    for (int i = M - 1; i >= 0; --i) {
        int word = i / 64;
        int bit = i % 64;
        res += ((a[word] >> bit) & 1) ? '1' : '0';
    }
    return res;
}

void add(const Element& a, const Element& b, Element& result) {
    for (int i = 0; i < WORDS; ++i) {
        result[i] = a[i] ^ b[i];
    }
}

void reduce(uint64_t temp[], Element& result) {
    for (int d = 2 * M - 2; d >= M; --d) {
        if ((temp[d / 64] >> (d % 64)) & 1) {
            for (int k = 0; k < 5; ++k) {
                int shift = polynomial_degrees[k];
                int pos = d - M + shift;
                temp[pos / 64] ^= (1ULL << (pos % 64));
            }
            temp[d / 64] &= ~(1ULL << (d % 64));
        }
    }
    for (int i = 0; i < WORDS; ++i)
        result[i] = temp[i];
}

void multiply(const Element& A, const Element& B, Element& result) {
    const int TW = 2 * WORDS;
    uint64_t temp[TW] = {0};

    for (int i = 0; i < M; ++i) {
        if ((A[i / 64] >> (i % 64)) & 1) {
            for (int j = 0; j < M; ++j) {
                if ((B[j / 64] >> (j % 64)) & 1) {
                    int pos = i + j;
                    temp[pos / 64] ^= (1ULL << (pos % 64));
                }
            }
        }
    }

    reduce(temp, result);
}

void square(const Element& a, Element& result) {
    const int TW = 2 * WORDS;
    uint64_t temp[TW] = {0};

    for (int i = 0; i < M; ++i) {
        if ((a[i / 64] >> (i % 64)) & 1) {
            int pos = 2 * i;
            temp[pos / 64] |= (1ULL << (pos % 64));
        }
    }

    reduce(temp, result);
}

void power(const Element& base, const string& bin_exp, Element& result) {
    Element res = {0};
    res[0] = 1;

    Element current;
    memcpy(current, base, sizeof(Element));

    for (int i = bin_exp.length() - 1; i >= 0; --i) {
        if (bin_exp[i] == '1') {
            Element temp;
            multiply(res, current, temp);
            memcpy(res, temp, sizeof(Element));
        }
        Element squared;
        square(current, squared);
        memcpy(current, squared, sizeof(Element));
    }

    memcpy(result, res, sizeof(Element));
}

string inverse_exponent() {
    string s(M, '1');
    s[M - 1] = '0';
    return s;
}

void inverse(const Element& a, Element& result) {
    string exp = inverse_exponent();
    power(a, exp, result);
}

void trace(const Element& a, uint64_t& out_bit) {
    Element acc = {0};
    memcpy(acc, a, sizeof(Element));

    Element temp = {0};
    memcpy(temp, a, sizeof(Element));

    for (int i = 1; i < M; ++i) {
        Element squared = {0};
        square(temp, squared);
        memcpy(temp, squared, sizeof(Element));

        for (int j = 0; j < WORDS; ++j) {
            acc[j] ^= temp[j];
        }
    }

    out_bit = 0;
    for (int i = 0; i < M; ++i) {
        out_bit ^= (acc[i / 64] >> (i % 64)) & 1;
    }
}

void zero(Element& a) {
    memset(a, 0, sizeof(Element));
}

void one(Element& a) {
    memset(a, 0, sizeof(Element));
    a[0] = 1;
}



int main(){
    string a = "11001010001111101010011001110001000111100110001010010011010010000111101010101011001001010111100000110100000111101001011001111100111100101011111001101001100000001110100000101100010101011000110010001111110101110110110110100001001010011001101100100111110000111100111100011010001101001011100101011100111100011110100000100011011110001111010011101000010011101011110";
    string b = "00110111010000110011000101001011100001001110001110001100101100011000010101010110100011010101011111000010010011111001011011100101111100000010100001010011001100001001010011100011101100110010110100110001100001001010000100110000100100111011110110101011000110000001010001111011010101000111011100101110111101010011111011100001100100011101001011100011001000001111110";
    string n = "00100011110000110100111011001001110101000001110011111010001111110010111101001000011101001001101011110110100010011101101011000100000000110000001011011111110011000000100101111101101001010101010000010110011101011001110000100011111100110000101110110010111111001101011101010101100110111111011000010101011001000001001110110110111100010010011111000011001001101100101";

    Element A = {0}, B = {0}, N = {0};

    from_string(a, A);
    from_string(b, B);
    from_string(n, N);
    


    //додавання
    Element C = {0};
    add(A, B, C);
    cout << "A + B = " << to_string(C) << "\n";

    //множення
    Element D = {0};
    multiply(A, B, D);
    cout << "A * B = " << to_string(D) << "\n";

    //квадрат
    Element A_squared = {0};
    square(A, A_squared);
    cout << "A^2 = " << to_string(A_squared) << endl;

    //степінь n
    Element A_pow = {0};
    power(A, n, A_pow);
    cout << "A^N = " << to_string(A_pow) << "\n";

    //обернений
    Element A_inv = {0};
    inverse(A, A_inv);
    cout << "A^-1  = " << to_string(A_inv) << "\n";

    //слід
    uint64_t Tr = 0;
    trace(A, Tr);
    cout << "Trace(A) = " << Tr << "\n"; // результат: 0 або 1



}