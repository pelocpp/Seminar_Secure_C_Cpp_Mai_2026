


// =====================================================================================
// vector.cpp // 
// =====================================================================================



#include <algorithm>
#include <map>
#include <print>
#include <vector>


void main_vector()
{
    std::vector<int> numbers;

    numbers.reserve(80);
   // numbers.resize(60);     // 80 + 40 = 120  + 60 + 180  // Default-C'tor

    for (std::size_t i = 0; i != 100; ++i) {

        numbers.push_back(2 * i);
        std::println("{}: {} - {}", i, numbers.size(), numbers.capacity());
    }


    auto* ptr1 = numbers.data();

    numbers.shrink_to_fit();    // Kostet Zeit // am Ende ist nur ein "wenig" Speicher gewonnen ....

    std::println("{} - {}", numbers.size(), numbers.capacity());

    auto* ptr2 = numbers.data();

    if (ptr1 != ptr2) {
        std::println("Neu allokiert");
    }

}