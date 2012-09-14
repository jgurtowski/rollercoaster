#include <assert.h>
#include <iostream>

#include "kmer_creator.h"


namespace rollercoaster{

  /**
   *KmerCreator Functionality
   */
  KmerCreator::KmerCreator(const std::string &read, int kmer_size):read_(read),
                                                                   kmer_size_(kmer_size){
    
  }

  void KmerCreator::set_read(const std::string &read){
    read_ = read;//make an internal copy of the outside read
  }

  KmerCreator::const_packed_reverse_iterator KmerCreator::packed_rbegin(){
    return KmerCreator::const_packed_reverse_iterator(read_, kmer_size_, read_.length() - kmer_size_);
  }

  KmerCreator::const_packed_reverse_iterator KmerCreator::packed_rend(){
    return KmerCreator::const_packed_reverse_iterator(read_, kmer_size_, -1);
  }

  KmerCreator::const_iterator KmerCreator::begin(){
    return KmerCreator::const_iterator(read_, kmer_size_, 0);
  }

  KmerCreator::const_iterator KmerCreator::end(){
    return KmerCreator::const_iterator(read_,kmer_size_, read_.length() - kmer_size_ + 1);
  }

  KmerCreator::const_reverse_iterator KmerCreator::rbegin(){
    return KmerCreator::const_reverse_iterator(read_,kmer_size_, read_.length() - kmer_size_);
  }

  KmerCreator::const_reverse_iterator KmerCreator::rend(){
    return KmerCreator::const_reverse_iterator(read_, kmer_size_, -1);
  }
  


  /**
   *base_iterator
   */

  KmerCreator::base_iterator::base_iterator(const std::string &input_str, int kmer_size, int kmer_index):
    read_(input_str),
    kmer_size_(kmer_size),
    kmer_idx_(kmer_index),
    base_waiting_(false)
  {
    //this static cast is super strange, for some reason this subtraction isn't returning a proper int
    assert(kmer_idx_ >= -1 && kmer_idx_ <= static_cast<int>(read_.length() + 1));

  }

  bool KmerCreator::base_iterator::operator==(const KmerCreator::base_iterator &rhs){
    return (&rhs.read() == &read_ && index() == rhs.index());
  }

  bool KmerCreator::base_iterator::operator!=(const KmerCreator::base_iterator &rhs){
    return (&rhs.read() != &read_ || index() != rhs.index());
  }

  
  

  /**
   *const_iterator
   */
  
  void KmerCreator::const_iterator::load_data(){
    if(!kmer_.is_set())
      kmer_.set_kmer(read_.begin()+kmer_idx_, read_.begin()+kmer_idx_+kmer_size_);
    if(base_waiting_){
      kmer_.add_base_right(read_[kmer_idx_]);
      base_waiting_ = false;
    }
  }

  const KmerCreator::const_iterator &KmerCreator::const_iterator::operator++(){
    ++kmer_idx_;
    base_waiting_ = true;
    
    return *this;
  } 
  
  const ListBackedKmer &KmerCreator::const_iterator::operator*(){
    load_data();
    return kmer_;
  }
  
  const ListBackedKmer *KmerCreator::const_iterator::operator->(){
    load_data();
    return &kmer_;
  }

  
  /**
   *const_reverse_iterator
   */

  void KmerCreator::const_reverse_iterator::load_data(){
    if(!kmer_.is_set())
      kmer_.set_kmer(read_.begin()+kmer_idx_, read_.begin()+kmer_idx_+kmer_size_);
    if(base_waiting_){
      kmer_.add_base_left(read_[kmer_idx_]);
      base_waiting_ = false;
    }
  }

  const KmerCreator::const_reverse_iterator &KmerCreator::const_reverse_iterator::operator++(){
    --kmer_idx_;
    base_waiting_ = true;
    return *this;
  } 
  
  const ListBackedKmer &KmerCreator::const_reverse_iterator::operator*(){
    load_data();
    return kmer_;
  }
  
  const ListBackedKmer *KmerCreator::const_reverse_iterator::operator->(){
    load_data();
    return &kmer_;
  }


  /**
   *for KmerCreator const_packed_reverse_iterator
   */
  void KmerCreator::const_packed_reverse_iterator::load_data(){
    if(!packed_kmer_.is_set())
      packed_kmer_.set_kmer(read_.begin()+kmer_idx_, read_.begin()+kmer_idx_+kmer_size_);
    if(base_waiting_){
      packed_kmer_.add_base(read_[kmer_idx_]);
      base_waiting_ = false;
    }
  }

  const PackedKmer &KmerCreator::const_packed_reverse_iterator::operator*(){
    load_data();
    return packed_kmer_;
  }
  
  const PackedKmer *KmerCreator::const_packed_reverse_iterator::operator->(){
    load_data();
    return &packed_kmer_;
  }

  const KmerCreator::const_packed_reverse_iterator &KmerCreator::const_packed_reverse_iterator::operator++(){
    --kmer_idx_;
    base_waiting_ = true;
    
    return *this;
  }

  
  
}//namespace rollercoaster
