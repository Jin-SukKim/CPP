/*
    C++에서 callable들을 객체의 형태로 저장할 수 있는 std::function을 제공한다.
    C에서 함수 포인터는 함수만 저장할 수 있었다면 이 클래스는 함수와 모든 Callable들을 보관해주는 객체이다.

    std::function은 템플릿 인자로 전달받을 함수의 타입을 갖는다.
    함수의 타입이란 return값과 함수의 인자를 의미한다.

    std::function은 모든 Callable을 마음대로 보관할 수 있는 객체이기 때문에
    이것을 이용해 함수 포인터로는 보관할 수 없던 Functor같은 것을 보관할 수 있다.

    하지만 std::function은 일반적인 Callable은 쉽게 보관할 수 있지만 멤버 함수들은 조금 다르다.
    멤버 함수 내의 this의 경우 자신을 호출한 객체를 의미하기 떄문에 만일 std::function안에
    멤버 함수를 넣는다면 this가 무엇을 의미하는지 모르게된다.

    멤버 함수들은 구현 상 자신을 호출한 객체를 인자로 암묵적으로 받고 있다.
    그래서 원래 인자에 추가적으로 객체를 받는 인자를 전달해줘야 된다. (Class B가 예시)
    이때 상수 함수는 상수로 아닌 경우 reference의 형태로 받으면 된다.

    이전 함수들과는 다르게 &B::some_func 와 같이 함수의 이름만으로 주소값을 전달할 수 없다.
    멤버 함수가 아닌 모든 함수들은 함수의 이름이 함수 주소값으로 암시적 변환이 이루어지지만,
    멤버 함수들의 경우 암시적 변환이 발생하지 않아 & 연산자를 통해 명시적으로 주소값을 전달해줘야 된다.

    호출하고자 하는 객체를 인자로 전달해주면 마치 해당 객체의 멤버 함수를 호출한 것과 같은 효과를 낼 수 있다.
*/
#include <functional>
#include <iostream>
#include <string>

int some_func1(const std::string &a)
{
    std::cout << "Func1 호출! " << a << std::endl;
    return 0;
}

struct S
{
    void operator()(char c) { std::cout << "Func2 호출! " << c << std::endl; }
};

class A
{
    int c;

public:
    A(int c) : c(c) {}
    int some_func() { std::cout << "내부 데이터 : " << c << std::endl; }
};

class B
{
    int c;

public:
    B(int c) : c(c) {}
    int some_func()
    {
        std::cout << "비상수 함수: " << ++c << std::endl;
        return c;
    }

    int some_const_function() const
    {
        std::cout << "상수 함수: " << c << std::endl;
        return c;
    }

    static void st() {}
};
int main()
{
    // int를 return하며 const std::string&를 인자로 받는다
    std::function<int(const std::string &)> f1 = some_func1;

    // S의 객체 전달시 함수처럼 받게된다. 인자로 char을 받고 return은 void이다.
    std::function<void(char)> f2 = S();

    // lambda 함수로 void를 return하고 인자는 아무것도 받지 않는다.
    std::function<void()> f3 = []() { std::cout << "Func3 호출! " << std::endl; };

    f1("hello");
    f2('c');
    f3();

    // std::function안에 멤버 함수가 들어가 this가 무엇인지 모르게 된다.
    A a(5);

    // f1 호출시 함수는 자신을 호출하는 객체가 무엇인지 몰라 c를 참조할 때 어떤 객체의 c인지 모른다.
    std::function<int()> f1 = a.some_func;

    B b(5);
    // 원래 인자에 추가적으로 객체를 받는 인자를 전달해준다.
    std::function<int(B &)> f1 = &B::some_func;
    std::function<int(const B &)> f2 = &B::some_const_function;

    f1(b);
    f2(b);
}