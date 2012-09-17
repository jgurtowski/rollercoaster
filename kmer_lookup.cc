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

  std::cout << kmer_lookup.kmer_size() << std::endl;

  return 0;

  rc::PackedKmer packed_kmer(kmer_lookup.kmer_size());

  std::string line;
  while(std::cin){
    getline(std::cin, line);
    if(static_cast<int>(line.length()) -1 != kmer_lookup.kmer_size()){
      std::cerr << "Input Kmer different size than in DB (db:you)" << kmer_lookup.kmer_size();
      std::cerr << ":" << line.length()<< std::endl;
      return 1;
    }
    packed_kmer.set_kmer<std::string>(line.begin(),line.end()-1);
    
    /*
    if(kmer_lookup.has_record(packed_kmer))
      std::cout << kmer_lookup.last_record() << std::endl;
    else
      std::cout << "Error finding: " << packed_kmer.str_kmer() << std::endl;
    */
  }

  return 0;
}
