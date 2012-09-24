#ifndef KMER_CREATOR_H
#define KMER_CREATOR_H

#include <string>
#include <list>

#include "list_backed_kmer.h"
#include "packed_kmer.h"



namespace rollercoaster{

  class AbstractKmer;

  /**
   *Class:KmerCreator
   * Creates kmers from a given string.
   * To actually get the kmers use one of the iterator implementations
   */

  class KmerCreator{
  public:


    /**
     *Class: base_iterator
     * base class iterator text kmers
     */
    class base_iterator{

    public:
      base_iterator(const std::string &input_str, int kmer_size, int kmer_index);

      virtual const base_iterator &operator++() = 0;
      virtual const AbstractKmer &operator*() = 0;
      virtual const AbstractKmer *operator->() = 0;
      virtual bool operator==(const base_iterator &rhs);
      virtual bool operator!=(const base_iterator &rhs);

      inline const std::string &read() const{ return read_;}
      inline int index() const { return kmer_idx_; }
      inline int kmer_size() const {return kmer_size_;}


    protected:
      const std::string &read_;
      int kmer_size_;
      int kmer_idx_;

      //this class should be subclassed
      //but not support polymorphism
      //no pointers to base class
      //use derived class only
      virtual ~base_iterator(){}

    };//class base_iterator


    /**
     *Class: const_iterator
     * Forward iterator that gives an unmodifiable kmer view of the string
     */

    class const_iterator: public base_iterator{

    public:

    const_iterator(const std::string &input_str, int kmer_size, int kmer_index):
      base_iterator(input_str, kmer_size, kmer_index),kmer_(kmer_size_){}

      const const_iterator &operator++();
      const ListBackedKmer &operator*();
      const ListBackedKmer *operator->();

      void load_data();

    private :
      ListBackedKmer kmer_;
      
    };//class const_iterator



    /**
     *Class: const_reverse_iterator
     * Forward iterator that gives an unmodifiable kmer view of the string
     */

    class const_reverse_iterator: public base_iterator{
      
    public:

    const_reverse_iterator(const std::string &input_str, int kmer_size, int kmer_index):
      base_iterator(input_str, kmer_size, kmer_index),kmer_(kmer_size_){}

      const const_reverse_iterator &operator++();
      const ListBackedKmer &operator*();
      const ListBackedKmer *operator->();

      void load_data();

    private :
      ListBackedKmer kmer_;
      
    };//class const_iterator




    /**
     *Class: const_packed_reverse_iterator
     *Iterate through Kmers, provide a packed version of each kmer
     *This is a reverse iterator because it is more efficient to
     *operate on the kmers in reverse due to the inner workings of
     *how things are packed
     */
    class const_packed_reverse_iterator: public base_iterator{
    public:

    const_packed_reverse_iterator(const std::string &input_str, int kmer_size, int kmer_index):
      base_iterator(input_str,kmer_size,kmer_index),packed_kmer_(kmer_size_){}

      virtual const const_packed_reverse_iterator &operator++();
      virtual const PackedKmer &operator*();
      virtual const PackedKmer *operator->();


    private:

      void load_data();
      PackedKmer packed_kmer_;
    };//class const_packed_reverse_iterator



    /**
     *Class: const_packed_iterator
     *Iterate through Kmers, provide a packed version of each kmer
     */
    class const_packed_iterator: public base_iterator{
    public:

    const_packed_iterator(const std::string &input_str, int kmer_size, int kmer_index):
      base_iterator(input_str,kmer_size,kmer_index),packed_kmer_(kmer_size_){}

      virtual const const_packed_iterator &operator++();
      virtual const PackedKmer &operator*();
      virtual const PackedKmer *operator->();

    private:

      void load_data();
      PackedKmer packed_kmer_;
    };//class const_packed_iterator


    KmerCreator(const std::string &read, int kmer_size);

    //Set the next read to be kmerized
    void set_read(const std::string &read);


    /**
     *Get const_packed_reverse_iterator to the beginning
     * (remember this is a reverse iterator)
     */
    inline const_packed_reverse_iterator packed_rbegin(){
      return const_packed_reverse_iterator(read_, kmer_size_, read_.length() - kmer_size_);
    }

    /**
     *Get const_packed_reverse_iterator to the end
     * (remember this is a reverse iterator)
     */
    inline const_packed_reverse_iterator packed_rend(){
      return const_packed_reverse_iterator(read_, kmer_size_, -1);
    }
    
    
    /**
     *Get const_packed_iterator to the beginning
     */
    inline const_packed_iterator packed_begin(){
      return const_packed_iterator(read_, kmer_size_, 0);
    }

    /**
     *Get const_packed_iterator to the end
     */
    inline const_packed_iterator packed_end(){
      return const_packed_iterator(read_, kmer_size_, read_.length() - kmer_size_ + 1);
    }
    
        
    /**
     *Get const_iterator to the beginning 
     */
    inline const_iterator begin(){
      return const_iterator(read_, kmer_size_, 0);
    }

    
    /**
     *Get const_iterator to end
     */
    inline const_iterator end(){
      return const_iterator(read_,kmer_size_, read_.length() - kmer_size_ + 1);
    }

    /**
     *Get const_reverse_iterator to the beginning
     */
    inline const_reverse_iterator rbegin(){
      return const_reverse_iterator(read_,kmer_size_, read_.length() - kmer_size_);
    }

    /**
     *Get const_reverse_iterator to the end
     */
    inline const_reverse_iterator rend(){
      return const_reverse_iterator(read_, kmer_size_, -1);
    }
    
    
  private:
    std::string read_;
    int kmer_size_;

    //disallow copying because we work from a reference to an outside string
    KmerCreator(const KmerCreator &);
    KmerCreator &operator=(const KmerCreator &);

  };//class KmerCreator

}//namespace rollercoaster

#endif
