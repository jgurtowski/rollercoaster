#include <assert.h>
#include <iostream>

#include "list_backed_kmer.h"
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
    kmer_idx_(kmer_index)
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
      kmer_.set_kmer<std::string>(read_.begin()+kmer_idx_,read_.begin()+(kmer_idx_+kmer_size_));
  }

  const KmerCreator::const_iterator &KmerCreator::const_iterator::operator++(){
    load_data();
    ++kmer_idx_;
    //if we're past the last kmer just return what we have
    if(kmer_idx_ >= static_cast<int>(read_.size()))
      return *this;
    //otherwise add the base to the kmer
    kmer_.add_base_right(read_[kmer_idx_ + kmer_size_ -1 ]);
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
    if(!kmer_.is_set()){
      kmer_.set_kmer<std::string>(read_.begin()+kmer_idx_,read_.begin()+(kmer_idx_+kmer_size_));
    }
  }

  const KmerCreator::const_reverse_iterator &KmerCreator::const_reverse_iterator::operator++(){
    load_data();
    
    --kmer_idx_;
    if(kmer_idx_ < 0)
      return *this;
    kmer_.add_base_left(read_[kmer_idx_]);
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
    if(!packed_kmer_.is_set()){
      packed_kmer_.set_kmer<std::string>(read_.begin()+kmer_idx_, read_.begin()+kmer_idx_+kmer_size_);
    }
  }

  const KmerCreator::const_packed_reverse_iterator &KmerCreator::const_packed_reverse_iterator::operator++(){
    --kmer_idx_;
    if(kmer_idx_ < 0)
      return *this;
    packed_kmer_.add_base(read_[kmer_idx_]);
    return *this;
  }

  const PackedKmer &KmerCreator::const_packed_reverse_iterator::operator*(){
    load_data();
    return packed_kmer_;
  }
  
  const PackedKmer *KmerCreator::const_packed_reverse_iterator::operator->(){
    load_data();
    return &packed_kmer_;
  }

  
}//namespace rollercoaster
