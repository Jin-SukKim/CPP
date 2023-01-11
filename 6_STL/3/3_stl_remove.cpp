// 원소제거 - remove, remove_if
/*
    대부분의 컨테이너는 원소를 삭제하는 함수를 포함하고 있따.
    std::vector<int> vec;
    vec.erase(vec.begin() + 3); // vec[3] 삭제

    하지만 만약 vector에서 위치가 아닌 값이 3인 원소를 삭제하려고 하면

    std::vector<int>::iterator itr = vec.begin();

    for (; itr != vec.end(); ++itr) {
        if (*itr == 3) {
            vec.erase(itr);
            itr = vec.begin();
        }
    }
    와 같이 for 문을 사용해야 할 것이다. 하지만 삭제할 때마다 반복자가 초기화되 벡터를 가르키는 위치로 다시 가져와야 한다.
    물론 일반 변수를 이용해 배열 다루듯 처리할 수 있지만 반복자로 원소접근을 수행한다는 약속과는 맞지 않다.
*/

// 해결방안
/*
    erase 함수는 2가지 형태가 있다.
    1) Iterator erase(Iterator pos);                    // 임의의 위치의 원소를 삭제
    2) Iterator erase(Iterator first, Iterator last);   // first부터 last 사이의 원소들을 삭제

    이 함수를 이용해 원하는 값의 원소를 삭제하기엔 부족함이 있는데 remove함수를 이용해 부족함을 채워준다.
    remove 함수는 원하는 값의 원소를 찾은 후 그 뒤의 값들을 1칸씩 왼쪽으로 shift해 삭제하고 싶은 원소가 끝부분에 위치하게 만들어 준다.
    그리고 삭제할 원소부터 벡터의 맨 뒤까지 삭제하면 원하는 값을 지울수 있게 된다.
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

    std::cout << "벡터에서 값이 3 인 원소 제거 ---" << std::endl;
    // remove 함수는 모든 컨테이너에서 사용가능하다.
    vec.erase(std::remove(vec.begin(), vec.end(), 3), vec.end());
    print(vec.begin(), vec.end());
}