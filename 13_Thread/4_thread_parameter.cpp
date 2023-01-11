// Thread에 인자전달 하기
/*
    Thread에 인자전달 하는 방법은 bind와 비슷하다.

    thread(Callable, parameter1, parameter2 ...)

    쓰레드의 첫번째 인자로 원하는 함수(Callable은 모두 된다)를 넣어주고 이어서 해당 함수에 전달할 인자들을 적어준다.

    this_thread::get_id를 이용해 현재 돌아가고 있는 쓰레드의 ID를 구할 수 있다.

    worker 함수에서 cout이 아닌 printf를 사용한 이유는 std::cout의 경우 << 를 실행하는 과정 중간 중간에 계속
    쓰레드들이 바뀌면 메시지가 섞여서 나온다.
    (ex: 
        std::cout << "쓰레드 " << hex << this_id << " 에서 " << dec << *start << " 부터 "
        << *(end - 1) << " 까지 계산한 결과 : " << sum << std::endl; 라면
        쓰레드 쓰레드 this_id값 에서 thid_id값 쓰레드
        와 같이 메시지가 섞여서 나온다.)

    즉 std::cout << A 와 같이 하나만 출력하면 내용이 보장되지만 std::cout << A << B와 같다면 operator<<가 두 번 불리는 것이므로
    중간에 context switching되면 중간에 내용이 섞여서 출력될 수 있는 것이다.

    반면 printf는 문자열을 출력할 때 context switch가 되도 다른 쓰레드들이 그 사이에 메시지를 집어넣지 못하게 막는다.

    쓰레드들은 메모리를 공유할 수 있어 data와 partial sum등에 서로 접근이 가능하다.
*/
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>
using std::thread;
using std::vector;

// 1 부터 10000 까지의 합을 여러 쓰레드들을 소환해서 빠르게 계산하는 방법

// 덧셈을 수행할 데이터의 시작과 끝을 받아서 해당 범위내의 원소를 모두 더한 후 결과를 result에 저장하는 함수
// thread는 return값이 없어 어떠한 결과를 return하고 싶다면 포인터의 형태를 이용해야된다.
void worker(vector<int>::iterator start, vector<int>::iterator end,
            int *result)
{   
    // 해당 범위 원소들의 덧셈을 수행
    int sum = 0;
    for (auto itr = start; itr < end; ++itr)
    {
        sum += *itr;
    }
    *result = sum;

    // 쓰레드의 id 를 구한다.
    thread::id this_id = std::this_thread::get_id();
    printf("쓰레드 %x 에서 %d 부터 %d 까지 계산한 결과 : %d \n", this_id, *start,
           *(end - 1), sum);
}

int main()
{
    vector<int> data(10000);
    for (int i = 0; i < 10000; i++)
    {
        data[i] = i;
    }

    // 각 쓰레드에서 계산된 부분 합들을 저장하는 벡터
    vector<int> partial_sums(4);

    // 각 쓰레드에게 덧셈을 수행할 범위를 지정해주는 부분(2500개씩 할당하고 있다)
    vector<thread> workers;
    for (int i = 0; i < 4; i++)
    {   
        // Thread의 인자로 worker 함수를 넘기고 worker함수에 필요한 
        // vector<int>::iterator start, vector<int>::iterator end, int *result 값들을 넘겨준다.
        workers.push_back(thread(worker, data.begin() + i * 2500,
                                 data.begin() + (i + 1) * 2500, &partial_sums[i]));
    }

    // 모든 쓰레드가 종료되기를 기다린다.
    for (int i = 0; i < 4; i++)
    {
        workers[i].join();
    }

    int total = 0;

    // 각 쓰레드에서 계산한 결과는 partial_sums에 저장되어 있다.
    for (int i = 0; i < 4; i++)
    {
        total += partial_sums[i];
    }
    std::cout << "전체 합 : " << total << std::endl;
}