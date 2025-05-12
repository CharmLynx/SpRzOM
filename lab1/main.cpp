#include "m_bit.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){

    //string b = "12345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF90123452222";
    //string a = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";   
    //string b = "123456789abcdef9";
    //string a = "abcdef9012345678";
    //string b = "123";
    //string a = "369";
    //string a = "fbcdef90";
    //string b = "12345678";
    string a = "12345678123456781234567812345699";
    //string b = "12345678123456781234567812345679";
    string b = "145afcd961278435aaacfdba12345678";
    //string a = "4";
    //string b = "1";

    int negative=0;
    //це для віднімання
    if(a<b){
        swap(a,b);
        negative=1;
    }

    uint64_t A[64]={0};
    int count_a=0;

    uint64_t B[64]={0};
    int count_b=0;
    
    auto start = high_resolution_clock::now();
    m_bit::hex_32(a, A, count_a);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання hex_32: " << duration.count() << " мкс" << endl;

    m_bit::hex_32(b, B, count_b);

    int carry;
    uint64_t C[64];
    string c;
    

    start = high_resolution_clock::now();
    m_bit::long_add(A, B, C, carry, count_a);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_add: " << duration.count() << " мкс" << endl;

    cout<<"add ";
    if(carry!=0){
        for (int j = count_a; j >=0; j--){
            cout << hex << C[j]<<"";
        }
    }
    else{
        for (int j = count_a-1; j >=0; j--){
            cout << hex << C[j]<<"";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<endl;

    //віднімання
    uint64_t S[64];
    start = high_resolution_clock::now();
    m_bit::long_sub(A, B, S, count_a);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_sub: " << duration.count() << " мкс" << endl;
    cout<<"sub ";
    if(negative==1) cout<<"-";
    for (int j = count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << S[j]<<"";
    }
    cout<<endl;
    cout<<endl;
    if (negative==1){
        swap(A, B);
        swap(count_a, count_b);
    }

    //множення
    uint64_t M[128]={0};
    start = high_resolution_clock::now();
    m_bit::long_mul(A, B, M, count_a);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mul: " << duration.count() << " мкс" << endl;
    cout<<"mull"<<endl;

    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << M[j]<<"";
    }
    cout<<endl;
    cout<<endl;

    //квадрат
    uint64_t G[128]={0};
    start = high_resolution_clock::now();
    m_bit::long_mul(A, A, G, count_a);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mul для квадрату: " << duration.count() << " мкс" << endl;
    cout<<"^2"<<endl;
    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << G[j]<<"";
    }
    cout<<endl;
    cout<<endl;

    //ділення
    uint64_t R[64]={0};
    uint64_t Q[64] = {0};
    start = high_resolution_clock::now();
    m_bit::long_div(A, B, count_a, count_b, Q, R);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_div: " << duration.count() << " мкс" << endl;

    
    cout<<"div "<<endl;
    cout << "q ";
    for (int j = m_bit::bit_length(Q)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << Q[j];
    }
    cout << endl;
    cout << "r ";
    for (int j = count_a - 1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << R[j];
    }
    cout << endl;
    cout<<endl;

    //багаторозрядний степінь
    string p = "3";
    int count_p;
    uint64_t P[64] = {0};
    m_bit::hex_32(p, P, count_p);
    uint64_t J[128] ={0};
    int count_a1 =count_a;
    start = high_resolution_clock::now();
    m_bit::long_power(A, P, J, count_a1, count_p);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_power: " << duration.count() << " мкс" << endl;

    cout<<"a^p "<<endl;
    
    for (int j = m_bit::bit_length(J)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << J[j];
    }
    cout << endl;

    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    m_bit::hex_32(a,A,count_a);
    m_bit::hex_32(b,B,count_b);
    
    //gcd
    uint64_t D[64]={0};
    m_bit::long_gcd(A, B, D);
    int count_d=m_bit::bit_length(D);
    cout<<"gcd ";
    for (int j = count_d-1; j >=0; j--) {
        cout << setw(8) << hex << D[j];
    }
    cout<<endl;
    cout<<endl;
    
    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    m_bit::hex_32(a,A,count_a);
    m_bit::hex_32(b,B,count_b);

    //lcm
    uint64_t Ql[64] = {0};
    m_bit::long_lcm(M, D, Ql);
    cout<<"lcm ";
    for (int j = m_bit::bit_length(Ql)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << Ql[j];
    }
    cout<<endl;
    cout<<endl;

    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    m_bit::hex_32(a,A,count_a);
    m_bit::hex_32(b,B,count_b);

    
    //barret reduction
    //мю
    string n = "1234567812345678";
    uint64_t N[64] = {0};      
    int count_n=0;
    m_bit::hex_32(n, N, count_n);
    uint64_t MU[128] = {0};            
    int count_mu = 0;
    m_bit::compute_mu(N, count_n, MU, count_mu);

    //+ mod
    uint64_t r[64]={0};
    m_bit::long_mod_add(A, B, N, MU, r);
    cout<<"(a+b) mod n "<<endl;
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //- mod
    if(a<b){
        swap(a,b);
        negative=1;
    }
    m_bit::long_mod_sub(A, B, N, MU, r, negative);
    if (negative==1){
        swap(A, B);
        swap(count_a, count_b);
    }
    cout<<"(a-b) mod n "<<endl;
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //* mod
    cout<<"(a*b) mod n "<<endl;
    m_bit::long_mod_mul(A, B, N, MU, r);
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //^2 mod
    cout<<"(a*a) mod n "<<endl;
    m_bit::long_mod_mul(A, A, N, MU, r);
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //^n mod
    m_bit::long_mod_pow(A, P, N, MU, r);
    cout<<"(a^b) mod n "<<endl;
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;

    


    return 0;
} 
