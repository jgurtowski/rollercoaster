#ifndef HET_CORRECTOR_H
#define HET_CORRECTOR_H

#include <utility>

#include "mmap_file.h"
#include "pthread_mutex.h"
#include "threaded_queue.h"
#include "runnable.h"
#include "fastq_record.h"

namespace rollercoaster{

  class HetCorrector: public Runnable{
    
  public:
    typedef std::pair<FastqRecord, FastqRecord> ReadPair;

  HetCorrector(MMapFile &mapped_file, ThreadedQueue<ReadPair> *read_queue):
    mapped_file_(mapped_file),
      read_queue_(read_queue){}


    virtual void run();
    virtual ~HetCorrector(){}
    
  private:
    MMapFile &mapped_file_;
    ThreadedQueue<ReadPair> *read_queue_;
    ReadPair read_pair_;
    

    //serialize io from multiple correctors
    static PthreadMutex __io_mutex;
    
  }; //class HetCorrector

}//namespace rollercoaster



#endif
