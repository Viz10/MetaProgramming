
#include <iostream>
#include <print>
#include <type_traits>
#include <tuple>

using std::cout;

namespace MPL {

    struct test_class {
        inline static int val = 10;
        static void method(int a) { cout << a; }
    };

    ///////////////////////////////////////////////////////////////////// MAKE STRING

    template<char...C>
    struct make_string {
        static constexpr char value[] = { C... };
    };

    ///////////////////////////////////////////////////////////////////// IS PTR

    template<typename T>
    struct is_ptr {
        static constexpr bool value = false;
    };

    template<typename T>
    struct is_ptr <T*> {
        static constexpr bool value = true;
    };

    template<typename T>
    inline constexpr bool is_ptr_v = is_ptr<T>::value;

    ///////////////////////////////////////////////////////////////////// CONJUNCTION

    template<bool... T>
    struct my_conjunction : std::bool_constant < (T && ...) > {}; /// or static constexpr bool

    template<bool... T>
    inline constexpr bool my_conjunction_v = my_conjunction<T...>::value;

    ///////////////////////////////////////////////////////////////////// CMMDC

    template<int a, int b> requires (a >= b)
        struct cmmdc {
        static constexpr int value = cmmdc<b, a% b>::value;
    };

    template<int a>
    struct cmmdc<a, 0> {
        static constexpr int value = a;
    };


    //////////////////////////////////////////////////////////////////// CONTAINS TYPE

    template<typename...Rest>
    struct is_in_pack : std::false_type {};


    template<typename T, typename First, typename...Rest>
    struct is_in_pack<T, First, Rest...> : is_in_pack<T, Rest...> {};


    template<typename First, typename...Rest>
    struct is_in_pack<First, First, Rest...> : std::true_type {};


    template<typename First, typename...Rest>
    constexpr bool is_in_pack_v = is_in_pack<First, Rest...>::value;

    //////////////////////////////////////////////////////////////////// PRINT TYPE IDS

    template<typename...Ts>
    struct print_wrapper {};

    template<typename First, typename...Ts>
    struct print_wrapper<First, Ts...> {
        static void print() {
            cout << typeid(First).name() << " ";
            print_wrapper<Ts...>::print(); /// callback
        }
    };

    template<typename First>
    struct print_wrapper<First> {
        static void print() {
            cout << typeid(First).name() << "\n";
        }
    };

    //////////////////////////////////////////////////////////////////// MALE INDEX SEQUENCE

    template<int...Ts>
    struct index_sequence { constexpr static std::size_t size = sizeof...(Ts); };

    template<int...Ts>
    struct index_sequence_helper {};

    template<int N, int Current, int...Ts>
    struct index_sequence_helper<N, Current, Ts...> : index_sequence_helper<N, Current + 1, Ts..., Current> {};

    template<int N, int Current, int...Ts> requires (N == Current) /// sau specializare cu N , N
        struct index_sequence_helper<N, Current, Ts...> { using type = index_sequence<Ts...>; };

    template<int N>
    using make_sequence = typename index_sequence_helper<N, 0>::type;

    //////////////////////////////////////////////////////////////////// IS PRIME 

    /*
        for(i from 2 to n/2)
            if n%i==0 return false
    */

    template<int n, int cnt>
    struct is_prime_helper {
        static constexpr bool value =
            std::conditional_t< (n % cnt == 0), std::false_type, std::bool_constant<is_prime_helper<n, cnt - 1>::value>>::value;
    };

    template<int n>
    struct is_prime_helper<n, 1> : std::true_type {};

    template<int cnt>
    struct is_prime_helper<1, cnt> : std::false_type {};

    template<int cnt>
    struct is_prime_helper<0, cnt> : std::false_type {};

    template<int n>
    constexpr bool is_prime = is_prime_helper<n, n / 2>::value;

    //////////////////////////////////////////////////////////////////// RANK

    template <typename T>
    struct rank : std::integral_constant<int, 0> {};

    template <typename T>
    struct rank<T[]> {
        static constexpr int value = 1 + rank<T>::value;
    };

    template <typename T, int N>
    struct rank<T[N]> {
        static constexpr int value = 1 + rank<T>::value;
    };

    template <typename T>
    constexpr int rank_v = rank<T>::value;

    //////////////////////////////////////////////////////////////////// NEGATE

    template <typename T>
    concept BooleanValuable = requires (std::remove_cvref_t<T>::value_type v) {
        requires std::convertible_to<decltype(v), bool>;
    };

    template<typename T> requires BooleanValuable<T>
    struct negate : std::bool_constant<!(bool)T::value> {};

    template<typename T>
    constexpr bool negate_v = negate<T>::value;

    //////////////////////////////////////////////////////////////////// REMOVE EXTENT(S)

    template <typename T>
    struct remove_extent : std::type_identity<T> {};

    template <typename T, unsigned int N>
    struct remove_extent<T[N]> : std::type_identity<T> {};

    template <typename T>
    struct remove_extent<T[]> : std::type_identity<T> {};

    template <typename T>
    using remove_extent_t = remove_extent<T>::type;

    template <typename T>
    struct remove_extents : std::type_identity<T> {};

    template <typename T, unsigned int N>
    struct remove_extents<T[N]> : std::type_identity<T> {};

    template <typename T>
    struct remove_extents<T[]> : std::type_identity<T> {};

    template <typename T>
    using remove_extents_t = remove_extents<T>::type;

    //////////////////////////////////////////////////////////////////// DISJUNCTION

    template <typename...T>
    struct my_disjunction : std::false_type {};

