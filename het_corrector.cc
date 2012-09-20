#include <iostream>
#include <algorithm>

#include "abstract_kmer_lookup.h"
#include "coverage_segment.h"
#include "mmap_kmer_lookup.h"
#include "het_corrector.h"
#include "reverse_complement.h"

namespace rollercoaster{

  PthreadMutex HetCorrector::__io_mutex;

  void implement_read_changes(const std::vector<HetCorrector::ReadChange> &changes, FastqRecord *record){
    std::vector<HetCorrector::ReadChange>::const_iterator it = changes.begin(), end=changes.end();
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
    std::vector<ReadChange> changes;

    while(read_queue_->pop(&read_pair_)){
      //got the read, correct it and print 
      if(SUCCESS == correct_read(read_pair_.first.sequence(), lookup, &changes)){
        implement_read_changes(changes, &read_pair_.first);
      }

      if(SUCCESS == correct_read(read_pair_.second.sequence(), lookup, &changes)){
        implement_read_changes(changes,&read_pair_.second);
      }

      __io_mutex.lock();
      std::cout << read_pair_.first << std::endl;
      std::cout << read_pair_.second << std::endl;
      __io_mutex.unlock();
    }
  }

  int HetCorrector::correct_read(std::string read, AbstractKmerLookup &lookup, std::vector<ReadChange> *changes){

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
      kmer_counts_for_read(read_pair_.first.sequence(),lookup, &counts);
      segment_counts(counts,config_.segment_threshold, &segments);

      itb = segments.begin();
      ite = segments.end();

      strange_segments = 0;
      for(;itb != ite; ++itb){
        if(itb->length() > config_.min_segment_length && itb->mean >= config_.max_repeat_coverage)
          return REPEAT;
        else if(itb->length() <= config_.min_segment_length || itb->mean >= config_.max_repeat_coverage)
          ++strange_segments;
      }

      if(strange_segments >= config_.max_strange_segments)
        return STRANGE_SEGMENTS;

      for(int i=0;i<static_cast<int>(segments.size()); ++i){
        if( i>0 && segments[i].mean > segments[i-1].mean){ //check left segment
          curr_pos = segments[i-1].right;
          new_base = find_max_kmer_mutate_base(lookup,std::string(read,curr_pos,lookup.kmer_size()), FIRST);
          if(new_base != read[curr_pos]){
            ReadChange read_change = {curr_pos,read[curr_pos],new_base};
            changes->push_back(read_change);
          }
        }
        if( i< static_cast<int>(segments.size())-1 && segments[i].mean > segments[i+1].mean){ //check to right
          curr_pos = segments[i+1].left;
          mod_pos = curr_pos + lookup.kmer_size() -1 ;
          new_base = find_max_kmer_mutate_base(lookup,std::string(read,curr_pos,lookup.kmer_size()), LAST);
          if( new_base != read[mod_pos] ){
            ReadChange read_change = {mod_pos, read[mod_pos], new_base};
            changes->push_back(read_change);
          }
        }
      }

      if(++num_iterations >= config_.max_corrections+1 )
        return TOO_MANY_CHANGES;

    }while(!std::equal(old_segments.begin(),old_segments.end(),segments.begin()));

    return SUCCESS;
  }


  char HetCorrector::find_max_kmer_mutate_base(AbstractKmerLookup &lookup, std::string kmer, BaseToModify mod_base){

    std::string rev_kmer;
    rev_kmer.resize(kmer.size());
    reverse_complement(kmer.rbegin(),kmer.rend(), rev_kmer.begin());

    int max = -1;
    char max_letter = kmer[0];

    PackedKmer packed_kmer(lookup.kmer_size());
    for(int i=0;i<4; ++i){
      if(mod_base == FIRST){
        kmer[0] = PackedKmer::decode_base(i);
        rev_kmer[rev_kmer.size()-1] = complement_lookup(i);
      }else{
        kmer[kmer.size()-1] = PackedKmer::decode_base(i);
        rev_kmer[0] = complement_lookup(i);
      }

      packed_kmer.set_kmer<std::string>(kmer.begin(),kmer.end());
      if(lookup.has_record(packed_kmer) && lookup.last_record().count() > max){
        max = lookup.last_record().count();
        max_letter = kmer[0];
      }

      packed_kmer.set_kmer<std::string>(rev_kmer.begin(),rev_kmer.end());
      if(lookup.has_record(packed_kmer) && lookup.last_record().count() > max){
        max = lookup.last_record().count();
        max_letter = kmer[0];
      }

    }
    return max_letter;
  }

}//namespace rollercoaster
