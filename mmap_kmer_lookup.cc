#include <assert.h>
#include <string.h>

#include "mmap_file.h"
#include "mmap_kmer_lookup.h"
#include "packed_kmer.h"
#include "packed_sequence.h"
#include "kmer_record.h"

namespace rollercoaster{

  
  MMapKmerLookup::MMapKmerLookup(MMapFile &file):
    mmap_db_(file.mapped_data()),
    header_size_(sizeof(int)),
    record_size_(),
    kmer_size_(),
    num_records_(),
    kmer_record_(){
    
    assert(file.is_mapped());
    
    memcpy(&kmer_size_, mmap_db_, header_size_);
    kmer_record_ = new KmerRecord(kmer_size_);
    record_size_ = PackedSequence::CalcBytesForBits( kmer_size_ * PackedKmer::BitsPerBase ) + sizeof(int);
    num_records_ = (file.size() - header_size_) / record_size_; 
  }

  MMapKmerLookup::~MMapKmerLookup(){
    delete kmer_record_;
  }

  
  bool MMapKmerLookup::has_record(const PackedKmer &packed_kmer){
    MMapKmerLookup::KmerIndex l = 0;
    MMapKmerLookup::KmerIndex r = num_records_;
    MMapKmerLookup::KmerIndex cur;

    int diff;
    while( l <= r){
      cur = (l+r)/2;
      read_record_at(cur);

      diff = compare(packed_kmer,kmer_record_->kmer());
      if(diff < 0 )
        r = cur -1;
      else if( diff > 0 )
        l = cur + 1;
      else
        return true;
    }
    kmer_record_->clear();
    return false;
  }


} //namespace rollercoaster

