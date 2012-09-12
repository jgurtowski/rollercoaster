#include <assert.h>

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

  PackedKmer::PackedKmer(int kmer_size):PackedSequence(BitsPerBase * kmer_size),
                                        kmer_size_(kmer_size){
    
  }

  void PackedKmer::set_kmer(const std::string::const_iterator &begin,
                            const std::string::const_iterator &end){

    for(std::string::const_iterator it = begin; it != end; ++it){
      push_bits(encode_base(*it),BitsPerBase);
    }
    
  }
    
  void PackedKmer::add_base(const char base){
    push_bits(encode_base(base),BitsPerBase);
  }

  PackedKmer::EncodedBase PackedKmer::encode_base(const char base){
    assert(base >=64 && base <= 84);
    return CharToBits[base];
  }


}//namespace rollercoaster
