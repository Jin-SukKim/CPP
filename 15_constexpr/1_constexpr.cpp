/*
    constexpr은 객체나 함수 앞에 붙일 수 있는 키워드로
    해당 객체나 함수의 return값을 컴파일 타임에 값을 알 수 있다는 의미를 전달하게 된다.

    컴파일러가 컴파일 타임에 어떠한 식의 값을 결정할 수 있다면 해당 식을 상수식(constant expression)이라고한다.
    이런 상수식들 중에서 값이 정수인 것을 정수 상수식(integral constant expression)이라고 하는데 매우 쓰임새가 많다.
    ex)
        int arr[size]; 이 배열 선언식이 컴파일되기 위해선 size가 정수 상수식이여야 하고

        template <int N>
        struct A {
            int operator()() { return N; }
        };
        A<number> a;
        템플릿 타입 인자의 경우도 number가 정수 상수식이여야만 한다.

        enum A { a = number, b, c };에서 enum에서 값을 지정해줄 때에 오는 number 역시 정수 상수식이여만 한다.
        이처럼 정수 상수식은 매우 많은 곳에서 쓰인다.

    constexpr은 어떤 식이 상수식(constant expression)이라고 명시해주는 키워드이다.
    만익, 객체의 정의에 constexpr이 오게된다면 해당 객체는 어떤 상수식에도 사용될 수 있다.

    constexpr와 const는 둘다 상수라 수정할 수 없기에 같아보이지만 차이가 크다.
*/
#include <iostream>

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{
    constexpr int size = 3;
    int arr[size]; // Good!

    constexpr int N = 10;
    A<N> a; // Good!
    std::cout << a() << std::endl;

    constexpr int number = 3;
    enum B
    {
        x = number,
        y,
        z
    }; // Good!
    std::cout << B::x << std::endl;
}