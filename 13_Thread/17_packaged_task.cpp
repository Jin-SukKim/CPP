// packed_task
/*
    promise-future 패턴을 비동기적 함수(정확히는 Callable - 즉 람다 함수, Functor 포함)의
    리턴값에 적용할 수 있는 packaged_task를 지원한다.

    package_task에 전달된 함수에 return할 때 그 return 값을 promise에 set_value하고
    만약에 예외를 던졌다면 promise의 set_exception을 한다.
    해당 future는 packaged_task가 return하는 future에서 접근할 수 있다.

    std::packaged_task<int(int)> task(some_task);
    packaged_task는 비동기적으로 수행할 함수 자체를 생성자의 인자로 받는다.
    템플릿 인자로 해당 함수의 타입을 명시해야 된다.
    packaged_task는 전달된 함수를 실행해 그 함수의 return 값을 promise에 설정한다.

    std::future<int> start = task.get_future();
    해당 promise에 대응되는 future는 get_future 함수로 얻을 수 있다.

    thread t(std::move(task), 5);
    생성된 packaged_task를 쓰레드에 전달한다. (packaged_task는 복사 생성이 불가능하다.)
    명시적으로 move해준다.

    비동기적으로 실행된 함수의 결과는 future의 get으로 받을 수 있다.
    packaged_task를 사용하면 쓰레드에 굳이 promise를 전달하지 않고 알아서 함수의 return 값을 처리해줘서 편리하다.

*/
#include <future>
#include <iostream>
#include <thread>

int some_task(int x) { return 10 + x; }

int main()
{
    // int(int) : int 를 리턴하고 인자로 int 를 받는 함수. (std::function 참조)
    std::packaged_task<int(int)> task(some_task);

    std::future<int> start = task.get_future();

    std::thread t(std::move(task), 5);

    std::cout << "결과값 : " << start.get() << std::endl;
    t.join();
}