    template <BooleanValuable First, typename...Rest>
    struct my_disjunction<First, Rest...>
        : std::conditional_t<(bool)First::value, std::true_type, my_disjunction<Rest...>> {
    }; /// short circuit

    template <typename...T>
    constexpr bool my_disjunction_v = my_disjunction<T...>::value;

    //////////////////////////////////////////////////////////////////// LOG2

    template<size_t number, int cnt>
    struct log2_helper : log2_helper<(number >> 1), cnt + 1> {};

    template<int cnt>
    struct log2_helper<1, cnt> : std::integral_constant<int, cnt> {};

    template<size_t number> requires ((number& (number - 1)) == 0)
        constexpr size_t log2 = log2_helper<number, 0>::value;

    //////////////////////////////////////////////////////////////////// VARIADIC CONDITIONAL WITH ALTERNATION

    template<typename T>
    concept validType = requires { typename std::type_identity_t<T>; };

    template<typename First, typename Second>
    concept well_formated = validType<Second> && requires { {First::value} -> std::convertible_to<bool>; };

    template<typename...Ts>
    struct conditional_variadic; /// base uncomplete case

    template<typename LastType> requires validType<LastType>
    struct conditional_variadic<LastType> : std::type_identity<LastType> {}; /// bool type bool type ... bool type type -> last param

    template<typename Bool, typename Type> requires well_formated<Bool, Type>
    struct conditional_variadic<Bool, Type> : std::type_identity<Type> {}; /// only one bool and type remain , returns always the type

    template<typename Bool, typename Type, typename...Ts> requires well_formated<Bool, Type> && ((bool)Bool::value == true)
        struct conditional_variadic<Bool, Type, Ts...> : std::type_identity<Type> {}; /// exit case

    template<typename Bool, typename Type, typename...Ts> requires well_formated<Bool, Type>
    struct conditional_variadic<Bool, Type, Ts...> : conditional_variadic<Ts...> {}; /// recursion base case

    template<typename...Ts>
    using conditional_variadic_t = typename conditional_variadic<Ts...>::type;

    /*
    template<typename...Ts>
    struct mp_list : std::integral_constant<int,sizeof...(Ts)> {};
    */

    //////////////////////////////////////////////////////////////////// VARIADIC CONDITIONAL WITH PAIR

    template<typename...Ts>
    struct conditional_variadic_pair; // holds pairs of type with ::value bool and return type

    template<typename Bool, typename Type> requires well_formated<Bool, Type>
    struct conditional_variadic_pair<std::pair<Bool, Type>> : std::type_identity<Type> {};

    template<typename LastType> requires validType<LastType>
    struct conditional_variadic_pair<LastType> : std::type_identity<LastType> {};

    template<typename Bool, typename Type, typename...Ts> requires well_formated<Bool, Type> && ((bool)Bool::value == true) // check
        struct conditional_variadic_pair<std::pair<Bool, Type>, Ts...> : std::type_identity<Type> {}; /// peel off 1st argument to be any kind of pair

    template<typename Bool, typename Type, typename...Ts> requires well_formated<Bool, Type>
    struct conditional_variadic_pair<std::pair<Bool, Type>, Ts...> : conditional_variadic_pair<Ts...> {};

    template<typename...Ts>
    using conditional_variadic_pair_t = typename conditional_variadic_pair<Ts...>::type;

    //////////////////////////////////////////////////////////////////// RESULT OF

    template<typename Func, typename...Args>
    concept invocable = requires (Func && f, Args&&...args) { std::invoke(std::forward<Func>(f), std::forward<Args>(args)...); };

    template<typename Func, typename...Args>  requires invocable<Func, Args...>
    struct result_of { using type = decltype(std::declval<Func>()(std::declval<Args>()...)); };

    template<typename Func, typename...Args>
    using result_of_t = typename result_of<Func, Args...>::type;

    //////////////////////////////////////////////////////////////////// REMOVING TYPE TRAITS

    template<typename T>
    struct remove_const : std::type_identity<T> {};

    template<typename T>
    struct remove_const<const T> : std::type_identity<T> {};

    template<typename T>
    using remove_const_t = typename remove_const<T>::type;

    template<typename T>
    struct remove_volatile : std::type_identity<T> {};

    template<typename T>
    struct remove_volatile<volatile T> : std::type_identity<T> {};

    template<typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template<typename T>
    struct remove_ref : std::type_identity<T> {};

    template<typename T>
    struct remove_ref<T&&> : std::type_identity<T> {};

    template<typename T>
    struct remove_ref<T&> : std::type_identity<T> {};

    template<typename T>
    using remove_ref_t = typename remove_ref<T>::type;

    template<typename T>
    using remove_cv_ref_t = remove_const_t<remove_volatile_t<remove_ref_t<T>>>;

    //////////////////////////////////////////////////////////////////// DECAY

    /// <summary>
    /// 1. T is an array -> decay to aray ptr.
    /// 2. T is a function type F or reference to one -> add ptr.
    /// 3. Else, remove_cv_ref of T
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T>
    struct decay {
        using type = conditional_variadic_t<
            std::is_array<T>, std::add_pointer_t<remove_extent_t<T>>,
            std::is_function<T>, std::add_pointer_t<T>,
            remove_cv_ref_t<T>
        >;
    };

    template<typename T>
    using decay_t = typename decay<T>::type;

    ////////////////////////////////////////////////////////////////////
}

template<typename...Ts, int...Us>
void print(const std::tuple<Ts...>& tuple, MPL::index_sequence<Us...>) {
    (..., (cout << std::get<Us>(tuple) << " "));
}


