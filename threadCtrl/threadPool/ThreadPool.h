//
// Created by Chunel on 2020/6/20.
// 写一个线程池，支持上层异步调用逻辑
//

#ifndef CAISS_THREADPOOL_H
#define CAISS_THREADPOOL_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

#include "ThreadPoolDefine.h"

using namespace std;

class ThreadPool {

public:
    explicit ThreadPool(unsigned int num) :
    thread_num_(num),
    running_(false) {
    }

    ~ThreadPool() {
        if (running_) {
            this->stop();
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator= (const ThreadPool& pool) = delete;

    void start();
    void stop();
    void appendTask(const ThreadTaskInfo& task);

protected:
    void work();

private:
    atomic_bool running_;
    mutex pool_mtx_;
    condition_variable cond_;
    unsigned int thread_num_;
    vector<thread> threads_;    // 线程数组
    queue<ThreadTaskInfo> tasks_;    // 任务队列
    RWLock func_lock_;    // 用于执行函数的全局锁
};


#endif //CAISS_THREADPOOL_H
