#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include "../../../Utilities/YVRLog.h"

template <typename T>
class TaskQueue
{
  public:
    TaskQueue() : queue(), mutex() {}
    ~TaskQueue() {}

    void Push(const T& data)
    {
        std::unique_lock<std::mutex> lock(mutex);
        queue.push(data);
    }

    bool Pop(T& data)
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue.empty()) return false;
        data = queue.front();
        queue.pop();
        return true;
    }

  private:
    std::queue<T> queue;
    std::mutex mutex;
};