#include <assert.h>

#include "kmer_creator.h"


namespace rollercoaster{

  KmerCreator::KmerCreator(const std::string &read):read_(read){
  

  KmerCreator::set_read(const std::string &read){
    read_ = read;
  }


  //for KmerCreator iterator
  KmerCreator::const_packed_reverse_iterator::const_packed_reverse_iterator(const std::string &input_str, int kmer_size, int kmer_index):
    read_(input_str),
    kmer_idx_(kmer_index),
    kmer_size_(kmer_size),
    packed_kmer_(kmer_size_){
      assert(kmer_idx_ >= -1 &&  kmer_idx_ < read_.size() - kmer_size_ + 1);
    
    }
  
  const PackedKmer &KmerCreator::const_packed_reverse_iterator::operator*(){
    if(!packed_kmer_.is_set())
      packed_kmer_.set_kmer(read_.begin()+kmer_idx_, read_.begin()+kmer_idx_+kmer_size_);
    return packed_kmer_;
  }

  const PackedKmer *KmerCreate::const_packed_reverse_iterator::operator->(){
    if(!packed_kmer_.is_set())
      packed_kmer_.set_kmer(read_.begin()+kmer_idx_, read_.begin()+kmer_idx_+kmer_size_);
    return &packed_kmer_;
  }

  void KmerCreate::const_packed_reverse_iterator::operator++(){
    packed_kmer_.add_base(read_[--kmer_idx_]);
  }

  bool KmerCreate::const_packed_reverse_iterator::operator==(const PackedKmer &rhs){
    //Do I need to make sure we're looking at the same string?
    return index() == rhs.index();
  }

  bool KmerCreate::const_packed_reverse_iterator::operator!=(const PackedKmer &rhs){
    return index() != rhs.index();
  }

}//namespace rollercoaster
