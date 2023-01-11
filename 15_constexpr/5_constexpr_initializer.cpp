// constexpr 생성자
/*
    constexpr 생성자의 경우 일반적인 constexpr 함수의 모든 제약조건이 모두 적용된다.
    그리고 constexpr 생성자의 인자들은 모두 literal 타입이어야 된다.

    constexpr 생성자를 가진 클래스는 다른 클래스를 가상 상속받을 수 없다.

    constexpr 객체의 constexpr 멤버 함수만이 constexpr을 준다.
    Literal은 값 그자체이고 const는 변수를 뜻한다.
*/
#include <iostream>

class Vector
{
public:
    // constexpr 생성자이다. literal 타입인 int 두 개를 받고 있다.
    constexpr Vector(int x, int y) : x_(x), y_(y) {}

    // constexpr 함수로 정의해 x_와 y_ 값을 constexpr 내부에서 꺼낼 수 있게 해줬다.
    constexpr int x() const { return x_; }
    constexpr int y() const { return y_; }

private:
    int x_;
    int y_;
};

constexpr Vector AddVec(const Vector &v1, const Vector &v2)
{
    return {v1.x() + v2.x(), v1.y() + v2.y()};
}

template <int N>
struct A
{
    int operator()() { return N; }
};

int main()
{   
    // constexpr 생성자를 만들었기 떄문에 constexpr 객체로 선언할 수 있었다.
    constexpr Vector v1{1, 2};
    constexpr Vector v2{2, 3};

    // constexpr 객체 내부의 멤버함수 접근을 contexpr 함수로 해 컴파일 타임에 접근했다.
    A<v1.x()> a;
    std::cout << a() << std::endl;

    // constexpr 함수를 사용해 constexpr 객체를 인자로 받아 constexpr 객체를 return해 컴파일 타입에 연산했다.
    A<AddVec(v1, v2).x()> b;
    std::cout << b() << std::endl;
}