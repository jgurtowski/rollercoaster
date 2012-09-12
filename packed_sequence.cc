#include <string.h>

#include "packed_sequence.h"


namespace rollercoaster{

  PackedSequence::PackedSequence(int required_bits):num_bits_(required_bits),
                                                    num_extra_bits_(required_bits % 8),
                                                    num_packed_bytes_(required_bits / 8 + (num_extra_bits_ > 0 ? 1: 0)),
                                                    extra_bit_mask_(255 << num_extra_bits_),
                                                    packed_bytes_(new PackedByte[num_packed_bytes_]){
    clear();
  }
  
  void PackedSequence::push_bits(PackedByte byte, int num_bits){
    for( int i=num_packed_bytes_-1; i>=1; --i){
      packed_bytes_[i] >>= num_bits;
      packed_bytes_[i] |= (packed_bytes_[i-1] & (255 >> (8-num_bits))) << (8-num_bits);
      if(i == num_packed_bytes_-1)//remove extra bits
        packed_bytes[i] &= extra_bit_mask_;
    }
    dest[0] >>= num_bits;
    dest[0] |= (byte << (8-num_bits));
  }

  PackedByte PackedSequence::pop_bits(int num_bits){
    PackedByte mask = 255 << (8-num_bits);
    PackedByte output = (packed_bytes_[0] & mask) >> (8-num_bits);
    for(int i=0;i<num_packed_bytes-1;++i){
      packed_bytes_[i] <<= num_bits;
      packed_bytes_[i] |= (packed_bytes_[i+1] & mask) >> (8-num_bits);
    }
    packed_bytes_[num_packed_bytes-1] <<= num_bits;
    return output;
  }

  int PackedSequence::num_bits() const{
    return num_bits_;
  }

  int PackedSequence::num_bytes() const{
    return num_packed_bytes_;
  }

  const PackedByte *PackedSequence::packed_bytes() const{
    return packed_bytes_;
  }

  void PackedSequence::clear(){
    memset(packed_bytes_,0,num_packed_bytes_);
  }
  
  PackedSequence::~PackedSequence(){
    delete []packed_bytes_;
  }
}
