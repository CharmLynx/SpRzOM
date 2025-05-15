#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <bitset>

using namespace std;

const int M = 359;
const int P = 2 * M + 1;

using Element = uint8_t[M];


void from_string(const string& bits, Element& result) {
    memset(result, 0, M);
    for (int i = 0; i < M && i < bits.size(); ++i)
        result[i] = (bits[i] == '1') ? 1 : 0;
}



string to_string(const Element& e) {
    string s;
    for (int i = 0; i < M; ++i)
        s += (e[i] ? '1' : '0');
    return s;
}

void add(const Element& a, const Element& b, Element& result) {
    for (int i = 0; i < M; ++i) {
        result[i] = a[i] ^ b[i];
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

void left_rotate(const Element& src, Element& dst, int shift) {
    for (int i = 0; i < M; ++i)
        dst[i] = src[(i + shift) % M];
}

void multiply(const Element& u, const Element& v, Element& z, bool Lambda[M][M]) {
    for (int i = 0; i < M; ++i) {
        uint8_t acc = 0;
        for (int j = 0; j < M; ++j) {
            if (!u[j]) continue;
            for (int k = 0; k < M; ++k) {
                if (!v[k]) continue;
                if (Lambda[(j + i) % M][(k + i) % M]) {
                    acc ^= 1;
                }
            }
        }
        //z[M - 1 - i] = acc;
        z[(M - i) % M] = acc;
    }
}

void square(const Element& a, Element& result) {
    for (int i = 0; i < M; ++i)
        result[i] = a[(i - 1 + M) % M];
}


void power(const Element& a, const Element& n, Element& result, bool Lambda[M][M]) {
    Element acc, base, temp;
    memcpy(base, a, M);
    memset(acc, 0, M);
    acc[0] = 1;
    bool started = false;
    for (int i = 0; i < M; ++i) {
        if (started) {
            square(acc, temp);
            memcpy(acc, temp, M);
        }
        if (n[i]) {
            if (started) {
                multiply(acc, base, temp, Lambda);
                memcpy(acc, temp, M);
            } else {
                memcpy(acc, base, M);
                started = true;
            }
        }
    }
    memcpy(result, acc, M);
}

void make_inverse_exponent(Element& n) {
    for (int i = 0; i < M; ++i){
        n[i] = 1;
    }
    n[M - 1] = 0;
}

void inverse(const Element& a, Element& result, bool Lambda[M][M]) {
    Element n = {0};
    make_inverse_exponent(n);
    power(a, n, result, Lambda);
}

uint8_t trace(const Element& a) {
    uint8_t t = 0;
    for (int i = 0; i < M; ++i)
        t ^= a[i];
    return t;
}

void zero(Element& e) {
    memset(e, 0, M);
}

void one(Element& a) {
    memset(a, 0, sizeof(Element));
    a[0] = 1;
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
    Element A_inv = {0};
    inverse(A, A_inv, Lambda);
    cout << "A^(-1) = " << to_string(A_inv) << "\n";

    //слід
    uint8_t Tr = trace(A);
    cout << "Tr(A) = " << (int)Tr << "\n";

    return 0;
}