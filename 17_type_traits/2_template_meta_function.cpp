// Template Meta Function
/*
    템플릿 메타 함수는 사실 함수가 아니라 마치 함수처럼 동작하는 템플릿 클래스들을 이야기한다.
    이들이 메타인 이유는 보통의 함수들은 값에 대해 연산을 수행하지만, 메타 함수들은 타입에 대해
    연산을 수행한다는 점이 다르다.

    보통 함수
    ex) 어떤 수가 음수인지 판별하는 함수 is_negative가 있다고 한다면
        if (is_negative(x)) {}와 같이 사용할 것이다.

    템플릿 메타 함수
    ex) 어떤 타입이 void인지 아닌지 판단하는 함수 is_void란 템플릿 메타 함수가 있다면
        if (is_void<T>::value) {} 와 같이 사용한다. (어떤 타입 T는 과연?)

    템플릿 메타 함수는 실제로는 함수가 아니다.
    함수라면 ()을 통해서 호출하지만 함수릐 인자가 아닌 <>을 이용해 템플릿 인자를 전달하고 있다.
*/
#include <iostream>
#include <type_traits>

template <typename T>
void tell_type()
{
    if (std::is_void<T>::value)
    {
        std::cout << "T 는 void ! \n";
    }
    else
    {
        std::cout << "T 는 void 가 아니다. \n";
    }
}

int main()
{
    tell_type<int>(); // void 아님!

    tell_type<void>(); // void!
}