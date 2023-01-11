// 보편적 레퍼런스 (Universal reference)

#include <iostream>

/*
    인자로 아예 rvalue reference를 받고있따.
    이런식으로 템플릿 인자 T를 rvalue reference로 받는 것을 universal reference라고 한다.
    이것은 rvalue만 받는 reference와는 다르다.

    #include <iostream>

    void show_value(int&& t) { std::cout << "우측값 : " << t << std::endl; }

    int main() {
    show_value(5);  // 우측값 ok!

    int x = 3;
    show_value(x);  // 애러
    }
    이것처럼 int&& t는 rvalue만을 인자로 받을 수 있다.

    하지만 Templet T의 rvalue reference는 lvalue도 인자로 받을 수 있다.
    lvalue가 오는 경우 레퍼런스 겹침 규칙 (reference collapsing rule) 에 따라 T 의 타입을 추론한다

        typedef int& T;
        T& r1;   // int& &; r1 은 int&
        T&& r2;  // int & &&;  r2 는 int&

        typedef int&& U;
        U& r3;   // int && &; r3 는 int&
        U&& r4;  // int && &&; r4 는 int&&

    즉 &는 1, &&은 0 이라 둔 뒤 OR 연산을 한다.
*/
template <typename T>
void wrapper(T &&u)
{
    /*
        T&& 란 rvalue reference를 호출하고 싶으나
        u는 lvalue인 const T& u가 호출된다. 결국 다시 rvalue로 바꿔줘야 한다.
        move를 사용하고 싶으나 move는 u가 rvalue reference일 때만 사용할 수 있다.
        forward는 move와 비슷하게 작동한다.

        forward함수는 다음과 같다.
        template <class S>
        S&& forward(typename std::remove_reference<S>::type& a) noexcept {
            return static_cast<S&&>(a);
        }

        S 가 A& 라면 (참고로 std::remove_reference 는 타입의 레퍼런스를 지워주는 템플릿 메타 함수이다)

        A&&& forward(typename std::remove_reference<A&>::type& a) noexcept {
           return static_cast<A&&&>(a);
        }
        가 되 reference 겹침 규칙에 따라 A& forward(A& a) noexcept { return static_cast<A&>(a); }가 되고
        S 가 A 라면, A&& forward(A& a) noexcept { return static_cast<A&&>(a); }가 되어 rvalue로 casting된다.

        따라서 결과적으로 위 그림 처럼 원본과 Wrapper 을 사용했을 때 모두 호출되는 함수가 동일함을 알 수있다.

        즉 move는 타입이 분명한 보통의 rvalue reference에서 사용하고
        forward는 universal_reference인 타입이 분명하지 않는 auto나 template t에서
        사용한다.
    */
    g(std::forward<T>(u));
}

class A
{
};

void g(A &a) { std::cout << "좌측값 레퍼런스 호출" << std::endl; }
void g(const A &a) { std::cout << "좌측값 상수 레퍼런스 호출" << std::endl; }
void g(A &&a) { std::cout << "우측값 레퍼런스 호출" << std::endl; }

int main()
{
    A a;
    const A ca;

    std::cout << "원본 --------" << std::endl;
    g(a);
    g(ca);
    g(A());

    std::cout << "Wrapper -----" << std::endl;
    wrapper(a);   // A& 로 추론
    wrapper(ca);  // const A&로 추론
    wrapper(A()); // A로 추론
}