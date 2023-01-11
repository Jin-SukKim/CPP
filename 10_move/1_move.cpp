// move 문법 (move semantics)
/*
    template <typename T>
    void my_swap(T &a, T &b) {
        T tmp(a);
        a = b;
        b = tmp;
    }
    swap은 복사를 3번이나 해 메모리와 시간 둘다 많이 소모한다.
    만약 복사가 아닌 전처럼 이동을 통해 주소값만 바꾸게 한다면 공간 낭비를 줄일 수 있다.
    다만 swap에서 lvalue(실체가있다)를 사용하기 떄문에 이동 생성자를 오버로딩 할 수 없다.

    이것을 해결하기 위해 utility lib에서 move 함수를 이용한다.
*/
#include <iostream>
#include <utility>

class A
{
public:
    A() { std::cout << "일반 생성자 호출!" << std::endl; }
    A(const A &a) { std::cout << "복사 생성자 호출!" << std::endl; }
    A(A &&a) { std::cout << "이동 생성자 호출!" << std::endl; }
};

int main()
{
    A a;

    std::cout << "---------" << std::endl;
    // 복사생성자 호출, a가 이름이 있는 lvalue이므로 lvalue reference가 참조하기 때문
    A b(a); 

    std::cout << "---------" << std::endl;
    // 이동생성자 호출, std::move()함수가 인자로 받은 객체를 rvalue로 변환해서 return해주기 때문이다.
    // 무언가를 이동시키는 것이 아닌 type을 변환시켜주는 것이다.
    A c(std::move(a));  
}