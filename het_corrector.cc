#include <iostream>


#include "abstract_kmer_lookup.h"
#include "mmap_kmer_lookup.h"
#include "het_corrector.h"

namespace rollercoaster{

  PthreadMutex HetCorrector::__io_mutex;


  
  void HetCorrector::run(){
    MMapKmerLookup lookup( mapped_file_ );
    std::vector<int> counts;
    
    while(read_queue_->pop(&read_pair_)){
      kmer_counts_for_read(read_pair_.first.sequence(),lookup, &counts);
      __io_mutex.lock();
      std::cout << read_pair_.first.sequence() << counts[0] << std::endl;
      __io_mutex.unlock();

      kmer_counts_for_read(read_pair_.second.sequence(),lookup, &counts);
      __io_mutex.lock();
      std::cout << read_pair_.second.sequence() << counts[0] << std::endl;
      __io_mutex.unlock();
    }
  }


}//namespace rollercoaster
