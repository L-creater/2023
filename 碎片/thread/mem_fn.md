当需要利用[类成员](https://so.csdn.net/so/search?q=类成员&spm=1001.2101.3001.7020)函数( MyClass::thread_func )来创建子线程时，需如下码码：

```
std::thread t(std::mem_fn(&MyClass::thread_func), Object, args..);    
```

例：

```c++
#include "threadpool.h"
#include <thread>

using namespace std;

namespace slave
{

void ThreadPool::init(int thread_num)
{
  for (int i = 0; i < thread_num; i++) {
    //mem_fn
    thread thr(std::mem_fn(&ThreadPool::run), this);
    thr.detach();
  }
}

void ThreadPool::enqueue(ExecuteFunction &&func)
{
  unique_lock<mutex> lck(mutex_);
  func_list_.push_back(func);
  if (func_list_.size() > 0) {
    cond_.notify_all();
  }
}

void ThreadPool::run()
{
  for ( ; ; ) {
    ExecuteFunction func;
    {
      unique_lock<mutex> lck(mutex_);
      while (!func_list_.size())
        cond_.wait(lck);
      func = func_list_.front();
      func_list_.pop_front();
    }
    func();
  }
}

}

```

