#ifndef LIST_BACKED_KMER_H
#define LIST_BACKED_KMER_H

#include <iterator>
#include <deque>
#include <string>
#include <iosfwd>

#include "abstract_kmer.h"

namespace rollercoaster{

  /**
   *Simple fixed length Kmer representation
   *backed by a double linked list
   */
  class ListBackedKmer: public AbstractKmer{

  public:

    typedef std::deque<char> list_type;

  ListBackedKmer(int kmer_size): is_set_(false),kmer_size_(kmer_size),list_kmer_(){}

    inline bool is_set() const {return is_set_;}

    inline void add_base_left(char base){
      list_kmer_.pop_back();
      list_kmer_.push_front(base);
    }

    inline void add_base_right(char base){
      list_kmer_.pop_front();
      list_kmer_.push_back(base);
    }

    inline list_type::const_iterator begin() const {
      return list_kmer_.begin();
    }

    inline list_type::const_iterator end() const {
      return list_kmer_.end();
    }

    /**
     *Set Kmer with char iterators
     */
    template<class T>
      void set_kmer(typename T::const_iterator begin,
                    typename T::const_iterator end){
      std::copy(begin, end, std::back_inserter(list_kmer_));
      is_set_=true;
    }


    friend int compare(const ListBackedKmer &lhs, const ListBackedKmer &rhs);

    virtual std::string str_kmer(){ return const_cast<const ListBackedKmer *>(this)->str_kmer();}

    std::string str_kmer() const;

    void str_kmer(std::string &out) const;




  private:
    bool is_set_;
    int kmer_size_;
    list_type list_kmer_;

  }; //class ListBackedKmer

  /***
   *Free Functions
   */

  int compare(const ListBackedKmer &lhs, const ListBackedKmer &rhs);


}//namespace rollercoaster


#endif
