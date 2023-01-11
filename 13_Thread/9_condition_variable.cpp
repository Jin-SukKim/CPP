// condition variable (조건 변수)
/*
    producer consumer 패턴에서 CPU 자원을 효율적으로 사용하기 위해 사용하는 도구이다.

    이전 예제에서는 어떠 어떠한 조건을 만족할 때 까지 자라는 명령을 하지 못해 consumer 쓰레드가 10ms마다 계속 일이 있는지 확인했다.
    이것을 condition variable을 이용하면 해결할 수 있다.

    condition_variable의 wait 함수는 인자로 조건을 받는데 해당 조건이 true일 때까지 기다리게 한다.
    ex)
        cv->wait(lk, [&] { return !downloaded_pages->empty() || *num_processed == 25; });

        lambda 함수를 이용해 조건을 전달했다. 
        lambda 함수의 [&]은 외부의 모든 변수들을 reference로 캡펴하겠다는 의미이고 뒤의 조건들이 참이 될 때까지 기다리게 된다.

        lk는 unique_lock으로 std::unique_lock<std::mutex> lk(*m); 사용했다.
        이것을 lock_guard와 거의 동일하다.
        lock_guard는 생성자 말고는 따로 lock을 할 수 없지만 unique_lock은 unlock한 후 다시 lock할 수 있다.

        그리고 unique_lock을 사용한 이유는 condition_variable의 wait 함수가 unique_lock을 인자로 받기 떄문이다.

    condition variable의 notify_one 함수는 조건이 false인탓에 잠들어 있는 쓰레드들 중 하나를 깨워 조건을 다시 검색하게 해준다.
    만약 조건이 true라면 쓰레드는 일을 시작할 것이다.

    producer들이 모두 일을 끝냈다면 자고 있는 일부 consumer 쓰레드들이 있을 수 있다.
    만약 condition_variable.notify_all() 함수를 사용하지 않는다면 자고 있는 consumer 쓰레드들은 join 되지 않는 문제가 발생한다.
    notify_all을 통해 모든 쓰레드들을 깨워 조건을 검사하게 한다.

    condition_vairable은 또 wait_for이나 wait_until 등 유용한 함수들도 있다.
*/
#include <chrono>             // std::chrono::miliseconds
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

void producer(std::queue<std::string> *downloaded_pages, std::mutex *m,
              int index, std::condition_variable *cv)
{
    for (int i = 0; i < 5; i++)
    {
        // 웹사이트를 다운로드 하는데 걸리는 시간이라 생각하면 된다.
        // 각 쓰레드 별로 다운로드 하는데 걸리는 시간이 다르다.
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
        std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" +
                              std::to_string(index) + ")\n";

        // data 는 쓰레드 사이에서 공유되므로 critical section 에 넣어야 한다.
        m->lock();
        downloaded_pages->push(content);
        m->unlock();

        // consumer 에게 content 가 준비되었음을 알린다.
        // 잠들어 있는 쓰레드들 중 하나를 꺠워 조건을 다시 확인한다.
        cv->notify_one();
    }
}

void consumer(std::queue<std::string> *downloaded_pages, std::mutex *m,
              int *num_processed, std::condition_variable *cv)
{
    while (*num_processed < 25)
    {
        std::unique_lock<std::mutex> lk(*m);

        // condition variable(조건변수)의 wait 함수에 어떤 조건이 true일 때까지 기다릴지 해당 조건을 인자로 전달한다.
        cv->wait(
            lk, [&]
            { return !downloaded_pages->empty() || *num_processed == 25; });

        // wait함수에서 탈춘한 이유가 모든 페이지를 처리해서인지 아니면 페이지가 추가되서인지 알 수 없기 때문에 
        // 만약 모든 페이지 처리가 끝나서 탈출했다면 쓰레드를 종료하기 위해 조건을 만들었다.
        if (*num_processed == 25)
        {
            lk.unlock();
            return;
        }

        // 맨 앞의 페이지를 읽고 대기 목록에서 제거한다.
        std::string content = downloaded_pages->front();
        downloaded_pages->pop();

        (*num_processed)++;
        lk.unlock();

        // content 를 처리한다.
        std::cout << content;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main()
{
    // 현재 다운로드한 페이지들 리스트로, 아직 처리되지 않은 것들이다.
    std::queue<std::string> downloaded_pages;
    std::mutex m;
    // 조건 변수
    std::condition_variable cv;

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++)
    {
        producers.push_back(
            std::thread(producer, &downloaded_pages, &m, i + 1, &cv));
    }

    int num_processed = 0;
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; i++)
    {
        consumers.push_back(
            std::thread(consumer, &downloaded_pages, &m, &num_processed, &cv));
    }

    for (int i = 0; i < 5; i++)
    {
        producers[i].join();
    }

    // 나머지 자고 있는 쓰레드들을 모두 깨운다.
    cv.notify_all();

    // 자고있던 모든 Consumer 쓰레드들을 꺠워 조건을 검사하도록 한다.
    // 이미 모든일이 끝났을 것이므로 num_processed = 25가 되 쓰레드들이 종료되게 해준다.
    for (int i = 0; i < 3; i++)
    {
        consumers[i].join();
    }
}