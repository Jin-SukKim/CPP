// 람다 함수(lambda function) - 이름없는 함수 즉 임시함수
/*
    람다 함수의 기본적인 사용법
    [capture list] (받는 인자) -> 리턴 타입 { 함수 본체 }

    capture list는 외부 변수를 마치 람다 함수 내부에 있는 변수처럼 사용할 수 있게 해주는 것이다.
    &capture list와 같이 reference로 받아도 되고 일반적인 capture list로 받으면 const로 사용하게 된다.

    또한 클래스에서 람다의 capture list를 사용한다면 this를 넘겨서 this->num와 같은 형식으로도 사용가능하다.

    capture list 사용법
    1. [] : 아무것도 캡쳐 안함

    2. [&a, b] : a 는 레퍼런스로 캡쳐하고 b 는 (변경 불가능한) 복사본으로 캡쳐

    3. [&] : 외부의 모든 변수들을 레퍼런스로 캡쳐

    4. [=] : 외부의 모든 변수들을 복사본으로 캡쳐

    ex) [](int i) -> bool { return i % 2 == 1; }, int i를 인자로 받고 bool을 return하는 함수
    만약 return type을 생략한다면 함수본체를 보고 컴파일러가 판단하다 복잡하다면 오류를 일으킨다.

    return 타입을 생략할 경우 사용법
    [capture list] (받는 인자) {함수 본체}

    ex) [](int i) { return i % 2 == 1; } , 컴파일러가 bool타입으로 자동으로 만들어준다.

    함수 사용법
    1.  [](int i) { return i % 2 == 1; }(3);  // true
    2.  auto func = [](int i) { return i % 2 == 1; };
        func(4);  // false;


*/
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename Iter>
void print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}
int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::cout << "처음 vec 상태 ------" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "벡터에서 홀수인 원소 제거 ---" << std::endl;
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [](int i) -> bool // lambda function이다.
                             { return i % 2 == 1; }),
              vec.end());
    print(vec.begin(), vec.end());

    std::vector<int> vec2;
    vec2.push_back(5);
    vec2.push_back(3);
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(3);
    vec2.push_back(4);

    std::cout << "처음 vec 상태 ------" << std::endl;
    print(vec2.begin(), vec2.end());

    std::cout << "벡터에서 홀수인 원소 ---" << std::endl;
    int num_erased = 0;

    // capture list(캡쳐 목록)을 이용해 외부변수를 사용할 수 있다.  
    // 즉 num_erased를 캡쳐해 마치 함수 내부변수인 것 처럼 사용할 수 있다.
    vec2.erase(std::remove_if(vec2.begin(), vec2.end(),
                             [&num_erased](int i)
                             {
                                 if (num_erased >= 2)
                                     return false;
                                 else if (i % 2 == 1)
                                 {
                                     num_erased++;
                                     return true;
                                 }
                                 return false;
                             }),
              vec2.end());
    print(vec2.begin(), vec2.end());
}