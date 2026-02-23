#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class MessageQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void enqueue(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
        cv.notify_one(); // Notify one waiting thread
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mtx);
        // Block until the queue is not empty (RTOS task waiting)
        cv.wait(lock, [this] { return !queue.empty(); });
        
        // safely pop the value from the queue to prevent exceptions like memory issues
        T value = std::move(queue.front()); // move the value to return
        queue.pop(); // delete from queue after copying to return value
        return value;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};