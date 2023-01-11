// basic_string
/*
    std::string은 basic_string이라는 클래스 템플릿의 인스턴스화 버전이다.
    basic_string 정의 :
        template <class CharT, class Traits = std::char_traits<CharT>,
            class Allocator = std::allocator<CharT> >
        class basic_string;

        basic_string 클래스는 CharT 타입의 객체들을 메모리에 연속적으로 저장하고, 여러가지 문자열 연산들을 지원해주는 클래스이다.
        Traits는 CharT문자들의 대한 기본적인 문자열 연산들은 정의해놓은 클래스.
            ㄴ 기본적인 문자열 연산 : 주어진 문자열 대소비교, 길이 등
            ㄴ std::char_traits클래스의 인스턴스화 버전을 전달. (ex: char_traits<char> - string 클래스 버전)

        즉 basic_string에서 문자열 보관 로직을 담당하고 Traits에서 문자열 연산 로직을 담당한다.
        로직을 분리한 이유는 사용자에게 자율성을 더 보장하기 위해서 이다.

    CharT자리에 char이 온다면 std::string이 된다.

    총 5가지의 인스턴스화 문자열 클래스들이 존재한다.

    1)  std::string     - std::basic_string<char>

    2)  std::wstring    - std::basic_string<wchar_t> :
            wchar_t 의 크기는 시스템 마다 다름. 윈도우에서는 2 바이트이고, 유닉스 시스템에서는 4 바이트
            (wchar_t의 크기가 시스템마다 달라 2byte type과 4byte type인 char16_t와 char32_t가 만들어졌다.)

    3)  std::u8string   - std::basic_string<char8_t> :
            C++ 20 에 새로 추가되었음; char8_t 는 1 바이트; UTF-8 문자열을 보관할 수 있음

    4)  std::u16string  - std::basic_string<char16_t> :
            char16_t 는 2 바이트; UTF-16 문자열을 보관할 수 있음

    5)  std::u32string  - std::basic_string<char32_t> :
            char32_t 는 4 바이트; UTF-32 문자열을 보관할 수 있음
*/

// Traits 활용 
// 문자열 비교시 숫자들의 순위가 알파벳 보다 낮은 문자열
// 원래는 ASCII Table에서 숫자의 값이 알파벳보다 작아 더 앞에 온다. (1a < a1)
#include <cctype>
#include <iostream>
#include <string>

// char_traits 의 모든 함수들은 static 함수 입니다. (연산을 제공하는 클래스라 굳이 데이터의 저장이 필요없기 떄문이다.)
struct my_char_traits : public std::char_traits<char>
{
    static int get_real_rank(char c)
    {
        // 숫자면 순위를 엄청 떨어트린다.
        if (isdigit(c))
        {
            return c + 256;
        }
        return c;
    }

    // char_traits에서 대소 비교하는 부분을 바꾸고 싶으므로 
    // 문자간 크기 비교하는 lt와 길이 n의 문자열 크기 비교하는 compare를 새로 정의한다.
    static bool lt(char c1, char c2)
    {
        return get_real_rank(c1) < get_real_rank(c2);
    }

    static int compare(const char *s1, const char *s2, size_t n)
    {
        while (n-- != 0)
        {
            if (get_real_rank(*s1) < get_real_rank(*s2))
            {
                return -1;
            }
            if (get_real_rank(*s1) > get_real_rank(*s2))
            {
                return 1;
            }
            ++s1;
            ++s2;
        }
        return 0;
    }
};

int main()
{
    std::basic_string<char, my_char_traits> my_s1 = "1a";
    std::basic_string<char, my_char_traits> my_s2 = "a1";

    std::cout << "숫자의 우선순위가 더 낮은 문자열 : " << std::boolalpha
              << (my_s1 < my_s2) << std::endl;

    std::string s1 = "1a";
    std::string s2 = "a1";

    std::cout << "일반 문자열 : " << std::boolalpha << (s1 < s2) << std::endl;
}