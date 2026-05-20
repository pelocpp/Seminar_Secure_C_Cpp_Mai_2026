// =====================================================================================
// Explicit.cpp // Keyword 'explicit'
// =====================================================================================

#include <algorithm>
#include <array>
#include <functional>
#include <print>
#include <variant>
#include <vector>
#include <iostream>

namespace KeywordExplicit {

    class Complex {
    private:
        double m_real;
        double m_imag;

    public:
        // c'tors
        Complex() : Complex{ 0.0, 0.0 } {}

         explicit  Complex(double real) :   // remove or add keyword 'explicit'
            m_real{ real }, m_imag{} 
        {
        }

        Complex(double real, double imag) :
            m_real{ real }, m_imag{ imag } {}

        // comparison operator
        bool operator == (const Complex& rhs) const
        {
            return (m_real == rhs.m_real and m_imag == rhs.m_imag);
        }
    };
}

void main_explicit_keyword()
{
    using namespace KeywordExplicit;

    // a Complex object
    Complex c{ 3.0, 0.0 };

    // Complex == double ?!?!?!?!?!?!

    // Auf der rechten Seite eine implizite Typkonvertierung:
    // Wie geht das bei Objekten:
    // double (3.0)   ===>   Complex(double real)

    // Man kann diese implizite Typkonvertierung abschalten: explicit

    if (c == static_cast<Complex>(3.0))  // remove or add cast '(Complex)'  3.0 als Complex mit imag = 0.0
    {
        std::cout << "Same objects" << std::endl;
    }
    else
    {
        std::cout << "Not same objects" << std::endl;
    }
}

// =====================================================================================
// End-of-File
// =====================================================================================