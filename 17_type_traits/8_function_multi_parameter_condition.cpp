/*
    만약 함수의 인자로 하나의 멤버 함수의 타입을 확인하는게 아니라
    여러개의 멤버 함수의 타입을 확인하고 싶을 떄는 반복자를 사용해서 확인해 주어야 된다.

    주어진 타입 T가 컨테이너인지 아닌지 쉽게 알 순 없지만 원소들을 출력하기 위해 begin과 end가
    정의되어 있다는 사실은 알 고 있다.
    따라서, T에 begin과 end가 정의되어 있는지 확인해야 한다.

    아래 코드는 잘 작동하지만 개선점이 존재하낟.
    바로 너무 많은 typename의 사용으로 함수가 정확이 어떤 템플릿 인자를 받는지 쉽게 알아보기 어렵다는 점이다.
    그래서 C++ 17 부터는 void_t라는 메타 함수가 추가됬다.
*/
#include <iostream>
#include <set>
#include <type_traits>
#include <vector>

template <typename Cont, typename = decltype(std::declval<Cont>().begin()),
          typename = decltype(std::declval<Cont>().end())>
void print(const Cont &container)
{
    std::cout << "[ ";
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "]\n";
}

// begin과 end가 정의되어 있지 않다.
struct Bad {
  void begin();
};


int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);

    std::set<char> s = {'a', 'b', 'f', 'i'};
    print(s);

    // print(Bad{}); // 컴파일 오류
}