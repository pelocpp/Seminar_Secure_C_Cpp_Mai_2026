// =====================================================================================
// ConstExpr.cpp
// =====================================================================================

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <map>
#include <numeric>
#include <print>
#include <vector>

#include <iostream>

// =====================================================================================
// variables

namespace ConstExprVariables {

    constexpr double Pi = 3.14159265359;

#define   PI   3.14159265359

    static void testVariables() {

        std::println("Pi: {}", Pi);
        std::println("PI: {}", PI);
    }
}

// =====================================================================================
// functions

namespace ConstExprFunctions {

    // ======================================================
    // limit a number in a range or in between two given numbers

    #define   CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

    static auto clamp = [](auto x, auto lo, auto hi) constexpr {
        return x < lo ? lo : (x > hi ? hi : x);
        }; 

    static void testFunctions_01() {

        constexpr int n = 1 + 2 + 3 + 4 + 5;
        constexpr float n2 = 1.5f + 2.5f + 3.5f + 4.5f + 5.5f;
        constexpr double n3 = 1.5 + 2.5 + 3.5 + 4.5 + 5.5;

        constexpr std::size_t a{ 3 };
        constexpr std::size_t b{ 8 };

        constexpr auto value1{ CLAMP(a + b, 0, 10) };
        constexpr auto value2{ clamp(a + b, 0, 10) };
        constexpr auto value3{ clamp(a + b, 5,  8) };
    }
    
    // ======================================================
    // calculate the number multiplied by itself

    #define   SQUARE(x)             x * x

    static constexpr auto square (auto x) {
        return x * x;
    };

    static auto squareLambda = [](auto x) constexpr {
        return x * x;
    };

    static void testFunctions_02() {

        // Note: macros can produce unexpected results

        std::size_t a{ 1 };
        std::size_t b{ 2 };
        std::size_t value{ SQUARE(a + b) };       // value == 5: Wrong result !!!    a + b * a + b  // 1 + (2 * 1) +2 
        assert(value == 5);
    }

    static void testFunctions_03() {

        // Note: constexpr functions don't have side effects !!!

        constexpr std::size_t a{ 1 };
        constexpr std::size_t b{ 2 };
        constexpr auto value{ square(a + b) };
        static_assert(value == 9);                  // correct result !!!

        constexpr double x{ 3.0 };
        constexpr double y{ 4.0 };
        constexpr auto dvalue{ squareLambda(x + y) };
        static_assert(dvalue == 49.0);              // correct result !!!
    }

    static constexpr auto squareOfTwo = square(2);
    static constexpr auto squareOfThree = square(3.0);

    static constexpr auto squareOfFour = squareLambda(4);
    static constexpr auto squareOfFive = squareLambda(5.0);

    // ======================================================
    // swap the values of two variables

    #define   SWAP(a, b)        \
          {                     \
              auto tmp = (a);   \
              (a) = (b);        \
              (b) = tmp;        \
          }

    static auto swap = [](auto& a, auto& b) constexpr {
        auto tmp = a;
        a = b;
        b = tmp;
    };

    // constexpr lambda as a compile-time function template (C++ 20)
    // enforces that both parameters have the same type
    //static auto swap = []<typename T>(T& a, T& b) constexpr {
    //    auto tmp = a;
    //    a = b;
    //    b = tmp;
    //    };

    static void testFunctions_04() {

        std::size_t a{ 1 };
        std::size_t b{ 2 };

        std::println("{} - {}", a, b);
        SWAP(a, b);
        std::println("{} - {}", a, b);
    }

    static void testFunctions_05() {

        std::size_t a{ 1 };
        std::size_t b{ 2 };

        std::println("{} - {}", a, b);
        swap(a, b);
        std::println("{} - {}", a, b);
    }

    // ======================================================

    static void testFunctions() {

        testFunctions_01();
        testFunctions_02();
        testFunctions_03();
        testFunctions_04();
        testFunctions_05();
    }
}

// =====================================================================================
// classes and objects

