// 덱 (deque - double ended queue)
/*
    deque는 vector와 비슷하게 O(1)의 속도로 임의의 위치의 원소에 접근 가능하고
    맨 뒤에 원소를 추가/제거 하는 작업도 O(1)의 속도로 가능하다.
    그리고 vector와 다르게 맨 앞에 원소를 추가/제거 하는 작업도 O(1)속도로 수행 가능하다.

    임의의 위치에 원소를 추가/제거하는 것도 O(n)속도로 가능하다(vector보다 빠르다).

    단점은 deque의 경우 vector와 다르게 원소들이 메모리에 연속적으로 존재하지 않는다.
    원소들의 위치 정보를 저장하기 위해 추가적인 메모리가 필요하다.
    (ex: 64bit libc++의 경우 1개의 원소를 보관하는 deque는 원소 크기에 비해 8배 많은 메모리를 필요로 한다.)

    원소들이 일정 크기로 잘려 존재하고 이 메모리 공간의 주소를 연속된 블록들에 저장된 벡터가 deque이다.
    기존 vector와 다르게 맨앞과 맨뒤에 공간을 남겨둔다. 이를 통해 O(1)의 속도로 앞과 뒤에 작업을 수행한다.
    즉 원소가 추가될 떄 vector와 다르게 새로 공간을 할당하고 복사하는게 아닌 단순히 새로운 블록을 생성해
    원소를 추가하면 된다.(물론 이 주소를 저장하는 vector가 꽉차면 새로 할당후 복사해줘야한다.)

    즉 deque는 속도를 위해 공간(메모리)를 많이 희생하는 Container이다.
*/

#include <deque>
#include <iostream>

template <typename T>
void print_deque(std::deque<T> &dq)
{
    // 전체 덱을 출력하기
    std::cout << "[ ";
    for (const auto &elem : dq)
    {
        std::cout << elem << " ";
    }
    std::cout << " ] " << std::endl;
}
int main()
{
    std::deque<int> dq;
    dq.push_back(10);
    dq.push_back(20);
    dq.push_front(30);
    dq.push_front(40);

    std::cout << "초기 dq 상태" << std::endl;
    print_deque(dq);

    std::cout << "맨 앞의 원소 제거" << std::endl;
    dq.pop_front();
    print_deque(dq);
}

/*
어떠한 컨테이너를 사용할지는 전적으로 이 컨테이너를 가지고 어떠한 작업들을 많이 하냐에 달려있습니다.

일반적인 상황에서는 그냥 벡터를 사용한다 (거의 만능이다!)

만약에 맨 끝이 아닌 중간에 원소들을 추가하거나 제거하는 일을 많이 하고, 원소들을 순차적으로만 접근 한다면 리스트를 사용한다.

만약에 맨 처음과 끝 모두에 원소들을 추가하는 작업을 많이하면 덱을 사용한다.

참고적으로 O(1)O(1) 으로 작동한다는 것은 언제나 이론적인 결과일 뿐이며 실제로 프로그램을 짜게 된다면, 
O(\log n)O(logn) 이나 O(n)O(n) 보다도 느릴 수 있습니다. 
(nn 의 크기에 따라서) 따라서 속도가 중요한 환경이라면 적절한 벤치마크를 통해서 성능을 가늠해 보는것도 좋습니다.

문제 1: deque 직접 재현하기
*/