// 리스트(list) : 양방향 연결구조를 가진 자료형
/*
    Vector와 다르게 임의의 위치에 있는 원소에 바로 접근할 수 없다.
    list 컨테이너는 시작 원소와 마지막 원소의 위치만 기억하기 때문에 하나씩 링크를 따라가야 원하는 위치에 갈 수 있다.
    그래서 아예 []나 at 함수가 없다.

    물론 장점도 있는데 vector는 임의의 위치에 원소를 추가/제거하는 속도가 O(n)이라면
    리스트는 원하는 위치의 앞과 뒤에 있는 링크값만 바꿔주면 되기 떄문에 O(1)이다.
    즉 추가시 전체 배열 복사가 필요없기 때문에 빠르다.
*/
#include <iostream>
#include <list>

/*
    리스트는 왼쪽 혹은 오른쪽을 가르키고 있는 원소들의 모임으로 만들어져 있기 떄문에
    메모리에 연속적으로 이루어져 있지 않을 수 있다. (Vector는 메모리에 연속적으로 있다.)
*/
template <typename T>
void print_list(std::list<T> &lst)
{
    std::cout << "[ ";
    // 전체 리스트를 출력하기 (이 역시 범위 기반 for 문을 쓸 수 있습니다)
    for (const auto &elem : lst)
    {
        std::cout << elem << " ";
    }
    std::cout << "]" << std::endl;
}

int main()
{
    std::list<int> lst;

    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);
    lst.push_back(40);

    // 리스트는 하나씩 링크를 따라가야 되기때문에 i++나 i--밖에 안된다.
    // vector와 같이 i + 4가 안된다.
    for (std::list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr)
    {
        std::cout << *itr << std::endl;
    }

    std::cout << "처음 리스트의 상태 " << std::endl;
    print_list(lst);
    
    for (std::list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr)
    {
        // 만일 현재 원소가 20 이라면
        // 그 앞에 50 을 집어넣는다.
        if (*itr == 20)
        {
            lst.insert(itr, 50);
        }
    }

    std::cout << "값이 20 인 원소 앞에 50 을 추가 " << std::endl;
    print_list(lst);

    // list는 vector와 다르게 원소를 지워도 각 원소들의 주소값이 변하지 않아서 iterator(반복자)가 무효화되지 않는다.
    for (std::list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr)
    {
        // 값이 30 인 원소를 삭제한다.
        if (*itr == 30)
        {
            lst.erase(itr);
            break;
        }
    }

    std::cout << "값이 30 인 원소를 제거한다" << std::endl;
    print_list(lst);
}