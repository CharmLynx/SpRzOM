#include "GF_2m.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include <bitset>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){
    string a = "11001010001111101010011001110001000111100110001010010011010010000111101010101011001001010111100000110100000111101001011001111100111100101011111001101001100000001110100000101100010101011000110010001111110101110110110110100001001010011001101100100111110000111100111100011010001101001011100101011100111100011110100000100011011110001111010011101000010011101011110";
    string b = "00110111010000110011000101001011100001001110001110001100101100011000010101010110100011010101011111000010010011111001011011100101111100000010100001010011001100001001010011100011101100110010110100110001100001001010000100110000100100111011110110101011000110000001010001111011010101000111011100101110111101010011111011100001100100011101001011100011001000001111110";
    string n = "00100011110000110100111011001001110101000001110011111010001111110010111101001000011101001001101011110110100010011101101011000100000000110000001011011111110011000000100101111101101001010101010000010110011101011001110000100011111100110000101110110010111111001101011101010101100110111111011000010101011001000001001110110110111100010010011111000011001001101100101";

    GF_2m::Element A = {0}, B = {0}, N = {0};

    GF_2m::from_string(a, A);
    GF_2m::from_string(b, B);
    GF_2m::from_string(n, N);
    

    //додавання
    GF_2m::Element C = {0};

    auto start = high_resolution_clock::now();
    GF_2m::add(A, B, C);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання +: " << duration.count() << " мкс" << endl;

    cout << "A+B = " << GF_2m::to_string(C) << "\n";
    cout<<endl;


    //множення
    GF_2m::Element D = {0};

    start = high_resolution_clock::now();
    GF_2m::multiply(A, B, D);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання * " << duration.count() << " мкс" << endl;

    cout << "A*B = " << GF_2m::to_string(D) << "\n";
    cout<<endl;


    //квадрат
    GF_2m::Element A_squared = {0};

    start = high_resolution_clock::now();
    GF_2m::square(A, A_squared);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання ^2 " << duration.count() << " мкс" << endl;
    
    cout << "A^2 = " << GF_2m::to_string(A_squared) << endl;
    cout<<endl;


    //степінь n
    GF_2m::Element A_pow = {0};

    start = high_resolution_clock::now();
    GF_2m::power(A, n, A_pow);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання ^n " << duration.count() << " мкс" << endl;

    cout << "A^N = " << GF_2m::to_string(A_pow) << "\n";
    cout<<endl;


    //обернений
    GF_2m::Element A_inv = {0};

    start = high_resolution_clock::now();
    GF_2m::inverse(A, A_inv);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання ^(-1) " << duration.count() << " мкс" << endl;

    cout << "A^(-1)  = " << GF_2m::to_string(A_inv) << "\n";

    cout<<endl;

    //слід
    uint64_t Tr = 0;

    start = high_resolution_clock::now();
    GF_2m::trace(A, Tr);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання Tr " << duration.count() << " мкс" << endl;

    cout << "Tr(A) = " << Tr << "\n"; // результат: 0 або 1



}