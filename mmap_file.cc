

#include "mmap_file.h"


namespace rollercoaster{

  int MMapFile::map(){
    
    file_handle_ = open(filename_.c_str(), O_RDONLY);
    if( 0 == file_handle_)
      return 1;
    fstat(file_handle_, &statbuf_);
    mapped_data_ = (char *)mmap(0,statbuf_.st_size,PROT_READ,MAP_SHARED,file_handle_,0);
    if( 0 == MAP_FAILED){
      close(file_handle_);
      return 1;
    }
    
    is_mapped_  = true;

    return 0;
  }

  MMapFile::~MMapFile(){
    munmap(mapped_data_, statbuf_.st_size);
    close(file_handle_);
  }

}//namespace rollercoaster
