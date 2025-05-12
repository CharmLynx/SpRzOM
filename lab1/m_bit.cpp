#include "m_bit.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;

void m_bit::hex_32(const string& hex_string, uint64_t digits[], int& count){

    int hex_length = hex_string.length();
    string part(8,'0');
    while(hex_length>0){
        if(hex_length>=8){
            for(int i=0;i<8;i++){
                part.at(i)=hex_string.at(hex_length-8+i);
            }
            hex_length=hex_length-8;
        }
        else{
            for(int i=0;i<8-hex_length;i++){
                part.at(i)='0';
            }
            for (int j=0; j<hex_length; j++) {
                part.at(8-hex_length+j) = hex_string.at(j);
            }
            hex_length=0;

        }
        uint32_t value = stoul(part, nullptr, 16);
        digits[count]=value;
        count++;
    }
}

void m_bit::t32_hex(uint64_t A[], string& a, int count){
    for(int i =count;i>=0;i--){
        stringstream ss;
        ss << setfill('0') << setw(8) << hex << A[i];
        a += ss.str();
    }
}

void m_bit::long_add(uint64_t A[], uint64_t B[], uint64_t C[], int& carry, int count){
    carry=0;   
    uint64_t temp; 
    int k=0;
    for(int i=0;i<=count-1;i++){
        temp=A[i]+B[i]+carry;
        C[i]=temp&(4294967296-1);
        C[i] = temp & 0xFFFFFFFF;
        carry=temp >> 32;
        k++;
    }
    if(carry!=0)C[k]=carry;
}

void m_bit::long_sub(uint64_t A[], uint64_t B[], uint64_t D[],int count){
    int borrow=0;
    int32_t temp;
    for(int i=0; i<=count-1; i++){
        if (A[i] < B[i] + borrow){
            D[i]=(4294967296)+A[i]-B[i]-borrow;
            borrow=1;
        }
        else{
            D[i]=A[i]-B[i]-borrow;
            borrow=0;
        }
        D[i] &= 0xFFFFFFFF;
    }

}

void m_bit::long_mul_one(uint64_t A[], uint64_t E[], uint64_t& carry, int count, uint64_t e){
    uint64_t temp; 
    int k=0;
    for(int i=0; i<=count-1;i++){
        temp=A[i]*e+carry;
        E[i]=temp&(4294967296-1);
        carry = temp >> 32;
        k++;
    }
    if(carry!=0)E[k]=carry;
}

void m_bit::long_shift_to_high(uint64_t A[], uint64_t L[], int i, int count){
    uint64_t n = 00000000;
    for(int j=0; j<i; j++){
        L[j]=n;
    }
    for(int j=i;j<=count+i;j++){
        L[j]=A[j-i];
    }
}

void m_bit::long_mul(uint64_t A[], uint64_t B[], uint64_t F[], int count_a){
    uint64_t shifted[128] = {0};
    uint64_t E[128] ={0};
    int count_sh=0;
    int count_e;
    int carry_mm;
    uint64_t carry_m;
    int carry;
    for(int i=0; i<=count_a-1;i++){
        
        carry_m=0;
        carry_mm=0;
        long_mul_one(A, E, carry_m, count_a, B[i]);
        if(carry_m!=0){
            long_shift_to_high(E, shifted, i, count_a);
            long_add(F, shifted, F, carry_mm, count_a+i+1);
        }
        else{
            long_shift_to_high(E, shifted, i, count_a-1);
            long_add(F, shifted, F, carry_mm, count_a+i);
        int carry_mm=0;
        long_add(F, shifted, F, carry_mm, count_sh);
        }
    }
}

int m_bit::long_compare(uint64_t A[], uint64_t B[], int count) {
    for (int i = count - 1; i >= 0; i--) {
        if (A[i] < B[i]){
            return -1;
            break;
        }
        if (A[i] > B[i]){
            return 1;
            break;
        }
    }
    return 0;
}

void m_bit::length(uint64_t A[], int& count){
    count=0;
    for(int i=0;i<64;i++){
        if(A[i]!=0){
            count++;
        }
        else{
            break;
        }
    }

}

int m_bit::bit_length(uint64_t A[]) {
    int count=0;
    for (int i = 63; i >= 0; i--) {
        if (A[i] != 0) {
            return i + 1;
        }
    }
    return 0;
}

int m_bit::bit_length_pro(uint64_t A[]) {
    for (int i = 63; i >= 0; i--) {
        if (A[i] != 0) {
            for (int j = 31; j >= 0; j--) {
                if ((A[i] >> j) & 1) {
                    return i * 32 + j + 1;
                }
            }
        }
    }
    return 0;
}

