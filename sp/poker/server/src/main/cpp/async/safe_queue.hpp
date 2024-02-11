#pragma once

#include <condition_variable>
#include <queue>

template <typename T>
class safe_queue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;

public:
    void push(T const& value){
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
    }

    T pop(){
        std::unique_lock<std::mutex> lock(mutex_);
        T value = queue_.front();
        queue_.pop();
        return value;
    }

    bool is_empty(){
        std::unique_lock<std::mutex> lock(mutex_);
        return this->queue_.empty();
    }


};




