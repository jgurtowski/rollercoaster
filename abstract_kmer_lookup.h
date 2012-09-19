#ifndef ABSTRACT_KMER_LOOKUP
#define ABSTRACT_KMER_LOOKUP

#include <string>
#include <vector>

namespace rollercoaster{

  class KmerRecord;
  class PackedKmer;

  class AbstractKmerLookup{
    
  public:

    /**
     *The lookup method should be able to tell the size of the 
     * kmers it stores
     */
    virtual int kmer_size() const = 0;

    /**
     *Determine if the lookup has the given kmer
     *@return true: has it, false: doesn't have it
     */
    virtual bool has_record(const PackedKmer &packed_kmer) = 0;
    
    /**
     *Returns an unmodifiable reference to the last 
     * kmer that was searched using has_kmer
     * If the kmer was not found, the count will be
     * set to -1
     */
    virtual const KmerRecord &last_record() = 0;
    
    virtual ~AbstractKmerLookup(){}
    
  };//class AbstractKmerLookup


  /**
   *Free Functions
   */
  
  void kmer_counts_for_read(const std::string &read, AbstractKmerLookup &lookup, std::vector<int> *out);
  

}//namespace rollercoaster
#endif
