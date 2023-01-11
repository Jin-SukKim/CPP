// memory_order
/*
    atomic 객체들의 경우 원자적 연산 시에 메모리에 접근할 때 어떠한 방식으로 접근하는지 지정할 수 있다.

    memory_order_relaxed는 가장 느슨한 조건이다.
    즉, memory_order_relaxed 방식으로 메모리에서 읽거나 쓰는 경우, 주위의 다른 메모리 접근들과 순서가 바뀌어도 무방하다.

    아래 예제는 컴파일 할 경우 x : 1, y : 0 또는  x : 0, y : 1 그리고 x : 1, y : 1 셋 중 하나의 결과를 볼 수 있다.
    store과 load는 atomic 객체들에 대해서 원자적으로 쓰기와 읽기를 지원해주는 함수이다.
    이때 추가적인 인자로 어떤 형태로 memory_order을 지정할지 전달할 수 있다.
    이번엔 가장 느슨한 조건인 memory_order_relaxed로 지정했다.

    하지만 과연 x : 0, y : 0의 결과는 나올 수 있을까?
    x와 y 둘다 0이 나오기 위해서는 a와 b가 1이 되기전에 0이 먼저 출력되야 한다는 것이다.
    아래 코드를 그냥 따라가면 상식적으로 불가능하지만 memory_order_relaxed는 메모리 연산들 사이에서
    어떠한 제약조건이 없기 때문에 서로 다른 변수의 relaxed 메모리 연산은 CPU 마음대로 재배치할 수 있다.
    (단일 쓰레드 관점에서 결과가 동일하다면.)

    ex)
        int x = a->load(memory_order_relaxed);  // x = a (읽기)
        b->store(1, memory_order_relaxed);      // b = 1 (쓰기)
        순으로 CPU가 순서를 재배치해 실행해도 무방하다.

    이렇게 되면 x와 y에 모두 0을 넣을 수 있다.
    memory_order_relaxed 는 CPU 에서 메모리 연산 순서에 관련해서 무한한 자유를 주는 것과 같다.
    결국 CPU에서 매우 빠른 속도로 실행할 수 있게 된다.

    relaxed 연산을 활용하면 예상치 못한 결과를 낼 수 있지만 종종 사욜할 수 있는 상황도 있다.
    (worker 함수)

    memory_order_relaxed는 너무 많은 자유를 CPU에게 주기 때문에 사용이 제한적이라
    memory_order_acquire과 memory_order_release를 사용한다.
    (producer - consumer 패턴 함수들)

    relaxed는 producer - consumer 패턴에서 사용하면 안된다.

    memory_order_release는 해당 명령 이전의 모든 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지한다.
    만약에 같은 변수를 memory_order_acquire으로 읽는 쓰레드가 있다면, memory_order_release 이전에 오는
    모든 메모리 명령들이 해당 쓰레드에 의해서 관찰될 수 있어야 한다.

    memory_order_acquire는 release와 반대로 해당 명령 뒤에 오는 모든 메모리 명령들이 해당 명령 위로 재배치되는 것을 금지한다.

    이와 같이 두 개의 쓰레드들이 같은 변수를 release와 acquire을 통해 동기화(synchronize)를 수행할 수 있다. 
*/
#include <atomic>
#include <cstdio>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

void t1(std::atomic<int> *a, std::atomic<int> *b)
{
    b->store(1, memory_order_relaxed);     // b = 1 (쓰기)
    int x = a->load(memory_order_relaxed); // x = a (읽기)

    printf("x : %d \n", x);
}

void t2(std::atomic<int> *a, std::atomic<int> *b)
{
    a->store(1, memory_order_relaxed);     // a = 1 (쓰기)
    int y = b->load(memory_order_relaxed); // y = b (읽기)

    printf("y : %d \n", y);
}

