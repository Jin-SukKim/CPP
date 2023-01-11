/*
    memory_order_release는 해당 명령 이전의 모든 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지한다.
    memory_order_acquire는 release와 반대로 해당 명령 뒤에 오는 모든 메모리 명령들이 해당 명령 위로 재배치되는 것을 금지한다.
    두 개의 쓰레드들이 같은 변수를 release와 acquire을 통해 동기화(synchronize)를 수행할 수 있다. 

    memory_order_acq_rel는 acquire과 release모두 수행하는 것이다.
    이것은 읽기와 쓰기 모두를 수행하는 명령들(fetch_add 등)에서 사용된다

    memory_order_seq_cst는 메모리 명령의 순차적 일관성(sequential consistency)을 보장해준다.

    순차적 일관성(sequential consistency)이란 메모리 명령 재배치도 없고, 모든 쓰레드에서 
    모든 시점에 동일한 값을 관찰할 수 있는 보통 생각하는 CPU의 작동방식이다.
*/
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

std::atomic<bool> is_ready;
std::atomic<int> data[3];

// data를 store하는 명령들은 relaxed로 CPU에서 순서가 마음대로 재배치될 수 있다.
// 하지만 release명령을 넘어가서 재배치될 수는 없다. (결국 consumer에서 data들의 값을 보면 언제나 1,2,3이 들어가있다.)
void producer()
{
    data[0].store(1, memory_order_relaxed);
    data[1].store(2, memory_order_relaxed);
    data[2].store(3, memory_order_relaxed);
    is_ready.store(true, std::memory_order_release); // 동기화
}

// consumer에서 data들의 값을 보면 언제나 1,2,3이 들어가있다.
void consumer()
{
    // data 가 준비될 때 까지 기다린다.
    while (!is_ready.load(std::memory_order_acquire)) // 동기화
    {
    }

    std::cout << "data[0] : " << data[0].load(memory_order_relaxed) << std::endl;
    std::cout << "data[1] : " << data[1].load(memory_order_relaxed) << std::endl;
    std::cout << "data[2] : " << data[2].load(memory_order_relaxed) << std::endl;
}

int main()
{
    std::vector<std::thread> threads;

    threads.push_back(std::thread(producer));
    threads.push_back(std::thread(consumer));

    for (int i = 0; i < 2; i++)
    {
        threads[i].join();
    }
}