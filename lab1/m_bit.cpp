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
        if(A[i]<=B[i]){
            D[i]=(4294967296)+A[i]-B[i]-borrow;
            borrow=1;
        }
        else{
            D[i]=A[i]-B[i]-borrow;
            borrow=0;
        }
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
        }
        int carry_mm=0;
        long_add(F, shifted, F, carry_mm, count_sh);
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

void m_bit::long_div(uint64_t A[], uint64_t B[], int count_a, int count_b, uint64_t Q[], uint64_t R[]){
    uint64_t H[64] = {0};
    for (int j = 0; j < count_a; j++) {
        R[j]=A[j];
    }
    int count_r = 0;
    length(R, count_r);
    int k = bit_length(B);
    int t;
    int bitpos;
    int word_index;
    int bit_index;
    while((long_compare(R, B, count_r))>=0){
        t=bit_length(R);
        long_shift_to_high(B, H, t-k, count_b);
        if(long_compare(R, H, count_r) < 0){
            t=t-1;
            long_shift_to_high(B, H, t-k, count_b);
        }
        length(R, count_r); 
        long_sub(R, H, R, count_r);
        bitpos = t - k;
        word_index = bitpos / 32;
        bit_index  = bitpos % 32;
        uint64_t carry_q = 1ULL << bit_index;

        for (int i = word_index; i < 64 && carry_q != 0; i++) {
            uint64_t temp = Q[i] + carry_q;
            carry_q = (temp < Q[i]) ? 1 : 0;
            Q[i] = temp;
        }
    }
}
int m_bit::bit_length_pro(uint64_t A[]) {
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

    int bit_len = bit_length_pro(P);
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
