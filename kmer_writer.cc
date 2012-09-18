#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

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
  std::copy(line.begin() + tab + 1 , line.end(), std::back_inserter(count));
  long int c =  atol(count.c_str());
  if( c > INT_MAX )
    out->count = INT_MAX;
  else
    out->count = static_cast<int>(c);

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

  int kmer_size = raw_record.kmer.length();
  if(fwrite(&kmer_size, sizeof(int), 1, stdout)){}
  fflush(stdout);
  rc::KmerRecord record(kmer_size);
  record.set_kmer<std::string>(raw_record.kmer.begin(), raw_record.kmer.end());
  record.set_count(raw_record.count);
  rc::write_to_binary_stream(record,stdout);
  fflush(stdout);

  uint64_t count = 2;
  while(std::cin){
    getline(std::cin, line);
    if( 0 == static_cast<int>(line.size()))
      continue;
    if(parse_kmer(line, &raw_record)){
      std::cerr << "Error on record " << count << std::endl;
      return 1;
    }
    
    if(0 == count % 1000000)
      std::cerr << count << std::endl;

    record.set_kmer<std::string>(raw_record.kmer.begin(), raw_record.kmer.end());
    record.set_count(raw_record.count);
    rc::write_to_binary_stream(record,stdout);
    fflush(stdout);
    ++count;
  }

  return 0;

}
