// 문자열 리터럴 정의 - 리터럴 연산자(literal operator)
/*
    C의 문자열 정의
        1. const char* s = "hello";
        2. char s[] = "hello"

    C++ 문자열 정의
        1. const char* s = "hello";
        2. string str = "hello";

    C++ 14부터는 Literal operator가 추가되었다
    literal operator - std::string operator"" s(const char *str, std::size_t len);

        auto str = "hello"s;

    이것과 같이 ""뒤에 s를 붙여주면 auto가 string으로 추론된다.
    즉 "hello"s는 operator""s("hello",5);가 된다.

    리터럴 연산자를 사용하기 위해서는 std::string_literals 네임 스페이스를 사용해야된다
    문자열 말고 정수나 부동 소수점도 사용가능하다.

    "" 말고 다른 방법으로 literal operator 사용하기.
    std::string str = "hello";     // char[]
    std::wstring wstr = L"hello";  // wchar_t[] 등등
*/

#include <iostream>
#include <string>
using namespace std::literals;

int main()
{
    auto s1 = "hello"s;
    std::cout << "s1 길이 : " << s1.size() << std::endl;

    // Raw string literal이란 것도 있다.
    std::string str = R"(asdfasdf
                        이 안에는
                        어떤 것들이 와도
                        // 이런것도 되고
                        #define hasldfjalskdfj
                        \n\n <--- Escape 안해도 됨
                        )";

    std::cout << str;

    // Raw string literal에서는 );을 사용할 수 없는데 구분 문자를 사용하면 된다.
    // Raw string literal 문법 - R"/* delimiter */( /* 문자열 */ )/* delimiter */"      delimiter는 아무것도 없어도 되고 아무거나 와도된다.
    std::string str = R"foo(
                        )"; <-- 무시됨
                        )foo";

    std::cout << str;
}