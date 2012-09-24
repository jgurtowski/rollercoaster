#include <iostream>

#include <vector>

#include "mmap_file.h"
#include "mmap_kmer_lookup.h"
#include "abstract_kmer_lookup.h"


int main(int argc, char *argv[]){
  
  namespace rc = rollercoaster;

  if(argc != 2){
    std::cout << "readcounts <in.db>" << std::endl;
    return 1;
  }

  std::string read;
  getline(std::cin , read);
  
  std::string db_file(argv[1]);
  rc::MMapFile db(db_file);
  if(db.map()){
    std::cout << "Error opening db" << std::endl;
    return 1;
  }    

  std::vector<int> counts;
  rc::MMapKmerLookup lookup(db);

  kmer_counts_for_read(read, lookup, &counts);
  std::cout << read << "\t";
  for(int i=0;i<static_cast<int>(counts.size());++i){
    std::cout << counts[i] << ",";
  }
  std::cout << std::endl;
  return 0;
}
