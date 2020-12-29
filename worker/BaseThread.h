
#ifndef __THREAD_H
#define __THREAD_H

#include <pthread.h>
#include <stdio.h>
#include <string>
#include <unistd.h>

class BaseThread
{
public:
    BaseThread(const char* thread_name = "");
    virtual ~BaseThread();

    bool start();
    bool join();

    pthread_t mThreadId;

protected:
    bool setThreadName(const char* thread_name);
    bool setThreadPriority(int level);

    bool started();
    bool isSelf();
    virtual void run() {};  // this should be pure virtual function, user finish it

private:
    pthread_t mInvalidId;
    int mPriority;
    std::string mThreadName;

    static void* func(void* arg) {
        puts("thread work start");
        
        BaseThread *self = static_cast<BaseThread*> (arg);
        self->run();

        puts("thread work done");
        return NULL;
    }
};

BaseThread::BaseThread(const char* thread_name)
        : mPriority(13)
        , mThreadName(thread_name ? thread_name : "")
{
    // 获取当前运行的线程作为 refs, 接下来与新生成的线程比较判断是否started
    mInvalidId = pthread_self();
    mThreadId = mInvalidId;
    // printf("constructor: mThreadId:%ld   mInvalidId:%ld \n", mThreadId, mInvalidId);
}

BaseThread::~BaseThread()
{
    // ! 有子类继承的时候, 会首先析构掉子类, this指针便指向的虚函数表便移动回父类上
    // ! 这时候一些多态的方法会回归到父类的方法上
    // ! 这个时候 子线程会随着子类而被回收掉, 因此之后想要再获取线程名字是不行的, 不存在这个线程
    sleep(3);
    puts("destructor");

    // join();              // 不能放这里执行, 主线程会发生阻塞, 直接卡在线程创建阶段; 可以放在Pool的析构里执行
    // pthread_exit(NULL);
}


bool BaseThread::setThreadName(const char* thread_name)
{
#ifdef __linux__
    if (!pthread_setname_np(mThreadId, mThreadName.c_str()))
#elif __APPLE__
    if (!pthread_setname_np(mThreadName.c_str()))
#endif
    {
        char name[32];
        pthread_getname_np(mThreadId, name, 32);
        printf("(%s) mThreadId: %ld, name: %s\n", __func__, mThreadId, name);
        return true;
    }
    return false;
}

bool BaseThread::setThreadPriority(int level)
{
    int schedPolicy;
    struct sched_param schedParam;
    if (!pthread_getschedparam(mThreadId, &schedPolicy, &schedParam)) {
        schedParam.sched_priority = mPriority;
        if (!pthread_setschedparam(mThreadId, schedPolicy, &schedParam)) {
            puts("set priority success");
            return true;
        }
    }
    return false;
}

// 比较两个tid, 不相同返回0, 取非调试started
bool BaseThread::started()
{
    return !pthread_equal(mInvalidId, mThreadId);
}

// tid相同返回1, 表示isSelf
bool BaseThread::isSelf()
{
    return !!pthread_equal(mInvalidId, mThreadId);
}

bool BaseThread::join()
{
    if(isSelf()) {
        return false;
    }
    return !pthread_join(mThreadId, NULL);
}

bool BaseThread::start()
{
    if (started()) {
        return true;
    }
    
    bool ret = false;
    pthread_attr_t attr;
    if (!pthread_attr_init(&attr)) {
        if (!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) {
            if (!pthread_create(&mThreadId, &attr, func, this)) {
                // printf("pthread created: mThreadId:%ld   mInvalidId:%ld \n", mThreadId, mInvalidId);
                setThreadPriority(mPriority);
                setThreadName(mThreadName.c_str());
                ret = true;
            }
        }
        
        if (pthread_attr_destroy(&attr) != 0) {
            puts("destory attribute failed");
        }

        // join();
    }

    return ret;
}

#endif // __THREAD_H