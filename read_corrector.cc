#include <stdlib.h>

#include <iostream>
#include <utility>

#include "fastq_record.h"
#include "het_corrector.h"
#include "thread_executor.h"
#include "threaded_queue.h"


int main(int argc, char *argv[]){
  
  namespace rc = rollercoaster;

  if(!(argc == 5)){
    std::cerr << "readcorrector <1.fq> <2.fq> <in.db> <num_threads>" << std::endl;
    return 1;
  }
  
  std::string fastq_1(argv[1]);
  std::string fastq_2(argv[2]);
  std::string db_filename(argv[3]);
  int num_threads = atoi(argv[4]);

  rc::FastqRecordReader reader_1(fastq_1);
  rc::FastqRecordReader reader_2(fastq_2);
  rc::MMapFile mmap_db(db_filename);

  if(!reader_1.is_open()){
    std::cerr << "unable to open " << fastq_1 << std::endl;
    return 1;
  }
  if(!reader_2.is_open()){
    std::cerr << "unable to open " << fastq_2 << std::endl;
    return 1;
  }

  if(mmap_db.map()){
    std::cerr << "unable to map file" << db_filename << std::endl;
    return 1;
  }

  rc::ThreadedQueue<rc::HetCorrector::ReadPair> read_queue;
  
  rc::HetCorrector::CorrectorConfig corrector_conf = { 
    /*.max_corrections =*/ 8,
    /*.max_strange_segments =*/ 4,
    /*.min_segment_length =*/ 3,
    /*.max_repeat_coverage =*/ 500,
    /*.segment_threshold = */ 17.5
  };
  
  rc::HetCorrector corrector(mmap_db, corrector_conf, &read_queue); 
  
  rc::ThreadExecutor<rc::HetCorrector> executor;

  executor.execute(corrector, num_threads);
  
  rc::HetCorrector::ReadPair read_pair;
  while( reader_1 >> read_pair.first && reader_2 >> read_pair.second){
    read_queue.put(read_pair);  
  }
  
  read_queue.finish();

  return 0;
}
