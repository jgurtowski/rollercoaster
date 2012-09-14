
#include "list_backed_kmer.h"


namespace rollercoaster{

  void ListBackedKmer::str_kmer(std::string &out) const{
    if(!(static_cast<int>(out.size()) == kmer_size_))
      out.resize(kmer_size_);
    std::copy(list_kmer_.begin(), list_kmer_.end(), std::back_inserter(out));
  }

  std::string ListBackedKmer::str_kmer() const{
    std::string t;
    str_kmer(t);
    return t;
  }

}//namespace rollercoaster

