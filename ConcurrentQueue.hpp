#pragma once
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>

template <typename T>
struct concurrent_queue {

    concurrent_queue() : max_size(100) {}

    bool try_push(T&& value) {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue.size() == max_size)
            return false;
        queue.push_back(std::move(value));
        lock.unlock();
        cv_empty.notify_one();
        return true;
    }

    bool try_push(T const& value) {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue.size() == max_size)
            return false;
        queue.push_back(value);
        lock.unlock();
        cv_empty.notify_one();
        return true;
    }

    void push(T&& value) {
        std::unique_lock<std::mutex> lock(mutex);
        cv_full.wait(lock, [&]
            {
                return queue.size() != max_size;
            });
        queue.push_back(std::move(value));
        lock.unlock();
        cv_empty.notify_one();
    }

    void push(T const& value) {
        std::unique_lock<std::mutex> lock(mutex);
        cv_full.wait(lock, [&]
            {
                return queue.size() != max_size;
            });
        queue.push_back(std::ref(value));
        lock.unlock();
        cv_empty.notify_one();
    }

    std::optional<T> try_pop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (queue.empty())
            return std::nullopt;
        T result = queue.front();
        queue.pop_front();
        lock.unlock();
        cv_full.notify_one();
        return result;
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        cv_empty.wait(lock, [&]
            {
                return !queue.empty();
            });
        T result = queue.front();
        queue.pop_front();
        lock.unlock();
        cv_full.notify_one();
        return result;
    }

private:
    mutable std::mutex mutex;
    std::condition_variable cv_empty;
    std::condition_variable cv_full;
    std::deque<T> queue;
    size_t max_size = 100;
};