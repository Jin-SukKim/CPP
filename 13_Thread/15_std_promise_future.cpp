// std::promise 와 std::future
/*
    결국 비동기적으로 실행하고 싶은 일은 어떠한 데이터를 다른 쓰레드를 통해 처리하는 일이다.

    어떤 쓰레드 T를 이용해 비동기적으로 값을 받아내겠다는 의미로,
    미래에(future) 쓰레드 T가 원하는 데이터를 돌려 주겠다 라는 약속(promise)라고 볼 수 있다.

    promise는 복사가 불가능하다.
    
    기초적인 promise - future 사용법.

    std::promise<string> p;
    promise 객체를 정의할 때 템플릿 인자로 연산 수행 후 돌려받을 타입을 넣어준다.

    std::future<string> data = p.get_future();
    연산이 끝난 다음 promise 객체는 자신이 가진 future 객체에 값을 넣는다.
    이때 promise에 대응되는 future 객체는 get_future을 통해 얻을 수 있다.
    하지만 data가 아직은 실제 연산이 끝난 값을 가지는 게 ㅏ니다.

    data가 실제 연산한 값을 가지기 위해서는
    p->set_value("some data");
    promise 객체가 자신의 future 객체에 데이터를 제공한 이후

    data.get()
    대응되는 future 객체의 get 함수를 통해 얻을 수 있다.

    data.wait();
    promise가 future에 값을 전달하기 전까지 wait 함수가 기다린다.
    wait 함수가 return했다면 get을 통해 future에 전달된 객체를 얻을 수 있다.

    굳이 wait 함수를 따로 호출할 필요는 없는데 get 함수를 호출해도 알아서 promise가 future에
    객체를 전달하기 전까지 기다린 다음에 return하기 때문이다.

    future에서 get을 호출하면 설정된 객체가 이동되기 때문에 get을 두 번 호출하면 안된다.
    (worker 함수)

    condition_variable을 사용해 promise - future 패턴 사용하기.

    결국 promise는 producer - consumer 패턴에서 producer 역할을 수행하고 future는 consumer 역할을 수행하는 것과 같다.
    따라서 condition_variable을 통해 promise - future 패턴을 구현할 수 있다.(worker2 함수)

    하지만, promise와 future을 이용하는 것이 더 깔끔하고 이해하기 슆다.
    condition_variable을 이용하는 것보다 장점은 future에 예외도 전달할 수 있기 때문에다.

    promise->set_exception() 함수는 예외 객체를 받지 않고 exception_ptr을 받는다.
    이 exception_ptr은 catch로 받은 예외 객체의 포인터가 아닌 현재 catch된 예외에 관한
    정보를 반환하는 current_exception 함수가 return하는 객체이다.

    catch로 받은 예외를 make_exception_ptr함수를 호출해 exception_ptr로 만들수도 있지만 current_exception이 더 편하다.

    future에 전달된 예외 객체는 get 함수를 호출했을 때 전달된 예외 객체가 불려지고 try - catch문을 사용한 것 처럼 예외처리를 할 수 있다.

    wait_for 함수 사용하기.

    wait() 함수는 promise가 future에 전달할 때까지 계속 기다리지만
    wait_for() 함수를 사용하면 정해진 시간 동안만 기다린다.

    wait_for 함수는 promise가 설정될 때까지 기다리는 대신 wait_for 함수에 전달된 시간만큼 기다렸다가 return한다.
    이때 return하는 값은 현재 future의 상태를 나타내는 future_status객체이다.

    future_status는 총 3가지 상태가 있다.
    1. future에 값이 설정됬을 때 - future_status::ready
    2. 지정된 시간이 지났지만 값이 설정되지 않았을 때 - future_status::timeout
    3. 결과값을 계산하는 함수가 채 실행되지 않았을 때 - future_status::deferred
*/
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>
using std::string;

std::condition_variable cv;
std::mutex m;
bool done = false;
std::string info;

void worker(std::promise<string> *p)
{
    // 약속을 이행하는 모습. 해당 결과는 future 에 들어간다.
    p->set_value("some data");
}

void worker2()
{
    {
        std::lock_guard<std::mutex> lk(m);
        info = "some data"; // 위의 p->set_value("some data") 에 대응
        done = true;
    }
    cv.notify_all();
}

void worker3(std::promise<string> *p)
{
    try
    {
        throw std::runtime_error("Some Error!");
    }
    catch (...)
    {
        // set_exception 에는 exception_ptr 를 전달해야 한다.
        p->set_exception(std::current_exception());
    }
}

void worker4(std::promise<void> *p)
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    p->set_value();
}

int main()
{
    // 기초적인 promise - future 사용법
    // 연산 후 돌려줄 객체 타입을 템플릿 인자로 받는다.
    std::promise<string> p;

    // 미래에 string 데이터를 돌려 주겠다는 약속.
    std::future<string> data = p.get_future();

    std::thread t(worker, &p);

    // 미래에 약속된 데이터를 받을 때 까지 기다린다.
    data.wait();

    // wait 이 리턴했다는 뜻이 future 에 데이터가 준비되었다는 의미.
    // 참고로 wait 없이 그냥 get 해도 wait 한 것과 같다.
    std::cout << "받은 데이터 : " << data.get() << std::endl;

    t.join();

    // condition_variable을 이용해 promise - future 패턴 만들기
    std::thread t2(worker2);

    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []
            { return done; }); // 위의 data.wait() 이라 보면 된다.
    lk.unlock();

    std::cout << "받은 데이터 : " << info << std::endl;

    t2.join();

    // promise - future을 이용해 예외 전달하기
    std::promise<string> p2;

    // 미래에 string 데이터를 돌려 주겠다는 약속.
    std::future<string> data2 = p2.get_future();

    std::thread t3(worker3, &p2);

    // 미래에 약속된 데이터를 받을 때 까지 기다린다.
    data2.wait();

    try
    {
        data2.get();
    }
    catch (const std::exception &e)
    {
        std::cout << "예외 : " << e.what() << std::endl;
    }
    t3.join();

    // wait_for 함수 사용하기
    // void 의 경우 어떠한 객체도 전달하지 않지만, future 가 set 이 되었냐
    // 안되었느냐의 유무로 마치 플래그의 역할을 수행할 수 있습니다.
    std::promise<void> p3;

    // 미래에 string 데이터를 돌려 주겠다는 약속.
    std::future<void> data3 = p3.get_future();

    std::thread t4(worker4, &p3);

    // 미래에 약속된 데이터를 받을 때 까지 기다린다.
    while (true)
    {
        std::future_status status = data3.wait_for(std::chrono::seconds(1));

        // 아직 준비가 안됨
        if (status == std::future_status::timeout)
        {
            std::cerr << ">";
        }
        // promise 가 future 를 설정함.
        else if (status == std::future_status::ready)
        {
            break;
        }
    }
    t.join();
}