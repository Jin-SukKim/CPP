// 모든 종류의 예외 받기
/*
    만약에 어떤 예외를 throw 하였는데, 이를 받는 catch 가 없다면
    runtime_error 예외를 발생시키며 프로그램이 비정상적으로 종료되었다고 뜬다.

    따라서, 언제나 예외를 던지는 코드가 있다면 적절하게 받아내는 것이 중요하다.
    하지만 때로는 예외객체를 하나하나 처리할 필요없이 나머지 전부를 받아야 되는 경우도 있다.
    (switch의 default와 if-else의 else와 같이)

    try - catch에선 이것을 ... 을 이용해 처리할 수 있다.
    try {} catch(...) {} 을 하면 나머지 예외 전부를 처리할 수 있다.
*/
#include <iostream>
#include <stdexcept>

int func(int c)
{
    if (c == 1)
    {
        throw 1;
    }
    else if (c == 2)
    {
        throw "hi";
    }
    else if (c == 3)
    {
        throw std::runtime_error("error");
    }
    return 0;
}

int main()
{
    int c;
    std::cin >> c;

    try
    {
        func(c);
    }
    catch (int e)
    {
        std::cout << "Catch int : " << e << std::endl;
    }
    catch (...)
    {
        std::cout << "Default Catch!" << std::endl;
    }
}