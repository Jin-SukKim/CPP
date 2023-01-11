// shared_future
/*
    future는 get을 호출하면 future 내부의 객체가 이동되 딱 한 번만 get을 할 수 있지만
    여러 개의 다른 쓰레드에서 future을 get할 필요성이 있을 때도 있다.

    이 경우 shared_future을 사용하면된다.

    shared_future은 future와 다르게 복사가 가능하고 복사본들이 모두 같은 객체를 공유하게 된다.
    따라서 reference나 pointer로 전달할 필요가 없다.
*/
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
using std::thread;

void runner(std::shared_future<void> start)
{
    start.get();
    std::cout << "출발!" << std::endl;
}

int main()
{
    std::promise<void> p;
    std::shared_future<void> start = p.get_future();

    thread t1(runner, start);
    thread t2(runner, start);
    thread t3(runner, start);
    thread t4(runner, start);

    // 참고로 cerr 는 std::cout 과는 다르게 버퍼를 사용하지 않기 때문에 터미널에
    // 바로 출력된다.
    std::cerr << "준비...";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cerr << "땅!" << std::endl;

    p.set_value();

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}