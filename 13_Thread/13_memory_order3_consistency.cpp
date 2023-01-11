// 순차적 일관성(sequential consistency)
/*
    memory_order_seq_cst는 메모리 명령의 순차적 일관성(sequential consistency)을 보장해준다.

    순차적 일관성(sequential consistency)이란 메모리 명령 재배치도 없고, 모든 쓰레드에서
    모든 시점에 동일한 값을 관찰할 수 있는 보통 생각하는 CPU의 작동방식이다.

    memory_order_seq_cst 를 사용하는 메모리 명령들 사이에선 이러한 순차적 일관성을 보장해준다.

    밑의 예재는 write_x와 read_x_then_y의 동기화, write_y와 read_y_then_x의 동기화가 이루어진다. (release - acquire)
    하지만 이건 read_x_then_y와 read_y_then_x 두 쓰레드가 같은 순서로 x.store와 y.sotre를 관찰한다는
    보장이 없다.
    어떤 store가 먼저 일어날지 모른다는 것이다.
    결국 두 if문 속 load가 false가 되 z가 0이 될 수도 있다는 점이다.
    (즉 쓰레드1에서 먼저 write이 일어나 값이 바뀌었는데 쓰레드2인 다른 코어에서는 관찰할 수 없어 나오는 문제이다.
    동기화는 write_x와 read_x또는 wrtie_y와 read_y사이에만 일어날 뿐 서로 다른 쓰레드끼리 일어나는 게 아니기 때문이다.)
    memory_order_seq_cst를 사용하면 해당 명령을 사용하는 메모리 연산들끼리는
    모든 쓰레드에서 동일한 연산 순서를 관찰할 수 있도록 보장해준다.

    참고로 atomic 객체는 defualt로 memory_order_seq_cst를 사용하고 있다.
    (counter ++ 은 사실 counter.fetch_add(1, memory_order_seq_cst)와 같다.)

    문제는 멀티 코어 시스템에서 memory_order_seq_cst 가 꽤나 비싼 연산이라는 것이다.
    인텔과 AMD는 순차적 일관성이 보장되 큰 차이가 없지만 ARM 계열의 CPU는 순차적 일관성이 보장되지 않아
    CPU의 동기화 비용이 크다.

    따라서 해당 명령은 꼭 필요한 상황에서만 사용해 주는 것이 좋다.

    memory_order_seq_cst를 사용하면 결과가 z : 1, z : 2만 나올 수 있고 0은 나오지 않는다.
    x.store 와 y.store 가 모두 memory_order_seq_cst 이므로, 
    read_x_then_y 와 read_y_then_x 에서 관찰했을 때 x.store 와 y.store 가 같은 순서로 발생해야된다.
    그래서 0은 나올 수 없다.
    (single total modification을 보장한다는 의미로 어떤 메모리 값이 변했다면 모든 코어에서 같은 값을 읽는게
    보장되므로 if 문에서 전부 fail이 일어나지 않아 0이란 값은 나올 수 없다.)

    최종 정리
            연산                        허용된 memory order

        쓰기 (store)        memory_order_relaxed, memory_order_release, memory_order_seq_cst

        읽기 (load)         memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst

        읽고 - 수정하고      memory_order_relaxed, memory_order_consume, 
        - 쓰기              memory_order_acquire, memory_order_release,
        (read - modify      memory_order_acq_rel, memory_order_seq_cst
        - write)

     
*/
#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> x(false);
std::atomic<bool> y(false);
std::atomic<int> z(0);

// z는 0, 1, 2 셋 중 한 결과를 만들어 낸다.
void write_x() { x.store(true, std::memory_order_release); }

void write_y() { y.store(true, std::memory_order_release); }

void read_x_then_y()
{
    while (!x.load(std::memory_order_acquire))
    {
    }
    if (y.load(std::memory_order_acquire))
    {
        ++z;
    }
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_acquire))
    {
    }
    if (x.load(std::memory_order_acquire))
    {
        ++z;
    }
}

void write_x_seq() { x.store(true, memory_order_seq_cst); }

void write_y_seq() { y.store(true, memory_order_seq_cst); }

void read_x_then_y_seq()
{
    while (!x.load(memory_order_seq_cst))
    {
    }
    if (y.load(memory_order_seq_cst))
    {
        ++z;
    }
}

void read_y_then_x_seq()
{
    while (!y.load(memory_order_seq_cst))
    {
    }
    if (x.load(memory_order_seq_cst))
    {
        ++z;
    }
}

int main()
{
    thread a(write_x);
    thread b(write_y);
    thread c(read_x_then_y);
    thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    std::cout << "z : " << z << std::endl;

    thread a(write_x_seq);
    thread b(write_y_seq);
    thread c(read_x_then_y_seq);
    thread d(read_y_then_x_seq);
    a.join();
    b.join();
    c.join();
    d.join();
    std::cout << "z : " << z << std::endl;
}