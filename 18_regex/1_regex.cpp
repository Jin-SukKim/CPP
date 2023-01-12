// regex(regural expression) : 정규 표현식
/*
    정규 표현식은 문자열에서 패턴을 찾는데 사용한다.

    1. 주어진 문자열이 주어진 규칙에 맞는지 확인할 때(std::regex_match)

    2. 주어진 문자열에서 원하는 패턴의 문자열을 검색할 때(std::regex_search)

    3. 주어진 문자열에서 원하는 패턴의 문자열로 치환할 때(std::regex_replace)

    와 같은 상황에서 매우 유용하게 사용된다.

    정규 표현식은 패턴을 찾기 위해 사용하는 언어가 있다.
    ex) db-\d*-log\.txt // 정규 표현식

        /d* 는 임의의 개수의 숫자를 의미한다.
        . 앞에 \ 를 붙여주는 이유는 . 을 그냥 사용한다면 임의의 문자로 해석되기 때문이다.

    C++에서는 std::regex 를 이용해 정규 표현식을 사용할 수 있다.
    std::regex를 이용해 객체를 생성해 사용한다.
    ex)
        std::regex re("db-\\d*-log\\.txt");

    정규 표현식 문법의 종류와 정규 표현식을 처리하는 엔진 역시 여러가지 종류가 있는데,
    std::regex를 이용해 생성자에 추가적인 인자로 전달할 수 있다.

    ex) grep의 정규 표현식 문법을 사용하고 싶다면 추가해준다.
        std::regex re("db-\\d*-log\\.txt", std::regex::grep);
    
    인자를 전달하지 않았다면 default로 std::regex::ECMAScript 가 들어간다.

    어떤 문법을 사용할 지 이외에도 몇 가지 특성들을 추가할 수 있다.

    대소문자를 구분하지 않고 싶다면 std::regex::icase를 추가해주면 된다.
    특성을 더 추가하는 방법은 '|' 을 사용하면 된다.
    ex)
        std::regex re("db-\\d*-log\\.txt", std::regex::grep | std::regex::icase);

    정규 표현식의 성능이 중요할 경우 std::regex::optimize를 추가적으로 전달할 수 있다.
    이것은 정규 표현식 객체를 생성하는데 조금 더 시간이 걸리지만 정규 표현식 객체를 
    사용하는 작업은 좀 더 빠르게 수행된다.

    정규 표현식 객체를 std::regex_match를 이용해 사용했다.
    ex)
        std::regex_match(file_name, re)
        
        첫 번째 인자로 문자열을 전달했고 두 번째 인자로 정규 표현식을 전달했다.
        전달된 정규 표현식과 완전히 매칭된다면 true를 return한다.
        완전히 매칭된다는 것은 문자열 전체가 정규 표현식 패턴에 부합해야 된다는 것이다.



*/

/*
    만약 어떤 서버에서 매 시간 로그 파일이 db-(시간)-log.txt 형태로 생성된다고 했을 때
    원하는 로그 파일들만 읽을 수 있을 까?
*/
#include <iostream>
#include <regex>
#include <vector>

int main()
{
    // 주어진 파일 이름들.
    std::vector<std::string> file_names = {"db-123-log.txt", "db-124-log.txt",
                                           "not-db-log.txt", "db-12-log.txt",
                                           "db-12-log.jpg"};
    std::regex re("db-\\d*-log\\.txt");
    for (const auto &file_name : file_names)
    {
        // std::boolalpha 는 bool 을 0 과 1 대신에 false, true 로 표현하게 해줍니다.
        std::cout << file_name << ": " << std::boolalpha
                  << std::regex_match(file_name, re) << '\n';
    }
}