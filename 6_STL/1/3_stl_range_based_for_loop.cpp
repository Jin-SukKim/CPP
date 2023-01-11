// 범위 기반 for 문 (range based for loop)
/*
    보통 for 문은 for (int i = 0; i < 10; i++)형식으로 많이 사용되는데
    C++ 11부터는 다른 방식도 제공된다. for (int elem : vec) { std::cout < elem << std::endl; }
    이걸 범위기반(range-base) for 문이라고 한다.
*/
#include <iostream>
#include <vector>

template <typename T>
void print_vector(std::vector<T> &vec)
{
    // 전체 벡터를 출력하기
    for (typename std::vector<T>::iterator itr = vec.begin(); itr != vec.end(); ++itr)
    {
        std::cout << *itr << std::endl;
    }
}

template <typename T>
void print_vector_range_based(std::vector<T> &vec)
{
    // 전체 벡터를 출력하기
    // 값을 복사하기 보다는 reference를 받는다.
    for (const auto &elem : vec) // vector 타입이 변경될 수 있으니 auto를 사용
    {
        std::cout << elem << std::endl;
    }
}

int main()
{
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    // range-based for 문
    for (int elem : vec) // elem에 vec의 원소들이 매 루프마다 복사된다. int elem = vec[i]와 같다.
    {
        std::cout << "원소 : " << elem << std::endl;
    }

    std::cout << "print_vector" << std::endl;
    print_vector(vec);
    std::cout << "print_vector_range_based" << std::endl;
    print_vector_range_based(vec);

    return 0;
}