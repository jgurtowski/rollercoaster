#ifndef PACKED_SEQUENCE_H
#define PACKED_SEQUENCE_H

namespace rollercoaster{

  /**
   *Packed DNA sequence
   */
  class PackedSequence{

    typedef unsigned char PackedByte;
    
    /**
     *How many bits will be handeled by the sequence
     */
    PackedSequence(int required_bits);
    
    /**
     *Clears the internal pack buffer
     */
    void clear();

    /**
     *Pushes the first (lowest order (right->left)) num_bits onto the sequence
     * if byte is 00000011 and num_bits = 3 
     * the bits 011 will be pushed onto the left of the bit array
     */
    void push_bits(PackedByte byte, int num_bits);
    
    /**
     *Return the number of bits handeled by this sequence
     */
    int num_bits() const;

    /**
     *Return the number of bytes used to hold the bits
     */
    int num_bytes() const;

    /**
     *Return pointer to unmodifiable view of the packed byte sequence
     */
    const PackedByte *packed_bytes() const;
        

    ~PackedSequence();

  private:
    int num_bits_;
    int num_extra_bits;
    int num_packed_bytes_;
    PackedByte extra_bit_mask_;
    PackedByte *packed_bytes_;
  };

}
#endif
