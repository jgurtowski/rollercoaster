#ifndef MMAP_FILE_H
#define MMAP_FILE_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include <string>

namespace rollercoaster{


  class MMapFile{
  public:

  MMapFile(std::string &filename):
    filename_(filename),
      mapped_data_(),
      file_handle_(),
      statbuf_(),
      is_mapped_(false)
      {}
    ~MMapFile();
    
    /**
     *Actually does the work of opening the file
     * The return code is important to determine whether the
     * mapping procedure was successful or not
     * @return 1 for failure , 0 for success
     */
    int map();
    
    /**
     *Returns pointer to the mapped data,
     */
    inline char *mapped_data(){ return mapped_data_; }

    inline uint64_t size(){ return statbuf_.st_size; }
    
    inline const std::string &file_name() const { return filename_;}

    inline bool is_mapped() const { return is_mapped_; }

    
  private:
    std::string filename_;
    char *mapped_data_;
    int file_handle_;
    struct stat statbuf_;
    bool is_mapped_;

  };//class MMapFile

}//namespace rollercoaster


#endif
