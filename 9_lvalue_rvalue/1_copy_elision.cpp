// 복사 생략(Copy Elision)
/*
    컴파일러가 복사를 생략하는 경우는 (함수의 인자가 아닌)함수의 내부에서 생성된 객체를 그래도 return할 떄 일어날수도 있다.
    
*/
#include <iostream>

class A
{
    int data_;

public:
    A(int data) : data_(data) { std::cout << "일반 생성자 호출!" << std::endl; }

    A(const A &a) : data_(a.data_)
    {
        std::cout << "복사 생성자 호출!" << std::endl;
    }
};

int main()
{
    A a(1); // 일반 생성자 호출
    A b(a); // 복사 생성자 호출

    // 그렇다면 이것은?
    /*
        복사생성자가 호출되지 않는데 A(2)를 만들면 굳이 A c를 또 만들 필요가 없기때문에 A c를 A(2)로 만든다.

        이런식으로 컴파일러 자체에서 복사를 생략하는 걸 Copy elisiond이라고 한다.   
    */
    A c(A(2));
}