namespace ConstExprClassesAndObjects {

    class Complex
    {
    private:
        float m_real;
        float m_imag;

    public:
        // c'tors
        constexpr Complex() : m_real{ }, m_imag{ } {}
        constexpr Complex(float real, float imag) : m_real{ real }, m_imag{ imag } {}

        // getter
        constexpr float real() const { return m_real; }
        constexpr float imag() const { return m_imag; }

        // operators
        constexpr Complex operator+ (const Complex& other) const
        {
            float real = m_real + other.m_real;
            float imag = m_imag + other.m_imag;
            return { real, imag };
        }
    };

    static void testComplex()
    {
        constexpr Complex c0{ };
        constexpr Complex c1{ 1.0, 2.0 };
        constexpr Complex c2{ 3.0, 3.0 };

        constexpr float r1{ c1.real() };
        constexpr Complex c3{ c1 + c2 };
        constexpr float r2{ c3.real() };

        // verify 'constness' with the help of disassembly and
        // https://www.h-schmidt.net/FloatConverter/IEEE754de.html
        
        std::println("Real: ", c3.real());
        std::println("Imag: ", c3.imag());

        // verify compile time computing
        static_assert (c1.real() == 1.0, "real part shoud be 1.0");
        static_assert (c3.real() == 4.0, "real part shoud be 4.0");
        static_assert (c3.imag() == 5.0, "imaginary part shoud be 5.0");
    }
}

// =====================================================================================
// dynamic variables

namespace ConstExprDynamicData {

    static constexpr int naiveSum(unsigned int n)
    {
        auto ip{ new int[n] };
        std::iota(ip, ip + n, 1);   // 1, 2, 3, 4, 5, 6, .....
        auto tmp{ std::accumulate(ip, ip + n, 0) };   // 1 + 2+ 3+ 4+ 5
        delete[] ip;
        return tmp;
    }

    static void testDynamicData()
    {
        constexpr int sum{ naiveSum(10) };
        std::println("Sum from 1 up to 10: {}", sum);
    }
}

// =====================================================================================
// example: compile-time table of power numbers

namespace ConstExprPow {

    constexpr std::size_t TableSize = 5;
    constexpr std::size_t Factor = 4;

    template<std::size_t F>
    constexpr auto powerTable = [] {

        std::array<std::size_t, TableSize> table = {};

        std::size_t index{ 1 };
        for (std::size_t& elem : table) {

            std::size_t tmp{ 1 };
            for (std::size_t i{}; i != F; ++i) {
                tmp *= index;
            }

            elem = tmp;
            ++index;
        }

        return table;
    } ();

    static constexpr std::size_t sumUpPowerTable()
    {
        static_assert (powerTable<Factor>[0] == 1, "Value should be ");
        static_assert (powerTable<Factor>[1] == 16, "Value should be ");
        static_assert (powerTable<Factor>[2] == 81, "Value should be ");
        static_assert (powerTable<Factor>[3] == 256, "Value should be ");
        static_assert (powerTable<Factor>[4] == 625, "Value should be ");

        std::size_t total{};

        for (std::size_t i{}; i != TableSize; ++i) {
            total += powerTable<Factor>[i];
        }

        return total;
    }

    static void testPower_01()
    {
        constexpr std::size_t total{ sumUpPowerTable() };
        static_assert (total == 979, "Sum should be 979");
        std::println("Total: ", total);
    }

    static void testPower_02()
    {
        int index = 1;
        for (std::size_t elem : powerTable<Factor>) {
            std::println("{:02}: {}", index++, elem);
        }

        constexpr std::size_t total = sumUpPowerTable();
        std::println("Total: {}", total);
    }

    static void testPower()
    {
        testPower_01();
        testPower_02();
    }
}

void main_constexpr()
{
    ConstExprVariables::testVariables();
    ConstExprFunctions::testFunctions();
    ConstExprClassesAndObjects::testComplex();
    ConstExprDynamicData::testDynamicData();
    ConstExprPow::testPower();
}

