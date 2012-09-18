#ifndef MMAP_KMER_LOOKUP
#define MMAP_KMER_LOOKUP

#include <stdint.h>

#include "kmer_record.h"
#include "abstract_kmer_lookup.h"

namespace rollercoaster{

  class MMapFile;
  class KmerRecord;
  class PackedKmer;

  class MMapKmerLookup: public AbstractKmerLookup{
    

  public:
    typedef uint64_t KmerIndex;
    
    MMapKmerLookup(MMapFile &file);
    ~MMapKmerLookup();

    inline int kmer_size() const { return kmer_size_; }

    virtual bool has_record(const PackedKmer &packed_kmer);
    virtual const KmerRecord &last_record(){return *kmer_record_;}
    

  private:
    char *mmap_db_;
    int header_size_;
    int record_size_;
    int kmer_size_;
    uint64_t num_records_;
    KmerRecord *kmer_record_;

    inline void  read_record_at(KmerIndex index){
      kmer_record_->read_from_binary_stream(mmap_db_ + (index * record_size_) + header_size_);
    }

    //disallow copying
    MMapKmerLookup(const MMapKmerLookup &);
    const MMapKmerLookup &operator=(const MMapKmerLookup &);
    
  };

}//namespace rollercoaster

#endif
