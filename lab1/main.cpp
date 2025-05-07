#include "m_bit.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){

    //string a ="fffffffe26019bbe3021d17424135fe8cfe865df3c00aa342bfe30aacef58e91266dce2714434c546b9932a0926496b5db81da8eaf31a899151348d12b347fa6d132f5e8db99007fbff5063f1452c4a3760f426863b4a7492a7d2214acffd25b90b635a1723e6b5e849b556d809a5d9bbcd9a72e7047b9419199327eb80b4d4af130a556f430e51723d999f39ebc9e81b48f3cf120dec7f22ef495613393a531bcc57d18f6f85f0fed5c74c84346b749a8b0f78b0715ff547c120b7a0bb76248c828042070e6b57334b29526aa11c61a33eb3cc6a1f461e08116f07767773375c5b474253f4b9291e4d2281f171644e80198cdf86cac333f394abd21c1c8fc04";

    //string a = "fffffffe26019bbe3021d17424135fe8cfe865df3c00aa342bfe30aacef58e91266dce2714434c546b9932a0926496b5db81da8eaf31a899151348d12b347fa6d132f5e8db99007fbff5063f1452c4a3760f426863b4a7492a7d2214acffd25b90b635a1723e6b5e849b556d809a5d9bbcd9a72e7047b9419199327eb80b4d4af130a556f430e51723d999f39ebc9e81b48f3cf120dec7f22ef495613393a531bcc57d18f6f85f0fed5c74c84346b749a8b0f78b0715ff547c120b7a0bb76248c828042070e6b57334b29526aa11c61a33eb3cc6a1f461e08116f07767773375c5b474253f4b9291e4d2281f171644e80198cdf86cac333f394abd21c1c8fc04";
    //string b = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";

    string b = "12345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF9012345ABCDEF90123452222";
    string a = "ffffffff1300cddeaa5d2750e2fabe17bc2289f575609de72dbd34d03ad2be472abec4f8cdb6653a8459867f72ff4840e9de7e9e3b8a08ce0427d24f14acf4f2ef1ace93e8b3ee9ec59f508c4e919a8a2e5cd550df1e31b387c67397f36423795907cc0c8a38f46c26979782030a9b5475db2902fac12161cc1ae853d68e00fe";   
    //string b = "123456789abcdef9";
    //string a = "abcdef9012345678";
    //string a = "fbcdef90";
    //string b = "12345678";
    //string a = "12345678123456781234567812345679";
    //string b = "12345678123456781234567812345679";
    //string b = "145afcd961278435aaacfdba12345678";
    //string a = "1";
    //string b = "0";
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
    uint64_t D[64];
    start = high_resolution_clock::now();
    m_bit::long_sub(A, B, D, count_a);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_sub: " << duration.count() << " мкс" << endl;
    cout<<"sub ";
    if(negative==1) cout<<"-";
    for (int j = count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << D[j]<<"";
    }
    cout<<endl;
    cout<<endl;
    if (negative==1){
        swap(A, B);
        swap(count_a, count_b);
    }

    //множення
    uint64_t F[128]={0};
    start = high_resolution_clock::now();
    m_bit::long_mul(A, B, F, count_a);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання long_mul: " << duration.count() << " мкс" << endl;
    cout<<"mull"<<endl;

    for (int j = 2*count_a-1; j >=0; j--){
        cout <<setfill('0') << setw(8)<< hex << F[j]<<"";
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

    


    return 0;
} 
