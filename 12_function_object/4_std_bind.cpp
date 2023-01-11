// std::bind
/*
    함수 객체 생성 시 인자를 특정한 것을 지정할 수 있다.

    bind함수는 원래 함수에 특정 인자를 붙여준다(bind)
    ex)
        std::bind(add, 2, std::placeholders::_1);

        add 함수에 첫 번째 인자로 2를 bind 시켜주고 두 번째 인자로는
        새롭게 만들어진 함수 객체의 첫 번째 인자를 전달해준다.

        따라서 add_with_2(3)을 하면 첫 번째 인자로 2가 들어가고
        두 번째 인자로는 add_with_2의 첫 번째 인자인 3이 들어가게 된다.

        만약 add_with_2(3, 4)를 하더라고 함수 객체의 첫 번째 인자만 전달되므로 4는 무시된다.

        auto negate = std::bind(subtract, std::placeholders::_2, std::placeholders::_1); 의 경우

        첫 번째 인자와 두 번째 인자를 서로 뒤바꿔서 subtract함수를 호출한다.

    placeholders의 _1과 _2는 일일히 정의된 객체들이다.
    이 개수는 lib마다 다른데 libstdc++ 경우(g++의 c++ lib) _1부터 _29까지 정의되어 있다.

    만약 reference를 인자로 받는 경우 주의해야 된다.
    bind 함수는 인자가 복사되어서 전달되기 때문에 do_something_with_s1처럼 그냥 전달한다면
    인자가 복사되기 때문에 s1의 data값이 변경되지 않는다.

    그래서 명시적으로 객체의 reference를 전달해줘야 된다.
    std::ref 함수의 경우 전달받은 인자를 복사 가능한 reference로 변환해준다.
    따라서 bind 함수 안으로 reference가 잘 전달할 수 있게 해준다.

    const reference의 경우 std:cref를 호출해준다.
*/
#include <functional>
#include <iostream>

void add(int x, int y)
{
    std::cout << x << " + " << y << " = " << x + y << std::endl;
}

void subtract(int x, int y)
{
    std::cout << x << " - " << y << " = " << x - y << std::endl;
}

struct S
{
    int data;
    S(int data) : data(data) { std::cout << "일반 생성자 호출!" << std::endl; }
    S(const S &s)
    {
        std::cout << "복사 생성자 호출!" << std::endl;
        data = s.data;
    }

    S(S &&s)
    {
        std::cout << "이동 생성자 호출!" << std::endl;
        data = s.data;
    }
};

// 첫 번째 인자의 data를 두 번째 인자의 data+3으로 만들어준다.
void do_something(S &s1, const S &s2) { s1.data = s2.data + 3; }

int main()
{
    auto add_with_2 = std::bind(add, 2, std::placeholders::_1);
    add_with_2(3);

    // 두 번째 인자는 무시된다.
    add_with_2(3, 4);

    auto subtract_from_2 = std::bind(subtract, std::placeholders::_1, 2);
    auto negate =
        std::bind(subtract, std::placeholders::_2, std::placeholders::_1);

    subtract_from_2(3); // 3 - 2 를 계산한다.
    negate(4, 2);       // 2 - 4 를 계산한다

    // reference 이용하기
    S s1(1), s2(2);

    std::cout << "Before : " << s1.data << std::endl;

    // s1 이 그대로 전달된 것이 아니라 s1 의 복사본이 전달됨! 제대로 계산되지 않는다.
    auto do_something_with_s1 = std::bind(do_something, s1, std::placeholders::_1);
    do_something_with_s1(s2);

    std::cout << "After(그냥 reference 이용) :: " << s1.data << std::endl;

    // 명시적으로 인자를 전달해준다.
    auto do_something_with_s2 = std::bind(do_something, std::ref(s1), std::placeholders::_1);
    do_something_with_s2(s2);

    std::cout << "After(명시적으로 reference전달) :: " << s1.data << std::endl;
}