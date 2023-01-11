// decltype은 타입 관련 연산에 요긴하게 쓰이는 키워드이다.
/*
    decltype은 키워드이지만 함수처럼 사용된다.

    기본 사용:
        decltype(타입을 알고자 하는 식)

    하지만 함수와는 달리 타입을 알고자하는 식의 타입으로 치환된다.

    decltype에 전달되는 식이 괄호로() 둘러싸이지 않은 식별자 표현식(id-expression)이라면
    해당식의 타입을 얻을 수 있다.

    식별자 표현식(id-expression)이란 변수의 이름, 함수의 이름, 
    enum 이름, 클래스 멤버 변수(a.b나 a->b 등)을 의미한다.

    쉽게 말해 어떠한 연산을 하지 않고 단순히 객체 하나만을 가리키는 식이라고 보면된다.

    decltype에 식별자 표현식이 아닌 식을 전달하면 해당 식의 값의 종류(value category)에 따라 달라진다.
    
        1. 만일 식의 값 종류가 xvalue 라면 decltype 는 T&& 가 됩니다.
        2. 만일 식의 값 종류가 lvalue 라면 decltype 는 T& 가 됩니다.
        3. 만일 식의 값 종류가 prvalue 라면 decltype 는 T 가 됩니다.
    ex)
        int a, b;
        decltype(a + b) c;  // c 의 타입은?

        a + b는 prvalue이므로 a + b의 실제 type인 int로 추론된다.

        int a;
        decltype((a)) b;  // b 의 타입은?
        
        (a)는 식별자 표현식이 아니기 때문에 어떤 value category에 들어가는지 알아야 된다.
        쉽게 생각해 &(a)하면 주소값을 알 수 있고, 이동은 불가능 하므로 lvalue가 된다.
        따라서 int가 아닌 int&로 추론된다.
        ()의 유무로 결과가 달라지는 경우이다.
*/
#include <iostream>

struct A
{
    double d;
};

int main()
{
    int a = 3;
    decltype(a) b = 2; // int

    int &r_a = a;
    decltype(r_a) r_b = b; // int&

    int &&x = 3;
    decltype(x) y = 2; // int&&

    A *aa;
    decltype(aa->d) dd = 0.1; // double
}