#ifndef QUEUEDTHREAD_H
#define QUEUEDTHREAD_H

#include <functional>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

class QueuedThread
{
public:
    QueuedThread();
    ~QueuedThread();

    template <typename T>
    requires std::is_invocable_r_v<void, T>
    void invoke(T&& task)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _deque.push_front(std::function<void(void)>(std::forward<T>(task)));
        }
        _condVar.notify_one();
    }
    template <typename T>
    requires std::is_invocable_r_v<void, T>
    void schedule(T&& task)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _deque.push_back(std::function<void(void)>(std::forward<T>(task)));
        }
        _condVar.notify_one();
    }

private:
    std::thread _thread;
    std::deque<std::function<void(void)>> _deque;
    std::mutex _mutex;
    std::condition_variable _condVar;
    bool _stop = false;

    void run();
};

#endif // QUEUEDTHREAD_H
