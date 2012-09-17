#include <stdlib.h>
#include <stdint.h>

#include <iostream>
#include <string>

#include "kmer_record.h"

typedef struct RawKmerRecord_struct{

  std::string kmer;
  int count;
  
}RawKmerRecord;


std::ostream &operator <<(std::ostream &out, const RawKmerRecord& record){
  return out << record.kmer << "\t" << record.count;
}

int parse_kmer(const std::string &line, RawKmerRecord *out){
  int tab = line.find("\t");
  if(tab < 0 || tab > static_cast<int>(line.size()) )
    return 1;

  std::string count;

  (out -> kmer).erase();
  std::copy(line.begin(), line.begin() + tab, std::back_inserter(out->kmer));
  std::copy(line.begin() + tab + 1 , line.end()-1, std::back_inserter(count));
  out->count = atoi(count.c_str());
  return 0;
}

int main(int argc, char *argv[]){

  namespace rc = rollercoaster;

  std::string line;
  RawKmerRecord raw_record;

  getline(std::cin, line);
  if(parse_kmer(line,&raw_record)){
    std::cerr << "Error reading kmer records" << std::endl;
    return 1;
  }
  
  std::cout << raw_record << std::endl;

  int kmer_size = raw_record.kmer.length();
  std::cerr << kmer_size << std::endl;
  std::cerr << "wrote :" << fwrite(&kmer_size,sizeof(int), 1 , stdout) <<std::endl;
  rc::KmerRecord record(kmer_size);
  record.set_kmer<std::string>(raw_record.kmer.begin(), raw_record.kmer.end());
  record.set_count(raw_record.count);
  rc::write_to_binary_stream(record, stdout);

  uint64_t count = 2;
  while(std::cin){
    getline(std::cin, line);
    if( line.size() == 0 )
      continue;
    if(parse_kmer(line, &raw_record)){
      std::cerr << "Error on record " << count << std::endl;
      return 1;
    }
    
    record.set_kmer<std::string>(raw_record.kmer.begin(), raw_record.kmer.end());
    record.set_count(raw_record.count);
    rc::write_to_binary_stream(record, stdout);
    
    ++count;
  }

  return 0;

}
