/*
    std::optional<T> 가 std::pair<bool, T> 와 가장 큰 차이점이 바로
    pair 와는 달리 아무 것도 들고 있지 않는 상태에서 디폴트 객체를 가질 필요가 없다 라는 점이다.

    optional 객체를 만들어도 아무런 객체를 전달받지 못한다면 optional 객체만 만들어 지고
    전달받은 타입의 객체를 복사 생성되지 않는다.

    즉 std::optional을 이용해 어떤 객체를 보관하거나 말거나라는 의미가 쉽게 이해할 수 있다.

    std::optional의 단점으로는 일반적인 방법으로는 reference를 포함할 수 없다는 점이다.
    ex) 컴파일 오류를 발생시킨다.
        std::optional<class&> temp_a = a;

    그렇다고 reference를 optional에 포함할 수 없는것은 아니다.
    std::reference_wrapper 를 사용해서 reference처럼 동작하는 wrapper 객체를 정의하면 된다.
    ex) 
        std::optional<std::reference_wrapper<A>> maybe_a = std::ref(a);
    
    std::reference_wrapper는 reference가 아니라 일반적인 객체이기 때문에 optional에 전달할 수 있다.
    reference_wrapper를 get() 함수를 이용해 reference하고 있는 객체를 얻어오게 된다.
    대신 reference_wrapper 객체를 생성하기 위해서는 std::ref() 함수를 사용해야 된다.

    optional은 *를 이용해 역참조 해도 되지만 -> 역시 정의되어 있어 가지고있는 값에 함수를 호출할 수 있다.

*/
#include <iostream>
#include <utility>

class A
{
public:
    A() { std::cout << "디폴트 생성" << std::endl; }

    A(const A &a) { std::cout << "복사 생성" << std::endl; }

    int data;
};

int main()
{
    A a;
    a.data = 5;

    std::cout << "Optional 객체 만듦 ---- " << std::endl;
    std::optional<A> maybe_a; // A의 복사생성자가 호출되지 않는다.

    std::cout << "maybe_a 는 A 객체를 포함하고 있지 않기 때문에 디폴트 생성할 "
                 "필요가 없다."
              << std::endl;
    maybe_a = a; // 복사생성자 호출

    // reference를 가지는 optional
    // 컴파일 오류 발생
    // std::optional<A&> maybe_a = a;

    // maybe_a 는 a 의 복사복이 아닌 a 객체 자체의 레퍼런스를 보관하게 된다.
    std::optional<std::reference_wrapper<A>> maybe_a = std::ref(a);

    // * 말고도 -> 연산자도 정의되어 있다.
    maybe_a->get().data = 3;

    // 실제로 a 객체의 data 가 바뀐 것을 알 수 있다.
    std::cout << "a.data : " << a.data << std::endl;
}