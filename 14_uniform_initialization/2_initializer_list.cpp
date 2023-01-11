// 초기화자 리스트 (Initializer list)
/*
    배열을 정의할 때 보통 int arr[] = {1, 2, 3, 4};와 같이 사용했다.

    {}를 이용해 위와 같은 효과를 낼 수 있다.
    vector<int> v = {1, 2, 3, 4};

    initializer_list는 {}를 이용해 생성자를 호출할 때, 클래스의 생성자들 중에
    initializer_list를 인자로 받는 생성자가 있다면 전달된다.

    () 사용시 initializer_list가 생성되지 않는다.

    initializer_list를 이용해 컨테이너들을 간단하게 정의할 수 있다.
*/

#include <iostream>

class A
{
public:
    // 생성자에 initializer_list를 인자로 받으면 {}를 이용해 호출해 컨테이너를 생성할 수 있다.
    A(std::initializer_list<int> l)
    {
        for (auto itr = l.begin(); itr != l.end(); ++itr)
        {
            std::cout << *itr << std::endl;
        }
    }
};

template <typename T>
void print_vec(const std::vector<T> &vec)
{
    std::cout << "[";
    for (const auto &e : vec)
    {
        std::cout << e << " ";
    }
    std::cout << "]" << std::endl;
}

template <typename K, typename V>
void print_map(const std::map<K, V> &m)
{
    for (const auto &kv : m)
    {
        std::cout << kv.first << " : " << kv.second << std::endl;
    }
}

int main()
{
    // 기초적인 initializer_list 사용법
    A a = {1, 2, 3, 4, 5};

    // initializer_list를 이용해 간단하게 컨테이너 생성하기
    std::vector<int> v = {1, 2, 3, 4, 5};
    print_vec(v);

    std::cout << "----------------------" << std::endl;
    // 첫번째 원소에 key를 두번째 원소에 value를 넣어 {}를 이용해 map의 pair를 생성할 수 있다.
    std::map<std::string, int> m = {
        {"abc", 1}, {"hi", 3}, {"hello", 5}, {"c++", 2}, {"java", 6}};
    print_map(m);
}