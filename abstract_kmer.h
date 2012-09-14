#ifndef ABSTRACT_KMER_H
#define ABSTRACT_KMER_H

#include <string>

namespace rollercoaster{

  class AbstractKmer{

  public:
    virtual std::string str_kmer() = 0;
    virtual ~AbstractKmer(){}

  };//class AbstractKmer

}//namespace rollercoaster

#endif
