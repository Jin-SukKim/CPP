// Short String Optimization(SSO) - 짧은 문자열 최적화
/*
    메모리를 할당하는 작업은 시간을 많이 잡아먹는다.
    basic_string이 저장하는 문자열 길이는 천차만별이다.
    문제는 거대한 문자열은 드물게 저장되는데 짧은 문자열은 자주 저장된다는 것이다.
    길이가 짧은 문자열로 인해 메모리를 계속 할당한다면 비효율적이다.

    따라서 basic_string 의 제작자들은 짧은 길이 문자열의 경우 따로 문자 데이터를 위한 메모리를 할당하는 대신에 그냥 객체 자체에 저장하게 했다.
    이를 짧은 문자열 최적화(SSO - short string optimization) 이라고 불린다.
*/
#include <iostream>
#include <string>

// 이와 같이 new 를 전역 함수로 정의하면 모든 new 연산자를 오버로딩 해버린다.
// (어떤 클래스의 멤버 함수로 정의하면 해당 클래스의 new 만 오버로딩됨)
// 메모리 할당을 확인하기 위해 만들었다.
void *operator new(std::size_t count)
{
    std::cout << count << " bytes 할당 " << std::endl;
    return malloc(count);
}

int main()
{
    std::cout << "s1 생성 --- " << std::endl;
    std::string s1 = "this is a pretty long sentence!!!";
    std::cout << "s1 크기 : " << sizeof(s1) << std::endl;   // 메모리할당

    std::cout << "s2 생성 --- " << std::endl;
    std::string s2 = "short sentence";
    std::cout << "s2 크기 : " << sizeof(s2) << std::endl;   // 메모리 할당 X(메모리 사용량을 조금 희생하고 성능 향상)
}