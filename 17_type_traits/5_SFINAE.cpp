// SFINAE(Substitution Failure Is Not An Error) - 치환 오류는 컴파일 오류가 아니다.
/*
    type_traits에서 is_class 코드를 봤었을 떄
            template <class T>
            char test(int T::*);
    라는 코드가 존재했는데 T가 클래스가 아닌 경우 컴파일 오류를 일으켜야 되지만
    컴파일 오류가 발생하지 않고 오버로딩 후보군에서 제외되 다른 함수가 오버로딩된다고 했었다.

    템플릿 함수를 사용할 때 컴파일러는 템플릿 인자의 타입들을 유추한 후에 
    템플릿 인자들을 해당 타입을 치환하게 된다.

    여기서 문제는 템플릿 인자들을 유추한 타입으로 치환할 때 문법적으로 말이 안되는 경우가 있는데
    아래 코드에서 test<A>(33); 은 템플릿 인자로 A를 넘겼으므로 void test(typename T::y b)는 
    문법적으로 올바르지 않은 식이다. (클래스 A에는 y가 없다.)

    하지만 컴파일러는 컴파일 오류를 발생시키지 않는다.
    SFINAE(치환 오류는 컴파일 오류가 아니다.)라는 원칙에 따라 템플릿 인자 치환 후에
    만들어진 식이 문법적으로 맞지 않는다면, 컴파일 오류를 발생시키는 대신에 함수의 오버로딩 후보군에서 제외시킨다.

    한가지 중요한 점은 컴파일러가 템플릿 인자 치환시에 함수 내용 전체가 문법적으로 올바른지 확인하는게 아니다.
    컴파일러는 단순히 함수의 인자들과 return 타입만이 문법적으로 올바른지 확인한다.
    따라서 함수 내에 문법적으로 올바르지 않는 코드가 있어도 오버로딩 후보군에 남아있게 된다.

    결국 SFINAE를 이용해 원하지 않는 타입들에 대해 오버로딩 후보군에서 제외할 수 있는데
    type_traits에서 해당 작업을 쉽게 해주는 메타 함수를 제공한다.
    enable_if이다.
*/
#include <iostream>

template <typename T>
void test(typename T::x a)
{
    std::cout << "T::x \n";
}

template <typename T>
void test(typename T::y b)
{
    std::cout << "T::y \n";
}

struct A
{
    using x = int;
};

struct B
{
    using y = int;
};

int main()
{
    test<A>(33);

    test<B>(22);
} 