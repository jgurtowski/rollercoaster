#include <iostream>
#include <string>

#include <algorithm>

#include "kmer_creator.h"
#include "reverse_complement.h"

int main(void){
  
  namespace rc = rollercoaster;
  
  std::string read = "ACCTTTCCCGTNG";
  std::string read_rc;
  read_rc.resize(read.size());
  rc::reverse_complement(read.rbegin(),read.rend(), read_rc.begin());
  std::cout << "read: " << read << std::endl;
  std::cout << "re_c: " << read_rc << std::endl;
  
  rc::KmerCreator kcreator_forward(read, 5);
  rc::KmerCreator kcreator_reverse(read_rc,5);

    
  rc::KmerCreator::const_iterator fb = kcreator_forward.begin(), fe = kcreator_forward.end();
  rc::KmerCreator::const_reverse_iterator rb = kcreator_reverse.rbegin(), re = kcreator_reverse.rend();

  
  for(; fb != fe && rb != re; ++rb,++fb){
    std::cout << fb->str_kmer() << ":" << rb->str_kmer() << std::endl;
  }
  return 0;
}
