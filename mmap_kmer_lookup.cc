#include <string.h>

#include "mmap_kmer_lookup.h"
#include "packed_kmer.h"
#include "packed_sequence.h"


namespace rollercoaster{

  
  MMapKmerLookup::MMapKmerLookup(char *mmap_data, uint64_t filesize):
    mmap_db_(mmap_data),
    header_size_(sizeof(int)),
    record_size_(),
    num_records_(),
    kmer_size_(){
    
    memcpy(&kmer_size_, mmap_db_, sizeof(int));
    record_size_ = PackedSequence::CalcBytesForBits( kmer_size_ * PackedKmer::BitsPerBase );
    num_records_ = (filesize - header_size_) / record_size_; 
  }

  
  bool MMapKmerLookup::has_kmer(const PackedKmer &packed_kmer){
    MMapKmerLookup::KmerIndex l = 0;
    MMapKmerLookup::KmerIndex r = num_records_;
    MMapKmerLookup::KmerIndex cur;

    int diff;
    while( l <= r){
      cur = (l+r)/2;
      read_record_at(cur);
      
      diff = compare(packed_kmer,kmer_record_.kmer());
      if(diff < 0 )
        r = cur -1;
      else if( diff > 0 )
        l = cur + 1;
      else
        return true;
    }
    kmer_record_.clear();
    return false;
  }


} //namespace rollercoaster

