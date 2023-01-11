// 스택 풀기 (stack unwinding)
/*
    예외가 발생되면 catch로 점프하는데 이 과정에서 stack에 정의된 객체들의 소멸자를 호출해
    소멸시키는 과정을 stack unwinding이라고 한다.

    주의할 점이 있는데 만약 생성자에서 예외가 발생할 경우 소멸자가 호출되지 않는다.
    따라서, 만일 예외를 throw하기 이전에 생성된 자원이 있다면 catch에서 잘 해제시켜줘야 한다.
*/
#include <iostream>
#include <stdexcept>

class Resource
{
public:
    Resource(int id) : id_(id) {}
    ~Resource() { std::cout << "리소스 해제 : " << id_ << std::endl; }

private:
    int id_;
};

// 예외 발생. func3은 func2가 호출하고 func2는 func1이 호출하고 마지막으로 func1은 main이 호출한다.
// 예외는 가장 가까운 catch문에서 받는다. 하지만 func2와 func1은 catch문을 가지고 있지 않으므로
// main의 catch문이 실행된다. 그리고 예외가 전파되며 각 함수들의 객체들이 잘 소멸되었다.
int func3()
{
    Resource r(3);
    throw std::runtime_error("Exception from 3!\n");
}
int func2()
{
    Resource r(2);
    func3();
    std::cout << "실행 안됨!" << std::endl;
    return 0;
}
int func1()
{
    Resource r(1);
    func2();
    std::cout << "실행 안됨!" << std::endl;
    return 0;
}

// 예외가 발생하지 않을 경우
int func6()
{
    Resource r(3);
    return 0;
}
int func5()
{
    Resource r(2);
    func6();
    std::cout << "실행!" << std::endl;
    return 0;
}
int func4()
{
    Resource r(1);
    func5();
    std::cout << "실행!" << std::endl;
    return 0;
}

int main()
{
    try
    {
        func1();
    }
    catch (std::exception &e)
    {
        std::cout << "Exception : " << e.what();
    }

    try
    {
        func4();
    }
    catch (std::exception &e)
    {
        std::cout << "Exception : " << e.what();
    }
}