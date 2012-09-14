#ifndef PACKED_KMER_H
#define PACKED_KMER_H

#include <string>

#include "abstract_kmer.h"
#include "packed_sequence.h"



namespace rollercoaster{

  class PackedKmer: public PackedSequence, public AbstractKmer{

  public:
    typedef unsigned char EncodedBase;

    static const int BitsPerBase = 3;

    static const EncodedBase CharToBits[];
    static const char BitsToChar[];

    PackedKmer(int kmer_size);

    /**
     *Sets kmer using the characters between the iterators
     *begin and end
     */
    void set_kmer(std::string::const_iterator begin,
                  std::string::const_iterator end);
    /**
     *Adds a base to the left most position in the packed Kmer
     *Because kmers are fixed length, the right most base will
     *be dropped to make room for the incoming base
     */
    void add_base(const char base);

    /**
     *Overrides the AbstractKmer method 
     *forwards requests to the const version
     */
    virtual std::string str_kmer(){ return const_cast<const PackedKmer *>(this)->str_kmer(); }
    
    /**
     *Get the string form of the kmer
     */
    std::string str_kmer() const;

    /**
     *Optimization function to avoid copying
     *puts the string representation of the kmer into kmer_out
     */
    void str_kmer(std::string *kmer_out) const;

    inline bool is_set() const {return is_set_;}

    /**
     *Encodes given base, encoded base will be BitsPerBase bits in size
     */
    static EncodedBase encode_base(const char base);

    /**
     *Decodes an EncodedBase and returns the char representation
     */
    static char decode_base(const EncodedBase base);

  private:
    int kmer_size_;
    bool is_set_;

  };


}//namespace rollercoaster


#endif
