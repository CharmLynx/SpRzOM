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
    static int bit_length_pro(uint64_t A[]);
    static int bit_length_proо(uint64_t A[]);

    static void bit_shift_to_high(uint64_t src[], uint64_t dst[], int shift_bits);
    static void long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]);

    static void long_power(uint64_t A[], uint64_t P[], uint64_t J[], int& count_a, int count_p);

    static void long_div2(uint64_t A[], int count);
    static void long_mul2(uint64_t A[], int& count);
    static void long_gcd(uint64_t A[], uint64_t B[], uint64_t D[]);
    static void long_lcm(uint64_t M[], uint64_t D[], uint64_t Q[]);

    static void kill_last_digits(uint64_t src[], int src_len, int digits_to_kill, uint64_t dest[]);
    static void compute_mu(uint64_t n[], int count_n, uint64_t mu[], int& count_mu);
    static void barret_reduction(uint64_t A[], uint64_t N[], uint64_t r[], uint64_t MU[]);

    static void long_mod_add(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[]);
    static void long_mod_sub(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[], int negative);
    static void long_mod_mul(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[]);
    static void long_mod_pow(uint64_t A[], uint64_t E[], uint64_t N[], uint64_t MU[], uint64_t R[]);
};

#endif