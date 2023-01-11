// TMP는 매우 복잡하고 오류를 잡기 어렵기 떄문에 사용하기 어렵다
// 주로 실행속도가 중요한 프로그램에서 사용된다.

/*
최대공약수 구하기 (유클리우드를 많이 사용)
int gcd(int a, int b) {
  if (b == 0) {
    return a;
  }

  return gcd(b, a % b);
}
*/

// TMp로 구현
#include <iostream>

template <int X, int Y>
struct GCD
{
    static const int value = GCD<Y, X % Y>::value;
};

template <int X>
struct GCD<X, 0>
{
    static const int value = X;
};

// Ratio 클래스는 유리수 를 오차 없이 표현해 주는 클래스(TMP 사용하지 않고 간단하게 구현가능)
template <int N, int D = 1>
struct Ratio
{
    typedef Ratio<N, D> type; // 자기 자신을 가리키는 타입 (class에서의 this와 비슷한 역할)
    static const int num = N; // 분자
    static const int den = D; // 분모
};

// 두 분수의 더한 결과를 Ratio 에 분자 분모로 전달하면 알아서 기약분수로 만들어 준다.
template <class R1, class R2>
struct _Ratio_add
{
    typedef Ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den> type;
};

// 덧셈을 수행하기 위해서는 typedef _Ratio_add<rat, rat2>::type result;와 같이 사용하면 된다.
// 만약 ::type을 치기 싫다면 Ratio_add 를 해 그 자체로 두 Ratio 가 더해진 타입으로 만든다.
// _Ratio_add<R1, R2>::type 를 상속 받는 Ratio_add 클래스를 만들어서 해결한다.
// 즉 Ratio_add는 마치 Ratio 타입처럼 사용할 수 있게 된다.
template <class R1, class R2>
struct Ratio_add : _Ratio_add<R1, R2>::type {};

int main()
{
    typedef Ratio<2, 3> rat;
    typedef Ratio<3, 2> rat2;
    typedef Ratio_add<rat, rat2> rat3;

    std::cout << rat3::num << " / " << rat3::den << std::endl;

    return 0;
}