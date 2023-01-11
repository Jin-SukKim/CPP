// 쓰레드풀(ThreadPool) - 쓰레드들의 직업 소개소, reference by https://github.com/progschj/ThreadPool, https://modoocode.com/285
/*
    쓰레드풀은 쓰레드가 일감을 받아 작업을 수행한 후 다시 돌아와 대기하는 프로그램이다.
    사용자는 원하는 만큼 쓰레드들을 생성해 놓고 무언가를 수행하고 싶다면 그냥 쓰레드풀에 추가하면 된다.

    처리해야할 작업들은 queue에 추가한다(linkedlist와 비슷하다.)
    queue는 pop을 하면 맨 처음 원소를 제거하지만 return하지는 않는다.
    front 함수를 사용해 맨 처음 원소에 접근해야된다.

    queue는 가장 처음 온 작업부터 순서대로 처리하는 방식인데 때로는 가장 최근에 추가된
    작업 요청을 먼저 처리해야 될수도 있다.
    이 경우 다른 자료구조를 사용하면 된다(deque 등)

    쓰레드풀에서 돌아가는 쓰레드를 worker라고 임의로 부른다.

*/
#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <chrono>

namespace ThreadPool
{
    class ThreadPool
    {
        // 총 worker 쓰레드의 개수(worker_threads_.size()와 같을 것이다.)
        size_t num_threads_;
        // worker 쓰레드를 보관하는 vector
        std::vector<std::thread> worker_threads_;
        // 할일을 보관하는 job 큐 (C++은 보통 함수 포인터를 저장하는 컨테이너가 없어 void 타입의 queue를 만들었다.)
        // 모든 작업 쓰레드들이 접근가능한 queue
        std::queue<std::function<void()>> jobs_;

        // race condition 방지와 producer - consumer 패턴 만들기 위해 필요한 것들
        std::condition_variable cv_job_q_;
        std::mutex m_job_q_;

        // 모든 쓰레드 종료
        // worker 쓰레드들은 기본적으로 job_을 처리하는 동안 무한 루프를 도는데 stop_all을 설정해 무한루프 빠져나오기
        bool stop_all;

        // jobs_에 작업이 추가될 때까지 대기하다 작업이 추가되면 받아서 처리한다.
        void WorkerThread();

    public:
        // ThreadPool 생성자, num_threads만큼 threads 생성
        ThreadPool(size_t num_threads);
        
        // 작업을 추가한다.
        void EnqueueJob(std::function<void()> job);

        // 소멸자
        ~ThreadPool();
    };

    /*
        외부함수에서 내부멤버에 접근하기 위해서는 mem_fn으로 감싸거나 람다 함수를 이용해야된다.
    */
    ThreadPool::ThreadPool(size_t num_threads) : num_threads_(num_threads), stop_all(false)
    {
        worker_threads_.reserve(num_threads_);
        for (size_t i = 0; i < num_threads_; ++i)
        {
            // 람다 함수에서 멤버함수에 접근하기 위해 this를 전달
            worker_threads_.emplace_back([this]()
                                         { this->WorkerThread(); });
        }
    }

    void ThreadPool::WorkerThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_job_q_);

            // condition_variable에서 jobs_에 원소가 있거나 stop_all이 설정될 때 까지 기다린다.
            cv_job_q_.wait(lock, [this]()
                           { return !this->jobs_.empty() || stop_all; });
            // 모든 작업들이 설정되어 있고 jobs_에 대기중인 작업이 없을 때 쓰레드를 종료한다.
            if (stop_all && this->jobs_.empty())
            {
                lock.unlock();
                return;
            }

            // job이 있을 때 jobs_의 맨 앞 작업을 가져온다.
            std::function<void()> job = std::move(jobs_.front());
            jobs_.pop();
            lock.unlock();

            // 해당 job을 수행한다.
            job();
        }
    }

    void ThreadPool::EnqueueJob(std::function<void()> job)
    {
        // stop_all이 설정되면 더이상 작업을 추가하면 안되므로 예외를 던진다.
        if (stop_all)
        {
            throw std::runtime_error("ThreadPool 사용중지");
        }
        else
        {
            // 작업을 추가한 뒤, 자고있는 쓰레드를 깨워 다른 일을 할 수 있도록 해준다.
            std::lock_guard<std::mutex> lock(m_job_q_);
            jobs_.push(std::move(job));
        }
        cv_job_q_.notify_one();
    }

    // stop_all을 설정하고 모든 Worker 쓰레드들에 알려줘 join을 해준다.
    ThreadPool::~ThreadPool()
    {
        stop_all = true;
        cv_job_q_.notify_all();

        for (auto &t : worker_threads_)
        {
            t.join();
        }
    }
}

void work(int t, int id)
{
    printf("%d start \n", id);
    std::this_thread::sleep_for(std::chrono::seconds(t));
    printf("%d end after %ds\n", id, t);
}

int main()
{
    ThreadPool::ThreadPool pool(3);

    for (int i = 0; i < 10; i++)
    {
        /*
            ThreadPool이 받는 함수의 형태가 return이 void이고 인자를
            받지 않는다.
            work함수 자체는 int 인자를 받아 전달할 수 없지만
            lambda로 감싸서 전달하면 void()형태로 전달할 수 있다.
        */
        pool.EnqueueJob([i]() { work(i % 3 + 1, i); });
    }
    return 0;
}