#include <string.h>
#include <stdio.h>

#include "kmer_record.h"

namespace rollercoaster{


  int KmerRecord::read_from_binary_stream(char *stream){
    int read = kmer_.read_from_stream(stream);
    memcpy(&count_,stream + read, sizeof(count_)); 
    return read + sizeof(count_);
  }


  int write_to_binary_stream(const KmerRecord &kmer, FILE *out){
    int nb = write_to_stream(kmer.kmer(), out);
    int count = kmer.count();
    int written = fwrite(&count,sizeof(KmerRecord::KmerCount),1,out) * sizeof(KmerRecord::KmerCount);
    return (nb + written);
  }

  std::ostream &operator << (std::ostream &out, const KmerRecord &kmer){
    return out << kmer.kmer() << "\t" << kmer.count();
  }

  
} //namespace rollercoaster
