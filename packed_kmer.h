#ifndef PACKED_KMER_H
#define PACKED_KMER_H

#include <iostream>
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
     *Read packed kmer from given stream
     *@return number of bytes read from stream
     */
    int read_from_stream(char *stream);

    
    

    /**
     *Sets kmer using the characters between the iterators
     *begin and end
     */
    
    template <class T> 
      void set_kmer(typename T::const_iterator begin,
                    typename T::const_iterator end){
      clear();
      end--;begin--;
      for(;end != begin; --end){
        push_bits(encode_base(*end),BitsPerBase);
      }
      is_set_ = true;
    }
    

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


  /**
   *Free Functions
   */

  /**
   *Write bytes to stream
   *@return number of bytes written to stream
   */
  
  int write_to_stream(PackedKmer &kmer, FILE *out);

  /**
   *Write out the string representation of the kmer
   */
  std::ostream &operator << (std::ostream &out, const PackedKmer &kmer);


}//namespace rollercoaster


#endif
