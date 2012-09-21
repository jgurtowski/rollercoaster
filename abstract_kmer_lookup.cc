#include "abstract_kmer_lookup.h"
#include "kmer_creator.h"
#include "packed_kmer.h"
#include "reverse_complement.h"
#include "kmer_record.h"

namespace rollercoaster{

  void kmer_counts_for_read(const std::string &read, AbstractKmerLookup &lookup, std::vector<int> *out){
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


}//namespace rollercoaster
