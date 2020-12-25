#ifndef __JOB_QUEUE_H
#define __JOB_QUEUE_H

#include <vector>

class Job
{
public:
    Job();
    ~Job();
};

typedef std::vector<Job> JobQueue;

class BaseJobQueue
{
public:
    BaseJobQueue(unsigned int max_size = 0);
    ~BaseJobQueue();

    bool enqueue(Job& job);

private:
    unsigned int mMaxSize;
    JobQueue mJobQueue;
};

BaseJobQueue::BaseJobQueue(unsigned int max_size)
    : mMaxSize(max_size)
{

}

BaseJobQueue::~BaseJobQueue()
{
}


bool BaseJobQueue::enqueue(Job& job)
{
    // lock
    if (!mMaxSize || mJobQueue.size() < mMaxSize) {
        mJobQueue.push_back(job);
        return true;
    }
    // unlock
    return false
}

#endif // __JOB_QUEUE_H