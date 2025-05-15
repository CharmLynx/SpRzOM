// optimized_gf2m.cpp
#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

using namespace std;

const int M = 359;
const int P = 2 * M + 1;
const int WORDS = (M + 63) / 64;
using Element = uint64_t[WORDS];

void from_string(const string& bits, Element& result) {
    memset(result, 0, sizeof(Element));
    for (int i = 0; i < M && i < bits.size(); ++i) {
        if (bits[i] == '1') {
            int pos = M - 1 - i;
            result[pos / 64] |= (1ULL << (pos % 64));
        }
    }
}

string to_string(const Element& e) {
    string s;
    for (int i = M - 1; i >= 0; --i) {
        int word = i / 64;
        int bit = i % 64;
        s += ((e[word] >> bit) & 1) ? '1' : '0';
    }
    return s;
}

void zero(Element& e) {
    memset(e, 0, sizeof(Element));
}

void one(Element& e) {
    memset(e, 0, sizeof(Element));
    e[(M - 1) / 64] |= (1ULL << ((M - 1) % 64));
}

void add(const Element& a, const Element& b, Element& result) {
    for (int i = 0; i < WORDS; ++i) {
        result[i] = a[i] ^ b[i];
    }
}

void square(const Element& a, Element& result) {
    zero(result);
    for (int i = 0; i < M; ++i) {
        int from = (i + 1) % M;
        int src_word = from / 64;
        int src_bit = from % 64;
        int dst_word = i / 64;
        int dst_bit = i % 64;

        if ((a[src_word] >> src_bit) & 1)
            result[dst_word] |= (1ULL << dst_bit);
    }
}

void build_lambda_matrix(bool Lambda[M][M]) {
    int pow2[M];
    pow2[0] = 1;
    for (int i = 1; i < M; ++i)
        pow2[i] = (2 * pow2[i - 1]) % P;

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            int s1 = (pow2[i] + pow2[j]) % P;
            int s2 = (pow2[i] - pow2[j] + P) % P;
            int s3 = (-pow2[i] + pow2[j] + P) % P;
            int s4 = (-pow2[i] - pow2[j] + 2 * P) % P;
            Lambda[i][j] = (s1 == 1 || s2 == 1 || s3 == 1 || s4 == 1);
        }
    }
}

void rotate(const Element& src, Element& dst, int shift) {
    zero(dst);
    for (int i = 0; i < M; ++i) {
        int from = (i + shift) % M;
        int src_word = from / 64;
        int src_bit = from % 64;
        int dst_word = i / 64;
        int dst_bit = i % 64;

        if ((src[src_word] >> src_bit) & 1)
            dst[dst_word] |= (1ULL << dst_bit);
    }
}

void multiply(const Element& u, const Element& v, Element& z, bool Lambda[M][M]) {
    zero(z);
    Element ur, vr;

    for (int i = 0; i < M; ++i) {
        rotate(u, ur, i);
        rotate(v, vr, i);

        uint8_t acc = 0;
        for (int j = 0; j < M; ++j) {
            if (!((ur[j / 64] >> (j % 64)) & 1)) continue;
            for (int k = 0; k < M; ++k) {
                if (!((vr[k / 64] >> (k % 64)) & 1)) continue;
                if (Lambda[j][k]) acc ^= 1;
            }
        }

        if (acc) {
            int idx = (M - i) % M;
            z[idx / 64] |= (1ULL << (idx % 64));
        }
    }
}

void power(const Element& a, const Element& n, Element& result, bool Lambda[M][M]) {
    Element acc, base, temp;
    memcpy(base, a, sizeof(Element));
    zero(acc);
    one(acc);

    for (int i = M - 1; i >= 0; --i) {
        square(acc, temp);
        memcpy(acc, temp, sizeof(Element));

        int word = i / 64;
        int bit = i % 64;
        if ((n[word] >> bit) & 1) {
            multiply(acc, base, temp, Lambda);
            memcpy(acc, temp, sizeof(Element));
        }
    }

    memcpy(result, acc, sizeof(Element));
}

void make_inverse_exponent(Element& n) {
    zero(n);
    for (int i = 0; i < M - 1; ++i) {
        int word = i / 64;
        int bit = i % 64;
        n[word] |= (1ULL << bit);
    }
}

void inverse(const Element& a, Element& result, bool Lambda[M][M]) {
    Element n = {0};
    make_inverse_exponent(n);
    power(a, n, result, Lambda);
}

uint8_t trace(const Element& a) {
    uint8_t t = 0;
    for (int i = 0; i < M; ++i)
        t ^= (a[i / 64] >> (i % 64)) & 1;
    return t;
}


int main(){

    

    string a = "11001010111001010111110010110000010111100011010101111100111110011111000110000000111111011001000110000000000101111111001110011001000010011101101001000010010000001111111011100000111111010000010100111010011001111101100000001001101000100010110110011110011111110000111100011110011110011011000110010010011110011111010100001000001110101011011100111101000110110000001";
    string b = "10101011011110100101010011111110011011011000101010011000010000001011100111000100011000011100111011110010100000100100000101011010000110110111011101101001001101100111011111111011010010011101001111000000001101111000101000011011111011001101001100101011110101110011010010110011010110111101100011011001110000110100000100101010111001010101000100101110010011110100001";
    string n = "01110010110011101000100111010000110001110100010111100110010100000101011010111011011111010111011010100100010000011101000100001000100001010011110110111000111010001101000110010001100110000010001100100101110010101010010000001010001101101100111010101011000000011100010111000100010010011011100111011010001110101100111011010010011101011100111111011110011001011010101";

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
    bool Lambda[M][M] = {};
    build_lambda_matrix(Lambda);
    multiply(A, B, D, Lambda);
    cout << "A * B = " << to_string(D) << "\n";

    //квадрат
    Element A_squared = {0};
    square(A, A_squared);
    cout << "A^2 = " << to_string(A_squared) << "\n";

    //степінь n
    Element AN = {0};
    power(A, N, AN, Lambda);
    cout << "A^n = " << to_string(AN) << "\n";

    //обернений
    Element Ainv = {0};
    inverse(A, Ainv, Lambda);
    cout << "A^(-1) = " << to_string(Ainv) << "\n";

    //слід
    uint8_t Tr = trace(A);
    cout << "Tr(A) = " << (int)Tr << "\n";


    


    return 0;
}