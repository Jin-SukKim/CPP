// shared_ptr
/*
    여러개의 스마트 포인터가 하나의 객체를 가르킬 경우 이 자원을 사용하는
    모든 객체들을 소멸시켜준 뒤 자원을 해제시켜 주어야한다.
    하지만 언제 이 객체들이 다 소멸되는지 알 수 없기 때문에
    특정 자원을 몇 개의 객체에서 가르키는지를 추적한 다음 그 수가 0이 되면 해제시켜주는 스마트 포인터가 shared_ptr 이다.

    unique_ptr과 다르게 shared_ptr은 한 객체를 여러 shared_ptr로 가르킬 수 있다.
    그래서 shared_ptr은 같은 객체를 몇 개의 포인터가 가르키는 지 숫자를 카운트하는 reference count(참조 개수)가 존재한다.
    이 참조 개수가 0이 되어야 메모리 해제가 가능하다.

    여러 포인터가 같은 객체를 가르킬 때 참조 개수 동기화를 위해 control block(제어 블록)을 동적으로 할당한 후
    포인터들이 이 블록에 필요한 정보를 공유한다.

    동적 할당은 시간과 공간 모두 많이 사용하는 연산이다.
    그래서 make_shared 함수를 이용하면 빠르게 생성할 수 있다.
        std::shared_ptr<A> p1(new A()); // 동적 할당이 두 번일어난다. (A객체 생성과 shared_ptr의 control block 생성)
        를 make_shared로 바꾸면

        std::shared_ptr<A> p1 = std::make_shared<A>(); // 동적 할당이 한번만 일어난다.(A 객체와 control block을 한번에 생성한 뒤 return해준다.)

    shared_ptr의 주의할 점은 shared_ptr의 인자로 주소값을 전달할 경우 자기가 해당 객체를 처음 소유하는 객체처럼 동작한다.
    ex)
        A* a = new A();
        std::shared_ptr<A> pa1(a);
        std::shared_ptr<A> pa2(a);

        pa1과 pa2의 control block이 각각 따로 생성된다. 즉 제어 블록이 하나만 생성되어야 하지만 두개가 생성된다.
        그래서 reference count가 1이되 한 포인터에서 메모리를 해제할 경우 다른 포인터가 가르키고 있어도 해제가 되 오류를 발생시킨다.

    객체 내부에서 자기 자신을 가르키는 shared_ptr도 마찬가지로 위와 같은 오류를 만들어 낸다.

    단 객체 내부에서 자신을 가르키는 shared_ptr은 enable_shared_from_this를 통해 해결할 수 있다.
    this를 이용해 shared_ptr을 사용하고 싶은 클래스가 있다면 enable_shared_from_this를 상속받으면 된다.

    enable_shared_from_this 클래스에는 shared_from_this라는 멤버 함수가 정의되어 있다.
    이 함수는 이미 정의되어 있는 control block을 이용해서 shared_ptr을 생성한다.

    따라서 같은 객체에 두 개의 control block이 생성되는 것을 막아준다.
    단 shared_from_this를 사용하기 위해서는 shared_ptr이 먼저 생성되어 있어야 된다.
    이 함수는 이미 있는 control block을 확인해 사용할 뿐 새로 생성해주지 않기 때문이다.

    서로 참조하는 shared_ptr, (순환 참조)는 참조 개수가 절대로 0이 될 수없다.
    shared_ptr은 reference count가 0이 되어야 해제할 수 있는데 shared_ptr1과 shared_ptr2가 서로를 참조하는 경우
    reference count가 0이 될 수 없어 서로 해제 하지 못한다.
    shared_ptr1의 객체가 소멸되려면 shared_ptr2가 먼저 소멸되야 되는데 shared_ptr2는 shared_ptr1이 소멸되어야 해서 그렇다.

    이런 문제는 shared_ptr에서는 해결할 수 없어서 나온게 weak_ptr이다.
*/
#include <iostream>
#include <memory>
#include <vector>

class A : public std::enable_shared_from_this<A>
{
    int *data;
    std::shared_ptr<A> other;

public:
    A()
    {
        data = new int[100];
        std::cout << "자원을 획득함!" << std::endl;
    }

    ~A()
    {
        std::cout << "소멸자 호출!" << std::endl;
        delete[] data;
    }

    // 자기 자신을 가르키는 shared_ptr, 오류를 발생
    std::shared_ptr<A> get_shared_ptr1() { return std::shared_ptr<A>(this); }

    // enable_shared_from_this 를 이용해 클래스 내부에서 자신을 가르키는 shared_ptr을 사용할 수 있게 해준다.
    std::shared_ptr<A> get_shared_ptr2() { return shared_from_this(); }

    // 서로를 참조하는 shared_ptr
    void set_other(std::shared_ptr<A> o) { other = o; }
};

int main()
{
    std::vector<std::shared_ptr<A>> vec;

    // vector의 모든 객체가 같은 A객체를 가르킨다.
    vec.push_back(std::shared_ptr<A>(new A()));
    vec.push_back(std::shared_ptr<A>(vec[0]));
    vec.push_back(std::shared_ptr<A>(vec[1]));

    // shared_ptr의 참조 개수
    std::cout << vec[0].use_count() << std::endl;

    // 모든 포인터가 소멸된 후 메모리를 해제할 수 있기 때문에 처음 두개의 erase는 아무것도 안하다 마지막에 소멸자를 호출한다.
    // 벡터의 첫번째 원소를 소멸 시킨다.
    std::cout << "첫 번째 소멸!" << std::endl;
    vec.erase(vec.begin());

    // 그 다음 원소를 소멸 시킨다.
    std::cout << "다음 원소 소멸!" << std::endl;
    vec.erase(vec.begin());

    // 마지막 원소 소멸
    std::cout << "마지막 원소 소멸!" << std::endl;
    vec.erase(vec.begin());

    std::cout << "프로그램 종료!" << std::endl;

    // 오류 발생 코드, control block이 두 개 생성된다.
    A *a = new A();

    std::shared_ptr<A> pa1(a);
    std::shared_ptr<A> pa2(a);

    std::cout << pa1.use_count() << std::endl;
    std::cout << pa2.use_count() << std::endl;

    // 자기 자신을 가르키는 shared_ptr
    // 오류 발생 코드
    std::shared_ptr<A> pa1 = std::make_shared<A>();
    std::shared_ptr<A> pa2 = pa1->get_shared_ptr1();

    std::cout << pa1.use_count() << std::endl;
    std::cout << pa2.use_count() << std::endl;

    // 정상적인 사용법
    std::shared_ptr<A> pa1 = std::make_shared<A>();
    std::shared_ptr<A> pa2 = pa1->get_shared_ptr2();

    std::cout << pa1.use_count() << std::endl;
    std::cout << pa2.use_count() << std::endl;

    // 순환 참조, 소멸자가 호출되지 않는다. weak_ptr을 통해 해결한다.
    std::shared_ptr<A> pa = std::make_shared<A>();
    std::shared_ptr<A> pb = std::make_shared<A>();

    pa->set_other(pb);
    pb->set_other(pa);
}