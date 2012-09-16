#ifndef KMER_RECORD_H
#define KMER_RECORD_H

#include "packed_kmer.h"

namespace rollercoaster{

  class KmerRecord{

  public:
    
  KmerRecord(int kmer_size):kmer_(kmer_size),count_(-1){}
  KmerRecord():kmer_(1),count_(-1){}

    inline const PackedKmer &kmer()const { return kmer_;}
    inline int count()const {return count_;}
    inline void clear(){kmer_.clear(); count_=-1;}

    int read_from_stream(char *stream);

  private:
    PackedKmer kmer_;
    int count_;
  
  }; // class KmerRecord
}//namespace rollercoaster

#endif
