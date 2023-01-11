// 객체의 유일한 소유권 - unique_ptr, 포인터 객체
/*
    C++에서 메모리 관리를 잘못할 경우 크게 두 가지 종류의 문제점이 발생할 수 있다.

        1. 메모리를 사용한 후 해제하지 않은 경우 (메모리 누수: memory leak), 큰 프로그램의 경우 메모리 부족으로 오류가 날 수 있다.
            ㄴ RAII 패턴을 사용해 메모리를 항상 해제해 메모리 누수를 막을 수 있다.

        2. 이미 해제한 메모리를 다시 참조하는 경우
            ex)
                Data* data = new Data();
                Date* data2 = data;

                // data 의 입장 : 사용 다 했으니 소멸시켜야지.
                delete data;

                // ...

                // data2 의 입장 : 나도 사용 다 했으니 소멸시켜야지
                delete data2;

            이미 소멸된 객체를 다시 소멸하려는 경우로 메모리 오류를 발생시킨다.
            double free버그라고 불린다.

            이 문제의 발생 원인은 만들어진 객체의 소유권이 명확하지 않아서 발생한다.
            만약 포인터에 객체의 유일한 소유권을 부여하면 이 포인터말고 다른 포인터는 객체를 소멸시킬 수 없을 것이다.

            즉 data에 new Data()로 만들어진 객체의 소유권을 준다면 data2는 메모리를 해제할 수 없게된다.

        이렇게 소유권을 주는 포인터는 unique_ptr이라고 한다.
        unique_ptr로 RAII 패턴 또한 사용할 수 있다.
        unique_ptr은 스택에 정의된 객체를 사용하기 때문에 함수가 종료될 때 자동으로 소멸자를 호출시킨다.
        소멸자 안에 메모리를 해제해 주면 자원을 해제할 수 있다.

        또다른 unique_ptr로 다른 unique_ptr을 가르킬 수 없다.
        C++ 11부터는 사용을 원치않는 함수를 삭제할 수 있다.
            
            #include <iostream>

            class A {
                public:
                    A(int a){};
                    A(const A& a) = delete; // const A& a를 인자로 받는 함수를 삭제한다. (즉 복사생성자 삭제)
            };

            int main() {
                A a(3);  // 가능
                A b(a);  // 불가능 (복사 생성자는 삭제됨)
            }

            복사생성자를 명시적으로 삭제해줬기 때문에 b(a)는 컴파일 오류를 일으킨다.

        unique_ptr의 특성상 어떠한 객체를 유일하게 소유해야되서 복사생성자가 delete을 이용해 명시적으로 삭제했다.
        unique_ptr을 복사 생성할 수 있다면 특정 객체를 여러개의 unique_ptr들이 소유하게 되는 문제가 발생해 double free가 일어날 수 있기 때문이다.
*/
#include <iostream>
#include <memory>

class A
{
    int *data;

public:
    A()
    {
        std::cout << "자원을 획득함!" << std::endl;
        data = new int[100];
    }

    void some() { std::cout << "일반 포인터와 동일하게 사용가능!" << std::endl; }

    ~A()
    {
        std::cout << "자원을 해제함!" << std::endl;
        delete[] data;
    }
};

void do_something()
{
    // 템플릿에 인자로 포인터가 가르킬 클래스를 전달한다.
    // A* pa = new A();와 동일하다.
    std::unique_ptr<A> pa(new A());

    // 포인터와 같이 사용하면 된다.
    pa->some();

    // pb는 pa를 가르킬수 없다. 컴파일 에러
    // std::unique_ptr<A> pb = pa;
}

int main() { do_something(); }