void m_bit::bit_shift_to_high(uint64_t src[], uint64_t dst[], int shift_bits) {
    for (int i = 0; i < 64; i++) dst[i] = 0;

    int word_shift = shift_bits / 32;
    int bit_shift = shift_bits % 32;

    for (int i = 63; i >= 0; i--) {
        if (i - word_shift < 0) break;

        dst[i] |= (src[i - word_shift] << bit_shift) & 0xFFFFFFFF;

        if (bit_shift != 0 && i - word_shift - 1 >= 0) {
            dst[i] |= (src[i - word_shift - 1] >> (32 - bit_shift)) & 0xFFFFFFFF;
        }
    }
}

void m_bit::long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]) {
    uint64_t H[64] = {0};
    for (int j = 0; j < count_a; j++){
        R[j] = A[j];
    }
    int k = bit_length_pro(B);
    int count_r = bit_length_pro(R);
    while (long_compare(R, B, max(count_a, count_b)) >= 0) {
        int t = bit_length_pro(R);
        int bitpos = t - k;
        if (bitpos < 0) break;
        bit_shift_to_high(B, H, bitpos);
        if (long_compare(R, H, 64) < 0) {
            bitpos--;
            if (bitpos < 0) break;
            bit_shift_to_high(B, H, bitpos);
            if (long_compare(R, H, 64) < 0) break;
        }
        long_sub(R, H, R, 64);
        if (bitpos / 32 < 64)
            Q[bitpos / 32] |= (1ULL << (bitpos % 32));
        count_r = bit_length_pro(R);
    }
}

int m_bit::bit_length_proо(uint64_t A[]) {
    for (int i = 63; i >= 0; i--) {
        if (A[i] != 0) {
            for (int j = 63; j >= 0; j--) {
                if ((A[i] >> j) & 1) {
                    return i * 64 + j + 1;
                }
            }
        }
    }
    return 0;
}

void m_bit::long_power(uint64_t A[], uint64_t P[], uint64_t J[], int& count_a, int count_p){
    string j = "1";
    int count_j=0;
    hex_32(j, J,count_j);
    uint64_t H[128]={0};
    uint64_t K[128]={0};
    int count_k;
    int bit_len = bit_length_proо(P);
    for (int i = 0; i < bit_len; i++) {
        if ((P[i / 32] >> (i % 32)) & 1) {
            for(int i=0; i<64; i++) H[i] = 0;
            long_mul(A, J, H, bit_length(A));
            for(int i=0; i<128; i++) J[i] = 0;    
            for(int i=0; i<=2*count_a-1; i++){
                J[i]=H[i];
            }
        }
        for(int i=0; i<128; i++) K[i] = 0;
        long_mul(A, A, K, count_a);
        count_a =2*bit_length(A);
        count_k=count_a;
        for(int i=0; i<64; i++) A[i] = 0;
        for(int i=0; i<count_k; i++){
            A[i]=K[i];
        }
        count_a=count_k;
    }
}

void m_bit::long_div2(uint64_t A[], int count) {
    uint32_t carry = 0;
    for (int i = count - 1; i >= 0; i--) {
        uint32_t current = static_cast<uint32_t>(A[i]);
        A[i] = ((current >> 1) | (static_cast<uint64_t>(carry) << 31)) & 0xFFFFFFFF;
        carry = current & 1;
    }
}

void m_bit::long_mul2(uint64_t A[], int& count) {
    uint64_t carry = 0;

    for (int i = 0; i < count; ++i) {
        uint64_t current = A[i];
        uint64_t new_carry = (current >> 31) & 1;
        A[i] = ((current << 1) | carry) & 0xFFFFFFFF;
        carry = new_carry;
    }
    if (carry != 0) {
        A[count] = carry;
        count++;
    }
}

void m_bit::long_gcd(uint64_t A[], uint64_t B[], uint64_t D[]){
    int count_a = bit_length(A);
    int count_b = bit_length(B);
    string d = "1";
    int count_d=0;
    hex_32(d, D, count_d);
    uint64_t T[64] ={0};
    while(((A[0] & 1) == 0)&&((B[0] & 1) == 0)){
        count_a=bit_length(A);
        long_div2(A, count_a);
        count_b=bit_length(B);
        long_div2(B,count_b);
        count_d=bit_length(D);
        long_mul2(D, count_d);
        
    }
    while((A[0] & 1)==0){
        count_a=bit_length(A);
        long_div2(A, count_a);
    }
    while(bit_length(B) != 0){
        while((B[0] & 1)==0){
            count_b=bit_length(B);
            long_div2(B, count_b);
        }
        count_a=bit_length(A);
        count_b=bit_length(B);
        if (long_compare(A, B, max( count_a, count_b)) <= 0) {
            count_a = bit_length(A);
            count_b = bit_length(B);
            long_sub(B, A, B, count_b);            
            count_a = bit_length(A);
            count_b = bit_length(B);
        } else {
            for (int i = 0; i < max(count_a, count_b); i++) {
                uint64_t temp = A[i];
                A[i] = B[i];
                B[i] = temp;
            }
            swap(count_a, count_b);
            count_b=bit_length(B);
            long_sub(B, A, T, count_b);
            count_b=bit_length(B);
        }
    
    count_a=bit_length(A);
    count_b=bit_length(B);
    }
    for(int i=0; i<64; i++) T[i] = 0;
    long_mul(D, A, T, count_a);
    for(int i=0; i<64; i++) D[i] = 0;
    for (int j = bit_length(T)-1; j >=0; j--) {
        D[j]=T[j];
    }

}

