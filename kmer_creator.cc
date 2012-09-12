
#include "kmer_creator.h"


namespace rollercoaster{

  KmerCreator::KmerCreator(const std::string &read):read_(read){
  

  KmerCreator::set_read(const std::string &read){
    read_ = read;
  }


  //for KmerCreator iterator
  KmerCreator::const_packed_iterator::const_packed_iterator(const std::string &input_str, int kmer_size):
    read_(input_str),
    kmer_size_(kmer_size),
    packed_sequence_(kmer_size_*BitsPerBase){
      for(int i=0;i<kmer_size;++i){
        //push bits on
      }
  }

  KmerCreator::const_packed_iterator::operator*(){
    
    
  }


}//namespace rollercoaster
