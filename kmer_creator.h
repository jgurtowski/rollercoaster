#ifndef KMER_CREATOR_H
#define KMER_CREATOR_H

#include <iterator>

#include "packed_sequence.h"

class std::string;


namespace rollercoaster{


  class KmerCreator{
  public:
    KmerCreator(const std::string &read);
    
    //Set the next read to be kmerized
    set_read(const std::string &read);
  

    /**
     *Class: const_packed_iterator
     *Iterate through Kmers, provide a packed version of each kmer
     */
    class const_packed_iterator : public std::iterator<std::forward_iterator_tag,PackedSequence> {
    public:
      
      const_packed_iterator(const std::string &input_str, int kmer_size);

      
      const_iterator operator++();
      const PackedSequence &operator*();
      const PackedSequence *operator->();
      bool operator==(const PackedSequence &rhs);
      bool operator!=(const PackedSequence &rhs);
            
    private:
      const std::string &read_;
      int kmer_size_;
      PackedSequence packed_sequence_;
      
    };
    


    
  private:
    const std::string &read_;
    
    //disallow copying because we work from a reference to an outside string
    KmerCreator(const KmerCreator &);
    KmerCreator &operator=(const KmerCreator &);
  };

}//namespace rollercoaster

#endif
