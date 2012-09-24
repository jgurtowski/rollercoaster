#ifndef PACKED_SEQUENCE_H
#define PACKED_SEQUENCE_H

#include <stdio.h>


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
    void push_bits_right(PackedByte byte, int num_bits);
    void push_bits_left(PackedByte byte, int num_bits);
  

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
    static inline int CalcBytesForBits(int bits){ return (bits / 8 + ((bits % 8) > 0 ? 1 : 0));} 

    /**
     *Calculate the padding bits that are unused at the end of the bit array
     */
    static inline int CalcPaddingBits(int bits){ return (bits % 8 > 0 ? (8 - (bits % 8)):0);}
    

    /**
     *Compare method for comparing two packed sequences
     */
    friend int compare(const PackedSequence &lhs, const PackedSequence &rhs);


    virtual ~PackedSequence();

  private:
    int num_bits_;
    int num_padding_bits_;
    int num_packed_bytes_;

    //disallow assignment
    const PackedSequence &operator=(const PackedSequence &);

  protected:
    /**
     *Copy constructor
     */
    PackedSequence(const PackedSequence &other);
    
    PackedByte *packed_bytes_;


  }; //class PackedSequence


  /**
   *Free Functions
   */
  
  /**
   *Compare two packed sequences by their raw bytes
   */
  int compare(const PackedSequence &lhs, const PackedSequence &rhs);

  /**
   *Write bytes to stream
   *@return number of bytes written to stream
   */
  
  int write_to_stream(const PackedSequence &sequence, FILE *out);

// ============================================================================
//                            INLINE DEFINITIONS
// ============================================================================

// FREE FUNCTIONS
inline
int compare(const PackedSequence &lhs, const PackedSequence &rhs)
{
    const PackedSequence::PackedByte *f = lhs.packed_bytes_ ,
          *s = rhs.packed_bytes_,
          *e = lhs.packed_bytes_ + lhs.num_packed_bytes_ ;
    int result = 0;
    while(!result && f != e ){
        result = *f++ - *s++;
    }
    return result;
}


}//namespace rollercoaster
#endif
