#include <iostream>


#include "fastq_record.h"


int main(void){
  
  namespace rc = rollercoaster;

  FILE *in_fq = fopen("td.fastq","r");
  rc::FastqRecordReader reader(in_fq);
  rc::FastqRecord record;

  while( reader >> record ){
    std::cout << "@" << record.name() <<std::endl;
    std::cout << record.sequence() << std::endl;
    std::cout << "+" << record.description() << std::endl;
    std::cout << record.quality() << std::endl;
  }

  
  fclose(in_fq);

  return 0;
}
