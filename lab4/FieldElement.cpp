#include "FieldElement.h"
#include <cstring>

using namespace std;

void FieldElement::from_string(const string& bits, Element& result) {
    memset(result, 0, M);
    for (int i = 0; i < M && i < bits.size(); ++i)
        result[i] = (bits[i] == '1') ? 1 : 0;
}

string FieldElement::to_string(const Element& e) {
    string s;
    for (int i = 0; i < M; ++i)
        s += (e[i] ? '1' : '0');
    return s;
}

void FieldElement::add(const Element& a, const Element& b, Element& result) {
    for (int i = 0; i < M; ++i) {
        result[i] = a[i] ^ b[i];
    }
}

void FieldElement::build_lambda_matrix(bool Lambda[M][M]) {
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

void FieldElement::left_rotate(const Element& src, Element& dst, int shift) {
    for (int i = 0; i < M; ++i)
        dst[i] = src[(i + shift) % M];
}

void FieldElement::multiply(const Element& u, const Element& v, Element& z, bool Lambda[M][M]) {
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
        z[(M - i) % M] = acc;
    }
}

void FieldElement::square(const Element& a, Element& result) {
    for (int i = 0; i < M; ++i)
        result[i] = a[(i - 1 + M) % M];
}

void FieldElement::power(const Element& a, const Element& n, Element& result, bool Lambda[M][M]) {
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

void FieldElement::make_inverse_exponent(Element& n) {
    for (int i = 0; i < M; ++i){
        n[i] = 1;
    }
    n[M - 1] = 0;
}

void FieldElement::inverse(const Element& a, Element& result, bool Lambda[M][M]) {
    Element n = {0};
    make_inverse_exponent(n);
    power(a, n, result, Lambda);
}

uint8_t FieldElement::trace(const Element& a) {
    uint8_t t = 0;
    for (int i = 0; i < M; ++i)
        t ^= a[i];
    return t;
}

void FieldElement::zero(Element& e) {
    memset(e, 0, M);
}

void FieldElement::one(Element& a) {
    memset(a, 0, sizeof(Element));
    a[0] = 1;
}