#include "GF_2m.h"

const int GF_2m::polynomial_degrees[5] = {0, 2, 4, 18, 359};

void GF_2m::from_string(const std::string& bits, Element& result) {
    std::memset(result, 0, sizeof(uint64_t) * WORDS);
    for (int i = 0; i < M; ++i) {
        if (bits[M - 1 - i] == '1') {
            int word = i / 64;
            int bit = i % 64;
            result[word] |= (1ULL << bit);
        }
    }
}

std::string GF_2m::to_string(const Element& a) {
    std::string res;
    for (int i = M - 1; i >= 0; --i) {
        int word = i / 64;
        int bit = i % 64;
        res += ((a[word] >> bit) & 1) ? '1' : '0';
    }
    return res;
}

void GF_2m::add(const Element& a, const Element& b, Element& result) {
    for (int i = 0; i < WORDS; ++i) {
        result[i] = a[i] ^ b[i];
    }
}

void GF_2m::reduce(uint64_t temp[], Element& result) {
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

void GF_2m::multiply(const Element& A, const Element& B, Element& result) {
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

void GF_2m::square(const Element& a, Element& result) {
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

void GF_2m::power(const Element& base, const std::string& bin_exp, Element& result) {
    Element res = {0};
    res[0] = 1;

    Element current;
    std::memcpy(current, base, sizeof(Element));

    for (int i = bin_exp.length() - 1; i >= 0; --i) {
        if (bin_exp[i] == '1') {
            Element temp;
            multiply(res, current, temp);
            std::memcpy(res, temp, sizeof(Element));
        }
        Element squared;
        square(current, squared);
        std::memcpy(current, squared, sizeof(Element));
    }

    std::memcpy(result, res, sizeof(Element));
}

void GF_2m::inverse(const Element& a, Element& result) {
    std::string s(M, '1');
    s[M - 1] = '0';
    power(a, s, result);
}

void GF_2m::trace(const Element& a, uint64_t& out_bit) {
    Element acc = {0};
    std::memcpy(acc, a, sizeof(Element));

    Element temp = {0};
    std::memcpy(temp, a, sizeof(Element));

    for (int i = 1; i < M; ++i) {
        Element squared = {0};
        square(temp, squared);
        std::memcpy(temp, squared, sizeof(Element));

        for (int j = 0; j < WORDS; ++j) {
            acc[j] ^= temp[j];
        }
    }

    out_bit = 0;
    for (int i = 0; i < M; ++i) {
        out_bit ^= (acc[i / 64] >> (i % 64)) & 1;
    }
}

void GF_2m::zero(Element& a) {
    std::memset(a, 0, sizeof(Element));
}

void GF_2m::one(Element& a) {
    std::memset(a, 0, sizeof(Element));
    a[0] = 1;
}
