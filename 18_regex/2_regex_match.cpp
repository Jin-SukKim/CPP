// 전체 문자열 매칭하기
/*
    std::regex_match를 사용하면 문자열 전체가 정규 표현식의 패턴에 부합해야 된다.
    사용법 :
        ex)
        std::regex_match(file_name, re)

        첫 번째 인자로 문자열을 전달했고 두 번째 인자로 정규 표현식을 전달했다.
        전달된 정규 표현식과 완전히 매칭된다면 true를 return한다.
        완전히 매칭된다는 것은 문자열 전체가 정규 표현식 패턴에 부합해야 된다는 것이다.

    하지만 문자열에서 패턴 일부분을 뽑아낼 수도 있다.

    정규 표현식에는 캡쳐 그룹(capture group)이란것이 존재한다.
    캡쳐 그룹을 사용하면 패턴 일부분을 얻을 수 있게 된다.
    ex)
        std::regex re("[01]{3}-(\\d{3,4})-\\d{4}");

    () 로 원하는 부분을 감싸게 된다면 해당 부분에 매칭된 문자열을 얻을 수 있게 된다.

    이렇게 매칭된 문자열은 std::smatch 객체에 저장할 수 있다.
    
    std::smatch match;

    smatch는 매칭된 문자열을 std::string의 형태로 돌려준다.
    이외에 const char* 로 돌려주는 cmatch도 존재한다.

    사용법 :
        std::regex_match(문자열, std::smatch, std::regex)

    매칭된 문자열은 match[i].str()을 통해 접근할 수 있다.
    ()로 여러 패턴을 묶어준 후 여러 패턴이 매칭됬다면 for문 등을 이용해 차레로 접근할 수 있다.

*/

/*
    전화번호 받는 프로그램
        1. (숫자)-(숫자)-(숫자) 꼴로 있어야 합니다.
        2. 맨 앞자리는 반드시 3 자리이며 0 과 1 로만 이루어져 있어야 합니다.
        3. 가운데 자리는 3 자리 혹은 4 자리 여야 합니다.
        4. 마지막 자리는 4 자리 여야 합니다.

        정규 표현식은 [01]{3}-\d{3,4}-\d{4} 가 될 것이다.

        [01]은 0 혹은 1이라는 의미이다.
        {3}은 해당 종류의 문자가 3번 나타날 수 있다는 의미이다.
        \d는 숫자 하나라는 의미이다.
*/
#include <iostream>
#include <regex>
#include <vector>

int main()
{
    std::vector<std::string> phone_numbers = {"010-1234-5678", "010-123-4567",
                                              "011-1234-5567", "010-12345-6789",
                                              "123-4567-8901", "010-1234-567"};

    // () 로 중간 패턴을 감싸서 전화번호의 중간부분을 얻을 수 있다.
    std::regex re("[01]{3}-(\\d{3,4})-\\d{4}");

    // 매칭된 결과를 string 으로 보관
    std::smatch match; 
    for (const auto &number : phone_numbers)
    {
        // 매칭된 결과를 보관할 match와 정규 표현식 re를 전달한다.
        if (std::regex_match(number, match, re))
        {
            for (size_t i = 0; i < match.size(); i++)
            {
                std::cout << "Match : " << match[i].str() << std::endl;
            }
            std::cout << "-----------------------\n";
        }
    }
}