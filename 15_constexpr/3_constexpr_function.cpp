// constexpr 함수
/*
    constexpr로 객체를 선언하면 컴파일 타임 상수로 정의된다.
    constexpr이 나오기 이전엔 컴파일 타임 상수인 객체를 만드는 함수를 작성할 수 없었따.
    ex)
        int factorial(int N) {
        int total = 1;
            for (int i = 1; i <= N; i++) {
                total *= i;
            }
            return total;
        }

        template <int N>
        struct A {
            int operator()() { return N; }
        };

        int main() { A<factorial(5)> a; }
        위 코드는 factorial(5)가 컴파일 타임 상수가 아니라서 컴파일 오류를 발생시킨다.
        컴파일러는 factorial(5)를 컴파일 타임에 계산할 줄 모르기 때문이다.

    이런 문제를 해결하기 위해 템플릿 메타프로그래밍을 사용해야됬다.
    하지만 이런 TMP 방식은 매우 복잡하다.
    조건문은 템플릿 특수화 방식으로 반복문은 재귀형태로 구현해야 되기 떄문이다.
    ex)
        template <int N>
        struct Factorial {
            static const int value = N * Factorial<N - 1>::value;
        };

        template <>
        struct Factorial<0> {
            static const int value = 1;
        };

        template <int N>
        struct A {
            int operator()() { return N; }
        };

        int main() {
            // 컴파일 타임에 값이 결정되므로 템플릿 인자로 사용 가능!
            A<Factorial<10>::value> a;

            std::cout << a() << std::endl;
        }

    하지만 constexpr을 사용하면 함수의 return값을 컴파일 타임 상수값으로 만들 수 있어
    훨씬 손쉽게 프로그램을 짤 수 있다.

    constexpr로 함수를 만드는 경우 하면 안되는 제약들이 있다.
    1.  goto 문 사용

    2.  예외 처리 (try 문; C++ 20 부터 가능하게 되었다.)

    3.  리터럴 타입이 아닌 변수의 정의

    4.  초기화 되지 않는 변수의 정의

    5.  실행 중간에 constexpr 이 아닌 함수를 호출하게 됨

    만약 위의 제약조건을 constexpr 함수 내에서 사용한다면 컴파일 오류를 발생시킨다.
    이 함수는 컴파일 타임 상수(constexpr int a = 10 등)을 받으면 컴파일 타임에 동작하지만
    일반 타입의 값(std::cin >> a; 등)을 전달하면 그냥 일반 함수와 같이 런타임에 작동한다.
*/
#include <iostream>

// 컴파일 타임에 계산되는 함수
constexpr int Factorial(int n)
{
    int total = 1;
    for (int i = 1; i <= n; i++)
    {
        total *= i;
    }
    return total;
}

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{
    A<Factorial(10)> a;

    std::cout << a() << std::endl;
}