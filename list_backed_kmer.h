#ifndef LIST_BACKED_KMER_H
#define LIST_BACKED_KMER_H

#include <iterator>
#include <list>
#include <string>

#include "abstract_kmer.h"

namespace rollercoaster{

  /**
   *Simple fixed length Kmer representation
   *backed by a double linked list
   */
  class ListBackedKmer: public AbstractKmer{

  public:

  ListBackedKmer(int kmer_size): is_set_(false),kmer_size_(kmer_size),list_kmer_(){}

    inline void set_kmer(std::string::const_iterator begin,
                         std::string::const_iterator end){
      std::copy(begin, end, std::back_inserter(list_kmer_));
      is_set_=true;
    }

    inline bool is_set() const {return is_set_;}

    inline void add_base_left(char base){
      list_kmer_.pop_back();
      list_kmer_.push_front(base);
    }

    inline void add_base_right(char base){
      list_kmer_.pop_front();
      list_kmer_.push_back(base);
    }

    virtual std::string str_kmer(){ return const_cast<const ListBackedKmer *>(this)->str_kmer();}
    
    std::string str_kmer() const;
    
    void str_kmer(std::string &out) const;


  private:
    bool is_set_;
    int kmer_size_;
    std::list<char> list_kmer_;
    

  }; //class ListBackedKmer

}//namespace rollercoaster


#endif
