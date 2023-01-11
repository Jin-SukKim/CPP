/*

*/
#include <iostream>
#include <vector>

template <typename T>
void wrapper(T &u)
{
    std::cout << "T& 로 추론됨" << std::endl;
    g(u);
}

// void wrapper(T &u)는 rvalue를 받을 수 없어서 const T &u를 따로 만들어주었다
template <typename T>
void wrapper(const T &u)
{
    std::cout << "const T& 로 추론됨" << std::endl;
    g(u);
}

// 문제는 인자를 하나가 아닌 여러개를 받을 경우 많은 특수화 템플릿을 만들어야 한다는것이다.
// 이것을 Universal reference를 통해 해결할 수 있다.
template <typename T>
void wrapper(T &u, T &v)
{
    g(u, v);
}
template <typename T>
void wrapper(const T &u, T &v)
{
    g(u, v);
}

template <typename T>
void wrapper(T &u, const T &v)
{
    g(u, v);
}
template <typename T>
void wrapper(const T &u, const T &v)
{
    g(u, v);
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
    wrapper(a);
    wrapper(ca);
    wrapper(A());
}