// =====================================================================================
// End-of-File
// =====================================================================================

// =====================================================================================
// Exercises_14_ConstExpr.cpp
// =====================================================================================

namespace Exercises_ConstExpr {

    namespace Exercise_01 {

        // works for int arguments
        static constexpr auto maximum(int a, int b, int c) {

            if (a > b and a > c)
            {
                return a;
            }
            else if (b > a and b > c)
            {
                return b;
            }
            else
            {
                return c;
            }
        };

        template <typename T>
        static constexpr auto maximumGeneric(T a, T b, T c) {

            if (a > b and a > c)
            {
                return a;
            }
            else if (b > a and b > c)
            {
                return b;
            }
            else
            {
                return c;
            }
        };

        static constexpr auto maximumAuto(auto a, auto b, auto c) {

            if (a > b and a > c)
            {
                return a;
            }
            else if (b > a and b > c)
            {
                return b;
            }
            else
            {
                return c;
            }
        };

        // works for int arguments
        static auto maximumLambda = [](int a, int b, int c) constexpr {

            if (a > b and a > c)
            {
                return a;
            }
            else if (b > a and b > c)
            {
                return b;
            }
            else
            {
                return c;
            }
            };

        // works for arguments of different type (?!?)
        // type of the ternary ?: expression is
        // the common type of its second and third argument
        static auto maximumAutoLambda = [](auto a, auto b, auto c) constexpr {
            return (a > b and a > c) ? a : ((b > a and b > c) ? b : c);
            };

        // works for arguments of different type (?!?)
        // type of the ternary ?: expression is
        // the common type of its second and third argument
        static auto maximumMoreAutoLambda = [](auto a, auto b, auto c) constexpr {

            if (a > b and a > c)
            {
                return a;
            }
            else if (b > a and b > c)
            {
                return b;
            }
            else
            {
                return c;
            }
            };

        // works for arbitrary arguments of the same type (!)
        template <typename T>
        static auto maximumGenericLambda = [](T a, T b, T c) constexpr {
            return (a > b and a > c) ? a : ((b > a and b > c) ? b : c);
            };

        // works for arbitrary arguments of the same type (!)
        // different syntax
        static auto maximumMoreGenericLambda = []<typename T>(T a, T b, T c) constexpr {
            return (a > b and a > c) ? a : ((b > a and b > c) ? b : c);
        };

        static void testExercise()
        {
            constexpr auto result01 = maximum(1, 2, 3);

            constexpr auto result02 = maximumGeneric(1.5, 2.5, 3.5);
            // constexpr auto result03 = maximumGeneric(1.5, 2.5f, 3.5);          // does NOT compile (!)
            constexpr auto result04 = maximumGeneric<double>(1.5, 2.5f, 3.5);

            constexpr auto result05 = maximumAuto(1.5, 2.5, 3.5);
            // constexpr auto result06 = maximumAuto(1.5, 2.5f, 3.5);             // does NOT compile (!)
            // constexpr auto result07 = maximumAuto<double>(1.5, 2.5f, 3.5);     // does NOT compile (!)

            constexpr auto result08 = maximumLambda(1, 2, 3);

            constexpr auto result09 = maximumAutoLambda(3, 1, 2);
            constexpr auto result10 = maximumAutoLambda(3, 2, 1);                 // Why does this compile, different argument types (?)

            constexpr auto result11 = maximumMoreAutoLambda(1.5, 3.5, 2.5);
            // constexpr auto result12 = maximumMoreAutoLambda(1.5, 3.5f, 2.5);   // does NOT compile (!)

            constexpr auto result13 = maximumAutoLambda(1.5, 3.5f, 2.5);          // Why does this compile (?)

            constexpr auto result14 = maximumGenericLambda<int>(5, 7, 6);
            constexpr auto result15 = maximumGenericLambda<double>(7.5, 8.5, 6.5f);  // Compiles, but different argument types (!)

            constexpr auto result16 = maximumMoreGenericLambda(5, 4, 3);
            // constexpr auto result17 = maximumMoreGenericLambda(5.5f, 4.5, 3.5);   // does NOT compile (!), compare with 'maximumGeneric' (?!?!)
        }
    }

