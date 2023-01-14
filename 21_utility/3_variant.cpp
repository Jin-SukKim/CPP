// std::variant(C++ 17 이상)
/*
    std::variant는 one-of를 구현한 클래스이다.
    즉 컴파일 타임에 정해진 여러가지 타입들 중에 한 가지 타입의 객체를 보관할 수 있는 클래스이다.

    공용체(union)을 이용해 해결할 수도 있지만 union이 현재 어떤 타입의 객체를
    보관하고 있는지 알 수 없기에 실제로는 매우 위험하다.

    variant를 정의할 때 포함하고자 하는 타입들을 명시해줘야 한다.
    ex) 정의된 int, std::string, double 중 하나의 타입을 가질 수 있다.
        // v 는 이제 int
        std::variant<int, std::string, double> v = 1;

        // v 는 이제 std::string
        v = "abc";

        // v는 이제 double
        v = 3.14;

    variant의 가장 큰 특징으로는 반드시 값을 들고 있어야 된다는 점이다.
    std::variant<int, std::string, double> v; 이라고 그냥 정의한다면
    v에는 첫 번쨰 타입의 인자인 int의 default 생성자가 호출되 0이 들어가게 된다.
    즉 비어있는 variant는 불가능하다.

    variant는 optimal과 비슷하게 객체의 대입 시에 어떠한 동적할당도 발생하지 않는다.
    따라서 굉장히 작은 오버헤드로 객체들을 보관할 수 있다.
    다만 variant의 객체 자체의 크기는 나열된 타입들 중 가장 큰 타입의 크기를 따라간다.

    variant는 여러 타입들 중 하나(one-of)를 표현하기에 매우 적합한 도구이다.
    ex) 조건에 따라 A와 B 객체를 return 할 수도 있는 함수
        class A {};
        class B {};

        ??? GetDataFromDB(bool is_a) {
            if (is_a) {
                return A();
            }
            return B();
        }

        다형성(polymorphism)을 이용해 만들수도 있따.
        이를 위해서는 A와 B 클래스의 공통 부모가 정의되어 있어야 한다.
        class Data {};
        class A : public Data {};
        class B : public Data {};

        std::unique_ptr<Data> GetDataFromDB(bool is_a) {
            if (is_a) {
                return std::make_unique<A>();
            }
            return std::make_unique<B>();
        }
        하지만 문제는 부모 클래스가 공통으로 정의되어 있어야 한다는 점과
        std::string이나 int 같은 표준 클래스의 객체들에는 적용할 수 없다는 점이다.

    std::variant를 이용해 이런 문제를 해결하고 매우 간단하게 표현할 수 있다.
        
    variant 역시 optional과 마찬가지로 각각의 타입의 객체를 받는 생성자가
    정의되기 때문에 그냥 A를 return하면 A를 가지는 variant가 다른 타입을 return하면
    해당 타입을 가지는 variant를 생성한다.

    현재 variant에 몇 번째 타입이 들어있는지 알고 싶다면 index() 함수를 사용하면 된다.
    아래의 코드는 A를 return하면 index()는 0이 B는 1을 return한다.

    다음으로 실제로 원하는 값을 뽑아내고 싶다면 외부에 정의되어 있는 함수인 std::get<T>을 이용한다.
    T는 뽑고싶은 타입이나 해당 타입의 index를 전달해주면 된다.

    여기서 한 가지 알 수 있는 점은 variant가 보관하는 객체들은 타입으로 구분된다는 점이다.
    따라서 variant를 정의할 떄 같은 타입을 여러번 써주면 컴파일 오류가 발생한다.
    ex) std::variant<std::string, std::string> v; // 오류 발생

    std::monostate

    만약 굳이 variant에 아무 것도 들고 있지 않은 상태를 표현하고 싶다면 
    std::monostate를 타입으로 사용하면 된다.
    이를 통해 마치 std::optional과 같은 효과를 낼 수 있다.
    ex)
        std::variant<std::monostate, A, B> v; 와 같이 정의한다면
        v에는 아무것도 안들어가거나 A혹은 B가 들어갈 수 있다.
    
    또한 varaint안에 정의된 타입들 중 default 생성자가 있는 타입이 
    하나도 없는 경우도 std::monostate를 사용하면 된다.
    ex)
        class A {
        public:
            A(int i) {}
        };

        class B {
        public:
            B(int i) {}
        };
        default 생산자가 없는 클래스 A랑 B로 std::variant<A, B>를 하면 컴파일 오류를 일으킨다.
        variant는 반드시 객체를 가지고 있어야되는데 생성자가 없어서
        객체를 가지고 있을 수 없기에 오류가 난다.
        std::variant<std::monostate, A, B> v;하면 default로 std::monostate가 v에 들어가 괜찮아진다.


*/
#include <iostream>
#include <memory>
#include <variant>

class A
{
public:
    void a() { std::cout << "I am A" << std::endl; }
};

class B
{
public:
    void b() { std::cout << "I am B" << std::endl; }
};

// 각각의 타입의 객체를 받는 생성자가 정의되어 있어 return 되는 타입을 가지는 variant를 생성한다.
std::variant<A, B> GetDataFromDB(bool is_a)
{
    if (is_a)
    {
        return A();
    }
    return B();
}

int main()
{
    auto v = GetDataFromDB(true);

    // 현재 variant에 몇번째 타입이 들어있는지 확인하기
    std::cout << v.index() << std::endl;
    std::get<A>(v).a(); // 혹은 std::get<0>(v).a()
}