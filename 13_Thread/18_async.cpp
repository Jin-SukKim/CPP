// std::async
/*
    promise나 packaged_task는 비동기적 실행을 하기위해 쓰레드를 명시적으로 생성해 실행해야 된다.
    하지만 std::async를 사용하면 아예 쓰레드를 알아서 만들어
    해당 함수를 비동기적으로 실행하고 return 값을 future에 전달해준다.

    async 함수는 인자로 받은 함수를 비동기적으로 실행한 후, 결과값을 보관할 future를 return한다.
    첫번째 인자로는 어떠한 형태로 실행할지를 전달하는 두가지 값이 가능하다.

    1. std::launch::async - 바로 쓰레드를 생성해 인자로 전달된 함수를 실행
    2. std::launch::deferred - future의 get이 호출됬을 때 실행(새로운 쓰레드를 생성하지 않는다.)

    async는 바로 쓰레드를 생성해 실행하고 deferred는 future의 get 했을떄 동기적으로 실행한다.
    즉 바로 비동기적으로 실행할 필요가 없다면 deferred 옵션을 사용하면 된다.

    get 함수를 통해 async가 return한 결과값을 포함한 future 객체를 future에서 얻을수 있따.

    결론 :
    promise, future, packaged_task, async 를 잘 활용하면 
    귀찮게 mutex 나 condition_variable 을 사용하지 않고도 
    매우 편리하게 비동기적 작업을 수행할 수 있다.

    한 번 발생하는 이벤트에 대해 promise - future 패턴을 이용하면 간단하게 처리가능하다.
    shared_future을 사용해 멀티 쓰레드 관리를 할 수 있다.
    packaged_task를 통해 원하는 함수의 promise - future 패턴을 손쉽게 생성할 수 있다.
    async를 사용해 원하는 함수를 비동기적으로 실행할 수 있다.
*/
#include <future>
#include <iostream>
#include <thread>
#include <vector>

// std::accumulate 와 동일
int sum(const std::vector<int> &v, int start, int end)
{
    int total = 0;
    for (int i = start; i < end; ++i)
    {
        total += v[i];
    }
    return total;
}

/*
    총 2개의 쓰레드가 실행된다.
    1~500까지 합은 async를 통해 생성된 쓰레드에서 더하고
    501~1000까지 합은 원래의 쓰레드에서 처리하낟.

    숫자가 클수록 속도의 차이가 체감될 것이다.
*/
int parallel_sum(const std::vector<int> &v)
{
    // lower_half_future 는 1 ~ 500 까지 비동기적으로 더함
    // 참고로 람다 함수를 사용하면 좀 더 깔끔하게 표현할 수 도 있다.
    // --> std::async([&v]() { return sum(v, 0, v.size() / 2); });
    std::future<int> lower_half_future =
        std::async(std::launch::async, sum, cref(v), 0, v.size() / 2);

    // upper_half 는 501 부터 1000 까지 더함
    int upper_half = sum(v, v.size() / 2, v.size());

    return lower_half_future.get() + upper_half;
}

// 동기적 비동기적 실행 시간 차이 확인해보기
int do_work(int x)
{
    // x 를 가지고 무슨 일을 한다.
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return x;
}

// 실행하는데 3초가 걸리는 do_work 함수를 비동기적으로 실행해 3개의 do_work함수를 실행해도 3초가 걸린다.
void do_work_parallel()
{
    auto f1 = std::async([]() { do_work(3); });
    auto f2 = std::async([]() { do_work(3); });
    do_work(3);

    f1.get();
    f2.get();
}

// 동기적으로 do_work 함수를 실행해서 총 9초가 걸린다.
void do_work_sequential()
{
    do_work(3);
    do_work(3);
    do_work(3);
}

int main()
{
    std::vector<int> v;
    v.reserve(1000);

    for (int i = 0; i < 1000; ++i)
    {
        v.push_back(i + 1);
    }

    std::cout << "1 부터 1000 까지의 합 : " << parallel_sum(v) << std::endl;

    // 시간 차이
    do_work_parallel();
    do_work_sequential();
}