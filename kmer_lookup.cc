#include <string>

#include "mmap_file.h"
#include "mmap_kmer_lookup.h"

int main(int argc, char *argv[]){

  namespace rc = rollercoaster;

  if(argc != 2){
    std::cerr << "kmerlookup <db>" << std::endl;
    return 1;
  }

  std::string db_file_name(argv[1]);
  rc::MMapFile db_file(db_file_name);

  if(db_file.map()){
    std::cerr << "error opening file" << db_file.file_name() << std::endl;
  }
  
  rc::MMapKmerLookup kmer_lookup(db_file);  

  rc::PackedKmer packed_kmer(kmer_lookup.kmer_size());

  std::string line;
  while(std::cin){
    getline(std::cin, line);
    if(0 == static_cast<int>(line.length()))
      continue;
    if(static_cast<int>(line.length()) != kmer_lookup.kmer_size()){
      std::cerr << "Input Kmer different size than in DB (db:you)" << kmer_lookup.kmer_size();
      std::cerr << ":" << line.length()<< std::endl;
      return 1;
    }

    packed_kmer.set_kmer<std::string>(line.begin(),line.end());
    
    
    if(kmer_lookup.has_record(packed_kmer))
      std::cout << kmer_lookup.last_record() << std::endl;
    else
      std::cerr << "Error finding: " << packed_kmer.str_kmer() << std::endl;
    
  }

  return 0;
}
