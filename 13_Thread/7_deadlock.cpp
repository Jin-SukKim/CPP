// Deadlock
/*
    밑의 worker1과 work2는 lock, unlock 모두 하지만 deadlock이 발생하는 상황이다.

    데드락은 4가지 조건이 있다

    1. 상호배제(Mutual exclusion) : 프로세스들이 필요로 하는 자원에 대해 배타적인 통제권을 요구한다.
    2. 점유대기(Hold and wait) : 프로세스가 할당된 자원을 가진 상태에서 다른 자원을 기다린다.
    3. 비선점(No preemption) : 프로세스가 어떤 자원의 사용을 끝낼 때까지 그 자원을 뺏을 수 없다.
    4. 순환대기(Circular wait) : 각 프로세스는 순환적으로 다음 프로세스가 요구하는 자원을 가지고 있다.

    밑은 worker1에서 m1을 lock하고 worker2에서 m2를 먼저 lock 하는데 이렇게 되면 worker1에서 m2를 lock 하거나
    worker2에서 m1을 lock할 수 없다. worker1에서 m2를 lock하려면 worker2에서 m2를 unlock해야되지만
    work2의 m2의 소멸자를 호출하려면 worker2의 m1을 lock을 먼저 시켜줘야된다.
    즉 데드락의 조건이 달성되고 이도저도 못하는 상황이 발생한다.

    데드락을 해결할 한가지 방법은 한 쓰레드에게 우선권을 주는 것이다.
    한 쓰레드가 다른 쓰레드에 비해 우위를 갖게 된다면, 한 쓰레드만 열심히 일하고
    다른 쓰레드는 일할 수 없는 기아 상태(starvation)가 발생할 수 있지만 데드락은 피할 수 있다.
    물론 완벽한 방법은 아니다.

    try_lock 함수를 사용해 우선권을 줄 수 있는데 이 함수는 mutex를 lock 할 수 있다면
    true를 return하고 아니라면 lock과 달리 기다리지 않고 false를 return한다.

    C++ Concurrency In Action 이란 책에선 데드락 상황을 피하기 위해 가이드라인을 제공하고 있는다.

    1. 중첩된 Lock 을 사용하는 것을 피해라
        - 모든 쓰레드들이 최대 1개의 lock만을 소유한다면 deadlock을 보통 피할 수 있다. (보통 lock은 1개면 충분하다.)

    2. Lock 을 소유하고 있을 때 유저 코드를 호출하는 것을 피해라
        - 유저 코드에서 lock을 소유할 수 있기에 중첩된 lock 얻기를 피하기 위해 유저 코드 호출을 지양한다.

    3. Lock 들을 언제나 정해진 순서로 획득해라
        - lock을 여러개를 소유하게 되는 상황이라면 반드시 lock들을 정해진 순서대로 획득해야된다.
        ex) worker1과 worker2에서 deadlock 발생한 이유는 worker1에선 m1을 worker2에선 m2를 각각 먼저 호출했기 때문인데
            만약 worker1과 worker2에서 모드 같은 mutext(m1이나 m2)를 먼저 lock했다면 deadlock은 발생하지 않았을 것이다.
*/

// deadlock 발생하는 상황
#include <iostream>
#include <mutex> // mutex 를 사용하기 위해 필요
#include <thread>

// deadlock 발생
void worker1(std::mutex &m1, std::mutex &m2)
{
    for (int i = 0; i < 10000; i++)
    {
        std::lock_guard<std::mutex> lock1(m1);
        std::lock_guard<std::mutex> lock2(m2);
        // Do something
    }
}

void worker2(std::mutex &m1, std::mutex &m2)
{
    for (int i = 0; i < 10000; i++)
    {
        std::lock_guard<std::mutex> lock2(m2);
        std::lock_guard<std::mutex> lock1(m1);
        // Do something
    }
}

// deadlock 해결법 중 한개, 우선권 주기
// worker3이 mutex를 가지고 경쟁할 때 우선권을 가진다
void worker3(std::mutex &m1, std::mutex &m2)
{
    for (int i = 0; i < 10; i++)
    {   
        // lock_guard와 같다.
        m1.lock();
        m2.lock();
        std::cout << "Worker1 Hi! " << i << std::endl;

        m2.unlock();
        m1.unlock();
    }
}

// worker3가 우선권을 가지고 있다.
void worker4(std::mutex &m1, std::mutex &m2)
{
    for (int i = 0; i < 10; i++)
    {
        while (true)
        {
            m2.lock();

            // m1 이 이미 lock 되어 있다면 "야 차 빼" 를 수행하게 된다.
            // m1이 lock되어 있다면 worker3이 이미 lock을 수행하고 있는 것이므로 worker3과 4의 우선권 순위에 따라
            // worker4의 m2를 unlock해 worker3이 먼저 수행될 수 있게 해준다.
            if (!m1.try_lock())
            {
                m2.unlock();
                continue;
            }

            std::cout << "Worker2 Hi! " << i << std::endl;
            m1.unlock();
            m2.unlock();
            break;
        }
    }
}

int main()
{
    int counter = 0;
    std::mutex m1, m2; // 우리의 mutex 객체

    std::thread t1(worker1, std::ref(m1), std::ref(m2));
    std::thread t2(worker2, std::ref(m1), std::ref(m2));

    t1.join();
    t2.join();

    std::cout << "끝!" << std::endl;

    std::mutex m3, m4; // 우리의 mutex 객체

    std::thread t3(worker3, std::ref(m3), std::ref(m4));
    std::thread t4(worker4, std::ref(m3), std::ref(m4));

    t1.join();
    t2.join();

    std::cout << "끝!" << std::endl;
}