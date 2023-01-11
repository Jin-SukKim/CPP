// unique_ptr
/*
    함수 인자로 전달하기

    unique_ptr

    1. unique_ptr 은 어떤 객체의 유일한 소유권을 나타내는 포인터 이며, unique_ptr 가 소멸될 때, 가리키던 객체 역시 소멸된다.

    2. 만약에 다른 함수에서 unique_ptr 가 소유한 객체에 일시적으로 접근하고 싶다면, get 을 통해 해당 객체의 포인터를 전달하면 된다.

    3. 만약에 소유권을 이동하고자 한다면, unique_ptr 를 move 하면 된다.
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

    void do_sth(int a)
    {
        std::cout << "무언가를 한다!" << std::endl;
        data[0] = a;
    }

    ~A()
    {
        std::cout << "자원을 해제함!" << std::endl;
        delete[] data;
    }
};

// 올바르지 않은 전달 방식
/*
    작동은 하지만 unique_ptr은 어떠한 객체의 소유권을 유일하게 가지고 있다는 원칙을 위반하게 된다.
    하지만 reference로 이 포인터를 전달한다면 do_something함수 내부에서
    unique_ptr은 더이상 유일한 소유권을 가지고 있다고 할 수 없다.
    reference이기 때문에 pa가 가르키는 객체를 파괴하지는 않지만 이 함수 내부에서 ptr을 통해
    객체를 소유할 수 있다. 원칙을 무시하고 포인터를 단순한 wrapper로 사용하는 것과 같다.
*/
void do_something(std::unique_ptr<A> &ptr) { ptr->do_sth(3); }

// 올바른 전달 방식
/*
    원래의 포인터 주소값을 전달해 준다.
    일반적인 포인터를 받고 있다.
    이 경우 소유권이라는 의미는 버리고 함수 내부에서 객체에 접근할 수 있는 권한을 주는 것이다.
*/
void do_something(A *ptr) { ptr->do_sth(3); }

int main()
{
    std::unique_ptr<A> pa(new A());
    do_something(pa);

    // unique_ptr의 get함수는 실제 객체의 주소값을 return해준다.
    do_something(pa.get());
}