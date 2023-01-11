// 예외를 발생시키지 않는 함수 - noexcept
/*
    어떤 함수가 예외를 발생시키지 않는다면 noexcept를 통해 명시할 수 있다.

    하지만 noexcept를 사용했다고 해서 절대로 예외를 thrwo하지 않는다는 것은 아니다.
    컴파일러는 noexcept가 써져있는 함수가 예외를 발생시키지 않는다는 전제하에 컴파일하게 되지만
    함수가 예외를 throw한다면 예외가 제대로 처리되지 않고 프로그램이 종료된다.

    noexcept는 최적화를 위해 주로 사용된다.
*/

#include <iostream>

// 예외를 발생시키지 않으므로 noexcept를 해 명시
int foo() noexcept {}

// 예외가 제대로 처리되지 않고 프로그램이 종료된다.
int bar() noexcept { throw 1; }

int main()
{
    foo();
    try
    {
        bar();
    }
    catch (int x)
    {
        std::cout << "Error : " << x << std::endl;
    }
}