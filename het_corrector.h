#ifndef HET_CORRECTOR_H
#define HET_CORRECTOR_H

#include <set>
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

      HetCorrector(MMapFile &mapped_file, const CorrectorConfig &config):
        mapped_file_(mapped_file),
        read_pair_(),
        config_(config){}

        virtual void run();
        virtual ~HetCorrector(){}

        /**
         *Operate on a copy of the read and return a set of changes
         *@return 0: success , 1:failure
         */
        CorrectionStat correct_read(std::string read, AbstractKmerLookup &lookup, std::set<ReadChange> *changes);

        /**
         *Find kmer with highest coverage
         */
        char find_max_kmer_mutate_base(AbstractKmerLookup &lookup, std::string kmer, BaseToModify mod_base);

	inline void set_debug(bool debug){debug_ = debug;}

  private:
        MMapFile &mapped_file_;
        ThreadedQueue<ReadPair> *read_queue_;
        ReadPair read_pair_;
        CorrectorConfig config_;
	bool debug_;
        //serialize io from multiple correctors
        static PthreadMutex __io_mutex;

  }; //class HetCorrector


  /**
   *Free Functions
   */

  void implement_read_changes(const std::set<HetCorrector::ReadChange> &changes, FastqRecord *record);

  std::ostream &operator << (std::ostream &out, const HetCorrector::ReadChange &change);

  bool operator < (const HetCorrector::ReadChange &lhs, const HetCorrector::ReadChange &rhs);

}//namespace rollercoaster



#endif
