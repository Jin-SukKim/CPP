// 템플릿 메타 프로그래밍 (Template Meta Programming - TMP)

/*
Template의 타입을 통해 객체에 무엇을 저장하느냐를 지정할 수도 있지만
타입에 어떠한 값을 부여할 수 있고, 이런 타입들을 가지고 연산을 할 수도 있다.

타입은 Compile TIme에 확정되어야 하므로 컴파일 타임에 연산이 끝난다.
이런게 Type을 이용해 컴파일 타임에 생성되는 코드를 이용한 프로그래밍을 메타 프로그래밍(Meta Programming)이라고 한다.
C++은 Template을 이용해 하기 때문에 TMP(Template Meta Programming)이라고 한다.

컴파일 타임에 연산이 끝나기 때문에 컴파일 시간은 길어지지만 프로그램의 실행 속도는 향상시킬 수 있다.
*/

/* 컴파일 타임 팩토리얼 계산 */
#include <iostream>

// 플릿으로는 for 문을 쓸 수 없기 때문에 위와 같은 재귀적 구조를 사용했다.
// 즉 TMP를 이용해 For문으로 구현 가능한 코드를 구현할 수 있다.
template <int N>
struct Factorial
{
    static const int result = N * Factorial<N - 1>::result;
};

// template specialization
/*
// 컴파일 타임 팩토리얼 계산
int factorial(int n) {
  if (n == 1) return 1;

  return n * factorial(n - 1);
}
*/
// 재귀 함수 호출을 끝내기 위해 1을 따로 처리해야 하는 것과 같은 작동을 한다.
template <>
struct Factorial<1>
{   
    // if 문을 템플릿 특수화롤 통해 TMP 로 구현했다.
    static const int result = 1;
};

/*
    실질적으로 값을 가지고 있는 객체는 없다. 즉 값을 가진 변수는 (std::cout)에서 출력할 때는 제외하고는 메모리 상에 없다 
    계산된 값은 단순히 컴파일러가 만들어낸 Factorial<6>이란 타입을 나타내고 있을 뿐이다.
*/
int main()
{
    std::cout << "6! = 1*2*3*4*5*6 = " << Factorial<6>::result << std::endl;
}