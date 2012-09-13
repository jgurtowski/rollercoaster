#ifndef KMER_CREATOR_H
#define KMER_CREATOR_H

#include <iterator>
#include <string>

#include "packed_kmer.h"


namespace rollercoaster{


  class KmerCreator{
  public:
    KmerCreator(const std::string &read);
    
    //Set the next read to be kmerized
    set_read(const std::string &read);

    /**
     *Class: const_packed_reverse_iterator
     *Iterate through Kmers, provide a packed version of each kmer
     *This is a reverse iterator because it is more efficient to
     *operate on the kmers in reverse due to the inner workings of
     *how things are packed
     */
    class const_packed_reverse_iterator : public std::iterator<std::reverse_iterator_tag,PackedKmer> {
    public:
      
      const_packed_reverse_iterator(const std::string &input_str, int kmer_size);
      
      void operator++();
      const PackedKmer &operator*();
      const PackedKmer *operator->();
      bool operator==(const PackedKmer &rhs);
      bool operator!=(const PackedKmer &rhs);
      
      inline int index(){ return kmer_idx_;}
      
    private:
      const std::string &read_;
      int kmer_idx_;
      int kmer_size_;
      PackedKmer packed_kmer_;
    };
    

    
  private:
    const std::string &read_;
    
    //disallow copying because we work from a reference to an outside string
    KmerCreator(const KmerCreator &);
    KmerCreator &operator=(const KmerCreator &);
  };

}//namespace rollercoaster

#endif