// memory_order_relaxed 활용
/*
    counter++와 동일한 작업을 하지만 fetch_add로 메모리 접근 방식을 설정했다.
    원자적으로 counter 값을 읽고 1을 더하고 결과를 써준다.

    memory_order_relaxed를 사용할 수 있었던 이유는 다른 메모리 연산들 사이에서
    굳이 counter를 증가시키는 걸 재배치 못하게 막을 필요가 없기 때문이다.
    다른 메모리보다 +1을 늦게 한다고 해도 결과적으로 증가되기만 하면 문제 될게 없기 때문이다.
*/
void worker(std::atomic<int> *counter)
{
    for (int i = 0; i < 10000; i++)
    {
        // 다른 연산들 수행

        counter->fetch_add(1, memory_order_relaxed);
    }
}

/*
    is_ready에 쓰는 연산이 memory_order_relaxed이므로 *data = 10 과 순서가 바뀌어서
    실행되면 is_ready가 true임에도 *data = 10이 실행안되고 끝날 수도 있다.
    결국 consumer 쓰레드에서 is_ready가 true가 되었음에도 제대로된 data를 읽을 수 없는 상황이 발생한다.
*/
void producer(std::atomic<bool> *is_ready, int *data)
{
    *data = 10;
    is_ready->store(true, memory_order_relaxed);
}

/*
    relaxed이기 때문에 data를 읽는 부분과 while 문이 읽히는 순서가 바뀌면 is_ready가 true가 되기전에
    data 값을 읽어버릴 수도 있다.
    따라서 producer - consumer 패턴에선 relaxed를 사용하지 못한다.
*/
void consumer(std::atomic<bool> *is_ready, int *data)
{
    // data 가 준비될 때 까지 기다린다.
    while (!is_ready->load(memory_order_relaxed))
    {
    }

    // 일반적으로 Data : 10이 나오지만 Data : 0도 나올 수 있다.
    std::cout << "Data : " << *data << std::endl;
}

// 올바른 producer - consumer 패턴에서 memory order 사용법
/*
    release를 사용해 is_ready->store(true, std::memory_order_release); 밑으로 *data = 10;은 올 수 없다
*/
void producer2(std::atomic<bool> *is_ready, int *data)
{
    *data = 10;
    is_ready->store(true, std::memory_order_release);
}

/*
    is_ready가 true가 되면 memory_order_acquire로 is_ready를 읽는 쓰레드에서
    data의 값을 확인 했을 때 10임을 관찰할 수 있다.
*/
void consumer2(std::atomic<bool> *is_ready, int *data)
{
    // data 가 준비될 때 까지 기다린다.
    while (!is_ready->load(std::memory_order_acquire))
    {
    }

    // 무조건 Data : 10이 나오고 0은 나올 수 없다.
    std::cout << "Data : " << *data << std::endl;
}

int main()
{
    std::vector<std::thread> threads;

    std::atomic<int> a(0);
    std::atomic<int> b(0);

    threads.push_back(std::thread(t1, &a, &b));
    threads.push_back(std::thread(t2, &a, &b));

    for (int i = 0; i < 2; i++)
    {
        threads[i].join();
    }

    std::vector<std::thread> threads2;

    std::atomic<int> counter(0);

    for (int i = 0; i < 4; i++)
    {
        threads2.push_back(std::thread(worker, &counter));
    }

    for (int i = 0; i < 4; i++)
    {
        threads2[i].join();
    }

    std::cout << "Counter : " << counter << std::endl;

    std::vector<std::thread> threads3;

    std::atomic<bool> is_ready(false);
    int data = 0;

    threads3.push_back(std::thread(producer, &is_ready, &data));
    threads3.push_back(std::thread(consumer, &is_ready, &data));

    for (int i = 0; i < 2; i++)
    {
        threads3[i].join();
    }

    std::vector<std::thread> threads4;

    std::atomic<bool> is_ready2(false);
    int data2 = 0;

    threads4.push_back(std::thread(producer, &is_ready2, &data2));
    threads4.push_back(std::thread(consumer, &is_ready2, &data2));

    for (int i = 0; i < 2; i++)
    {
        threads4[i].join();
    }
}