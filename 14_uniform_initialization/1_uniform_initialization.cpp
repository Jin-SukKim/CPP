// 균일한 초기화(Uniform Initialization)
/*
    #include <iostream>

    class A {
    public:
        A() { std::cout << "A 의 생성자 호출!" << std::endl; }
    };

    public:
        B(A a) { std::cout << "B 의 생성자 호출!" << std::endl; }
    };

    int main() {
        A a();  // ?

        B b(A());
    }
    위의 코드는 생성자를 호출해 출력이 될 것 같지만 아무것도 출력되지 않는다.

    왜냐하면 A의 객체 a를 만드는 것이 아닌 A를 return하고 인자를 받지않는 함수 a를 정의한 것을 된 것이다.
    C++ 컴파일러는 함수의 정의처럼 보이는 것들을 모두 함수의 정의로 해석하기 떄문이다.

    B b(A());도 그저 인자로 A를 return하고 인자가 없는 함수를 받고 return 타입이 B인 함수 b를 정의한 것이다.

    이런 문제는 () 가 함수의 인자들을 정의하는데도 사용되고, 그냥 일반적인 객체 생성자를 호출하는데도 사용되기 떄문이다.

    이런한 점들을 해결하기 위해 uniform initialization을 사용하고 있다.

    생성자 호출을 위해 ()가 아닌 {}를 사용한다.
    그냥 기존 ()의 위치에 {}를 대신 사용하면 되지만 {}의 경우 일부 암시적 타입 변환들을 불허하고 있다는 점이다.
    모든 데이터 손실이 있는(Narrowing) 변환들이 불가능해 진다

    1. 부동 소수점 타입에서 정수 타입으로의 변환 (double -> int 등)

    2. long double 에서 double 혹은 float 으로의 변환, double 에서 float 으로의 변환.

    3. 정수 타입에서 부동 소수점 타입으로의 변환
    등등

    {}을 이용해 원하지 않는 type-casting을 막아줄 수 있는 것이다.

    또한 {}는 함수 return시 굳이 생성하는 객체의 타입을 다시 명시하지 않아도 된다.
*/
#include <iostream>

class A
{
public:
    A(int x) { std::cout << "A 의 생성자 호출!" << std::endl; }
    A(int x, double y) { std::cout << "A 생성자 호출" << std::endl; }
};

A func()
{
    return {1, 2.3}; // A(1, 2.3) 과 동일
}

class B
{
public:
    B() { std::cout << "A 의 생성자 호출!" << std::endl; }
};
int main()
{   
    // unifor initialization 기본 사용법, ()위치에 {}를 넣어준다.
    B b{}; // 균일한 초기화!

    // {}을 사용하면 암시적 타입 변환이 안된다. (type-casting을 막는다.)
    A a(3.5); // Narrow-conversion 가능
    A b{3.5}; // Narrow-conversion 불가

    // 함수 return시 굳이 생성하는 객체의 타입을 명시하지 않아도 된다.
    func();
}