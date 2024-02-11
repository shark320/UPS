#pragma once

#include <condition_variable>
#include <queue>

template <typename T>
class blocking_queue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;

public:
    void push(T const& value){
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        cond_.notify_one();
    }

    T pop(){
        std::unique_lock<std::mutex> lock(mutex_);
        while(queue_.empty()) {
            cond_.wait(lock);
        }
        T value = queue_.front();
        queue_.pop();
        return value;
    }

};




