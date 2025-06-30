#include "queuedthread.h"



QueuedThread::QueuedThread() { run(); }

QueuedThread::~QueuedThread()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
    }
    _condVar.notify_one();
    //wait for run to finish
    if(_thread.joinable())
    {
        _thread.join();
    }
}

template <typename T, std::enable_if<std::is_invocable_r_v<void, T>>>
void QueuedThread::invoke(T&& task)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _deque.push_front(std::function<void(void)>(std::forward<T>(task)));
    }
    _condVar.notify_one();
}

template <typename T, std::enable_if<std::is_invocable_r_v<void, T>>>
void QueuedThread::schedule(T&& task)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _deque.push_back(std::function<void(void)>(std::forward<T>(task)));
    }
    _condVar.notify_one();
}

void QueuedThread::run()
{
    std::function<void(void)> task;
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);

            _condVar.wait(lock, [this](){
                return _stop || !_deque.empty();
            });

            if(_stop && _deque.empty())
            {
                return;
            }

            task = std::move(_deque.front());
            _deque.pop_front();
        }
        task();
    }
}
