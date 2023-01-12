/*
    이전의 enable_if에선 조건을 만족하는 타입을 인자로 받는 함수를 만들고 싶을 때 사용했다.

    만약 원하는 조건의 멤버 함수가 있는 타입을 인자로 받는 함수를 만들고 싶을 경우는 어떻게 해야될까?
    decltype과 declval을 사용하면 만들 수 있다.
    원하는 조건을 만족하는 인자가 들어가면 정상적으로 작동하지만 조건을 만족하지 못하면 컴파일 에러가 난다.

    만약 멤버 함수의 타입뿐만이 아닌 retunr값의 타입마저 강제하고 싶다면 enable_if도 같이 사용하면 된다.
*/
#include <iostream>
#include <type_traits>

// 멤버 함수로 func()을 가진 클래스만 받고 싶을 경우
template <typename T, typename = decltype(std::declval<T>().func())>
void test(const T &t)
{
    std::cout << "t.func() : " << t.func() << std::endl;
}

// T 는 반드시 정수 타입을 리턴하는 멤버 함수 func 을 가지고 있어야 한다.
template <typename T, typename = std::enable_if_t<
                        std::is_integral_v<decltype(std::declval<T>().func())>>>
void test2(const T& t) {
  std::cout << "t.func() : " << t.func() << std::endl;
}

struct A
{
    int func() const { return 1; }
};

// func() 멤버 함수가 없어서 컴파일 오류
struct B
{
};

struct C {
  char func() const { return 'a'; }
};

// func() 멤버 함수가 있지만 retunr값의 타입이 정수가 아니라서 컴파일 오류
struct D {
  A func() const { return A{}; }
};

int main()
{
    test(A{});
    // test(B{}); // 컴파일 오류 발생
    test2(C{});
    // test2(D{}); // 컴파일 오류 발생
}