void m_bit::long_lcm(uint64_t M[], uint64_t D[], uint64_t Q[]){
    uint64_t R[64] = {0};
    int count_m=bit_length(M);
    int count_d=bit_length(D);
    long_div(M, D,count_m, count_d, Q, R);
}

void m_bit::kill_last_digits(uint64_t src[], int src_len, int digits_to_kill, uint64_t dest[]) {
    int j = 0;
    for (int i = digits_to_kill; i < src_len; i++) {
        dest[j++] = src[i];
    }
    while (j < 64) {
        dest[j++] = 0;
    }
}

void m_bit::compute_mu(uint64_t n[], int count_n, uint64_t mu[], int& count_mu) {
    uint64_t beta2k[64] = {0};
    beta2k[2 * count_n] = 1;

    uint64_t rem[64] = {0};
    long_div(beta2k, n, 2 * count_n + 1, count_n, mu, rem);

    count_mu = bit_length_pro(mu);
}
void m_bit::barret_reduction(uint64_t A[], uint64_t N[], uint64_t r[], uint64_t MU[]){
    uint64_t q1[64]={0};
    uint64_t q2[64]={0};
    uint64_t q3[64]={0};
    uint64_t qn[64]={0};
    int count_a =bit_length(A);
    int count_n = bit_length(N);
    kill_last_digits(A,count_a, count_n-1, q1);
    long_mul(q1, MU, q2, bit_length(q1));
    kill_last_digits(q2,bit_length(q2), count_n+1, q3);
    
    long_mul(q3, N, qn, bit_length(q3));
    long_sub(A, qn, r, count_a);
    while(long_compare(r, N, max(bit_length(r), count_n))>=0){
        long_sub(r, N,r,bit_length(r));
    }
}

void m_bit::long_mod_add(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[]) {
    uint64_t tmp[64] = {0};
    int carry = 0;
    long_add(A, B, tmp, carry, max(bit_length(A), bit_length(B)));
    barret_reduction(tmp, N, R, MU);
}

void m_bit::long_mod_sub(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[], int negative) {
    uint64_t tmp[64] = {0};
    long_sub(A, B, tmp, max(bit_length(A), bit_length(B)));
    if(negative==1){    
        barret_reduction(tmp, N, R, MU);
        long_sub(N, R, R, bit_length(N));
    }
    else{
        barret_reduction(tmp, N, R, MU);
    }
}

void m_bit::long_mod_mul(uint64_t A[], uint64_t B[], uint64_t N[], uint64_t MU[], uint64_t R[]) {
    uint64_t A_mod[64] = {0};
    uint64_t B_mod[64] = {0};
    uint64_t product[64] = {0};
    barret_reduction(A, N, A_mod, MU);
    barret_reduction(B, N, B_mod, MU);
    if((bit_length(A_mod)==0)||(bit_length(A_mod)==0)){
        cout<<"0"<<endl;
    }
    else{
        int count = max(bit_length(A_mod), bit_length(B_mod));
        long_mul(A_mod, B_mod, product, count );
        barret_reduction(product, N, R, MU);
        
    }
}
void m_bit::long_mod_pow(uint64_t A[], uint64_t E[], uint64_t N[], uint64_t MU[], uint64_t R[]) {
    uint64_t base[64] = {0};
    uint64_t result[64] = {0};
    uint64_t tmp[64] = {0};
    result[0] = 1;
    barret_reduction(A, N, base, MU);
    int bitlen = bit_length_pro(E);
    for (int i = 0; i < bitlen; i++) {
        if ((E[i / 32] >> (i % 32)) & 1) {
            long_mod_mul(result, base, N, MU, tmp);
            for (int j = 0; j < 64; j++) result[j] = tmp[j];
        }
        long_mod_mul(base, base, N, MU, tmp);
        for (int j = 0; j < 64; j++) base[j] = tmp[j];
    }
    for (int j = 0; j < 64; j++) R[j] = result[j];
}