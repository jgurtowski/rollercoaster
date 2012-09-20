#ifndef HET_CORRECTOR_H
#define HET_CORRECTOR_H

#include <vector>
#include <utility>

#include "abstract_kmer_lookup.h"
#include "mmap_file.h"
#include "pthread_mutex.h"
#include "threaded_queue.h"
#include "runnable.h"
#include "fastq_record.h"

namespace rollercoaster{

  class HetCorrector: public Runnable{
  public:
    typedef std::pair<FastqRecord, FastqRecord> ReadPair;

      enum BaseToModify{
        FIRST,
        LAST
      };
      
      enum CorrectionStat{
        SUCCESS,
        REPEAT,
        STRANGE_SEGMENTS,
        TOO_MANY_CHANGES
      };

      /**
       *A change to a read
       */
      struct ReadChange{
        int position;
        char old_base;
        char new_base;
      };

      /**
       *Structure to configure the corrector
       */
      struct CorrectorConfig{
        int max_corrections;
        int max_strange_segments;
        int min_segment_length;
        int max_repeat_coverage;
        float segment_threshold;
      };
      
  HetCorrector(MMapFile &mapped_file, const CorrectorConfig &config, ThreadedQueue<ReadPair> *read_queue):
      mapped_file_(mapped_file),
        read_queue_(read_queue),
        read_pair_(),
        config_(config){}


    virtual void run();
    virtual ~HetCorrector(){}

    /**
     *Operate on a copy of the read and return a vector of changes
     *@return 0: success , 1:failure
     */
    int correct_read(std::string read, AbstractKmerLookup &lookup, std::vector<ReadChange> *changes);

    /**
     *Find kmer with highest coverage
     */
    char find_max_kmer_mutate_base(AbstractKmerLookup &lookup, std::string kmer, BaseToModify mod_base);

  private:
    MMapFile &mapped_file_;
    ThreadedQueue<ReadPair> *read_queue_;
    ReadPair read_pair_;
    CorrectorConfig config_;
    
    //serialize io from multiple correctors
    static PthreadMutex __io_mutex;

  }; //class HetCorrector


  /**
   *Free Functions
   */
  
  void implement_read_changes(const std::vector<HetCorrector::ReadChange> &changes, FastqRecord *record);
  

}//namespace rollercoaster



#endif
