// 정렬(sort) - sort, stable_sort, partial_sort
/*
    sort : 일반적인 정렬 함수

    stable_sort : 정렬을 하되 원소들 간의 순서를 보존한다. 즉 만약에 벡터에 [a, b] 순으로 있었는데,
                  a 와 b 가 크기가 같다면 정렬을 [a,b] 혹은 [b,a] 로 할 수 있다.
                  sort 의 경우 그 순서가 랜덤으로 정해지지만 stable_sort 의 경우 그 순서를 반드시 보존한다.
                  즉 컨테이너 상에서 [a,b] 순으로 있엇다면 정렬 시에도 (크기가 같다면) [a,b] 순으로 나오게되는데 때문에 sort보다 좀 더 느리다.

    partial_sort : 배열의 일부분만 정렬합니다(sort보다 오래걸린다.)
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
        std::cout << *begin << " ";
        begin++;
    }
    std::cout << std::endl;
}

// sort의 3번쨰 인자로 특수한 조건을 전달해 정렬순서를 내림차순으로 바꾼다.
struct int_compare
{
    bool operator()(const int &a, const int &b) const { return a > b; }
};

// stable sort : 원소가 삽입되어 있는 순서를 보존하는 정렬 방식
template <typename Iter>
void stable_print(Iter begin, Iter end)
{
    while (begin != end)
    {
        std::cout << "[" << *begin << "] ";
        begin++;
    }
    std::cout << std::endl;
}
struct User
{
    std::string name;   
    int age;            

    User(std::string name, int age) : name(name), age(age) {}

    bool operator<(const User &u) const { return age < u.age; }
};

std::ostream &operator<<(std::ostream &o, const User &u)
{
    o << u.name << " , " << u.age;
    return o;
}

int main()
{
    std::vector<int> vec;
    vec.push_back(5);
    vec.push_back(3);
    vec.push_back(1);
    vec.push_back(6);
    vec.push_back(4);
    vec.push_back(7);
    vec.push_back(2);

    std::cout << "sort 정렬 전 ----" << std::endl;
    print(vec.begin(), vec.end());
    // sort 함수는 위와 같이 정렬할 원소의 시작 위치와, 마지막 위치 바로 뒤를 반복자로 받는다. (기본적으로 오름차순 정렬)
    // 이 함수는 벡터와 데크만 가능하고 나머지 컨테이너는 sort 함수를 적용할 수 없다.
    std::sort(vec.begin(), vec.end());

    std::cout << "정렬 후 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::cout << "sort 정렬 전 ----" << std::endl;
    print(vec.begin(), vec.end());
    // 정렬 방법 바꾸기
    std::sort(vec.begin(), vec.end(), int_compare());
    // std::sort(vec.begin(), vec.end(), greater<int>()); functional 해더에 다음과 같은 템플릿 클래스가 존재해 greater<int>()로 해도된다.

    std::cout << "정렬 후 ----" << std::endl;
    print(vec.begin(), vec.end());

    std::vector<int> vec2;
    vec2.push_back(5);
    vec2.push_back(3);
    vec2.push_back(1);
    vec2.push_back(6);
    vec2.push_back(4);
    vec2.push_back(7);
    vec2.push_back(2);

    // 일부만 정렬, 중간에 들어가는 위치까지만 전체에서 가장 작은 애들만 순서대로 저장한다.
    std::cout << "pratial sort 정렬 전 ----" << std::endl;
    print(vec2.begin(), vec2.end());
    std::partial_sort(vec2.begin(), vec2.begin() + 3, vec2.end());

    std::cout << "정렬 후 ----" << std::endl;
    print(vec2.begin(), vec2.end());

    // stable sort는 삽입된 순서를 보존한다.
    // 즉 age가 같은 경우, 삽입된 순서인 name순으로 나올것이다.
    std::vector<User> vec3;
    for (int i = 0; i < 100; i++)
    {
        // string 순서
        std::string name = "";
        name.push_back('a' + i / 26);
        name.push_back('a' + i % 26);
        vec3.push_back(User(name, static_cast<int>(rand() % 10))); // int는 0-10 랜덤 숫자
    }

    std::vector<User> vec4 = vec3;

    std::cout << "정렬 전 ----" << std::endl;
    stable_print(vec3.begin(), vec3.end());

    std::sort(vec3.begin(), vec3.end());

    std::cout << "정렬 후 ----" << std::endl;
    stable_print(vec3.begin(), vec3.end());

    std::cout << "stable_sort 의 경우 ---" << std::endl;
    std::stable_sort(vec4.begin(), vec4.end());
    stable_print(vec4.begin(), vec4.end());
}