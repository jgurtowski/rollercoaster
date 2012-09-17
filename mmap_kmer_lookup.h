#ifndef MMAP_KMER_LOOKUP
#define MMAP_KMER_LOOKUP

#include <stdint.h>

#include "abstract_kmer_lookup.h"
#include "kmer_record.h"
#include "packed_kmer.h"

namespace rollercoaster{

  class MMapFile;

  class MMapKmerLookup: public AbstractKmerLookup{
    

  public:
    typedef uint64_t KmerIndex;
    
    MMapKmerLookup(MMapFile &file);

    inline int kmer_size() const { return kmer_size_; }

    virtual bool has_record(const PackedKmer &packed_kmer);
    virtual const KmerRecord &last_record(){return kmer_record_;}
    

  private:
    char *mmap_db_;
    int header_size_;
    int record_size_;
    int num_records_;
    int kmer_size_;
    KmerRecord kmer_record_;

    inline void  read_record_at(KmerIndex index){
      kmer_record_.read_from_binary_stream(mmap_db_ + (index * record_size_) + header_size_);
    }

  };

}//namespace rollercoaster

#endif
