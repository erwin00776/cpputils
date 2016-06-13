#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "MutexLock.h"
#include <pthread.h>

namespace ylemon{

class Condition{
private:
    MutexLock &mutexLock_;
    pthread_cond_t cond_;
public:
    Condition(MutexLock &mutexLock): mutexLock_(mutexLock){
        MCHECK(pthread_cond_init(&cond_, NULL));
    }

    ~Condition(){
        MCHECK(pthread_cond_destroy(&cond_));
    }

    void wait(){
        MutexLock::UnassignGuard ug(mutexLock_);
        MCHECK(pthread_cond_wait(&cond_, mutexLock_.getMutex()));
    }

    void notify(){
        MCHECK(pthread_cond_signal(&cond_));
    }

    void notifyAll(){
        MCHECK(pthread_cond_broadcast(&cond_));
    }
};

}

#endif