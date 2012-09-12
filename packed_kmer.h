#ifndef PACKED_KMER_H
#define PACKED_KMER_H

#include <string>

#include "packed_sequence.h"



namespace rollercoaster{


  class PackedKmer: public PackedSequence{
  public:  
    typedef unsigned char EncodedBase;

    static const int BitsPerBase = 3;
    
    static const EncodedBase CharToBits[];
    
    PackedKmer(int kmer_size);

    /**
     *Sets kmer using the characters between the iterators
     *begin and end
     */
    void set_kmer(const std::string::const_iterator &begin, 
                  const std::string::const_iterator &end);
    /**
     *Adds a base to the left most position in the packed Kmer 
     *Because kmers are fixed length, the right most base will
     *be dropped to make room for the incoming base
     */
    void add_base(const char base);

    /**
     *Encodes given base, encoded base will be BitsPerBase in size
     */
    static EncodedBase encode_base(const char base);


  private:
    int kmer_size_;
    

  };


}//namespace rollercoaster


#endif
