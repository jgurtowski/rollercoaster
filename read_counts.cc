#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <iostream>
#include <utility>
#include <vector>

#include "mmap_file.h"
#include "mmap_kmer_lookup.h"
#include "abstract_kmer_lookup.h"
#include "thread_executor.h"
#include "threaded_queue.h"
#include "runnable.h"
#include "pthread_mutex.h"



class ThreadedCountLookup: public rollercoaster::Runnable{

public:
  
  ThreadedCountLookup(rollercoaster::MMapFile &mapped_file, rollercoaster::ThreadedQueue<std::string> *read_queue):
    queue_(read_queue),
    mapped_file_(mapped_file){}

  virtual void run(){

    std::vector<int> counts;
    rollercoaster::MMapKmerLookup lookup(mapped_file_);
    std::string read;
  
    while( queue_ -> pop( &read )){

      kmer_counts_for_read(read, lookup, &counts);

      __io_mutex.lock();
      std::cout << read << "\t";
      for(unsigned int i=0;i<counts.size()-1;++i){
	std::cout << counts[i] << ",";
      }
      std::cout << counts[counts.size()-1] << std::endl;
      __io_mutex.unlock();
    }

  }

private:

  rollercoaster::ThreadedQueue<std::string> *queue_;
  rollercoaster::MMapFile &mapped_file_;
  static rollercoaster::PthreadMutex __io_mutex;

}; // class ThreadedCountLookup

rollercoaster::PthreadMutex ThreadedCountLookup::__io_mutex;


int main(int argc, char *argv[]){
  
  namespace rc = rollercoaster;

  if(argc != 3){
    std::cout << "readcounts <in.db> <# threads>" << std::endl;
    return 1;
  }
  
  int num_threads = atoi(argv[2]);

  rc::ThreadedQueue<std::string> read_queue;

  std::string db_file(argv[1]);
  rc::MMapFile db(db_file);
  if(db.map()){
    std::cout << "Error opening db" << std::endl;
    return 1;
  }    

  ThreadedCountLookup tlookup(db, &read_queue);
  
  rc::ThreadExecutor<ThreadedCountLookup> executor;
  
  executor.execute(tlookup, num_threads);

  size_t buf_size = 10204;
  char *line_buffer = new char[buf_size];
  
  std::string read;
  int line_size;
  uint64_t count = 0;
  while(-1 != (line_size = getline(&line_buffer, &buf_size, stdin))){
    read.assign(line_buffer, line_size-1);
    read_queue.put(read);
    if(0 == ++count % 1000000){
      std::cerr << "Processed Reads: " << count << std::endl;
    }
  }

  read_queue.finish();

  delete []line_buffer;

  return 0;
}
