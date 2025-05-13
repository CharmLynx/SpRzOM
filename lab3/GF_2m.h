#ifndef GF_2m_H
#define GF_2m_H

#include <string>
#include <cstdint>
#include <cstring>

class GF_2m {
public:
    static const int M = 359;
    static const int WORDS = (M + 63) / 64;
    using Element = uint64_t[WORDS];

    static void from_string(const std::string& bits, Element& result);
    static std::string to_string(const Element& a);

    static void add(const Element& a, const Element& b, Element& result);
    static void multiply(const Element& A, const Element& B, Element& result);
    static void square(const Element& a, Element& result);
    static void power(const Element& base, const std::string& bin_exp, Element& result);
    static void inverse(const Element& a, Element& result);
    static void trace(const Element& a, uint64_t& out_bit);

    static void zero(Element& a);
    static void one(Element& a);

private:
    static const int polynomial_degrees[5];
    static void reduce(uint64_t temp[], Element& result);
};
#endif