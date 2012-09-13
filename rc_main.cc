#include <iostream>
#include <string>

#include "packed_kmer.h"

int main(void){
  
  std::string kmer = "ACGTACGTACGTACGT";

  rollercoaster::PackedKmer p_kmer(kmer.size());
  p_kmer.set_kmer(kmer.begin(), kmer.end());
  std::cout << p_kmer.packed_bytes() << std::endl;

  std::string output;
  p_kmer.str_kmer(&output);
  std::cout << output << std::endl;
    
  return 0;
}
