#ifndef PACKED_SEQUENCE_H
#define PACKED_SEQUENCE_H

namespace rollercoaster{

  /**
   *Pack Sequence of bits
   */
  class PackedSequence{

  public:
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
     *Pops bits from the underlying bit array
     *Bits are popped from the left most bit in the left most byte (array style)
     */
    PackedByte pop_bits(int num_bits);
    
    /**
     *Retrieves the bits at position index without modifying
     *the underlying bit array.
     *The bits will be moved to the least significant position
     *on the returned PackedByte
     * Warning: Doesn't work for odd number num_bits (must
     *divide evenly into 8
     */
    PackedByte bits_at(int index,int num_bits);

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

    /**
     *Static utility function for calculating the number of
     * bytes you need to hold a certain number of bits
     */
    static inline int CalcBytesForBits(int bits){ return (8 / bits + (8 % bits > 0 ? 1 : 0));} 


    /**
     *Compare method for comparing two packed sequences
     */
    friend int compare(const PackedSequence &lhs, const PackedSequence &rhs);


    virtual ~PackedSequence();

  protected:
    /**
     *Copy constructor
     */
    PackedSequence(const PackedSequence &other);
    
    PackedByte *packed_bytes_;

  private:
    int num_bits_;
    int num_padding_bits_;
    int num_packed_bytes_;

    
    //disallow assignment
    const PackedSequence &operator=(const PackedSequence &);

  }; //class PackedSequence

}//namespace rollercoaster
#endif
