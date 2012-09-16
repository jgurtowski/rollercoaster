#ifndef ABSTRACT_KMER_LOOKUP
#define ABSTRACT_KMER_LOOKUP


namespace rollercoaster{

  class KmerRecord;
  class PackedKmer;

  class AbstractKmerLookup{
    
  public:

    /**
     *Determine if the lookup has the given kmer
     *@return true: has it, false: doesn't have it
     */
    virtual bool has_kmer(const PackedKmer &packed_kmer) = 0;

    /**
     *Returns an unmodifiable reference to the last 
     * kmer that was searched using has_kmer
     * If the kmer was not found, the count will be
     * set to -1
     */
    virtual const KmerRecord &last_kmer() = 0;
    
    virtual ~AbstractKmerLookup(){}
    
  };//class AbstractKmerLookup
  

}//namespace rollercoaster
#endif
