#include <string.h>

#include "kmer_record.h"

namespace rollercoaster{


  int KmerRecord::read_from_stream(char *stream){
    int read = kmer_.read_from_stream(stream);
    memcpy(&count_,stream + read, sizeof(count_)); 
    return read + sizeof(count_);
  }


} //namespace rollercoaster
