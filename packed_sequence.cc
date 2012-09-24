#include <assert.h>
#include <string.h>


#include "packed_sequence.h"


namespace rollercoaster{

  PackedSequence::PackedSequence(int required_bits):num_bits_(required_bits),
                                                    num_padding_bits_(CalcPaddingBits(num_bits_)),
                                                    num_packed_bytes_(CalcBytesForBits(num_bits_)),
                                                    packed_bytes_(new PackedByte[num_packed_bytes_]){
    clear();
  }
  
  PackedSequence::PackedSequence(const PackedSequence &other):num_bits_(other.num_bits()),
                                                              num_padding_bits_(CalcPaddingBits(num_bits_)),
                                                              num_packed_bytes_(other.num_bytes()),
                                                              packed_bytes_(new PackedByte[num_packed_bytes_]){

    memcpy(packed_bytes_, other.packed_bytes(), num_packed_bytes_);
  }


  
  void PackedSequence::push_bits_right(PackedSequence::PackedByte byte, int num_bits){
    PackedByte *cur = packed_bytes_ + (num_packed_bytes_ -1);
    PackedByte transfer_mask = 255 >> (8 - num_bits );
    while( cur != packed_bytes_){
      *cur >>= num_bits;
      *cur |= (*(cur-1) & transfer_mask) << (8-num_bits);
      --cur;
    }
    packed_bytes_[num_packed_bytes_-1] &= (255 << num_padding_bits_); //remove extra bits in last byte
    *packed_bytes_ >>= num_bits;
    *packed_bytes_ |= byte << (8-num_bits);
  }
  
  void PackedSequence::push_bits_left(PackedSequence::PackedByte byte, int num_bits){
    PackedByte *cur = packed_bytes_;
    PackedByte *last = packed_bytes_ + (num_packed_bytes_ - 1);
    PackedByte transfer_mask = 255 << ( 8 - num_bits );
    
    while( cur != last ){
      *cur <<= num_bits;
      *cur |= (*(cur+1) & transfer_mask) >> (8 - num_bits);
      ++cur;
    }

    //special case where there an overhang into the last byte by less than 
    //the number of bits added
    int overhang = num_padding_bits_ == 0 ? 0 : (8 - num_padding_bits_);
    if(overhang < num_bits && num_packed_bytes_ > 1){ 
      *(last-1) |= (byte | ((255 >> (8 - (num_bits - overhang) )) << overhang)) >> overhang;
    }
    *last <<= num_bits;
    *last |= byte << num_padding_bits_;
  }


  /*  void PackedSequence::push_bits_slow(PackedSequence::PackedByte byte, int num_bits){
    for( int i=num_packed_bytes_-1; i>=1; --i){
      packed_bytes_[i] >>= num_bits;
      packed_bytes_[i] |= (packed_bytes_[i-1] & (255 >> (8-num_bits))) << (8-num_bits);
    }
    packed_bytes_[num_packed_bytes_-1] &= (255 << num_padding_bits_); //remove extra bits in last byte
    packed_bytes_[0] >>= num_bits;
    packed_bytes_[0] |= (byte << (8-num_bits));
    }*/

  PackedSequence::PackedByte PackedSequence::pop_bits(int num_bits){
    PackedSequence::PackedByte mask = 255 << (8-num_bits);
    PackedSequence::PackedByte output = (packed_bytes_[0] & mask) >> (8-num_bits);
    for(int i=0;i<num_packed_bytes_-1;++i){
      packed_bytes_[i] <<= num_bits;
      packed_bytes_[i] |= (packed_bytes_[i+1] & mask) >> (8-num_bits);
    }
    packed_bytes_[num_packed_bytes_-1] <<= num_bits;
    return output;
  }


  //doesn't handle byte boundaries well, only use num_bits that divide evenly into 8
  PackedSequence::PackedByte PackedSequence::bits_at(int index,int num_bits){
    assert(index * num_bits + num_bits <= num_bits_ );
    int byte_index = index / 8;
    int bit_index_in_byte = index % 8;
    PackedSequence::PackedByte mask = (255 << (8-num_bits)) >> bit_index_in_byte;
    return (packed_bytes_[byte_index] & mask) >> ( 8 - (bit_index_in_byte + num_bits));
  }

  int PackedSequence::num_bits() const{
    return num_bits_;
  }

  int PackedSequence::num_bytes() const{
    return num_packed_bytes_;
  }

  const PackedSequence::PackedByte *PackedSequence::packed_bytes() const{
    return packed_bytes_;
  }

  void PackedSequence::clear(){
    memset(packed_bytes_,0,num_packed_bytes_);
  }
  
  PackedSequence::~PackedSequence(){
    delete []packed_bytes_;
  }

  /**
   *Free Functions
   */


  

  int write_to_stream(const PackedSequence &sequence, FILE *stream){
    return fwrite(sequence.packed_bytes(),sizeof(char), sequence.num_bytes(), stream) * sizeof(char);
  }

}
