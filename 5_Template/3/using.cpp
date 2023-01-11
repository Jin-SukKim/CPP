#include <iostream>
#include "5_template/3/using.cpp"
// typedef 대신에 좀 더 직관적인 using 이라는 키워드를 사용할 수 있습니다.

/*
typedef Ratio_add<rat, rat2> rat3;
using rat3 = Ratio_add<rat, rat2>; 는 같은 의미를 가진다.
*/

/*
만약 함수 포인터의 경우 만일 void 를 리턴하고 int,
int 를 인자로 받는 함수의 포인터의 타입을 func 라고 정의하기 위해서는 typedef로는

typedef void (*func)(int, int);

와 같이 선언해야 됬다면 using을 이용하면

using func = void (*)(int, int);

와 같이 직관적으로 만들수 있다.
*/

// 마치 Ratio 클래스의 객체를 생성한 것 같지만, 실제로 생성된 객체는 한 개도 없고,
// 단순히 using을 이용해 타입들을 컴파일러가 만들어낸 것 뿐입니다.
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

template <int N, int D = 1>
struct Ratio
{
private:
    const static int _gcd = GCD<N, D>::value;

public:
    typedef Ratio<N / _gcd, D / _gcd> type;
    static const int num = N / _gcd;
    static const int den = D / _gcd;
};
template <class R1, class R2>
struct _Ratio_add
{
    using type = Ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den>;
};

template <class R1, class R2>
struct Ratio_add : _Ratio_add<R1, R2>::type
{
};

template <class R1, class R2>
struct _Ratio_subtract
{
    using type = Ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den>;
};

template <class R1, class R2>
struct Ratio_subtract : _Ratio_subtract<R1, R2>::type
{
};

template <class R1, class R2>
struct _Ratio_multiply
{
    using type = Ratio<R1::num * R2::num, R1::den * R2::den>;
};

template <class R1, class R2>
struct Ratio_multiply : _Ratio_multiply<R1, R2>::type
{
};

template <class R1, class R2>
struct _Ratio_divide
{
    using type = Ratio<R1::num * R2::den, R1::den * R2::num>;
};

template <class R1, class R2>
struct Ratio_divide : _Ratio_divide<R1, R2>::type
{
};

int main()
{
    using r1 = Ratio<2, 3>;
    using r2 = Ratio<3, 2>;

    using r3 = Ratio_add<r1, r2>;
    std::cout << "2/3 + 3/2 = " << r3::num << " / " << r3::den << std::endl;

    using r4 = Ratio_multiply<r1, r3>;
    std::cout << "13 / 6 * 2 /3 = " << r4::num << " / " << r4::den << std::endl;
}