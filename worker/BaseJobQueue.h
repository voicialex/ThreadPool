#ifndef __JOB_QUEUE_H
#define __JOB_QUEUE_H

#include <deque>    // 在末尾或开头插入元素, 复杂度O(1)
// #include <vector>   // 在末尾插入元素,复杂度O(1), 其他地方复杂度O(n) 与到vector结尾的距离成线性
#include <memory>
// #include <mutex>
class BaseJob
{
#define URGENT      1<<1
#define RUN_SUCCESS 1<<3
typedef std::shared_ptr<BaseJob> Ptr;

public:
    BaseJob(int flag = 0);
    virtual ~BaseJob() {};

    bool urgent() const
    {
        return !!(mFlag & URGENT);
    }

    virtual void run() {puts("this is base job");}

protected:

private:
    int mFlag;

    void urgent(bool active)
    {
        if (active) {
            mFlag |= URGENT;
        } else {
            mFlag &= ~URGENT;
        }
    }
};

BaseJob::BaseJob(int flag)
    :mFlag(flag)
{
}


/***********************************/
// can be intergated in Thread Pool Manager
typedef std::deque<BaseJob> JobQueue;

class BaseJobQueue
{
public:
    BaseJobQueue(unsigned int max_size = 0);
    ~BaseJobQueue();

    bool enqueue(BaseJob& job);

    JobQueue mJobQueue;

private:
    unsigned int mMaxSize;
};

BaseJobQueue::BaseJobQueue(unsigned int max_size)
    : mMaxSize(max_size)
{

}

BaseJobQueue::~BaseJobQueue()
{
}


bool BaseJobQueue::enqueue(BaseJob& job)
{
    // lock
    if (!mMaxSize || mJobQueue.size() < mMaxSize) {
        mJobQueue.emplace_back(job);
        return true;
    }
    // unlock
    return false;
}

#endif // __JOB_QUEUE_H