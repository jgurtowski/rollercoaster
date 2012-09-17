#include <list>

#include "list_backed_kmer.h"


namespace rollercoaster{

  void ListBackedKmer::str_kmer(std::string &out) const{
    out.resize(kmer_size_);
    out.erase();
    std::copy(list_kmer_.begin(), list_kmer_.end(), std::back_inserter(out));
  }

  std::string ListBackedKmer::str_kmer() const{
    std::string t;
    str_kmer(t);
    return t;
  }


  int compare(const ListBackedKmer &lhs, const ListBackedKmer &rhs){
    std::list<char>::const_iterator lbegin = lhs.list_kmer_.begin(), 
      lend = lhs.list_kmer_.end(), 
      rbegin = rhs.list_kmer_.begin();
    
    for(;lbegin != lend; ++lbegin, ++rbegin){
      if(*lbegin < *rbegin)
        return -1;
      else if(*lbegin > *rbegin)
        return 1;
    }
    return 0;
  }

}//namespace rollercoaster

