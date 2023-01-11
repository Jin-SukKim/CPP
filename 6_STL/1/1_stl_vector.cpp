/*
    C++는 많은 표준 라이브러리를 가지고 있다. (iostream, chrono, regex, 등)

    보통 C++ 표준 Lib를 의미하는 3가지가 있다.

    1. 임의 타입의 객체를 보관할 수 있는 컨테이너 (container)

    2. 컨테이너에 보관된 원소에 접근할 수 있는 반복자 (iterator)

    3. 반복자들을 가지고 일련의 작업을 수행하는 알고리즘 (algorithm)
*/

/*
    C++ STL 컨테이너는 크게 두개가 있다.

    1. Sequence Container - 배열처럼 순차적으로 객체들을 보관한다(vector, list, deque)
    2. Associative Container - 키를 바탕으로 대응되는 값을 찾아준다()

*/

// std::vector - STL Container : 가변 길이 배열
/*
    벡터에는 원소들이 메모리 상에서 실제로 순차적으로 저장되어 있어서 임의의 위치에 있는 원소에 매우 빠르게 접근 가능하다.

    컴퓨터 공학에서 이러한 작업 수행 속도를 복잡도(Complexity, Big O)라 부른다.
    N개의 데이터가 존재할 때 작업을 수행할 떄 몇변의 작업이 필요한지에 대한 표현 방식이다. - 복잡도가 클수록 수행 시간이 늘어난다.

    ex) bubble sort - O(N^2), 라 표현된다.
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (arr[i] > arr[j]) {
            swap(arr, i, j)
            }
        }
    }

    벡터의 경우 임의의 위치에 있는 원소 접근을 O(1)로 수행 가능하다.
        - find : [], at                         (O(1))
        
        - add/delete(at the end)  : push_back/pop_back
                                                (amortizedO(1) - 평균 O(1)이나 최악의 경우 O(n), 벡터가 꽉찰 경우 기존 배열을 복사후 다시 생성해야되서)
        
        - add/delete(anywhere) : insert/erase   O(n)

*/
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec;
    vec.push_back(10); // 맨 뒤에 10 추가
    vec.push_back(20); // 맨 뒤에 20 추가
    vec.push_back(30); // 맨 뒤에 30 추가
    vec.push_back(40); // 맨 뒤에 40 추가

    for (std::vector<int>::size_type i = 0; i < vec.size(); i++)    // size() return type = size_type
    {
        std::cout << "vec 의 " << i + 1 << " 번째 원소 :: " << vec[i] << std::endl;
    }
}
