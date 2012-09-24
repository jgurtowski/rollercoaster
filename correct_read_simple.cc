#include <iostream>
#include <string>

#include "het_corrector.h"
#include "mmap_file.h"
#include "mmap_kmer_lookup.h"

int main(int argc, char *argv[]){

  namespace rc = rollercoaster;

  if(argc != 2){
    std::cout << "correctread <in.db>" << std::endl;
    return -1;
  }
  
  std::string db_filename(argv[1]);
  rc::MMapFile mmap_db(db_filename);
  
  if(mmap_db.map()){
    std::cerr << "unable to map file " << db_filename << std::endl;
    return -1;
  }

  rc::MMapKmerLookup lookup(mmap_db);

  rc::HetCorrector::CorrectorConfig corrector_conf = {
    /*.max_corrections =*/ 15,
    /*.max_strange_segments =*/ 4,
    /*.min_segment_length =*/ 3,
    /*.max_repeat_coverage =*/ 500,
    /*.segment_threshold = */ 55
    
  };
  
  rc::HetCorrector corrector(mmap_db, corrector_conf);
  corrector.set_debug(true);
  
  std::set<rc::HetCorrector::ReadChange> changes;
  std::string read;
  while(std::getline(std::cin, read)){
    corrector.correct_read(read, lookup, &changes);
  }

  return 0;
}
