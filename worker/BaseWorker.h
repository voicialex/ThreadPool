#ifndef __WORKER_H
#define __WORKER_H

#include <mutex>
#include <condition_variable>
#include "BaseThread.h"
#include "BaseJobQueue.h"

class BaseWorker : public BaseThread
{
public:
    BaseWorker(std::mutex* pool_mutex, std::condition_variable* pool_condition, const char* thread_name = "");
    virtual ~BaseWorker();

    virtual bool tearup() {puts("tearup"); return true;}
    virtual bool teardown() {puts("teareown"); return true;}

    void run()
    {
        while(1)
        {
            BaseJob job;
            {
                std::unique_lock<std::mutex> lock(*mMutex);
                mCondition->wait(lock, NULL);

                
            }
        }
    }

    void run(BaseJob& job)
    {
        try {
            if(!tearup()) {
                return;
            }
        } catch(...) {
            puts("excepton caught in worker::tearup");
        }

        job.run();

        try {
            if(!teardown()) {
                return;
            }
        } catch(...) {
            puts("excepton caught in worker::teardown");
        }
    }

private:
    std::string mName;
    std::mutex* mMutex;
    std::condition_variable* mCondition;
};
 
BaseWorker::BaseWorker(std::mutex* pool_mutex, std::condition_variable* pool_condition, const char* thread_name)
    : BaseThread(thread_name)
    , mMutex(pool_mutex)
    , mCondition(pool_condition)
{
    mName = thread_name;
    puts("BaseWorker");
}

BaseWorker::~BaseWorker()
{
    puts("~BaseWorker");
}



#endif // __WORKER_H