#include <iostream>
#include <algorithm>

#include "abstract_kmer_lookup.h"
#include "coverage_segment.h"
#include "mmap_kmer_lookup.h"
#include "het_corrector.h"
#include "reverse_complement.h"
#include "print_func.h"

namespace rollercoaster{

  PthreadMutex HetCorrector::__io_mutex;

  void implement_read_changes(const std::set<HetCorrector::ReadChange> &changes, FastqRecord *record){
    std::set<HetCorrector::ReadChange>::const_iterator it = changes.begin(), end=changes.end();
    for(;it != end; ++it){
      record->set_sequence_char( it->position, it->new_base);
      record->set_quality_char( it->position, ':' );
    }
  }

  void HetCorrector::run(){
    if(!mapped_file_.is_mapped()){
      std::cerr << "db not mapped" << std::endl;
      return;
    }
    
    MMapKmerLookup lookup( mapped_file_ );
    std::set<ReadChange> changes;
    
    HetCorrector::CorrectionStat stat;
    while(read_queue_->pop(&read_pair_)){
      
      //got the read, correct it and print 
      if(SUCCESS == (stat = correct_read(read_pair_.first.sequence(), lookup, &changes))){
        implement_read_changes(changes, &read_pair_.first);
      }
      
      if(SUCCESS == (stat = correct_read(read_pair_.second.sequence(), lookup, &changes))){
        implement_read_changes(changes,&read_pair_.second);
      }
      
      __io_mutex.lock();
      std::cout << read_pair_.first << std::endl;
      std::cout << read_pair_.second << std::endl;
      __io_mutex.unlock();
      
    }
    
  }

  HetCorrector::CorrectionStat HetCorrector::correct_read(std::string read, AbstractKmerLookup &lookup, std::set<ReadChange> *changes){

    changes->clear();

    std::vector<int> counts;
    std::vector<Segment> segments;
    std::vector<Segment> old_segments;

    std::vector<Segment>::iterator itb, ite;

    int strange_segments;
    int curr_pos, mod_pos;
    char new_base;
    int num_iterations =0;
    do{
      old_segments = segments;
      kmer_counts_for_read(read,lookup, &counts);
      segment_counts(counts,config_.segment_threshold, &segments);

      if(debug_)
	std::cout<< read << "\t" << counts << "\t" << segments << std::endl;

      itb = segments.begin();
      ite = segments.end();

      strange_segments = 0;
      for(;itb != ite; ++itb){
        if(itb->length() > config_.min_segment_length && itb->mean >= config_.max_repeat_coverage){
	  if(debug_)
	    std::cout << "REPEAT" << std::endl;
	  return REPEAT;
	}
        else if(itb->length() <= config_.min_segment_length || itb->mean >= config_.max_repeat_coverage)
          ++strange_segments;
      }

      if(strange_segments >= config_.max_strange_segments){
        if(debug_)
	  std::cout << "Strange segments" << std::endl;
	return STRANGE_SEGMENTS;
      }      
	
      for(int i=0;i<static_cast<int>(segments.size()); ++i){
        if( i>0 && segments[i].mean > segments[i-1].mean){ //check left segment
          curr_pos = segments[i-1].right;
	  new_base = find_max_kmer_mutate_base(lookup,std::string(read,curr_pos,lookup.kmer_size()), FIRST);
	  if(new_base != read[curr_pos]){
            ReadChange read_change = {curr_pos,read[curr_pos],new_base};
            changes->insert(read_change);
	    read[curr_pos] = new_base;
          }

        }
        if( i< static_cast<int>(segments.size())-1 && segments[i].mean > segments[i+1].mean){ //check to right
	  curr_pos = segments[i+1].left;
          mod_pos = curr_pos + lookup.kmer_size() -1 ;
	  new_base = find_max_kmer_mutate_base(lookup,std::string(read, curr_pos, lookup.kmer_size()), LAST);
          if( new_base != read[mod_pos] ){
            ReadChange read_change = {mod_pos, read[mod_pos], new_base};
            changes->insert(read_change);
	    read[mod_pos] = new_base;
          }
        }
      }

      if(debug_)
	std::cout << "Changes: " << *changes << std::endl;

      if(++num_iterations >= config_.max_corrections+1 ){
	if(debug_)
	  std::cout << "Too many Changes " << std::endl;
        return TOO_MANY_CHANGES;
      }
    }while(segments.size() != old_segments.size() || !std::equal(segments.begin(), segments.end(), old_segments.begin()));

    if(debug_)
      std::cout << "SUCCESS" << std::endl;
    return SUCCESS;
  }


  char HetCorrector::find_max_kmer_mutate_base(AbstractKmerLookup &lookup, std::string kmer, BaseToModify mod_base){
    
    std::string rev_kmer;
    rev_kmer.resize(kmer.size());
    reverse_complement(kmer.rbegin(),kmer.rend(), rev_kmer.begin());

    int max = -2;
    char max_letter = mod_base == FIRST ? kmer[0] : kmer[kmer.size()-1];
    
    PackedKmer packed_kmer(lookup.kmer_size());
    for(int i=0;i<5; ++i){
      if(mod_base == FIRST){
        kmer[0] = PackedKmer::decode_base(i);
        rev_kmer[rev_kmer.size()-1] = complement_lookup(PackedKmer::decode_base(i));
      }else{
	kmer[kmer.size()-1] = PackedKmer::decode_base(i);
        rev_kmer[0] = complement_lookup(PackedKmer::decode_base(i));
      }

      packed_kmer.set_kmer<std::string>(kmer.begin(),kmer.end());
      if(lookup.has_record(packed_kmer) && lookup.last_record().count() > max){
	max = lookup.last_record().count();
        max_letter = mod_base == FIRST ? kmer[0] : kmer[kmer.size()-1];
      }
      
      packed_kmer.set_kmer<std::string>(rev_kmer.begin(),rev_kmer.end());
      if(lookup.has_record(packed_kmer) && lookup.last_record().count() > max){
	max = lookup.last_record().count();
        max_letter = mod_base == FIRST ? kmer[0] : kmer[kmer.size()-1];
      }
    }
    return max_letter;
  }


  std::ostream &operator << (std::ostream &out, const HetCorrector::ReadChange &change){
    out << "{" << change.position <<  "," << change.old_base << "," << change.new_base << "}";
    return out;
  }

  bool operator<(const HetCorrector::ReadChange &lhs, const HetCorrector::ReadChange &rhs){
    return lhs.position < rhs.position;
  }


}//namespace rollercoaster















