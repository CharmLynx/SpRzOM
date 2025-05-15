#ifndef FIELD_ELEMENT_H
#define FIELD_ELEMENT_H

#include <string>
#include <cstdint>
#include <chrono>


using namespace std;

class FieldElement {
public:
    static const int M = 359;
    static const int P = 2 * M + 1;
    using Element = uint8_t[M];

    static void from_string(const string& bits, Element& result);
    static string to_string(const Element& e);
    static void add(const Element& a, const Element& b, Element& result);
    static void build_lambda_matrix(bool Lambda[M][M]);
    static void left_rotate(const Element& src, Element& dst, int shift);
    static void multiply(const Element& u, const Element& v, Element& z, bool Lambda[M][M]);
    static void square(const Element& a, Element& result);
    static void power(const Element& a, const Element& n, Element& result, bool Lambda[M][M]);
    static void make_inverse_exponent(Element& n);
    static void inverse(const Element& a, Element& result, bool Lambda[M][M]);
    static uint8_t trace(const Element& a);
    static void zero(Element& e);
    static void one(Element& a);
};

#endif
