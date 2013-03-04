#include <algorithm>

#include "abstract_kmer_lookup.h"
#include "kmer_creator.h"
#include "packed_kmer.h"
#include "reverse_complement.h"
#include "kmer_record.h"

namespace rollercoaster{

  /*
    void kmer_counts_for_read_slow(const std::string &read, AbstractKmerLookup &lookup, std::vector<int> *out){
    out->clear();
    
    std::string reverse;
    reverse.resize(read.size()); //not pretty
    KmerCreator fw(read, lookup.kmer_size());
    reverse_complement(read.rbegin(),read.rend(), reverse.begin());
    KmerCreator rv(reverse, lookup.kmer_size());
    
    KmerCreator::const_iterator fw_b = fw.begin(), fw_e = fw.end();
    KmerCreator::const_reverse_iterator rv_b = rv.rbegin(), rv_e = rv.rend(); 

    int diff;
    PackedKmer packed_kmer(lookup.kmer_size());
    for(;fw_b != fw_e && rv_b != rv_e; 
        ++fw_b, ++rv_b){
      diff = compare(*fw_b, *rv_b);
      if(diff < 0)
        packed_kmer.set_kmer<ListBackedKmer::list_type>(fw_b->begin(), fw_b->end());
      else
        packed_kmer.set_kmer<ListBackedKmer::list_type>(rv_b->begin(), rv_b->end());
      out->push_back( lookup.has_record(packed_kmer) ? lookup.last_record().count():-2 );
    }
    }
  */

  void kmer_counts_for_read(const std::string &read, AbstractKmerLookup &lookup, std::vector<int> *out){

    out -> clear();
    std::string reverse;
    reverse.resize(read.size());
    reverse_complement(read.rbegin(),read.rend(), reverse.begin());

    KmerCreator fw(read, lookup.kmer_size());
    KmerCreator rv(reverse, lookup.kmer_size());

    KmerCreator::const_packed_reverse_iterator fw_b = fw.packed_rbegin(), fw_e = fw.packed_rend();
    KmerCreator::const_packed_iterator rv_b = rv.packed_begin(), rv_e = rv.packed_end();
    //int diff;
    int count;
    for(; fw_b != fw_e && rv_b != rv_e; ++fw_b, ++rv_b){

      count = 0;
      //lookup both forward and reverse kmer
      if(lookup.has_record(*fw_b)){
	count += lookup.last_record().count();
      }
      if(lookup.has_record(*rv_b)){
	count += lookup.last_record().count();
      }
      out->push_back(count);
      
      ///lookup only the alphabetically lower kmer
      /*diff = compare( *fw_b, *rv_b);
      if(diff < 0)
	out->push_back(lookup.has_record(*fw_b) ? lookup.last_record().count(): -2 );
      else
      out->push_back(lookup.has_record(*rv_b) ? lookup.last_record().count(): -2 );
      */

    }
    std::reverse(out->begin(), out->end());
  }


}//namespace rollercoaster
