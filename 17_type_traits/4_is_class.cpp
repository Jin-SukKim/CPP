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

    이 코드를 이해하기 위해서는 std::integral_constant가 뭔지를 먼저 알아야된다.
    std::integral_constant<T, T v>로 정의되어 있는 v를 static인자로 가지는 클래스이다.
    쉽게 말해 어떠한 값을 static 객체로 가지고 있는 클래스를 만들어주는 템플릿이다.
    ex)
        std::integral_constant<bool, false> 는 std::integral_constant<bool, false>::value가 false인 클래스이다.

    따라서 위의 코드에서 sizeof(detail::test<T>(0)) == 1 && !std::is_union<T>::value 가 false라면
    is_class는 
        template <class T>
        struct is_class : std::integral_constant<bool, false> {};
    가 되 is_class::value는 false가 될 것이다.
    반면 true면 is_class::value가 true가 된다.

    결국 sizeof(detail::test<T>(0)) == 1 && !std::is_union<T>::value에서 T가 클래스이면 true, 아니면 false가 된다.

    그렇다면 sizeof(detail::test<T>(0)) == 1 은 어떻게 클래스일 떄 1이 될까?

    데이터 멤버를 가리키는 포인터 (Pointer to Data member)라는 것이 있다.

    template <class T>
    char test(int T::*);

    int T::* 는 T의 int 멤버를 가리키는 포인터를 의미한다.
    이 문법은 클래스에만 적용할 수 있다.

    따라서 위의 char test(int T::*); 는 T가 클래스가 아니라면 불가능한 문장이다.
    참고로 위의 코드는 T가 클래스라면 해당 클래스에 int가 없어도 유효한 문장이다.
    다만 아무것도 가리킬 수 없을 뿐이다.

    우리는 클래스 인지 아닌지만 판별하기 위해 필요하므로 지금은 상관없다.

    is_class 코드의 
        struct two {
            char c[2];
        };
        template <class T>
        two test(...);
    의 test함수는 T가 무엇이냐에 관계없이 항상 인스턴스화 될 수 있다.
    test 함수 자체도 이전에 가변 길이의 템플릿 함수에서 다룬 것 처럼 임의의 개수의 인자를 받는 함수이다.

    만약 is_class에서 T가 클래스라고 했을 때 detail::test<T>(0)를 컴파일 시 컴파일러는
    두 가지 템플릿 중 하나를 골라서 오버로딩을 하게 된다.
        1.  template <class T>
            char test(int T::*);  // (1)

        2.  struct two {
                char c[2];
            };
            template <class T>
            two test(...);  // (2)
        
    T가 클래스인 경우 1번이 좀 더 구체적(인자가 명시되어 있다.)이라 우선순위가 높아 1번으로 오버로딩된다.
    따라서 test<T>(0)의 return 타입은 char이 되고 sizeof(char)은 1이므로 1 == 1 -> true가 된다.

    T가 클래스가 아닌 경우 1번은 '컴파일 오류가 발생하는 것이 아니라 오버로딩 후보군에서 제외'되 2번이 유일한 후보군이 된다.
    detail::test<T>(0)의 return 타입이 two가 되는데 two는 char c[2]이므로 sizeof(c[2])는 2가 된다.
    결국 is_class::value는 2 == 1 이므로 false가 된다.

    결론
    sizeof(detail::test<T>(0)) == 1 && !std::is_union<T>::value의 앞부분은 T가 클래스일 때 true가 되고
    아니라면 false인 것을 알 수 있다.

    참고로 C++에서 데이터 멤버를 가리키는 포인터가 허용되는 것은 클래스와 공용체(union) 딱 두가지가 있어
    T가 클래스가 아닌 공용체인 경우도 앞부분이 true가 성립해 is_union을 통해 공용체가 아닌지를 확인해야된다.
    (C++에선 공용체를 구분할 수 없어 컴파일러에 의존한다.)
*/

// Pointer to Data member
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
    // 이 때 p_n은 실존하는 어떤 int 멤버를 가르키는 것이 아니다.
    // int A::*p_n;

    // p_n은 A의 int 멤버 n을 가리키는 포인터
    int A::*p_n = &A::n;

    A a(3);
    std::cout << "a.n : " << a.n << std::endl;  // 3
    std::cout << "a.*p_n : " << a.*p_n << std::endl;    // 3
}