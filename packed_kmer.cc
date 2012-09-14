#include <assert.h>
#include <iostream>
#include <string>

#include "packed_kmer.h"


namespace rollercoaster{

  const PackedKmer::EncodedBase PackedKmer::CharToBits[] ={
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,0,0,
    0,2,0,0,0,0,0,0,3,0,
    0,0,0,0,4
  };
  
  const char PackedKmer::BitsToChar[] = {
    'A','C','G','N','T'
  };


  PackedKmer::PackedKmer(int kmer_size):PackedSequence(BitsPerBase * kmer_size),
                                        kmer_size_(kmer_size),
                                        is_set_(false){
    
  }

  void PackedKmer::set_kmer( std::string::const_iterator begin,
                             std::string::const_iterator end){
    end--;begin--;
    for(;end != begin; --end){
      push_bits(encode_base(*end),BitsPerBase);
    }
    
    if(!is_set_)
      is_set_ = true;
  }
      
  void PackedKmer::add_base(const char base){
    push_bits(encode_base(base),BitsPerBase);
    
    if(!is_set_)
      is_set_ = true;
  }

  void PackedKmer::str_kmer(std::string *kmer_out) const{
    if(kmer_size_ != static_cast<int>(kmer_out->size()))
      kmer_out->resize(kmer_size_);

    //make a copy of this object so we don't modify our internal
    //bit array
    //kind of bad because we malloc every time we decode a kmer
    //may need to be fixed in the future
    PackedKmer copy(*this);
    for(int i=0;i<kmer_size_;++i){
      (*kmer_out)[i] = decode_base(copy.pop_bits(BitsPerBase));
    }
  }


  std::string PackedKmer::str_kmer() const{
    std::string kmer;
    str_kmer(&kmer);
    return kmer;
  }

  PackedKmer::EncodedBase PackedKmer::encode_base(const char base){
    assert(base >=64 && base <= 84);
    return CharToBits[static_cast<int>(base)];
  }

  char PackedKmer::decode_base(const EncodedBase base){
    assert(base >=0 && base <= 4);
    return BitsToChar[base];
  }

}//namespace rollercoaster
