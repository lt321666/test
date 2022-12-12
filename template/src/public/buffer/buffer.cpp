#include "buffer.h"

static bool g_exit = false;

template<typename T>
Buffer<T>::Buffer(const unsigned int count_max_queue):mutex_queue_(), count_max_queue_(count_max_queue),
			queue_(), cv_queue_not_full(), cv_queue_not_empty(), mtx_pro(), run_thread_() {
    run_thread_ = std::move(std::thread([this]() {
        while(!g_exit) {
            std::unique_lock<std::mutex> lk(mutex_queue_);
            cv_queue_not_empty.wait(lk, [this]()->bool{return ((queue_.size() > 0) || g_exit);});
            if(g_exit) break;
            if(func_.operator bool()) {
                func_(queue_.front());
            }
            queue_.pop();
            cv_queue_not_full.notify_one();
            usleep(1000);
        }
    }));
}

template<typename T>
Buffer<T>::~Buffer() {
    g_exit = true;
    std::unique_lock<std::mutex> lk(mutex_queue_);
    cv_queue_not_empty.notify_one();
    cv_queue_not_full.notify_one();
    run_thread_.join();
    while(!queue_.empty()) {
        queue_.pop();
    }
}

template<typename T>
void Buffer<T>::pushData(const T &value) {
    std::unique_lock<std::mutex> lk(mutex_queue_);
    std::lock_guard<std::mutex> lock(mtx_pro);
    cv_queue_not_full.wait(lk, [this]()->bool{return ((queue_.size() != count_max_queue_) || g_exit);});
    if(g_exit) return;
    queue_->push(value);
    cv_queue_not_empty.notify_one();
}

template<typename T>
void Buffer<T>::registerProcessFunc(std::function<void(const T&)> func) {
    func_ = func;
}