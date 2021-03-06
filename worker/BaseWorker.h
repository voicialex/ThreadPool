#ifndef __WORKER_H
#define __WORKER_H

#include <mutex>
#include <condition_variable>
#include <vector>
#include "BaseThread.h"
#include "BaseJobQueue.h"

class BaseWorker;

class ThreadPoolManager
{
public:
    ThreadPoolManager(int threads_num = 3);
    ~ThreadPoolManager();

    bool enqueue(BaseJob* job);

    std::vector<BaseWorker*> workers;
    std::deque<BaseJob*> jobs;
    // unsigned int mTasksSize;

    std::mutex mMutex;
    std::condition_variable mCondition;
    bool stoped;

};


/* I put ThreadPoolManager and ThreadWorker together, because they are mutual aggregation*/
class BaseWorker : public BaseThread
{
public:
    BaseWorker(ThreadPoolManager* owner, const char* thread_name = "");
    virtual ~BaseWorker();

    virtual bool tearup(BaseJob* job) 
    {
        printf("%s tearUp\n", mName.c_str());
        if (job) {
            return true;
        } else {
            return false;
        }
    }
    virtual bool teardown(BaseJob* job)
    {
        printf("%s tearDown\n", mName.c_str());
        if (job) {
            delete job;
            job = NULL;
        }
        return true;
    }

    void run();
    void runJob(BaseJob* job);

    std::string mName;
    ThreadPoolManager* mPoolManager;

};
 
BaseWorker::BaseWorker(ThreadPoolManager* owner, const char* thread_name)
    : BaseThread(thread_name)
    , mPoolManager(owner)
{
    mName = thread_name;
    printf("BaseWorker: %s \n", mName.c_str());
}

BaseWorker::~BaseWorker()
{
    puts("~BaseWorker");
}

void BaseWorker::run()
{
    while(1)
    {
        BaseJob* job;
        {
            // 线程间互斥
            printf("%s unlock +1 ... \n", mName.c_str());
            std::unique_lock<std::mutex> lock(mPoolManager->mMutex);

            mPoolManager->mCondition.wait(lock, [this](){return mPoolManager->stoped || !mPoolManager->jobs.empty();});

            if (mPoolManager->stoped && mPoolManager->jobs.empty()) {
                puts("<< ready to end ThreadPool lifecycle >>");
                return ;
            }
            // job = std::move(mPoolManager->jobs.front());
            job = mPoolManager->jobs.front();
            mPoolManager->jobs.pop_front();
        }
        printf("%s lock -1 ... \n", mName.c_str());
        
        runJob(job);
    }
}

void BaseWorker::runJob(BaseJob* job)
{
    try {
        if(!tearup(job)) {
            return;
        }
    } catch(...) {
        puts("excepton caught in worker::tearup");
    }

    job->run();

    try {
        if(!teardown(job)) {
            return;
        }
    } catch(...) {
        puts("excepton caught in worker::teardown");
    }
}


ThreadPoolManager::ThreadPoolManager(int threads_num)
    :stoped(false)
{
    for (int i = 0; i < threads_num;  ++i) {
        std::string thread_name = "thread__" + std::to_string(i);
        BaseWorker *worker = new BaseWorker(this, thread_name.c_str());

        workers.emplace_back(worker);
        worker->start();
    }
}

ThreadPoolManager::~ThreadPoolManager()
{
    {
        std::unique_lock<std::mutex> mMutex;
        stoped = true;
    }
    mCondition.notify_all();

    for (auto worker:workers)
    {
        printf("---------------%s join\n", worker->mName.c_str());
        workers.front()->join();
    }

}

bool ThreadPoolManager::enqueue(BaseJob* job)
{
    {
        // puts("Pool lock ...");
        std::unique_lock<std::mutex> lock(mMutex);
        jobs.emplace_back(job);   
    }
    mCondition.notify_one();
    // puts("notify ...");

    return true;
}


#endif // __WORKER_H