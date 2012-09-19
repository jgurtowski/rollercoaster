#ifndef THREADED_QUEUE_H
#define THREADED_QUEUE_H

#include <pthread.h>


namespace rollercoaster{

  
  template<class T>
    class ThreadedQueue{

  public:
  
  ThreadedQueue():
    data_(),mutex_(),cond_(),empty_(true),done_(false){
      pthread_mutex_init(&mutex_, NULL);
      pthread_cond_init(&cond_, NULL);
    }

    ~ThreadedQueue(){
      pthread_mutex_destroy(&mutex_);
      pthread_cond_destroy(&cond_);
    }
    
    /**
     *@param input data to copy into queue
     *Copies the item into the queue
     */
    void put(const T &input){
      pthread_mutex_lock(&mutex_);
      while(!empty_){
        pthread_cond_wait(&cond_,&mutex_);
      }
      data_ = input;
      empty_ = false;
      pthread_cond_broadcast(&cond_);      
      pthread_mutex_unlock(&mutex_);
    }

    /**
     *Pops waiting item from the queue
     *Copies data to object pointed to by 'out'
     *@param out place to save the item to
     *@return 1: successfully got data, 0: No data 
     */
    int pop(T *out){
      pthread_mutex_lock(&mutex_);
      while(empty_ && !done_){
        pthread_cond_wait(&cond_,&mutex_);
      }

      if(done_){
        pthread_mutex_unlock(&mutex_);
        return 0;
      }

      *out = data_;
      empty_ = true;
      pthread_cond_broadcast(&cond_);
      pthread_mutex_unlock(&mutex_);
      return 1;
    }


    void finish(){
      pthread_mutex_lock(&mutex_);
      while(!empty_){
        pthread_cond_wait(&cond_,&mutex_);
      }
      done_ = true;
      pthread_cond_broadcast(&cond_);
      pthread_mutex_unlock(&mutex_);
    }


  private:
    T data_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    bool empty_;
    bool done_;
  };
    
  

}//namespace rollercoaster


#endif
