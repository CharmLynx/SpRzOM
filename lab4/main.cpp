#include <iostream>
#include "FieldElement.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){

    string a = "11001010111001010111110010110000010111100011010101111100111110011111000110000000111111011001000110000000000101111111001110011001000010011101101001000010010000001111111011100000111111010000010100111010011001111101100000001001101000100010110110011110011111110000111100011110011110011011000110010010011110011111010100001000001110101011011100111101000110110000001";
    string b = "10101011011110100101010011111110011011011000101010011000010000001011100111000100011000011100111011110010100000100100000101011010000110110111011101101001001101100111011111111011010010011101001111000000001101111000101000011011111011001101001100101011110101110011010010110011010110111101100011011001110000110100000100101010111001010101000100101110010011110100001";
    string n = "01110010110011101000100111010000110001110100010111100110010100000101011010111011011111010111011010100100010000011101000100001000100001010011110110111000111010001101000110010001100110000010001100100101110010101010010000001010001101101100111010101011000000011100010111000100010010011011100111011010001110101100111011010010011101011100111111011110011001011010101";

    FieldElement::Element A = {0}, B = {0}, N = {0};
    
    FieldElement::from_string(a, A);
    FieldElement::from_string(b, B);
    FieldElement::from_string(n, N);

    //додавання
    FieldElement::Element C = {0};

    auto start = high_resolution_clock::now();
    FieldElement::add(A, B, C);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання +: " << duration.count() << " мкс" << endl;

    cout << "A + B = " << FieldElement::to_string(C) << "\n";


    //множення
    FieldElement::Element D = {0};
    bool Lambda[FieldElement::M][FieldElement::M] = {};

    start = high_resolution_clock::now();
    FieldElement::build_lambda_matrix(Lambda);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час побудови лямбди: " << duration.count() << " мкс" << endl;

    start = high_resolution_clock::now();
    FieldElement::multiply(A, B, D, Lambda);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання * " << duration.count() << " мкс" << endl;

    cout << "A * B = " << FieldElement::to_string(D) << "\n";


    //квадрат
    FieldElement::Element A_squared = {0};

    start = high_resolution_clock::now();
    FieldElement::square(A, A_squared);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання ^2 " << duration.count() << " мкс" << endl;

    cout << "A^2 = " << FieldElement::to_string(A_squared) << "\n";


    //степінь n
    FieldElement::Element AN = {0};

    start = high_resolution_clock::now();
    FieldElement::power(A, N, AN, Lambda);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання ^n " << duration.count() << " мкс" << endl;

    cout << "A^n = " << FieldElement::to_string(AN) << "\n";

    //обернений
    FieldElement::Element A_inv = {0};
    start = high_resolution_clock::now();
    FieldElement::inverse(A, A_inv, Lambda);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання ^(-1) " << duration.count() << " мкс" << endl;

    cout << "A^(-1) = " << FieldElement::to_string(A_inv) << "\n";

    //слід
    start = high_resolution_clock::now();
    uint8_t Tr = FieldElement::trace(A);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "Час виконання Tr " << duration.count() << " мкс" << endl;

    cout << "Tr(A) = " << (int)Tr << "\n";

    return 0;
}
