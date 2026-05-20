// ===========================================================================
// Secure_Programming_Additional_Material.cpp
// ===========================================================================

#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>

extern void main_weak_pointer();
extern void main_variant();
extern void main_unique_ptr();
extern void main_string_view();
extern void main_shared_ptr();
extern void main_optional();
extern void main_lambdas();
extern void main_generic_functions();
extern void main_expected();
extern void main_casts();
extern void main_brace_initialization();
extern void main_auto();
extern void main_algorithms();
extern void main_raii_conceptual_example();
extern void main_initializer_list();
extern void main_vector();
extern void main_explicit_keyword();
extern void main_discard();

void map_vs_unordered_map()
{
    std::map<std::string, int> aMap;                     // _Tree //  ordered red-black tree of {key, mapped} values, unique keys

    std::unordered_map<std::string, int> aUnorderedMap;  // hash table of {key, mapped} values, unique keys

  // Wenn überhaupt: Wo suchen wir ... std::find

   // BEIDE Container sind ASSOZIATIV-Container:

    int key = aMap["Hans"];   // tel. Nr.    // Links, Rechts Suche .....

   //  std::find(aMap.begin(), aMap.end(), 123);

    int key2 = aUnorderedMap["Hans"];   // tel. Nr.   // Der Index // der Knoten wird BERECHNET !!!!

    // Normalfall: Go for  std::unordered_map

    // Wann map: Die ist SORTIERT

}


int main()
{
    //main_weak_pointer();
    //main_variant();
    //main_unique_ptr();
    //main_string_view();
    //main_shared_ptr();
    //main_optional();
    //main_lambdas();
    //main_generic_functions();
   // main_expected();
    // main_casts();
    //main_brace_initialization();
   // main_auto();
    //main_algorithms();
    //main_raii_conceptual_example();
    main_initializer_list();
    //main_vector();
   // main_explicit_keyword();
    //main_discard();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
