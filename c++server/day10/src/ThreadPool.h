#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;   //对任务对列进行加锁解锁
    std::condition_variable cv; // 解决---当任务队列为空时CPU不应该不断轮询耗费CPU资源
    bool stop;
public:
    ThreadPool(int size = 2);
    ~ThreadPool();

    void add(std::function<void()>);
};