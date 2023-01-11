/*
    ThreadPool의 v2는 Broken promise 예외가 던져지므로 정삭적으로 작동하지 않는다.
    Broken promise는 promise에 set_value를 하기전 이미 promise의 future 객체가 파괴되었으면 발생하는 예외이다.

    Enqueue 함수의 std::packaged_task<return_type()> job(std::bind(f, args...)) 에서 job은 지역 변수이다.
    즉, Enqueue 함수가 return하면 파괴되는 객체인 것이다.
    [&job] () {job();}안에서 job에 접근했을 때는 이미 그 객체는 파괴되고 없어진 것이다.

    해결방안은 두가지가 있다.
    1. packaged_task를 따로 컨테이너에 보관한다.
    2. shared_ptr에 packaged_task를 보관한다.

    1번 방식은 packaged_task를 더이상 사용하지 않아도 컨테이너가 남아있다는 문제가 있다.
    하지만 2번은 packaged_task를 사용하지 않을땐 알아서 shared_ptr가 객체를 소멸시켜주므로 훤씬 관리하기 편하다(스마트포인터).

*/
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <chrono>
#include <type_traits>
#include <future>

namespace ThreadPool
{
    class ThreadPool
    {
        // worker 쓰레드 총 개수
        size_t num_threads_;
        // worker 쓰레드 보관 벡터
        std::vector<std::thread> worker_threads_;
        // 할일을 보관하는 queue
        std::queue<std::function<void()>> jobs_;
        // jobs queue를 위한 cv와 m
        std::condition_variable cv_job_q_;
        std::mutex m_job_q_;

        // 모든 쓰레드 종료
        bool stop_all;

        // Worker 쓰레드
        void WorkerThread();

    public:
        ThreadPool(size_t num_threads);
        ~ThreadPool();

        // 임의의 형태의 함수를 받고, 함수의 return값을 보관하는 future를 return
        /*
            class...은 가변 길이의 템플릿으로 임의의 길이의 인자들을 받는다.
            EnqueueJob은 전달받은 함수 F의 return값을 가지는 future를 retunr해야한다.
            함수 F의 return값은 std::result_of를 사용하면 알 수 있다.
            즉 typename std::result_of<F(Args...)>::type은 f 의 리턴값이다.
            typename은 std::result_of<F(Args...)>::type이 멤버변수나 다른게 아닌 type이라고 말해주는 것이다.
            std::future<f의 return 타입> EnqueueJob(F f, Args... args)이다.

            std::result_of는 C++ 17이후로 사용되지 않고 invoke_result로 대체되었다.
            #include <type_traits>로 불러올 수 있다.
            invoke_result<Callable, Args...> 이다.
        */
        template <class F, class... Args>
        std::future<typename std::invoke_result<F, Args...>::type> EnqueueJob(F f, Args... args);
    };

    // 처음 객체생성 시 원하는 Thread 수만큼 worker 쓰레드 생성
    ThreadPool::ThreadPool(size_t num_threads) : num_threads_(num_threads), stop_all(false)
    {
        worker_threads_.reserve(num_threads_);
        for (size_t i = 0; i < num_threads_; ++i)
        {
            worker_threads_.emplace_back([this]()
                                         { this->WorkerThread(); });
        }
    }

    // Worker 쓰레드
    void ThreadPool::WorkerThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_job_q_);

            // 일이 들어오거나 모든 작업이 끝날때까지 대기
            cv_job_q_.wait(lock, [this]()
                           { return !this->jobs_.empty() || stop_all; });

            if (stop_all && this->jobs_.empty())
            {
                lock.unlock();
                return;
            }

            // 맨 앞 작업부터 시작
            std::function<void()> job = std::move(jobs_.front());
            jobs_.pop();
            lock.unlock();

            // 해당 작업 수행
            job();
        }
    }

    // 소멸자
    ThreadPool::~ThreadPool()
    {
        // 모든 작업 그만하기
        stop_all = true;
        cv_job_q_.notify_all();

        for (auto &t : worker_threads_)
        {
            t.join();
        }
    }

    /*
        임의의 함수와 원소들을 받아서 컨테이너에 그냥 저장할 수는 없고 void()형태로 바꿔줘야 넣을 수 있다.
        lambda를 이용해 void()로 바꿔서 넣어준다.
        jobs_.push([f, args...] () {f(args...);});
        worker쓰레드 안에서 f(args...)를 실행할 수 있다.
        하지만 f(args...)의 return값을 얻을 수 없다는 단점이 있다.

        비동기적으로 실행되는 함수의 return값을 얻기 위해선 promise - future을 사용하면 되는데
        packaged_task를 이용하면 훨씬 간편하다.
    */
    template <class F, class... Args>
    std::future<typename std::invoke_result<F, Args...>::type> ThreadPool::EnqueueJob(F f, Args... args)
    {
        // stop_all이 설정되면 더이상 작업을 추가하면 안되므로 예외를 던진다.
        if (stop_all)
        {
            throw std::runtime_error("ThreadPool 사용중지");
        }

        // f의 return type을 보관하는 타입
        using return_type = typename std::invoke_result<F, Args...>::type;

        /*
            make_shared를 통해 shared_ptr을 생성하였다.
            f의 실행결과를 저장하는 promise
            실제 job을 수행하기 위해 job(args...)와 같이 호출하거나 f에 인자들을 bind시켜준다.
        */
        auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(f, args...));

        // job의 실행결과를 보관하는 future
        std::future<return_type> job_result_future = job->get_future();

        std::lock_guard<std::mutex> lock(m_job_q_);
        /*
            job을 실행, f의 return값이 job_result_future에 들어가고 쓰레드풀 사용자가 접근할 수 있게된다.
            lambda 함수에 shared_ptr의 복사본을 전달해 람다 함수안에서 packaged_task의
            shared_ptr하나를 가지고 있게해줬다.
            따라서 job을 실행할 떄 packaged_task는 없어지지 않고 남아있게 된다.
        */
        jobs_.push([job]()
                   { (*job)(); });
        cv_job_q_.notify_one();

        return job_result_future;
    };
}

int work(int t, int id)
{
    printf("%d start \n", id);
    std::this_thread::sleep_for(std::chrono::seconds(t));
    printf("%d end after %ds\n", id, t);
    return t + id;
}

int main()
{
    ThreadPool::ThreadPool pool(3);

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 10; i++)
    {
        futures.emplace_back(pool.EnqueueJob(work, i % 3 + 1, i));
    }
    for (auto &f : futures)
    {
        printf("result : %d \n", f.get());
    }
}