#ifndef KMER_RECORD_H
#define KMER_RECORD_H

#include <iostream>

#include "packed_kmer.h"

namespace rollercoaster{

  class KmerRecord{

  public:
    
    typedef int KmerCount;
    
  KmerRecord(int kmer_size):kmer_(kmer_size),count_(-1){}
  KmerRecord():kmer_(1),count_(-1){}

    inline const PackedKmer &kmer()const { return kmer_;}
    inline int count()const {return count_;}
    inline void clear(){kmer_.clear(); count_=-1;}
    
    inline void set_count(int count){ count_ = count; } 
    
    template<class T>
      void set_kmer(typename T::const_iterator begin,
                    typename T::const_iterator end){
      kmer_.set_kmer<T>(begin,end);
    }
    
    int read_from_binary_stream(char *stream);
    

  private:
    PackedKmer kmer_;
    int count_;
  
  }; // class KmerRecord


  /**
   *Free Functions
   */


  /**
   *Write binary kmer record to stream
   */
  int write_to_binary_stream(const KmerRecord &kmer, FILE *stream);

  /**
   *Print out string representation of the record
   */
  std::ostream &operator << (std::ostream &out, const KmerRecord &kmer);


}//namespace rollercoaster

#endif
