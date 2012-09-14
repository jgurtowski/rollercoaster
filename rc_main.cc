#include <iostream>
#include <string>

#include "kmer_creator.h"

int main(void){
  
  namespace rc = rollercoaster;
  
  std::string read = "ACGTACGTACGTACGT";

  rc::KmerCreator kcreator(read, 5);
  
  
  rc::KmerCreator::const_iterator fb = kcreator.begin(), fe = kcreator.end();
  rc::KmerCreator::const_reverse_iterator rb = kcreator.rbegin(), re = kcreator.rend();

  std::cout << "read: " << read << std::endl;
  for(; fb != fe && rb != re; ++rb,++fb){
    std::cout << fb->str_kmer() << ":" << rb->str_kmer() << std::endl;
  }
      
  return 0;
}
