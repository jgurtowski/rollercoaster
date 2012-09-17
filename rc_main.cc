#include <iostream>
#include <string>

#include "packed_kmer.h"
#include "list_backed_kmer.h"
#include "kmer_creator.h"
#include "reverse_complement.h"

int main(void){
  
  namespace rc = rollercoaster;
  
  int kmer_size = 5;

  std::string read = "ACCTTTCCCGTNG";
  std::string read_rc;
  read_rc.resize(read.size());
  rc::reverse_complement(read.rbegin(),read.rend(), read_rc.begin());
  std::cout << "read: " << read << std::endl;
  std::cout << "re_c: " << read_rc << std::endl;
  
  rc::KmerCreator kcreator_forward(read, kmer_size);
  rc::KmerCreator kcreator_reverse(read_rc,kmer_size);
    
  rc::KmerCreator::const_iterator fb = kcreator_forward.begin(), fe = kcreator_forward.end();
  rc::KmerCreator::const_reverse_iterator rb = kcreator_reverse.rbegin(), re = kcreator_reverse.rend();
  
  int diff;
  rc::PackedKmer packed_kmer_f(kmer_size), packed_kmer_r(kmer_size);
  for(; fb != fe && rb != re; ++rb,++fb){
    packed_kmer_f.set_kmer<rc::ListBackedKmer::list_type>(fb->begin(),fb->end());
    packed_kmer_r.set_kmer<rc::ListBackedKmer::list_type>(rb->begin(),rb->end());
    diff = rc::compare(*fb,*rb);
    std::cout << fb->str_kmer() << ":" << rb->str_kmer() << ":" << (diff < 0 ? packed_kmer_f.str_kmer() : packed_kmer_r.str_kmer()) << std::endl;
  }
  return 0;
}
