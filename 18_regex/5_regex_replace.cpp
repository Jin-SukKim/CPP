// 원하는 패턴 치환하기
/*
    std::regex_replace를 이용해 원하는 패턴의 문자열을 다른 문자열로 치환(replace)할 수 있다.

    replace 해줄 패턴은 캡쳐 그룹을 이용해 표현해준다. () 를 이용한다.
    ex)
        sk-circle(\d)

    replace 할 때 첫번째 캡쳐 그룹을 표현하기 위해 $1 라고 명시한다.
    (이와 같은 요소를 back reference라고 한다.)
    ex)
        $1-sk-circle

    만약 캡쳐 그룹이 중첩 됬다면 괄호가 열리는 순서대로 $1, $2 .. 등으로 표현해주면 된다.
    ex)
        (sk-circle(\d) sk-circle) // 전체가 $1, \d 부분이 $2이다

    regex_replace 사용법 :
        std::regex re(R"r(sk-circle(\d))r");
        std::smatch match;
        std::regex_replace(html, re, "$1-sk-circle");

        첫번쨰 인자 - 문자열, 두번쨰 인자 - 정규표현식, 세번째 인자 - replace할 패턴

    regex_replace의 오버로딩 형태는 여러가지 존재한다.
    위와 같은 형태는 replace한 문자열을 돌려준다.

    만약 replace된 문자열을 생성하지 않고 stdoud에 출력하고 싶다면

    std::regex_replace(std::ostreambuf_iterator<char>(std::cout), html.begin(),
                        html.end(), re, "$1-sk-circle");
    와 같이 쓰면 된다.

    이 경우 첫번째 인자로 출력할 위치의 시작점을 가리키는 반복자를 넣어준다.
*/

/*
    sk-circle1 과 같은 문자열을 1-sk-circle 로 바꿔보는 정규 표현식 프로그램

    sk-circle\d 패턴을 숫자-sk-circle 패턴으로 바꿔보는 프로그램이다.

    캡쳐 그룹을 이용해 replace할 부분을 표시해준다.

    sk-circle(\d)

    replace하기 위해 $1을 사용해 패턴을 표시해준다

    $1-sk-circle
*/
#include <iostream>
#include <regex>

int main()
{
    std::string html = R"(
        <div class="social-login">
          <div class="small-comment">다음으로 로그인 </div>
          <div>
            <i class="xi-facebook-official fb-login"></i>
            <i class="xi-google-plus goog-login"></i>
          </div>
        </div>
        <div class="manual">
          <div class="small-comment">
            또는 직접 입력하세요 (댓글 수정시 비밀번호가 필요합니다)
          </div>
          <input name="name" id="name" placeholder="이름">
          <input name="password" id="password" type="password" placeholder="비밀번호">
        </div>
        <div id="adding-comment" class="sk-fading-circle">
          <div class="sk-circle1 sk-circle">a</div>
          <div class="sk-circle2 sk-circle">b</div>
          <div class="sk-circle3 sk-circle">asd</div>
          <div class="sk-circle4 sk-circle">asdfasf</div>
          <div class="sk-circle5 sk-circle">123</div>
          <div class="sk-circle6 sk-circle">aax</div>
          <div class="sk-circle7 sk-circle">sxz</div>
        </div>
  )";

    std::regex re(R"r(sk-circle(\d))r");
    std::smatch match;

    std::string modified_html = std::regex_replace(html, re, "$1-sk-circle");
    std::cout << modified_html;

    // 중첩된 캡쳐 그룹
    std::regex re2(R"r((sk-circle(\d) sk-circle))r");
    std::smatch match2;

    std::string modified_html2 = std::regex_replace(html, re2, "$2-sk-circle");
    std::cout << modified_html2;
}