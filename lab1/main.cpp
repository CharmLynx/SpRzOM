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
    //string a = "12345678123456781234567812345699";
    //string b = "12345678123456781234567812345679";
    //string b = "145afcd961278435aaacfdba12345678";
    //string a = "4";
    //string b = "1";
    string a ="22db0e8ffda60911f0dc9b498b1ff7d3bada705c0006639ecf399987ed982b9c3d3c35ca8fe12ce207c2f2a8b0a926a151f4fe2d5b086159000be263714ad84d3b927071ee6dc7241921dddb1ea0c3e97c2a87fe72a0f7c5e1a9d63352630fd6503877dd25d7e925d56390cce265982d663d4116e6549cea4d9a5892b1ed093a";
    string b = "1db4de4688e9d4e3629c0dc4ccf03cdabc5f1399667b1fe21566b12aa0906bd7fb3f1aec02c0e509951e66c2658d6a5011c3985113174fe4720e3b5ffc9185d9bd596ff80f0b922a955cf81e4a3a81978f2b6ce394fb3b7cb4f738e31b7aced8f1f352740da93208907a7d9902246d5b9e74d24fb165d53e889d80cb78130d73";

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
    start = high_resolution_clock::now();
    m_bit::long_gcd(A, B, D);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_gcd: " << duration.count() << " мкс" << endl;
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
    start = high_resolution_clock::now();
    m_bit::long_lcm(M, D, Ql);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_lcm: " << duration.count() << " мкс" << endl;
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
    //string n = "1234567812345678";
    string n = "6c4d85eca23bafeba8e4eb3d6319df32dcf5a929839d965e79164f19e38847f09021cdcdcf952f1a89c344ebc637362d1d7cf4c870c32275d0c4ed10fd3a376d616b019e037a05fd22ea31262beb2c85126fdb6ca3b56fab0de535497c7334e2feeb170db6724a469763bb4ff79a45f91e8cd11e9646c11e5833e3550298c701";
    uint64_t N[64] = {0};      
    int count_n=0;
    m_bit::hex_32(n, N, count_n);
    uint64_t MU[128] = {0};            
    int count_mu = 0;
    start = high_resolution_clock::now();
    m_bit::compute_mu(N, count_n, MU, count_mu);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання compute_mu: " << duration.count() << " мкс" << endl;
    

    //+ mod
    uint64_t r[64]={0};
    start = high_resolution_clock::now();
    m_bit::long_mod_add(A, B, N, MU, r);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mod_add: " << duration.count() << " мкс" << endl;
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
    start = high_resolution_clock::now();
    m_bit::long_mod_sub(A, B, N, MU, r, negative);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mod_sub: " << duration.count() << " мкс" << endl;
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
    start = high_resolution_clock::now();
    m_bit::long_mod_mul(A, B, N, MU, r);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mod_mul: " << duration.count() << " мкс" << endl;
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    //^2 mod
    cout<<"(a*a) mod n "<<endl;
    start = high_resolution_clock::now();
    m_bit::long_mod_mul(A, A, N, MU, r);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mod_mul для квадрату: " << duration.count() << " мкс" << endl;
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;
    cout<<endl;
    for(int i=0; i<64; i++) r[i] = 0;

    for(int i=0; i<64; i++) A[i] = 0;
    for(int i=0; i<64; i++) B[i] = 0;
    count_a=0;
    count_b=0;
    m_bit::hex_32(a,A,count_a);
    m_bit::hex_32(b,B,count_b);

    //^n mod
    start = high_resolution_clock::now();
    m_bit::long_mod_pow(A, B, N, MU, r);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mod_pow: " << duration.count() << " мкс" << endl;
    cout<<"(a^b) mod n "<<endl;
    for (int j = m_bit::bit_length(r)-1; j >= 0; j--) {
        cout <<setfill('0') << setw(8)<< hex << r[j];
    }
    cout<<endl;

    


    return 0;
} 
