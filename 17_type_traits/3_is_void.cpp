/*
    템플릿 메타프로그래밍에서 if 문은 템플릿 특수화(template specialization)을 통해 구현한다.
    실제로 is_void 는 클래스로 구현되어 있다.

    type_traits 라이브러리는 is_void 처럼 타입들에 대한 여러가지 연산을 수행할 수 있는
    메타 함수들을 제공하고 있다.

    정수 타입을 확인할 수 있는 is_integral 등도 있다.

    그리고 statis_assert는 컴파일 타임동안 전달된 값이 true라면 해당 식이 무시되고
    false라면 컴파일 에러를 일으키는 키워드이다(함수가 아님).

    이 static_assert와 type_traits의 메타 함수들을 잘 사용하면
    특정 타입들만 받는 함수를 작성할 수 있다.
*/

// 실제 is_void클래스는 아니지만 원리를 보기위한 비슷한 클래스이다.
#include <iostream>
#include <type_traits>

// 모든 타입 T에 대해서 매칭된다.
template <typename T>
struct is_void
{
    // static은 전역변수로 static 메모리에 남아 프로그램이 돌아가는 동안 공유된다.
    static constexpr bool value = false;
};

// void에 특화된 템플릿이다. (template specialization)
template <>
struct is_void<void>
{
    static constexpr bool value = true;
};

template <typename T>
void tell_type()
{
    // is_void의 value가 true, false일때 각각 처리 가능하다.
    if (is_void<T>::value)
    {
        std::cout << "T 는 void ! \n";
    }
    else
    {
        std::cout << "T 는 void 가 아니다. \n";
    }
}

// type_traits의 is_integral 사용해보기
class A
{
};

// 정수 타입만 받는 함수
// 컴파일 오류를 발생 시킨다.
template <typename T>
void only_integer(const T &t)
{   
    /*
        static_assert는 C++ 11에 추가된 '키워드'로(함수가 아니다)
        인자로 전달된 식이 참인지 아닌지를 컴파일 타임에 확인한다.
        즉, bool 타입의 constexpr만 static_assert로 확인할 수 있고 
        그 외의 경우는 컴파일 오류가 발생한다.

        만약 statis_assert에 전달된 식이 true라면 컴파일러에 의해
        해당 식은 무시되고, false라면 해당 문장에서 컴파일 오류를 발생시킨다.

        따라서 statis_assert와 is_integral을 사용해 T가 반드시 정수 타입임을
        강제할 수 있다.
    */
    static_assert(std::is_integral<T>::value);
    std::cout << "T is an integer \n";
}

int main()
{
    // 일반적인 템플릿에 매칭되 value가 false가 된다.
    tell_type<int>(); // void 아님!

    // 템플릿 특수화에 매칭되 value가 true가 된다.
    tell_type<void>(); // void!

    int n = 3;
    // T가 int이므로 is_intergral<T>::value가 true
    only_integer(n); 

    A a;
    // T는 클래스 객체 A이므로 false가 되 static assertion failed 에러를 일으킨다.
    only_integer(a); 
}