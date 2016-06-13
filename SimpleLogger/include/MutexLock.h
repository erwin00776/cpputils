#ifndef __MUTEX_LOCK__
#define __MUTEX_LOCK__

#include <pthread.h>
#include <assert.h>

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret); \
                        assert(errnum==0); (void) errnum; })


namespace ylemon{


class MutexLock{
private:
    pthread_mutex_t mutex_;
    pid_t pid_;

public:
    MutexLock(){
        MCHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MutexLock(){
        assert(pid_!=0);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    void assignHolder(){
        //pid_ = ;
    }

    void unassignHolder(){
        pid_ = 0;
    }

    pthread_mutex_t* getMutex(){
        return &mutex_;
    }

    void lock(){
        MCHECK(pthread_mutex_lock(&mutex_));
        assignHolder();
    }

    void unlock(){
        MCHECK(pthread_mutex_unlock(&mutex_));
        unassignHolder();
    }

    class UnassignGuard{
    private:
        MutexLock owner_;
    public:
        UnassignGuard(MutexLock &mutexLock): owner_(mutexLock){
            owner_.unassignHolder();
        }

        ~UnassignGuard(){
            owner_.assignHolder();
        }
    };
};

class MutexLockGuard{
private:
    MutexLock mutexLock_;
public:
    MutexLockGuard(MutexLock mutexLock): mutexLock_(mutexLock){
        mutexLock_.lock();
    }

    ~MutexLockGuard(){
        mutexLock_.unlock();
    }
};

}

#endif