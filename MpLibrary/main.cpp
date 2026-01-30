
#include "MpLibrary.hpp"


int main()
{
    /*
    static_assert(my_conjunction_v<is_ptr_v<float*>, true> == true);
    static_assert(my_conjunction_v<false, true> == false);
    static_assert(my_conjunction_v<true, is_ptr_v<float>> == false);
    static_assert(my_conjunction_v<false, false> == false);

    using Hi = make_string< 'A', 'N', 'A',' ','A','R','E',' ', 'M','E','R','E' >;
    static_assert(Hi::value[0] == 'A' && Hi::value[1] == 'N', "");

    cout << test_class::val;
    test_class::val = 55; /// daca e constexpr nu mai e modifiable lval
    cout << test_class::val;
    test_class::method(2);
    test
    static_assert( is_float_v<int> ==false);
    static_assert(test::cmmdc<16, 4>::value == 4);
    */

    //static_assert(MPL::is_in_pack_v<int, std::pair<int,int>, const char*, double, int, long>);

    //MPL::print_wrapper<int, char, bool>::print();

    //print( std::make_tuple(1, 2, 3, 4, 5, true, "aa") , MPL::make_sequence<7>{});

    //static_assert(MPL::is_prime<17>);

    //static_assert(MPL::rank_v<int[][11][2]> == 3);

    //static_assert(MPL::negate_v<std::is_same<int, int>> == false);

    //static_assert(std::is_same_v<MPL::remove_extent_t<int>, int>);

    //static_assert(MPL::my_disjunction_v<std::false_type, std::true_type> == true);

    //static_assert(MPL::log2<10> == 10);

    //static_assert(std::is_same_v<MPL::conditional_variadic_t<std::bool_constant<false>,double, std::bool_constant<false>,double,double>,double>);

    /*
    static_assert(std::is_same_v<
        MPL::conditional_variadic_pair_t<
        std::pair<std::bool_constant<false>,int>,
        std::pair<std::bool_constant<false>,double>,
        std::pair<std::bool_constant<false>,char>
        >
        ,char>);
    */

    /*
    auto lambda = [](int x, double y) { return x + y; };
    using res = MPL::result_of_t<decltype(lambda), int, double>;
    static_assert(std::is_same_v<res, double>);
     */

     //static_assert(std::is_same_v<MPL::remove_cv_ref_t<const volatile std::tuple<int>&&>,std::tuple<int>>);
     //static_assert(std::is_same_v<MPL::decay_t<const volatile std::tuple<int>&&>,std::tuple<int>>);

     //int a[1][2];
     //static_assert(std::is_same_v<MPL::decay_t<decltype(a)>, int(*)[2]>); /// ptr to an array of 2 ints
}
