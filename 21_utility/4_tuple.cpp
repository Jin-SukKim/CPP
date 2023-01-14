// std::tuple (C++ 11 이상 - <tuple>)
/*
    std::tuple은 서로 다른 타입들의 묶음을 간단하게 다룰수 있도록 제공한다.
    C++에서 같은 타입 객체들을 다루기 위해 vector나 배열 등을 사용했다.

    하지만 다른 타입의 객체들을 여러 개 다루는 방법은 너무 복잡하다.
    보통은 구조체를 이용하는 경우가 많다.
        struct Collection {
            int a;
            std::string s;
            double d;
        };
    하지만 매번 struct를 생성하는 것은 코드를 매우 복잡하게 만든다.

    C++ 11부터는 std::tuple 라이브러리가 추가되 python과 같이
    서로 다른 타입들의 집학을 생성할 수 있게 되었다.

    std::tuple 정의하기 :
        std::tuple<type, type, etc..> tp;
        와 같이 원하는 타입들을 템플릿 인자로 넘기면 된다.

    tuple 객체 생성하기 :
        tp = std::make_tuple(values);
        values에는 tuple에 정의했던 타입들의 객체를 넘기면 된다.
    
    tuple 원소에 접근 :
        std::get<num>(tp) 
        variant와 같이 std::get을 이용해 접근하면 된다.
        보통 index 넘버로 접근하고 타입을 인자로 넘겨줘서 접근해도 되지만
        같은 타입의 객체가 2개 이상 존재한다면 오류를 일으킨다.
*/

#include <iostream>
#include <string>
#include <tuple>

int main()
{   
    // int, double, std::string 타입의 객체들을 보관하는 컨테이너
    std::tuple<int, double, std::string> tp;
    tp = std::make_tuple(1, 3.14, "hi");

    std::cout << std::get<0>(tp) << ", " << std::get<1>(tp) << ", "
              << std::get<2>(tp) << std::endl;
}