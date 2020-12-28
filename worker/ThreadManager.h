#ifndef __THREAD_MANAGER_H
#define __THREAD_MANAGER_H

#include <mutex>
#include <condition_variable>
#include <vector>
// #include <functional>

#include "BaseWorker.h"
#include "BaseJobQueue.h"

class ThreadManager
{
public:
    ThreadManager(int threads_num = 3);
    ~ThreadManager();

    bool enqueue(BaseJob& job);

    void dispatch();
    bool notify();

private:
    std::vector<BaseWorker> workers;
    BaseJobQueue jobs;
    unsigned int mTasksSize;

    std::mutex mMutex;
    std::condition_variable mCondition;
    bool stoped;
};

ThreadManager::ThreadManager(int threads_num)
    :stoped(false)
{
    for (int i = 0; i < threads_num;  ++i) {
        // workers.emplace_back([this]{
        //     while(1)
        //     {
        //         BaseJob job;
        //         {
        //             std::unique_lock<std::mutex> lock(this->mMutex);
        //             this->mCondition.wait(lock, [this](){return this->stoped;});

        //             job = std::move(this->jobs.mJobQueue.front());
        //             this->jobs.mJobQueue.pop_front();
        //         }
        //         task.run();
        //     }
        // });
        std::string thread_name = "thread__" + std::to_string(i);
        BaseWorker *worker = new BaseWorker(thread_name.c_str());

        workers.emplace_back(worker);
        worker->run();
    }
}

ThreadManager::~ThreadManager()
{
}

bool ThreadManager::enqueue(BaseJob& job)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        jobs.mJobQueue.emplace_back(job);   
    }
    mCondition.notify_one();

    return true;
}


#endif // __THREAD_MANAGER_H