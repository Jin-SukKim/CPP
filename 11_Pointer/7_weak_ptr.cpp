// weak_ptr
/*
    순환 참조 문제를 해결하기 위해 만든어진 포인터이다.

    만약 트리 구조를 지원하는 클래스를 생성하는 경우, 한 개의 노드는 여러개의 자식 노드를 가질 수 있지만
    단 한 개의 부모 노드를 가진다. (부모 노드는 여러 자식 노드를 가지지만 자식은 하나의 부모 노드만 가진다.)
   
    ex) 기본적인 자료 구조
        class Node {
            std::vector<std::shared_ptr<Node>> children;
            // 어떤 타입이 와야할까? parent;

        public:
            Node(){};
            void AddChild(std::shared_ptr<Node> node) { children.push_back(node); }
        };

    부모가 여러개의 자식 노드를 가질 수 있으므로 shared_ptr들의 vector로 나타낼 수 있다.
    자식 노드들도 부모 노드가 있으므로 부모 노드를 가르키는 포인터가 존재한다.

    부모 노드를 가르키는 parent를 무슨 타입으로 정의해야 될까?
    
    문제 1: 만약에 일반 포인터(Node *) 로 하게 된다면, 메모리 해제를 까먹고 하지 않을 경우 혹은 예외가 발생하였을 경우 
            적절하게 자원을 해제하기 어렵습니다. 물론 이미 해제된 메모리를 계속 가리키고 있을 위험도 있다.
    
    문제 2: 하지만 이를 shared_ptr 로 하게 된다면 앞서 본 순환 참조 문제가 생긴다.
            부모와 자식이 서로를 가리키기 때문에 참조 개수가 절대로 0 이 될 수 없다. 
            따라서, 이 객체들은 프로그램 끝날 때 까지 절대로 소멸되지 못하고 남아있게 된다.
        
    weak_ptr은 shared_ptr과 일반 포인터의 사이에 위치한 스마트 포인터이다.
    스마트 포인터처럼 객체를 안전하게 참조할 수 있게 해주지만, shared_ptr과는 다르게 참조 개수(reference count)를 늘리지는 않는다.
    따라서 weak_ptr로 가르키고 있다고 하더라도 다른 shared_ptr들이 가르키고 있지 않다면 객체를 소멸할 수 있다.

    이것 때문에 weak_ptr 자체로는 원래 객체를 참조할 수는 없고, 반드시 shared_ptr로 변환해서 사용해야 된다.
    이 때 가르키고 있던 객체가 이미 소멸되었다면 빈 shared_ptr로 변환되고 아닌경우 해당 객체를 가르키는 shared_ptr로 변경된다.

    shared_ptr은 reference count가 0일 때 객체를 해제하는 데 control block은 메모리에서 해제하지 않는다.
    weak_ptr이 남아있을 수도 있기 때문이다.
    따라서 control block에는 weak count(약한 참조 개수)도 존재한다.

    즉 control block을 해제하기 위해서는 shared_ptr과 weak_ptr모두 0개 여야된다.
*/
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class A
{
    std::string s;
    std::weak_ptr<A> other;

public:
    A(const std::string &s) : s(s) { std::cout << "자원을 획득함!" << std::endl; }

    ~A() { std::cout << "소멸자 호출!" << std::endl; }

    // weak_ptr은 생성자로 shared_ptr이나 다른 weak_ptr을 받을 수 있다.
    // 또한 shared_ptr과는 다르게 이미 제어 블록이 만들어진 객체만 의미를 가져 평범한 주소값으로 weak_ptr을 생성할 수 없다.
    void set_other(std::weak_ptr<A> o) { other = o; }

    /*
        weak_ptr로는 원소를 참조할 수 없어 shared_ptr로 변환 후 사용해야 되는데 lock함수를 이용해 수행할 수 있다.
        lock()함수는 만일 weak_ptr이 가르키는 객체가 아직 메모리에 살아 있다 하더라도(참조 개수가 0이 아니라면)
        해당 객체를 가르키는 shared_ptr을 return하고 이미 해제 되었다면 아무것도 가르키지 않는 빈 shared_ptr을 반환한다.
    */
    void access_other()
    {
        std::shared_ptr<A> o = other.lock();
        // 빈 shared_ptr은 bool값 false로 형변환된다.
        if (o)
        {
            std::cout << "접근 : " << o->name() << std::endl;
        }
        else
        {
            std::cout << "이미 소멸됨 ㅠ" << std::endl;
        }
    }
    std::string name() { return s; }
};

int main()
{
    std::vector<std::shared_ptr<A>> vec;
    vec.push_back(std::make_shared<A>("자원 1"));
    vec.push_back(std::make_shared<A>("자원 2"));

    vec[0]->set_other(vec[1]);
    vec[1]->set_other(vec[0]);

    // pa 와 pb 의 ref count 는 그대로다.
    std::cout << "vec[0] ref count : " << vec[0].use_count() << std::endl;
    std::cout << "vec[1] ref count : " << vec[1].use_count() << std::endl;

    // weak_ptr 로 해당 객체 접근하기
    vec[0]->access_other();

    // 벡터 마지막 원소 제거 (vec[1] 소멸)
    vec.pop_back();
    vec[0]->access_other(); // 접근 실패!
}