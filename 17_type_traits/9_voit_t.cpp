/*
    void_t 정의 :
        template <class...>
        using void_t = void;

    가변길이 템플릿을 사용해 void_t에 템플릿 인자로 임의의 개수의 타입들을 전달할 수 있다.
    void_t는 결국 void와 같다.
    ex)
        void_t<A, B, C, D> // 결국 void

    void_t에 전달된 템플릿 인자 중 문법적으로 올바르지 못한 템플릿 인자가 있다면
    해당 void_t를 사용한 템플릿 함수의 경우 void가 되는 대신 SFINAE에 의해
    오버로딩 목록에서 제외된다.
    ex) 8_function_multi_parameter_condition.cpp
        template <typename Cont, typename = decltype(std::declval<Cont>().begin()),
                    typename = decltype(std::declval<Cont>().end())>

        이 코드를 void_t를 이용해 더 간단하게 만들 수 있다.

        template <typename Cont,
                    typename = std::void_t<decltype(std::declval<Cont>().begin()),
                                            decltype(std::declval<Cont>().end())>>

        즉 void_t에 전달된 decltype(std::declval<Cont>().begin())이나
        decltype(std::declval<Cont>().end()) 중 하나라도 문법적으로 맞지 않는다면 SFINAE에 의해
        해당 print 함수는 오버로딩 후보군에서 제외된다.
        반면 vector처럼 문법적으로 성립하면 print는 잘 오버로딩 될것이다.

    하지만 주의할 점이 있다.
    만약 실수로 템플릿에 컨테이너 말고 인자를 한 개 더 전달했을 시 컴파일 오류는 뜨지만
    print 함수가 오버로딩 후보군에서 제외되지 않는다.
    템플릿에 인자를 하나 더 전달했을 경우 default인자가 사용되지 않아서 타입 체크를 생략한다.
    결국 오버로딩 후보구네서 제외되지 않는다.

    만약 표준 라이브러리 함수들처럼 여러 사용자들을 고려해야 한다면, 사용자의 실수 했을 때도
    정상적으로 작동할 수 있도록 설계해야된다.
    이를 위해 타입 체크하는 부분을 다른 곳으로 빼주어야 된다.

        template <typename Cont>
        std::void_t<decltype(std::declval<Cont>().begin()),
                    decltype(std::declval<Cont>().end())>
        print(const Cont& container)

    함수의 return 타입 역시 SFINAE가 적용되는 부분이므로 동일한 효과를 낼 수 있다.
    뿐만 아니라 템플릿의 정의 부분에 불필요한 default 인자가 들어가 있지 않아 사용자의 실수로 부터 안전하다.
*/
#include <iostream>
#include <type_traits>

// 사용자가 실수로 인자를 하나 더 보내면 SFINAE가 적용되지 않는다.
template <typename Cont,
          typename = std::void_t<decltype(std::declval<Cont>().begin()),
                                 decltype(std::declval<Cont>().end())>>
void print(const Cont &container)
{
    std::cout << "[ ";
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "]\n";
}

// 사용자의 실수를 감안해 타입 체크를 함수의 return 타입으로 옮겼다.
template <typename Cont>
std::void_t<decltype(std::declval<Cont>().begin()),
            decltype(std::declval<Cont>().end())>
print(const Cont &container)
{
    std::cout << "[ ";
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "]\n";
}

struct Bad
{
};

int main()
{
    // 위 print 는 오버로딩 후보군에서 제외되지 않음!
    print<Bad, void>(Bad{});
}