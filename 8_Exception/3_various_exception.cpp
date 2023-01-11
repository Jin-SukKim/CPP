// 다양한 종류의 예외처리
/*
    catch는 다양한 종류의 예외를 받을 수 있다.
    이를 위해 하나의 try안에 받고자 하는 모든 종류의 예외를 catch로 받는다.
*/
#include <iostream>
#include <string>
#include <exception>

// 여러종류의 catch 한번에 쓰기
int func(int c)
{
    if (c == 1)
    {
        throw 10;
    }
    else if (c == 2)
    {
        throw std::string("hi!");
    }
    else if (c == 3)
    {
        throw 'a';
    }
    else if (c == 4)
    {
        throw "hello!";
    }
    return 0;
}

// 기반 클래스와 파생 클래스의 경우 처리하는 방식
class Parent : public std::exception
{
public:
    // what 은 std::exception 에 정의된 함수로, 이 예외가 무엇인지 설명하는 문자열을
    // 리턴하는 함수 입니다.
    virtual const char *what() const noexcept override { return "Parent!\n"; }
};

class Child : public Parent
{
public:
    const char *what() const noexcept override { return "Child!\n"; }
};

int func(int c)
{
    if (c == 1)
    {
        throw Parent();
    }
    else if (c == 2)
    {
        throw Child();
    }
    return 0;
}

int main()
{
    int c;
    std::cin >> c;

    // switch 처럼 catch도 다양한 종류의 throw 객체를 모두 받을 수 있따.
    try
    {
        func(c);
    }
    catch (char x)
    {
        std::cout << "Char : " << x << std::endl;
    }
    catch (int x)
    {
        std::cout << "Int : " << x << std::endl;
    }
    catch (std::string &s)
    {
        std::cout << "String : " << s << std::endl;
    }
    catch (const char *s)
    {
        std::cout << "String Literal : " << s << std::endl;
    }

    int c;
    std::cin >> c;

    // 기반 클래스와 파생 클래스의 경우 처리
    try
    {
        func(c);
    }
    // Parent와 Child 두개의 throw 모두 Parent의 catch가 실행된다
    catch (Parent &p)
    {
        std::cout << "Parent Catch!" << std::endl;
        std::cout << p.what();
    }
    // Child의 catch는 실행되지 않는데 catch문은 가장 먼저 대입가능한 객체를 받는다
    // Child는 Parent& p = Child()가 가능한 파생 클래스라 Parent catch가 먼저 받아서 실행되지 않는 것이다.
    catch (Child &c)
    {
        std::cout << "Child Catch!" << std::endl;
        std::cout << c.what();
    }

    // 파생클래스와 부모 클래스의 catch문 순서를 바꿔쓰면 Child catch로 사용가능하다.
    try
    {
        func(c);
    }
    catch (Child &c)
    {
        std::cout << "Child Catch!" << std::endl;
        std::cout << c.what();
    }
    catch (Parent &p)
    {
        std::cout << "Parent Catch!" << std::endl;
        std::cout << p.what();
    }
}