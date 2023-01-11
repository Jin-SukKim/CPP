// Mutex(뮤텍스) : race condition을 해결하기 위해 사용
/*
    여러 쓰레드들이 한 자원을 공유해 사용한다면 mutex를 이용해 한 번에 한 쓰레드만 같은 자원을 사용할 수 있게해준다.

    mutex는 mutual exclusion(상호배제)에서 따왔다.

    mutex 사용법

        mutex.lock() 함수는 mutex를 한 쓰레드에서 사용하겠다고 하는 것이다.
        한번에 한 쓰레드에서만 mutex의 사용권한을 가지게 되므로 다른 쓰레드가 mutex.lock()을 하면
        mutex권한을 가진 쓰레드가 mutex.unlock() 함수를 이용해 mutex를 반환하는 것을 기다리게 된다.

    따라서 mutex를 이용해 race condition을 막을 수 있다.
    이런 lock()과 unlock() 함수들 사이의 코드를 critical section(임계영역)이라고 부른다.

    만약 mutex권한을 가져간 후 unlock()하지 않는다면 다른 모든 쓰레드와 권한을 가진 쓰레드 모두가 기다리게되고
    결국 연산을 진행하지 못한다.
    이러한 상황을 deadlock(데드락)이라고 한다.

    포인터도 이런 비슷한 상황이 있었는데 이걸 unique_ptr의 소멸자를 이용해 자동해제해 처리하게 했다.

    mutex도 사용 후 해제 패턴을 따르기에 동일하게 소멸자에서 처리할 수 있다.

    std::lock_guard<std::mutex> lock(mutex);

    lock_guard 함수는 mutex를 인자로 받아서 생성되는데 이 떄 생성자에서 lock 이 된다.
    그리고 lock_guard가 소멸될 때 소멸자에서 mutex를 unlock 하게 된다.

    하지만 이렇게 한다고 해도 무조건 deadlock 일어나지 않는다는 것은 아니다.
*/
#include <iostream>
#include <mutex> // mutex 를 사용하기 위해 필요
#include <thread>
#include <vector>

// mutex를 각 쓰레드에서 사용하기 위해 인자로 전달했다.
void worker(int &result, std::mutex &m)
{
    for (int i = 0; i < 10000; i++)
    {
        m.lock();
        result += 1;
        m.unlock();
    }
}

// mutex 소멸자 이용해서 해제하기
void worker2(int &result, std::mutex &m)
{
    for (int i = 0; i < 10000; i++)
    {
        // lock 생성 시에 m.lock() 을 실행한다고 보면 된다.
        std::lock_guard<std::mutex> lock(m);
        result += 1;

        // scope 를 빠져 나가면 lock 이 소멸되면서
        // m 을 알아서 unlock 한다.
    }
}

int main()
{
    int counter = 0;
    std::mutex m; // 우리의 mutex 객체

    std::vector<std::thread> workers;
    for (int i = 0; i < 4; i++)
    {
        workers.push_back(std::thread(worker, std::ref(counter), std::ref(m)));
    }

    for (int i = 0; i < 4; i++)
    {
        workers[i].join();
    }

    std::cout << "Counter 최종 값 : " << counter << std::endl;

    // mutex 소멸자 이용하기
    counter = 0;
    std::mutex m2; // 우리의 mutex 객체

    std::vector<std::thread> workers2;
    for (int i = 0; i < 4; i++)
    {
        workers2.push_back(std::thread(worker2, std::ref(counter), std::ref(m)));
    }

    for (int i = 0; i < 4; i++)
    {
        workers2[i].join();
    }

    std::cout << "Counter 최종 값 : " << counter << std::endl;
}