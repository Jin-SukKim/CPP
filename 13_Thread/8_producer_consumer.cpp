// 생산자(Producer) 와 소비자(Consumer) 패턴
/*
    보통 멀티 쓰레드 프로그램에서 많이 사용하는 패턴이다.

    생산자(producer)는 무언가 처리할 일을 받아오는 쓰레드이다.

    소비자(consumer)는 받은 일을 처리하는 쓰레드이다.

    만약 인터넷에서 페이지를 긁어서 분석하는 크롤링 프로그램이 있다면
    생산자(proucer)는 페이지를 긁어오는 쓰레드가 되고 소비자(consumer)는 긁어온 페이지는 분석하는 쓰레드가 될 것이다.

    아래의 코드는 크롤링 프로그램이다.

    producer 쓰레드는 웹사이트에서 페이지를 계속 다운로드하는 역할을 한다.
    이 때 다운로드한 페이지들을 downlooaded_pages 라는 queue에 저장한다.
    std::queue<std::string> downloaded_pages;

    queue를 사용한 이유는 큐는 먼저 들어온 것이 먼저 나간다(First In First Out, FIFO)구조 이기 때문이다.
    
    즉 먼저 다운로드한 페이지를 먼저 분석하기 위해 사용했다. 
    (vector등도 상관없다 하지만 vector는 첫번째 원소를 제거하는 작업이 매우 느리다. 차라리 deque를 이용하자)

    (c++은 인터넷 페이지를 다운로드하는 기능을 제공하지 않아 상황을 가정하고 프로그램을 만든다.)

    consumer 쓰레드는 언제 일이 들어올지를 알 수 없다.
    따라서 downloaded_pages 가 비어있지 않을 때 까지 계속 while 루프를 돌아야된다.
    이 때 문제는 cpu속도보다 인터넷의 다운로드 속도가 매우 느리다는 것이다.
    예로 들자면 다운로드에 100ms가 든다면 cpu는 ownloaded_pages->empty()를 수십만번 호출할 수 있다. (CPU 낭비)

    그래서 sleep 함수를 이용해 시간을 delay시켲줬다.
    하지만 일이 들어오기 전까지 계쏙 10ms마다 할 일이 있는지 확인하는 것은 매우 비효율적이다.
    매 번 언제 올지 모르는 데이터를 확인하기 위해 지속적으로 mutex 를 lock 하고, 큐를 확인해야하기 떄문이다.

    차라리 producer에서 데이터가 가끔 들어오는 것을 알고 있다면 consumer를 재워놓고, 
    producer에서 일이 온다면 호출하는 것이 쓰레드가 일이 올때까지 다른 쓰레드가 일을 할 수 있으므로 
    CPU를 훨씬 효율적으로 사용할 수 있다.

    이와 같은 형태를 C++에서 구현할 수있도록 여러 도구를 제공하고 있다.
    그 중 하나인 condition_variable(조건변수)를 사용해 자원을 효율적으로 사용할 수 있다.
*/
#include <chrono> // std::chrono::miliseconds
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

// 무언가 처리할 일을 받아오는 쓰레드
void producer(std::queue<std::string> *downloaded_pages, std::mutex *m,
              int index)
{   
    // 총 5개의 쓰레드 생성
    for (int i = 0; i < 5; i++)
    {
        // 웹사이트를 다운로드 하는데 걸리는 시간이라 생각하면 된다.
        // 각 쓰레드 별로 다운로드 하는데 걸리는 시간이 다르다.
        // std::this_thread::sleep_for 함수는 인자로 받은 시간만큼 쓰레드를 sleep 시킨다. 인자는 chrono의 시간 객체를 받는다.
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));

        // content = 다운받은 웹사이트의 내용
        std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" +
                              std::to_string(index) + ")\n";

        // 쓰레드는 5개로 dowloaded_pages에 접근하는 쓰레드들 사이에 race condition이 발생할 수도 있어 mutex를 사용하낟.
        // data 는 쓰레드 사이에서 공유되므로 critical section 에 넣어야 한다.
        m->lock();
        downloaded_pages->push(content);
        m->unlock();
    }
}

// 받아온 일을 처리하는 쓰레드
void consumer(std::queue<std::string> *downloaded_pages, std::mutex *m,
              int *num_processed)
{
    // 전체 처리하는 페이지 개수가 5 * 5 = 25 개.
    while (*num_processed < 25)
    {
        m->lock();
        // 만일 현재 다운로드한 페이지가 없다면 다시 대기.
        if (downloaded_pages->empty())
        {   
            // deadlock 방지를 위해 사용한다.
            m->unlock(); // (Quiz) 여기서 unlock 을 안한다면 어떻게 될까요?

            // cpu 낭비를 줄이기 위해 sleep 사용
            // 10 밀리초 뒤에 다시 확인한다.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // 맨 앞의 페이지를 읽고 대기 목록에서 제거한다.
        std::string content = downloaded_pages->front();
        downloaded_pages->pop();

        // unlock을 해줘 다음 쓰레드가 다음 원소를 바로 처리할 수 있게 해준다.
        (*num_processed)++;
        m->unlock();

        // content 를 처리한다. (처리하는 속도는 80ms가 소모된다고 가정한다.)
        std::cout << content;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main()
{
    // 현재 다운로드한 페이지들 리스트로, 아직 처리되지 않은 것들이다.
    std::queue<std::string> downloaded_pages;
    std::mutex m;

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++)
    {
        producers.push_back(std::thread(producer, &downloaded_pages, &m, i + 1));
    }

    int num_processed = 0;
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; i++)
    {
        consumers.push_back(
            std::thread(consumer, &downloaded_pages, &m, &num_processed));
    }

    for (int i = 0; i < 5; i++)
    {
        producers[i].join();
    }
    for (int i = 0; i < 3; i++)
    {
        consumers[i].join();
    }
}