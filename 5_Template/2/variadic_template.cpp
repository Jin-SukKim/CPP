// variadic template(가변 길이 템플릿) : 템플릿을 사용해서 임의의 개수의 인자를 받는 방법
/*
    파이썬에서는 print함수를 사용해 다양한 인자의 개수를 출력할 수 있다
    print(1, 3.1, "abc")
    print("adfasf", var)
*/

// C++을 이용해 다양한 인자를 받아 python print와 같이 만들기
#include <iostream>

template <typename T>
void print(T arg)
{
  std::cout << arg << std::endl;
}

/*
    typename 뒤에 오는 "..."을 parameter pack(템플릿 파라미터 팩)이라고 한다.
    template parameter pack은 0개 이상의 탬플릿 인자들을 나타낸다.
    마찬가지로 void print(T arg, Types... args) {} 와 같은 함수 파라미터 팩도 있다.
*/
template <typename T, typename... Types>
void print(T arg, Types... args)
{
  std::cout << arg << ", ";
  print(args...); // print()에 전달된 첫번쨰 parameter를 제외한 나머지
}

/*
    print(T arg)와 print(T arg, Types... args)의 순서를 바꿔서 쓰면
    컴파일러는 print(T arg)를 인지하지 못하기 때문에 에러가 난다.
*/
int main()
{
  print(1, 3.1, "abc");
  print(1, 2, 3, 4, 5, 6, 7);
}

/* ex2: std::string 문자열 합치기
  concat = s1 + s2 + s3; 는 concat = s1.operator+(s2).operator+(s3); 와 같다.
  결국 s2와 s3를 더할때 각각 메모리가 할당되는데 이러면 시간 낭비가 생긴다.

  이미 합쳐진 문자열의 크기를 알고 있으니 차라리 한번에 메모리를 할당하는 것이 훨씬 낫다.

  std::string concat;
  concat.reserve(s1.size() + s2.size() + s3.size());  // 여기서 할당 1 번 수행
  concat.append(s1);
  concat.append(s2);
  concat.append(s3);

  위와 같은 작업을 한번에 끝낼수도 있다.

  std::string concat = StrCat(s1, "abc", s2, s3);

  문제는 StrCat 함수가 임의의 개수의 인자를 받아야 된다는 것이다.

#include <cstring>
#include <iostream>
#include <string>

// 총 문자열의 길이를 구해준다
size_t GetStringSize(const char* s) { return strlen(s); }

size_t GetStringSize(const std::string& s) { return s.size(); }

template <typename String, typename... Strings>
size_t GetStringSize(const String& s, Strings... strs) {
  return GetStringSize(s) + GetStringSize(strs...);
}

// 문자열을 뒤에 붙여주는 함수
void AppendToString(std::string* concat_str) { return; }

template <typename String, typename... Strings>
void AppendToString(std::string* concat_str, const String& s, Strings... strs) {
  concat_str->append(s);
  AppendToString(concat_str, strs...);
}

template <typename String>
std::string StrCat(const String& s) {
  return std::string(s); // s가 항상 string이 아니라 std::string으로 감싸준다.
}

template <typename String, typename... Strings>
std::string StrCat(const String& s, Strings... strs) {
  // 먼저 합쳐질 문자열의 총 길이를 구한다.
  size_t total_size = GetStringSize(s, strs...);

  // reserve 를 통해 미리 공간을 할당해 놓는다.
  std::string concat_str;
  concat_str.reserve(total_size);

  concat_str = s;

  // concat_str 에 문자열들을 붙인다.
  AppendToString(&concat_str, strs...);

  return concat_str;
}

int main() {
  // std::string 과 const char* 을 혼합해서 사용 가능하다.
  std::cout << StrCat(std::string("this"), " ", "is", " ", std::string("a"),
                      " ", std::string("sentence"));
}

*/

/*
sizeof 연산자는 인자의 크기를 리턴하지만 파라미터 팩에 sizeof... 을 사용할 경우 전체 인자의 개수를 리턴

#include <iostream>

// 재귀 호출 종료를 위한 베이스 케이스
int sum_all() { return 0; }

template <typename... Ints>
int sum_all(int num, Ints... nums) {
  return num + sum_all(nums...);
}

template <typename... Ints>
double average(Ints... nums) {
  return static_cast<double>(sum_all(nums...)) / sizeof...(nums);
}

int main() {
  // (1 + 4 + 2 + 3 + 10) / 5
  std::cout << average(1, 4, 2, 3, 10) << std::endl;
}

*/