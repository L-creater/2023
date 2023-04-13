#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;   //对任务对列进行加锁解锁
    std::condition_variable cv; // 解决---当任务队列为空时CPU不应该不断轮询耗费CPU资源
    bool stop;
public:
   ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    // void add(std::function<void()>);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
};

//不能放在cpp文件， 原因是c++不支持模板的分离编译
template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename
std::result_of<F(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task](){(*task)();});
    }   
    cv.notify_one();
    return res;

}