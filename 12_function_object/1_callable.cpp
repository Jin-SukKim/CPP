// Callable : 호출할 수 있는 모든 것(함수 등)
/*
    C++에서는 주고 () 로 호출 가능한 것을 callable이라고 한다.
    (함수, Functor 등등)
*/

#include <iostream>

struct S
{
    void operator()(int a, int b) { std::cout << "a + b = " << a + b << std::endl; }
};

int main()
{
    S some_obj;

    // some_obj.operator(3,5) 이므로 객체지만 함수처럼 사용했다.
    some_obj(3, 5);

    // f 역시 labmda 함수로 일반적인 함수는 아니지만 ()를 이용해 호출가능하다.
    auto f = [](int a, int b)
    { std::cout << "a + b = " << a + b << std::endl; };
    f(3, 5);
}