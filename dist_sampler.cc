#include <stdlib.h>
#include <stdint.h>

#include <iostream>
#include <utility>

#include "fastq_record.h"
#include "het_corrector.h"
#include "thread_executor.h"
#include "threaded_queue.h"
#include "runnable.h"
#include "pthread_mutex.h"
#include "mmap_kmer_lookup.h"

class DistSampler : public rollercoaster::Runnable{

public:
  typedef rollercoaster::HetCorrector::ReadPair ReadPair;
  

  DistSampler(rollercoaster::MMapFile &file, rollercoaster::ThreadedQueue<ReadPair> *read_queue):
    queue_(read_queue),
    mmap_file_(file){}

  struct RangeCounts{
    int g200;
    int b60100;
    int b100160;
  };

  void getRangeCounts(const std::vector<int> &counts, RangeCounts *range_counts){
    std::vector<int>::const_iterator b,e;
    e = counts.end();
    range_counts->g200 = 0; range_counts->b60100 = 0; range_counts->b100160 = 0;
    for(b = counts.begin(); b != e; ++b){
      if( *b > 200)
        ++range_counts->g200;
      else if( *b >= 60 && *b <=  100)
        ++range_counts->b60100;
      else if( *b >= 100 && *b <= 160)
        ++range_counts->b100160;
    }
  }

  bool keep_pair(const std::vector<int> &counts_one, const std::vector<int> &counts_two ){
    RangeCounts counts, counts2;
    getRangeCounts(counts_one, &counts);
    if(counts.g200 > 1)
      return false;
    getRangeCounts(counts_two, &counts2);
    if( counts2.g200 > 1)
      return false;
    if( counts.b60100 > 40 && counts2.b60100 > 40){
      std::cout << "hi";
    }else if( counts.b100160 > 40 && counts2.b100160 > 40){
      std::cout << "hi";
    }
    return false;
  }
  
  virtual void run(){
    std::vector<int> counts1, counts2;
    rollercoaster::MMapKmerLookup lookup(mmap_file_);
    ReadPair read_pair;
    
    while( queue_ -> pop( &read_pair ) ){
      //kmer_counts_for_read(read_pair.first.sequence(),lookup, &counts1);
      //kmer_counts_for_read(read_pair.second.sequence(), lookup, &counts2);
      if(keep_pair(counts1,counts2)){
        __io_mutex.lock();
        std::cout << rand() % 100 << std::endl;
        //std::cout << read_pair.first << std::endl;
        //std::cout << read_pair.second << std::endl;
        __io_mutex.unlock();
      }
    }

  }

private:
  rollercoaster::ThreadedQueue<ReadPair> *queue_;
  rollercoaster::MMapFile &mmap_file_;
  static rollercoaster::PthreadMutex __io_mutex;
  static rollercoaster::PthreadMutex __rand_mutex;

};


rollercoaster::PthreadMutex DistSampler::__io_mutex;


int main(int argc, char *argv[]){
  
  namespace rc = rollercoaster;

  if(!(argc == 5)){
    std::cerr << "readcorrector <1.fq> <2.fq> <in.db> <num_threads>" << std::endl;
    return 1;
  }
  
  srand( time(NULL) );

  std::string fastq_1(argv[1]);
  std::string fastq_2(argv[2]);
  std::string db_filename(argv[3]);
  int num_threads = atoi(argv[4]);

  rc::FastqRecordReader reader_1(fastq_1);
  rc::FastqRecordReader reader_2(fastq_2);
  rc::MMapFile mmap_db(db_filename);

  if(!reader_1.is_open()){
    std::cerr << "unable to open " << fastq_1 << std::endl;
    return 1;
  }
  if(!reader_2.is_open()){
    std::cerr << "unable to open " << fastq_2 << std::endl;
    return 1;
  }

  if(mmap_db.map()){
    std::cerr << "unable to map file" << db_filename << std::endl;
    return 1;
  }

  rc::ThreadedQueue<rc::HetCorrector::ReadPair> read_queue;
  
  DistSampler dist_sampler(mmap_db, &read_queue);
  
  rc::ThreadExecutor<DistSampler> executor;

  executor.execute(dist_sampler, num_threads);
  
  rc::HetCorrector::ReadPair read_pair;
  uint64_t count = 0;
  while( reader_1 >> read_pair.first && reader_2 >> read_pair.second){
    read_queue.put(read_pair);  
    if(0 == ++count % 1000000)
      std::cerr << "Processed Pairs: " << count << std::endl;
  }
  
  read_queue.finish();

  return 0;
}
