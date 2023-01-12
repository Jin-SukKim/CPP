// 패턴을 만족하는 문자열 일부를 검색하기
/*
    regex_match이 통해 문자열 전체가 패턴에 부합하는지 확인하는 작업이라면
    regex_search를 이용해 문자열의 일부가 패턴에 부합하는지 검색할 수 있다.

    사용법 :
        regex_search(문자열, std::smatch, std::regex)
        
        첫번째 인자에 문자열, 두번째 인자에 일치된 패턴을 보관할 match 객체, 세번쨰 인자에 정규 표현식

        매칭되는 패턴이 존재한다면 true를 return한다.

    매칭된 객체는 match.str()와 같이 접근할 수 있다.

    문제는 그냥 다시 regex_search()를 다시 실행한다면 그냥 이전에 찾았던 패턴을 다시 매칭할 것이다.
    검색된 패턴의 바로 뒤부터 다시 검색해야 다음에 또 다른 패턴이 있는지 찾을 수 있다.
    
    match.suffix();는 std::sub_match() 객체를 return한다.
    sub_match()는 단순히 어떠한 문자열의 시작과 끝을 가리키는 반복자 두 개를 가지고 있다.

    이때 suffic 함수의 경우, 원래 문자열에서 검색된 패턴 바로 뒤부터, 이전 문자열 끝까지 해당하는
    sub_match() 객체를 return 한다.

    정규 표현식 패턴으로 검색해 match에 들어간 패턴을 match.str() 부분이라면 이 이전의 문자열 부분은
    match.prefix()부분이고 match.str() 부분 뒤부터는 match.suffix() 부분이다.

    sub_match 클래스에는 string으로 변환할 수 있는 캐스팅 연산자가 들어가 있다.
    따라서 문자열에 대입하게 되면 알아서 문자열로 변환되어 들어가 이미 찾은 부분의 뒤부터 바로 시작할 수 있다.
*/

/*
    만약 html 문서에서 <div></div> 패턴을 검색하고 싶다면
    <div class="sk[\w -]*">\w*</div> 정규표현식을 사용하면 된다.
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
    while (std::regex_search(html, match, re))
    {
        std::cout << match.str() << '\n';
        html = match.suffix();
    }
}