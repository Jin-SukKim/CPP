// std::regex_iterator
/*
    regex_iterator를 사용하면 좀 더 편리하게 검색을 수행할 수 있다.
    이 것은 주어진 문자열에서 정규 표현식으로 매칭된 문자열들을 연속적으로 뽑아내는 반복자이다.

    사옹볍 :
        auto start = std::sregex_iterator(html.begin(), html.end(), re);

        위 코드와 같이 생성자를 호출함으로써 생성할 수 있다.
        첫 번째와 두번째 인자로 검색할 문자열의 시작과 끝을 전달하고,
        세번쨰 인자로 정규 표현식을 전달한다.

        sregex_iterator()는 string을 사용하는 반복자이다.
    
    regex_iterator의 경우 처음 생성될 때와, ++ 연산자로 증감 될 때마다, 
    regex_search를 통해 초기에 주어진 문자열 범위 내에서 패턴에 맞는 문자열을 검색한다.
    또한 * 연산자를 통해 역참조 하게 된다면 현재 검색된 패턴에 대한 match_results 객체를 얻어낼 수 있다.

    매칭된 문자열 뽑아내기
    
        while (start != end) {
            std::cout << start->str() << std::endl;
            ++start;
        }
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

    std::regex re(R"(<div class="sk[\w -]*">\w*</div>)");
    std::smatch match;

    auto start = std::sregex_iterator(html.begin(), html.end(), re);
    auto end = std::sregex_iterator();

    while (start != end)
    {
        std::cout << start->str() << std::endl;
        ++start;
    }
}