#ifndef REVERSE_COMPLEMENT_H
#define REVERSE_COMPLEMENT_H

#include <assert.h>

#include <algorithm>
#include <string>

namespace rollercoaster{

  static const char RCLookupTable[] = {
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,'T',0,'G',0,0,
    0,'C',0,0,0,0,0,0,'N',0,
    0,0,0,0,'A'
  };


  inline char complement_lookup(char base){
    assert(base >= 65 && base <=84); //only A->T
    return RCLookupTable[static_cast<int>(base)];
  }  

  /**
   *Reverse Complement, give a begin and end input iterator
   *then a single output iterator
   */
  inline void reverse_complement(std::string::const_reverse_iterator rbegin,
                                 std::string::const_reverse_iterator rend,
                                 std::string::iterator out){
    std::transform(rbegin,rend,out,complement_lookup);
  }

  
}//namespace rollercoaster


#endif

