#ifndef PTHREAD_MUTEX_H
#define PTHREAD_MUTEX_H

#include <pthread.h>

namespace rollercoaster{

  class PthreadMutex{

  public:
    PthreadMutex(){ pthread_mutex_init(&mutex_,NULL);}
    ~PthreadMutex(){ pthread_mutex_destroy(&mutex_); }
    inline void lock(){ pthread_mutex_lock(&mutex_); }
    inline void unlock(){ pthread_mutex_unlock(&mutex_);}

  private:
    pthread_mutex_t mutex_;
    
  };

}//namespace PthreadMutex

#endif
