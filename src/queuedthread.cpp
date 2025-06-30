#include "queuedthread.h"



QueuedThread::QueuedThread() : _thread(std::thread(&QueuedThread::run, this)) {}

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
