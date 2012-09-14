#ifndef KMER_RECORD_H
#define KMER_RECORD_H

#include <string.h>

namespace rollercoaster{

  class KmerRecord{

  public:
    
    inline const PackedKmer &kmer()const { return kmer_;}
    inline int count()const {return count;}

  private:
    PackedKmer kmer_;
    int count;
  
  }; // class KmerRecord
}//namespace rollercoaster

#endif
