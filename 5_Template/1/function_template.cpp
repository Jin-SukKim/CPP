#include <iostream>
#include <string>

// 인스턴스화 되기 전 까지는 컴파일 시에 아무런 코드로 변환되지 않는다.
template <typename T>
T max(T &a, T &b)
{
    return a > b ? a : b;
}

int main()
{
    int a = 1, b = 2;
    // class와 다르게 max<type>()이라고 안적어도 되는데 컴파일러에서 자동으로 변환 후 작업하기 떄문이다.
    std::cout << "Max (" << a << "," << b << ") ? : " << max(a, b) << std::endl; // max(a,b) 호출시 함수가 인스턴스화 된다.

    std::string s = "hello", t = "world";
    std::cout << "Max (" << s << "," << t << ") ? : " << max(s, t) << std::endl;
}