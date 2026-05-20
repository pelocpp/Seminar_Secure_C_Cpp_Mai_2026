// ===========================================================================
// Performance_Optimization_Getting_Started.cpp
// ===========================================================================

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace PerformanceBestPractices {


    class AnotherPerson
    {
    private:
        std::uint8_t m_gender;    // f / m
        std::uint8_t m_age;
        short        m_value;
        std::uint8_t m_xx;
        std::uint8_t m_yy;
        std::uint8_t m_yy1;
        std::uint8_t m_yy2;
        std::uint8_t m_yy3;
        double       m_aa;

        std::string  m_firstName;  
        std::string  m_lastName;  

    };





    class Person
    {
    private:
        std::string m_name;  // Uhhhhhhhhhhhhh  // Nooo

    public:
        // c'tor
        Person(const std::string& name) 
            : m_name{ name } 
        {}

        std::string getName() { return m_name; }   // kann man diskutieren ...  50.50 %

        const std::string& getNameByRef() { return m_name; }   // das ist mehr als Okay ... JA, dieses Objekt MUSS Länger leben

    };

    void  test_Person()
    {
        std::string hans{ "Hans" };

        Person jack{ hans };

        // const std::string& s = jack.getNameByRef();   // mit diesem s will ich bis zum Ende DIEsES Scopes arbeiten ...

        const auto& s = jack.getNameByRef();   // ACHTUNG: Stolperfalle:  auto VERLIERT & und const
       
    }



    void funcTueWas1(int n)
    {
        int m = n;
    }

    void funcTueWas2(int& n)
    {
        int m = n;
    }

    void funcTueWas3(int n)
    {
        int m = 0;

        if (n == 1) {
            m = 5 * n;
        }
        else if (n == 2) {
            m = 6 * n;
        }
        else if (n == 3) {
            m = 7 * n;
        }
        else if (n == 4) {
            m = 8 * n;
        }
        else
        {
            // Rest
            m = 9 * n;
        }
    }

    void funcTueWas4(int n)
    {
        int m = n;

        switch (n)
        {
        case 1:
            m = 5 * n;
            break;
        case 2:
            m = 6 * n;
            break;
        case 3:
            m = 7 * n;
            break;
        case 4:
            m = 8 * n;
            break;
        default:
            m = 9 * n;
            break;
        }
    }

    void funcTueWas5(int n)
    {
        int m = n;

        switch (n)
        {
        case 1:
            m = 5 * n;
            break;
        case 2:
            m = 6 * n;
            break;
        case 3:
            m = 7 * n;
            break;
        case 4:
            n = 2 * n;
            m = 8 * n;
            break;
        case 5:
            m = 8 * n;
            break;
        case 6:
            m = 9 * n;
            break;
        case 7:
            m = 10 * n;
            break;
        case 8:
            n = 4 * n;
            m = 11 * n;
            break;
        case 9:
            m = 12 * n;
            break;
        case 10:
            m = 13 * n;
            break;
        default:
            m = 99 * n;
            break;
        }
    }

    void funcTueWas6(int n)
    {
        int m = 0;

        if (n == 1) {
            m = 5 * n;
        }
        else if (n == 2) {
            m = 6 * n;
        }
        else if (n == 3) {
            m = 7 * n;
        }
        else if (n == 4) {
            m = 8 * n;
        }
        else if (n == 5) {
            m = 5 * n;
        }
        else if (n == 6) {
            m = 16 * n;
        }
        else if (n == 7) {
            m = 17 * n;
        }
        else if (n == 8) {
            m = 18 * n;
        }
        else if (n == 9) {
            m = 17 * n;
        }
        else if (n == 10) {
            m = 18 * n;
        }
        else
        {
            // Rest
            m = 9 * n;
        }
    }



    // =======================================================================
    // Avoid Unnecessary Copying

    static std::string prefix(const std::string& str) {
        if (str.length() >= 5) {
            // extract a part of string
            auto substr = str.substr(1, 4);
            // substr is a std::string
            // ...
            return substr;
        }
        return {};
    }

    static std::string_view prefix(std::string_view str) {
        if (str.length() >= 5) {
            // extract a part of string
            auto substr = str.substr(1, 4);
            // substr is a std::string_view
            // ...
            return substr;
        }
        return {};
    }

    static void best_practices_01_avoid_unnecessary_copying()
    {
        std::string s1{ "1234567890" };
        auto p1{ prefix(s1) };

        std::string_view s2{ "1234567890" };
        auto p2{ prefix(s2) };
    }

    // =======================================================================
    // Prefer Stack Allocation

    static int best_practices_02_prefer_stack_allocation()
    {
        int value = 123;          // stack allocation
        // ...
        return 0;                 // automatically deallocated
    }

    // =======================================================================
    // Optimize Loops

    static void best_practices_03_optimize_loops()
    {
        std::vector<int> numbers{ 1, 2, 3, 4, 5 };

        int sum{};
        for (const int& num : numbers) {
            sum += num;
        }
    }

    // =======================================================================
    // Reduce Function Calls

    inline static int square(int x) {
        return x * x;
    }

    static void best_practices_04_reduce_function_calls()
    {
        int result{ square(5) }; // inlined function
    }
}

// =======================================================================
// *Raw*-Wiederholungschleifen

static void processMoreData(const std::vector<int>& data) {}

static void processData(const std::vector<int>& data)
{
    bool inRange{ true };

    for (const auto& elem : data) {
        if (elem < 50 || elem > 100) {
            inRange = false;
            break;
        }
    }

    if (inRange) {
        processMoreData(data);
    }
}

static void processDataEx(const std::vector<int>& data)
{
    auto inRange = [](int value) { 
        return value < 50 || value > 100;
    };

    const bool allInRange{ std::all_of(data.begin(), data.end(), inRange) };

    if (allInRange) {
        processMoreData(data);
    }
}

static void best_practices_0x_raw_loops()
{
    std::vector<int> data = { 50, 70, 40, 80, 90 };

    processData(data);
    processDataEx(data);
}

// =================================================================

void performance_best_practices()
{
    using namespace PerformanceBestPractices;

    int n = 123;

    //funcTueWas1(n);
    //funcTueWas2(n);

    test_Person();

    funcTueWas3(n);
    funcTueWas4(n);
    funcTueWas5(n);
    funcTueWas6(n);

    best_practices_01_avoid_unnecessary_copying();
    best_practices_02_prefer_stack_allocation();
    best_practices_03_optimize_loops();
    best_practices_04_reduce_function_calls();

    best_practices_0x_raw_loops();
}

// ===========================================================================
// End-of-File
// ===========================================================================
