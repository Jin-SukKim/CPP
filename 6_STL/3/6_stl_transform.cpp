// 원소 수정하기 (transform)
/*
    transform을 사용하면 컨테이너(vector, list, etc) 원소에 변화를 주는 작업을 도와준다
    ex) 모든 원소에 1을 더하기 등.

    기본 사용법
    transform (시작 반복자, 끝 반복자, 결과를 저장할 컨테이너의 시작 반복자, Pred)
    ex) std::transform(vec.begin(), vec.end(), vec.begin(), [](int i) { return i + 1; });

    주의점은 결과를 저장할 컨테이너가 원래의 컨테이너보다 크거나 같아야 한다.
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

    std::cout << "벡터 전체에 1 을 더한다" << std::endl;
    std::transform(vec.begin(), vec.end(), vec.begin(),
                   [](int i)
                   { return i + 1; });
    print(vec.begin(), vec.end());

    // vec2 에는 6 개의 0 으로 초기화 한다.
    std::vector<int> vec2(6, 0);

    std::cout << "처음 vec 과 vec2 상태 ------" << std::endl;
    print(vec.begin(), vec.end());
    print(vec2.begin(), vec2.end());

    std::cout << "vec 전체에 1 을 더한 것을 vec2 에 저장 -- " << std::endl;
    std::transform(vec.begin(), vec.end(), vec2.begin(),
                   [](int i)
                   { return i + 1; });
    print(vec.begin(), vec.end());
    print(vec2.begin(), vec2.end());
}