// std::declval
/*
    decltype처럼 C++ 11에서 추가된 std::declval 키워드는 <utility>에 정의된 함수이다.

    ex) 어떤 타입 T의 f라는 함수의 return type을 정의하고 싶을 떄 decltype을 사용하기
        struct A {
            int f() { return 0; }
        };

        decltype(A().f()) ret_val;  // int ret_val; 이 된다.

        실제로 객체 A가 생성되거나 함수 f가 실행되지는 않는다.
        decltype안에 들어가면 그저 식의 형태로 존재할 뿐 컴파일 시에 decltype() 전체 식이
        타입을 변환하기 떄문에 decltype 안에 있는 식은 런타임 시에 실행된는게 아니다.

    그렇다고 틀린 식을 전달하면 안되고 문법은 모두 지켜야된다.
    decltype에 생성자를 이용해 전달하는 경우 default 생성자가 아니면 문법을 맞춰줘야 된다.
    ex)
        template <typename T>
        decltype(T().f()) call_f_and_return(T& t) {
            return t.f();
        }
        struct A {
            int f() { return 0; }
        };
        struct B {
            B(int x) {}
            int f() { return 0; }
        };

        int main() {
            A a;
            B b(1);

            call_f_and_return(a);  // ok
            call_f_and_return(b);  // BAD
        }

        즉 모든 타입이 default 생성자를 사용하고 있지 않을 수도 있어 T()란 생성자를 전달 받지 못해 오류를 일으킬 수도 있다.

    따라서 직접 생성자를 사용하면 전달되는 타입들의 생성자가 모두 같은 꼴이 아니면 문제가 발생하는데
    std::declval을 이용해 해결할 수 있다.

    std::declval<T>() 는 생성자가 존재하지 않아도 마치 T()를 한 것과 같은 효과를 낸다.
    따라서 생성자가 달라서 발생하는 오류를 막아준다.

    declval 함수는 타입 연산에만 사용해야지 런타임에 사용하면 오류를 발생시킨다.
    ex) 오류 발생
        #include <utility>

        struct B {
            B(int x) {}
            int f() { return 0; }
        };

        int main() { B b = std::declval<B>(); }

    C++ 14부터는 함수의 return 타입을 컴파일러가 알아서 추론해주는 auto가 생겨서
    함수의 return 타입을 그냥 auto로 지정하면 된다.

    하지만 <type_traits> 라이브러리를 사용하면 decltype과 declval을 사용한
    템플릿 메타프로그래밍(TMP) 기법들이 있다.
*/

#include <utility>

// declval을 사용하면 T의 생성자를 직접 호출하지 않더라도 T가 생성된 객체를 나타낼 수 있다.
template <typename T>
decltype(std::declval<T>().f()) call_f_and_return(T &t)
{
    return t.f();
}

// C++ 14부터 추가된 컴파일러가 자동으로 타입을 추론하는 auto로 똑같은 작어블 해주는 함수
template <typename T>
auto call_f_and_return2(T &t)
{
    return t.f();
}
struct A
{
    int f() { return 0; }
};

struct B
{
    B(int x) {}
    int f() { return 0; }
};

int main()
{
    A a;
    B b(1);

    call_f_and_return(a); // ok
    call_f_and_return(b); // ok

    call_f_and_return2(a); // ok
    call_f_and_return2(b); // ok
}