// 자원(resource) 관리의 중요성 - Resource Acquisition Is Initialization - RAII
/*
    C++이후 나온 언어(Jaca등)은 garbage collector - GC라고 불리는 자원 청소기가 기본적으로 내장되어있따.
    이 GC의 역할은 프로그램 상에서 더이상 사용되지 않는 자원을 자동으로 해제해 주는 역할을 한다.

    하지만 C++은 이러한 경우가 없어 직업해제 해줘야한다.
    해제하지 않을 경우 프로그램이 종료되기 전까지 메모리에 계속 남아있고 종료되면 운영체제가 해제해준다.

    그래서 C++ 창시자는 자원을 관리하는 방법으로 한 디자인 패턴을 제안했다.
    Resource Acquisition Is Initialization - RAII라 불리는 자원의 획득은 초기화다.
    이는 자원 관리를 스택에 할당한 객체를 통해 수행하는 것이다.

    예외가 발생하면 stack unwinding이 발생하고 예외가 발생하지 않으면 함수가 종료될 시 소멸자가 호출된다.

    그렇다면 만약 이 소멸자들 안에 다 사용한 자원들을 해제하는 루틴을 넣으면 어떻게 될까?

    do_something()의 pointer pa의 경우 객체가 아니기 때문에 소멸자가 호출되지 않는다.
    그 대신 pa를 일반적인 pointer가 아닌 포인터 '객체'로 만들면 자신이 소멸될 때 자신이 가르키고 있는 데이터도 같이 delete하게 된다.
    즉, 자원(메모리) 관리를 스택의 객체(포인터 객체)를 통해 수행하게 되는 것이다.

    이렇게 작동하는 포인터 객체를 smart pointer라고 불린다.
    바로 unique_ptr과 shared_ptr이다.    
*/
#include <iostream>

class A
{
    int *data;

public:
    A()
    {
        data = new int[100];
        std::cout << "자원을 획득함!" << std::endl;
    }

    ~A()
    {
        std::cout << "자원을 해제함!" << std::endl;
        delete[] data;
    }
};

void thrower()
{
    // 예외를 발생시킴!
    throw 1;
}

void do_something()
{
    A *pa = new A();
    thrower();

    // 발생된 예외로 인해 delete pa 가 호출되지 않는다!
    // delete을 하지 않는다면 할당된 객체가 소멸되지 않는다.
    // 즉, 400 바이트 (4 * 100) 만큼의 메모리 누수 발생
    delete pa;
}

int main()
{
    try
    {
        do_something();
    }
    catch (int i)
    {
        // 예외는 정삭적으로 처리되었지만 delete pa가 실행되지 않아 메모리 누수를 피할수 없다.
        std::cout << "예외 발생!" << std::endl;
        // 예외 처리
    }
}