    namespace Exercise_02 {

        template <typename T, typename ... TArgs>
        static constexpr T maximum(T first, TArgs ... args)
        {
            T result{ first };

            ((result = result > args ? result : args), ...);

            return result;
        }

        static constexpr auto maximumAuto(auto first, auto ... args)
        {
            auto result{ first };

            ((result = result > args ? result : args), ...);

            return result;
        }

        static void testExercise()
        {
            constexpr int m1{ maximum(1, 5, 3, 9, 7) };

            constexpr int m2{ maximum(2, 4, 6, 8, 6, 4, 2) };

            static_assert(m1 == 9, "wrong maximum value");
            static_assert(m2 == 8, "wrong maximum value");
        }
    }

    namespace Exercise_03 {

#pragma warning (push)
#pragma warning (disable : 4244)
#pragma warning (disable : 4302)
#pragma warning (disable : 4305)
#pragma warning (disable : 4309)

        // windef.h -- Basic Windows Type Definitions
        typedef unsigned char       BYTE;
        typedef unsigned short      WORD;
        typedef unsigned long       DWORD;

        // wingdi.h
        typedef DWORD               COLORREF;

        // wingdi.h
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

        namespace MacrosDiscouragedUsage
        {
            static void test_macro_rgb_01()
            {
                // Color constants - examples from Microsoft
                const COLORREF rgbRed = 0x000000FF;
                const COLORREF rgbGreen = 0x0000FF00;
                const COLORREF rgbBlue = 0x00FF0000;
                const COLORREF rgbBlack = 0x00000000;
                const COLORREF rgbWhite = 0x00FFFFFF;

                COLORREF red = RGB(255, 0, 0);
                COLORREF lightBlue = RGB(25, 55, 200);
                COLORREF aqua = RGB(128, 255, 255);

                // wrong use cases
                COLORREF unknown1 = RGB(55.55, 66, 70);     // no warning, no error
                COLORREF unknown2 = RGB(55, 66.66f, 70);    // no warning, no error
                COLORREF unknown3 = RGB(-1, -2, -3);        // no warning, no error
                COLORREF unknown4 = RGB(-1, -2, "ABC");     // 'type cast' : truncation from 'const char *' to 'BYTE'
            }
        }

        // =====================================================================================

        namespace ConstexprRecommendedUsage
        {
            static constexpr std::uint32_t Rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b)
            {
                return
                    static_cast<std::uint32_t>(r) |
                    static_cast<std::uint32_t>(g) << 8 |
                    static_cast<std::uint32_t>(b) << 16;
            }

            class Color
            {
            private:
                std::uint8_t m_r;
                std::uint8_t m_g;
                std::uint8_t m_b;

            public:
                constexpr explicit Color(std::uint8_t r, std::uint8_t g, std::uint8_t b)
                    : m_r(r), m_g(g), m_b(b)
                {}

                constexpr explicit Color(const std::string_view s)
                    : Color(parse(s))
                {}

                constexpr Color(const Color& rhs) = default;
                constexpr Color& operator= (const Color& rhs) = default;

            private:
                static constexpr Color parse(std::string_view str)
                {
                    if (str.size() != 7) {
                        return Color{ 0, 0, 0 };
                    }
                    else if (str[0] != '#') {
                        return Color{ 0, 0, 0 };
                    }
                    else {

                    }

                    std::string_view svR{ str.substr(1, 2) };
                    std::string_view svG{ str.substr(3, 2) };
                    std::string_view svB{ str.substr(5, 2) };

                    auto r{ byte(svR) };
                    auto g{ byte(svG) };
                    auto b{ byte(svB) };

                    if (r.has_value() and g.has_value() and b.has_value()) {
                        return Color{ r.value() , g.value() , b.value() };
                    }
                    else {
                        return Color{ 0, 0, 0 };
                    }
                }

