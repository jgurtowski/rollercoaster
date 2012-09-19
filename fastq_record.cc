#include <stdio.h>
#include <string.h>

#include "fastq_record.h"


namespace rollercoaster{

  FastqRecordReader::FastqRecordReader(std::string &filename):input_file_(fopen(filename.c_str(), "r")),
                                                              line_buffer_(new char[LineBufferSize]){
  }
  
  bool operator >> (FastqRecordReader &reader, FastqRecord &record){
    size_t buf_size = FastqRecordReader::LineBufferSize;


    bool status = getline(&reader.line_buffer_, &buf_size, reader.input_file_) == -1 ? false : true;
    if(status && (status = ('@' == reader.line_buffer_[0]))){
      record.name_.assign(reader.line_buffer_ + 1, strlen(reader.line_buffer_) - 2);
      
      status = getline(&reader.line_buffer_, &buf_size, reader.input_file_);
      record.sequence_.assign(reader.line_buffer_, strlen(reader.line_buffer_) -1);
      
      status = getline (&reader.line_buffer_, &buf_size, reader.input_file_) == -1 ? false : true;
      if(status && (status = ('+' == reader.line_buffer_[0]))){
        record.description_.assign(reader.line_buffer_ + 1, strlen(reader.line_buffer_) - 2 );
        status = getline (&reader.line_buffer_, &buf_size, reader.input_file_);
        record.quality_.assign(reader.line_buffer_ , strlen(reader.line_buffer_) - 1);
      }
    }
    
    return status;
  }


}//namespace rollercoaster
