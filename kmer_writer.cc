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

int parse_line(char *buffer, RawKmerRecord *out){
  char delims[] = {'\t', '\n'};
  char *result = strtok( buffer, delims);
  if(NULL == result)
    return 1;
  out->kmer.assign(result);
  result = strtok( NULL , delims);
  if(NULL == result)
    return 1;
  long int c = atol(result);
  if( c > INT_MAX)
    out->count = INT_MAX;
  else
    out->count = static_cast<int>(c);

  return 0;
}


int main(int argc, char *argv[]){

  namespace rc = rollercoaster;
  
  static size_t bufsize = 1024;
  char *buffer = new char[bufsize];
  
  RawKmerRecord raw_record;

  getline(&buffer,&bufsize, stdin);
  if(parse_line(buffer,&raw_record)){
    std::cerr << "Error reading kmer records" << std::endl;
    return 1;
  }

  int kmer_size = raw_record.kmer.length();
  if(fwrite(&kmer_size, sizeof(int), 1, stdout)){}
  rc::KmerRecord record(kmer_size);
  record.set_kmer<std::string>(raw_record.kmer.begin(), raw_record.kmer.end());
  record.set_count(raw_record.count);
  rc::write_to_binary_stream(record,stdout);

  uint64_t count = 2;
  while(-1 != getline(&buffer, &bufsize,stdin)){
    
    if(parse_line(buffer, &raw_record)){
      std::cerr << "Error on record " << count << std::endl;
      return 1;
    }
    
    if(0 == count % 1000000)
      std::cerr << count << std::endl;

    record.set_kmer<std::string>(raw_record.kmer.begin(), raw_record.kmer.end());
    record.set_count(raw_record.count);
    rc::write_to_binary_stream(record,stdout);
    ++count;
  }

  delete []buffer;

  return 0;
}
