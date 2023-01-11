/*
C++은 객체를 알아서 추측할 수 있는 경우가 많이 있다.
ex: (??) a = 3; 에서 ??는 int,
some_class a;
(??) b = a; // ??는 some_class 가 된다.

즉 객체가 복사 생성 될 떄, 복사 생성되는 타입을 확실히 알 수 있다면 굳이 그 객체의 타입을
명시하지 않아도 컴파일러가 알아낼 수 있다.

물론 알아낼 수 없을 때도 있느데
ex: unit_lib.cpp에서 quantity<double, Dim<one, zero, zero>> kg(1);의 경우 타입 부분을 가리고
(???) kg(1)인 경우 컴파일러는 단순히 1을 보고 int로 초기화한다 생각해 원하는 타입이 들어갈 수 없다.

반면
(??) F = kg * meter / (second * second);의 F는
타입을 적지 않아도 오른쪽 계산을 통해 컴파일러가 타입을 알 수 있다.

이와 같이 컴파일러가 타입을 정확히 알아낼 수 있는 경우 굳이 그 길고 긴 타입을 적지 않고 간단히 auto 로 표현할 수 있습니다.
그리고 그 auto 에 해당하는 타입은 컴파일 시에 컴파일러에 의해 추론됩니다.
*/
#include <iostream>
#include <typeinfo>

int sum(int a, int b) { return a + b; }

class SomeClass
{
    int data;

public:
    SomeClass(int d) : data(d) {}
    SomeClass(const SomeClass &s) : data(s.data) {}
};

int main()
{
    auto c = sum(1, 2);   // 함수 리턴 타입으로 부터 int 라고 추측 가능
    auto num = 1.0 + 2.0; // double 로 추측 가능!

    SomeClass some(10);
    auto some2 = some;

    auto some3(10); // SomeClass 객체를 만들까요?

    std::cout << "c 의 타입은? :: " << typeid(c).name() << std::endl;
    std::cout << "num 의 타입은? :: " << typeid(num).name() << std::endl;
    std::cout << "some2 의 타입은? :: " << typeid(some2).name() << std::endl;
    std::cout << "some3 의 타입은? :: " << typeid(some3).name() << std::endl;
}