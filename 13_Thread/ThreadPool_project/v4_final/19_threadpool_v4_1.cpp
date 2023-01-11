/*
    ThreadPool의 v3버전의 개선점이 있다.
    바호 Thread::EnqueueJob(F f, Args... args)는 인자들의 복사본을 받는다는 것이다.
    불필요한 복사를 하게되므로 perfect forwarding pattern을 사용해 효율적으로 바꿀수 있다.

    즉, EnqueueJob의 인자들을 rvalue reference로 바꿔주면 된다.
    인자들을 다른 함수에 넘길 때에는 EnqueueJob 인자들은 universial reference로 받으므로 forward를 사용한다.
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
        // 불필요한 복사를 막아 효율적으로 메모리를 사용하기 위해 rvalue reference로 인자를 넘긴다.
        template <class F, class... Args>
        std::future<typename std::invoke_result<F, Args...>::type> EnqueueJob(F &&f, Args &&...args);
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
        rvalue reference를 통해 인자를 넘겨 불필요한 복사를 막아준다.
        rvalue reference중에도 보통 여러가지 타입이 올 수 있어 universial reference를 하는 것이다.
        universial reference이므로 다른 함수에 인자들을 넘길때 move보다는 forward를 사용해 제대로 넘겨준다.
    */
    template <class F, class... Args>
    std::future<typename std::invoke_result<F, Args...>::type> ThreadPool::EnqueueJob(F &&f, Args &&...args)
    {
        // stop_all이 설정되면 더이상 작업을 추가하면 안되므로 예외를 던진다.
        if (stop_all)
        {
            throw std::runtime_error("ThreadPool 사용중지");
        }

        // f의 return type을 보관하는 타입
        using return_type = typename std::invoke_result<F, Args...>::type;

        // forward함수로 인자를 전달해준다.
        auto job = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

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