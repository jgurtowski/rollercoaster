#ifndef FASTQ_RECORD_H
#define FASTQ_RECORD_H
#include <stdio.h>

#include <iostream>
#include <string>

namespace rollercoaster{

  class FastqRecordReader;

  class FastqRecord{
  public:
    const std::string &name() const {return name_;}
    const std::string &sequence() const {return sequence_;}
    const std::string &description() const {return description_;}
    const std::string &quality() const {return quality_;}
    
    void set_sequence_char(int idx, char base){ sequence_[idx] = base; }
    void set_quality_char(int idx, char base){ quality_[idx] = base; }

    friend bool operator >> (FastqRecordReader &reader, FastqRecord &record);

  private:
    
    std::string name_;
    std::string sequence_;
    std::string description_;
    std::string quality_;


  };//class FastqRecord


  class FastqRecordReader{
  public:
    static const size_t LineBufferSize = 10024;

    FastqRecordReader(std::string &filename);
    
    bool is_open() const { return (NULL == input_file_ ? false : true); }

    ~FastqRecordReader(){
      if(NULL != input_file_)
        fclose(input_file_);
      delete []line_buffer_;
    }

    friend bool operator >> (FastqRecordReader &reader, FastqRecord &record);
    

  private:
    FILE *input_file_;
    char *line_buffer_;
 
  };//class FastqRecordReader


  /**
   *Free Functions
   */

  bool operator >> (FastqRecordReader &reader, FastqRecord &record);

  std::ostream &operator << (std::ostream &out, const FastqRecord &record);

}//namespace rollercoaster


#endif
