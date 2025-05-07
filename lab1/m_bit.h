#ifndef M_BIT_H
#define M_BIT_H

#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <iomanip>

class m_bit {
public:
    static void hex_32(const std::string& hex_string, uint64_t digits[], int& count);
    static void t32_hex(uint64_t A[], std::string& a, int count);
    static void long_add(uint64_t A[], uint64_t B[], uint64_t C[], int& carry, int count);
    static void long_sub(uint64_t A[], uint64_t B[], uint64_t D[], int count);
    static void long_mul_one(uint64_t A[], uint64_t E[], uint64_t& carry, int count, uint64_t e);
    static void long_shift_to_high(uint64_t A[], uint64_t L[], int i, int count);
    static void long_mul(uint64_t A[], uint64_t B[], uint64_t F[], int count_a);
    static int long_compare(uint64_t A[], uint64_t B[], int count);
    static void length(uint64_t A[], int& count);
    static int bit_length(uint64_t A[]);
    static void long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]);
    static int bit_length_pro(uint64_t A[]);
    static void long_power(uint64_t A[], uint64_t P[], uint64_t J[], int& count_a, int count_p);
};

#endif