                // decode a nibble
                static constexpr std::optional<std::uint8_t> nibble(char n) {
                    if (n >= '0' and n <= '9') {
                        return n - '0';
                    }
                    else if (n >= 'a' and n <= 'f') {
                        return n - 'a' + 10;
                    }
                    else if (n >= 'A' and n <= 'F') {
                        return n - 'A' + 10;
                    }
                    else {
                        return std::nullopt;
                    }
                }

                // decode a byte
                static constexpr std::optional<std::uint8_t> byte(std::string_view sv) {
                    auto upperNibble = nibble(sv[0]);
                    auto lowerNibble = nibble(sv[1]);

                    if (lowerNibble.has_value() and upperNibble.has_value()) {
                        return upperNibble.value() << 4 | lowerNibble.value();
                    }
                    else {
                        return std::nullopt;
                    }
                }

            public:
                // getter
                constexpr std::uint8_t GetRValue() const { return m_r; };
                constexpr std::uint8_t GetGValue() const { return m_g; };
                constexpr std::uint8_t GetBValue() const { return m_b; };

                constexpr std::uint32_t value() const
                {
                    return
                        static_cast<std::uint32_t> (m_r) |
                        static_cast<std::uint32_t> (m_g) << 8 |
                        static_cast<std::uint32_t> (m_b) << 16;
                }
            };
        }

        // ===============================================================================================

        // function Rgb
        static void test_constexpr_color_01()
        {
            COLORREF red = RGB(255, 0, 0);
            COLORREF lightBlue = RGB(173, 216, 230);
            COLORREF aqua = RGB(0, 255, 255);
            COLORREF darkGreen = RGB(1, 50, 32);

            std::cout << std::format("{:#010X}", red) << std::endl;
            std::cout << std::format("{:#010X}", lightBlue) << std::endl;
            std::cout << std::format("{:#010X}", aqua) << std::endl;
            std::cout << std::format("{:#010X}", darkGreen) << std::endl;
            std::cout << std::endl;
        }

        static void test_constexpr_color_02()
        {
            COLORREF unknown1 = RGB(255.0, 0.0, 0.0);             // No warning, no error
            COLORREF unknown2 = RGB('173', '216', '230');         // No warning, no error
            COLORREF unknown3 = RGB("0", "255", "255");           // Warning: 'type cast' : truncation from 'const char *' to 'BYTE'
            COLORREF unknown4 = RGB(1000, 5000, 32000);           // No warning, no error

            std::cout << std::format("{:#010X}", unknown1) << std::endl;
            std::cout << std::format("{:#010X}", unknown2) << std::endl;
            std::cout << std::format("{:#010X}", unknown3) << std::endl;
            std::cout << std::format("{:#010X}", unknown4) << std::endl;
            std::cout << std::endl;
        }

        static void test_constexpr_color_03()
        {
            using namespace ConstexprRecommendedUsage;

            constexpr std::uint32_t red{ Rgb(255, 0, 0) };
            constexpr std::uint32_t lightBlue{ Rgb(173, 216, 230) };
            constexpr std::uint32_t aqua{ Rgb(0, 255, 255) };
            constexpr std::uint32_t darkGreen{ Rgb(1, 50, 32) };

            std::cout << std::format("{:#010X}", red) << std::endl;
            std::cout << std::format("{:#010X}", lightBlue) << std::endl;
            std::cout << std::format("{:#010X}", aqua) << std::endl;
            std::cout << std::format("{:#010X}", darkGreen) << std::endl;
            std::cout << std::endl;
        }

