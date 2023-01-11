// unique_ptr을 원소로 가지는 Container
/*
    다른 포인터와 동일하지만 복사생성자가 없으므로 move와 perfect forwading을 활용해
    container를 사용해야 된다.
*/
#include <iostream>
#include <memory>
#include <vector>

class A
{
    int *data;

public:
    A(int i)
    {
        std::cout << "자원을 획득함!" << std::endl;
        data = new int[100];
        data[0] = i;
    }

    void some() { std::cout << "일반 포인터와 동일하게 사용가능!" << std::endl; }

    ~A()
    {
        std::cout << "자원을 해제함!" << std::endl;
        delete[] data;
    }
};

int main()
{
    std::vector<std::unique_ptr<A>> vec;
    std::unique_ptr<A> pa(new A(1));

    // vector의 push_back 함수는 전달된 인자를 복사해 집어 넣는다.
    // vector가 삭제된 unique_ptr의 복사생성자에 접근하려고 하기 때문에 오류가 발생한다.
    vec.push_back(pa); // 오류 발생

    /*
        복사 문제를 방지하기 위해 명시적으로 unique_ptr을 vector안으로 이동시켜줘야 한다.
        즉 push_back의 rvalue reference 받는 버전이 overloading될 수 있도록 해주는 것이다.
    */
    vec.push_back(std::move(pa));

    /*
        또 emplace_back을 이용하면 vector안에 unique_ptr을 직접 생성하면서 집어넣는다. 즉 이동과정을 생략시켜준다.
        vec.push_back(std::unique_ptr<A>(new A(1))); 과 동일
        emplace_back 함수는 전달된 인자를 perfect forwarding을 통해 직접 unique_ptr의 생성자에 전달해
        vector 맨 뒤에 unique_ptr의 객체를 생성하게 된다. 그래서 이동과정이 필요없다.
    */
    vec.emplace_back(new A(1));

    vec.back()->some();
}