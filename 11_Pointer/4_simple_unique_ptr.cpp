// unique_ptr 을 쉽게 생성하기, make_unique 함수를 이용해 간단하게 unique_ptr을 만들 수 있따.
#include <iostream>
#include <memory>

class Foo
{
    int a, b;

public:
    Foo(int a, int b) : a(a), b(b) { std::cout << "생성자 호출!" << std::endl; }
    void print() { std::cout << "a : " << a << ", b : " << b << std::endl; }
    ~Foo() { std::cout << "소멸자 호출!" << std::endl; }
};

int main()
{   
    // std::unique_ptr<Foo> ptr(new Foo(3, 5));와 같다  
    auto ptr = std::make_unique<Foo>(3, 5);
    ptr->print();
}