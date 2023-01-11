// string_view : 문자열을 소유하지 않고 읽기만 하는 클래스
/*
    보통 문자열을 읽기만 한다면 const char* s나 std::string& str로 받으면 된다.
    하지만 각각 문제를 가지고 있다.

    하지만
*/
#include <iostream>
#include <string>

void *operator new(std::size_t count)
{
    std::cout << count << " bytes 할당 " << std::endl;
    return malloc(count);
}

// 문자열에 "very" 라는 단어가 있으면 true 를 리턴함
/*
    이 함수는 인자를 읽기 만 하므로, const string& 의 형태로 받으면된다.

    하지만 이 함수에 문자열 literal을 전달한다면 (이는 const char*), 인자는 string만 받을 수 있기 때문에
    암묵적으로 string 객체를 생성해 불필요한 메모리 할당이 발생한다.

    만약 이 함수를 const char* 형태의 인자를 받도록 바꾸면 두 가지 문제가 발생한다.
    string을 함수에 직접 전달할 수 없고 c_str함수를 통해 string에서 const char* 주소값을 뽑아내야 한다.
    이 과정에서 문자열의 길이에 대한 정보를 읽어버리게 된다.
    만약 함수 내부에 문자열 길이 정보가 필요하다면 매번 다시 계산해야된다.

    이러한 연유로, contains_very 함수를 합리적으로 만들기 위해서는 const string& 을 인자로 받는 오버로딩 하나,
    그리고 const char* 을 인자로 받는 오버로딩 하나를 각각 준비해야 된다는 문제점이 있다.

    이 문제를 C++ 17에서 string_view를 통해 해결했다.
*/
bool contains_very(const std::string &str)
{
    return str.find("very") != std::string::npos;
}

// 문자열을 소유하지 않고 읽기만 한다.
// 문자열에 "very" 라는 단어가 있으면 true 를 리턴함
/*
    소유하지 않고 읽기만 한다. 즉 어딘가 존재하는 문자열을 참고해서 읽기만 하는 것이다.
    그러므로 string_view가 현재 보고있는 문자열이 소멸된다면 undefined behavior 오류가 발생한다.

    문자열을 소유하지 않고 읽기만 하기 때문에 string_view 객체 생성시 메모리 할당이 일어나지 않고
    문자열 시작 주소값만 복사해온다.
    따라서 const char*을 전달해도 메모리 할당을 하지 않고 문자열 길이가 그대로 전달되 따로 계산할 필요도 없다. (효율적)

    string_view의 연산들은 원본 문자열을 수정하지 않는 연산들이다.(find, substr 등등)
    특히 string_view의 substr은 string의 O(n)과 다르게 O(1)으로 빠르다.

*/
bool contains_very2(std::string_view str)
{
    return str.find("very") != std::string_view::npos;
}

// undefined behavior 오류
std::string_view return_sv()
{
    std::string s = "this is a string";
    std::string_view sv = s;

    return sv;
}

int main()
{
    // 암묵적으로 std::string 객체가 불필요하게 생성된다.
    std::cout << std::boolalpha << contains_very("c++ string is very easy to use")
              << std::endl;

    std::cout << contains_very("c++ string is not easy to use") << std::endl;

    // string_view 생성 시에는 메모리 할당이 필요 없다.
    std::cout << std::boolalpha << contains_very2("c++ string is very easy to use")
              << std::endl;

    std::cout << contains_very2("c++ string is not easy to use") << std::endl;

    std::string str = "some long long long long long string";
    std::cout << "--------------------" << std::endl;
    std::cout << contains_very2(str) << std::endl;

    // string이므로 메모리 할당
    std::cout << "string -----" << std::endl;
    std::string s = "sometimes string is very slow";
    std::cout << "--------------------" << std::endl;
    std::cout << s.substr(0, 20) << std::endl
              << std::endl;

    std::cout << "string_view -----" << std::endl;
    std::string_view sv = s;
    std::cout << "--------------------" << std::endl;
    std::cout << sv.substr(0, 20) << std::endl;

    std::string_view sv = return_sv(); // <- sv 가 가리키는 s 는 이미 소멸됨!

    // Undefined behavior!!!!
    std::cout << sv << std::endl;
}