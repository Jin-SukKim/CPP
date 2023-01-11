// initializer_list 주의점
/*
    만일 {}을 이용해 객체를 생성할 경우 생성자 오버로딩 시에
    initializer_list를 인자로 받는 생성자가 '최우선'으로 고려된다.
    ex) vector의 생성자
        vector(size_type count); // count의 개수 만큼 원소 자리를 미리 생성해두는 생성자

        vector v{10} 으로 vector를 정의할 경우 그냥 원소 1개짜리 initializer_list라 생각해
        10을 보관하고 있는 vector를 생성하게 된다.

        이런 경우 vector v(10)으로 ()를 이용하면 원소가 10개인 vector를 생성할 수 있게 된다.

    즉, 컴파일러는 최선을 다해 해당 생성자와 매칭시키려고 노력한다는 뜻이다.

    아래의 코드의 경우 ()를 이용해 객체 A를 생성하면 생성자 A(int x, double y)를 불러와
    문제가 없다.
    하지만 {}를 이용해 객체 A를 생성하면 A(int x, double y)보다는 initializer_list를 인자로 받는
    생성자가 최우선으로 고려된다.
    컴파일러는 initializer_list를 이용하도록 최대한 노력하려고해 1.5는 int가 아니지만,
    double에서 int로 암시적 타입 변환이 가능해 이 생성자를 선택한다.
    그렇지만 {}는 데이터 손실이 있는 타입 변환은 불가능해 오류가 발생한다.

    이런 오류가 발생하지 않으려면 {}를 사용하지 않거나 타입 변환 자체가 불가능한 경우여야 된다(class B)
*/
#include <initializer_list>
#include <iostream>

class A
{
public:
    A(int x, double y) { std::cout << "일반 생성자! " << std::endl; }

    A(std::initializer_list<int> lst)
    {
        std::cout << "초기화자 사용 생성자! " << std::endl;
    }
};

class B
{
public:
    B(int x, double y) { std::cout << "일반 생성자! " << std::endl; }

    B(std::initializer_list<std::string> lst)
    {
        std::cout << "초기화자 사용 생성자! " << std::endl;
    }
};
int main()
{
    A a(3, 1.5); // Good
    // 오류 발생
    A b{3, 1.5}; // Bad!

    // 타입 변환 자체가 불가능한 경우
    A a(3, 1.5);       // 일반
    A b{3, 1.5};       // 일반
    A c{"abc", "def"}; // 초기화자
}