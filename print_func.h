#ifndef PRINT_FUNC_H
#define PRINT_FUNC_H

#include <iostream>
#include <vector>
#include <set>

namespace rollercoaster{


  
  /**
   *Free Functions
   */

  template<class T>
    std::ostream &operator << (std::ostream &out, const std::vector<T> &vec){
    typename std::vector<T>::const_iterator b = vec.begin(), e = vec.end();
    out << "[ ";
    for(; b != e; ++b){
      out << *b << " ";
    }
    out << "]";
    return out;
  }

  template<class T>
    std::ostream &operator << (std::ostream &out, const std::set<T> &s){
    typename std::set<T>::const_iterator b = s.begin(), e = s.end();
    out << "{ ";
    for(; b != e; ++b){
      out << *b << " ";
    }
    out << "}";
    return out;
  }


}//namespace rollercoaster
#endif
