#ifndef THREAD_EXECUTOR_H
#define THREAD_EXECUTOR_H
#include <vector>

#include <pthread.h>

#include "runnable.h"

namespace rollercoaster{
  
  template <class T>
  class ThreadExecutor{
  public:
    typedef std::vector<pthread_t> ThreadVector;
    
    void execute(T &runnable){
      thread_v_.push_back(pthread_t(0));
      pthread_create(&thread_v_.back(), 0, ThreadExecutor::launch_thread, &runnable);
    }

    void execute(T &runnable, int num_threads){
      for(int i=0;i<num_threads;++i){
        execute(runnable);
      }
    }
    
    void join_all(){
      ThreadVector::iterator begin = thread_v_.begin();
      ThreadVector::iterator end = thread_v_.end();
      for(; begin != end; ++begin){
        pthread_join(*begin, NULL);
      }
    }

    ~ThreadExecutor(){
      join_all();
    }

  private:
    ThreadVector thread_v_;

    static void *launch_thread(void *runnable){
      T *runnable_ptr = static_cast<T *>(runnable);
      T tmp(*runnable_ptr);
      tmp.run();
      return NULL;
    }

  };

} //namespace rollercoaster


#endif