        static void test_constexpr_color_04()
        {
            using namespace ConstexprRecommendedUsage;

            constexpr std::uint32_t red = Rgb(255.0, 0.0, 0.0);             // Warning: 'argument': conversion from 'double' to 'const uint8_t', possible loss of data
            constexpr std::uint32_t lightBlue = Rgb('173', '216', '230');   // Warning: 'argument': truncation from 'int' to 'const uint8_t'
            //constexpr std::uint32_t aqua = Rgb("0", "255", "255");        // Error: cannot convert argument 1 from 'const char [2]' to 'const uint8_t'
            constexpr std::uint32_t darkGreen = Rgb(1000, 5000, 32000);     // Warning:  'argument': truncation from 'int' to 'const uint8_t'

            std::cout << std::format("{:#010X}", red) << std::endl;
            std::cout << std::format("{:#010X}", lightBlue) << std::endl;
            //std::cout << std::format("{:#010X}", aqua) << std::endl;
            std::cout << std::format("{:#010X}", darkGreen) << std::endl;
            std::cout << std::endl;
        }

        // ===============================================================================================
        // class Color

        static void test_constexpr_color_10()
        {
            using namespace ConstexprRecommendedUsage;

            constexpr Color cyan{ 0, 255, 255 };
            
            constexpr auto value{ cyan.value() };
            std::cout << std::format("{:#010X}", value) << std::endl;
        }

        static void test_constexpr_color_11()
        {
            using namespace ConstexprRecommendedUsage;

            constexpr Color cyan{ "#123456" };

           // auto x = "#00FFFF";

            constexpr auto r{ cyan.GetRValue() };
            constexpr auto g{ cyan.GetGValue() };
            constexpr auto b{ cyan.GetBValue() };
            std::cout << std::format("{:#04X}:{:#04X}:{:#04X}", r, g, b) << std::endl;

            constexpr auto value{ cyan.value() };
            std::cout << std::format("{:#010X}", value) << std::endl;
        }

        static void test_constexpr_color_12()
        {
            using namespace ConstexprRecommendedUsage;

            constexpr Color red{ 255, 0, 0 };
            constexpr Color lightBlue{ 173, 216, 230 };
            constexpr Color aqua{ 0, 255, 255 };
            constexpr Color darkGreen{ 1, 50, 32 };

            std::cout << std::format("{:#010X}", red.value()) << std::endl;
            std::cout << std::format("{:#010X}", lightBlue.value()) << std::endl;
            std::cout << std::format("{:#010X}", aqua.value()) << std::endl;
            std::cout << std::format("{:#010X}", darkGreen.value()) << std::endl;
        }

#pragma warning(pop)

        static void testExercise()
        {
            test_constexpr_color_01();
            test_constexpr_color_02();
            test_constexpr_color_03();
            test_constexpr_color_04();

            test_constexpr_color_10();
            test_constexpr_color_11();
            test_constexpr_color_12();
        }
    }

    namespace Exercise_04 {

        template<typename T1, typename T2>
        static constexpr bool sameType(T1, T2)
        {
            return std::is_same<T1, T2>::value;
        }

        template<typename T1, typename T2, typename... TRest>
        static constexpr bool sameType(T1 arg1, T2 arg2, TRest... args)
        {
            return std::is_same<decltype(arg1), decltype(arg2)>::value and sameType(arg2, args...);
        }

        // Oder

        //template<typename T1, typename T2, typename... TRest>
        //constexpr bool sameType(T1 arg1, T2 arg2, TRest... args)
        //{
        //    return std::is_same<T1, T2>::value and sameType(arg2, args...);
        //}

        static void testExercise()
        {
            constexpr bool result1{ sameType(43, false, "hello") };
            std::cout << std::boolalpha << result1 << std::endl;

            constexpr bool result2{ sameType(1, 2, 3, 4, 5, 6, 7, 8, 9) };
            std::cout << std::boolalpha << result2 << std::endl;
        }
    }
}

void test_exercises_constexpr()
{
    using namespace Exercises_ConstExpr;

    Exercise_01::testExercise();
    Exercise_02::testExercise();
    Exercise_03::testExercise();
    Exercise_04::testExercise();
}

// =====================================================================================
// End-of-File
// =====================================================================================
