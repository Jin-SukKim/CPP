/*
    type_traits에 정의되어 있는 메타 함수들 중 is_class가 있는데 이 메타 함수는
    인자로 전달된 타입이 클래스인지 아닌지 확인하는 메타 함수이다.

    보통 클래스인지 아닌지를 확인하는 코드를 생각해보면 방법이 생각나지 않는다.
    실제로 is_class가 구현된 방법은 매우 기괴하다.
    // reference by https://en.cppreference.com/w/cpp/types/is_class
    namespace detail {
    template <class T>
    char test(int T::*);

    struct two {
        char c[2];
    };

    template <class T>
    two test(...);
    }  // namespace detail

    template <class T>
    struct is_class
        : std::integral_constant<bool, sizeof(detail::test<T>(0)) == 1 &&
                                        !std::is_union<T>::value> {};

    또는

    namespace detail {
    template <class T>
    std::integral_constant<bool, !std::is_union<T>::value> test(int T::*);
    
    template <class>
    std::false_type test(...);
    }
    
    template <class T>
    struct is_class : decltype(detail::test<T>(nullptr))
    {};
*/
#include <iostream>
#include <type_traits>

class A
{
public:
    int n;

    A(int n) : n(n) {}
};

int main()
{
    int A::*p_n = &A::n;

    A a(3);
    std::cout << "a.n : " << a.n << std::endl;
    std::cout << "a.*p_n : " << a.*p_n << std::endl;
}