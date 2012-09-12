#include <string.h>
#include <stdint.h>

#include <iostream>

#include "memcache_klookup.h"

namespace rollercoaster{

  MemcacheKlookup::MemcacheKlookup(const std::vector<MemcacheHost> &server_list){
    using namespace std;
    
    memcached_ = memcached_create(NULL);
    
    for(vector<MemcacheHost>::const_iterator it = server_list.begin(); 
        it != server_list.end(); ++it){
      memcached_server_add(memcached_, (*it).hostname().c_str(), (*it).port());
    }

  }

  MemcacheKlookup::~MemcacheKlookup(){
    memcached_free(memcached_);
  }

  
  bool MemcacheKlookup::set_kmer(const std::string &kmer, MemcacheKlookup::KmerCount value){
    return memcached_set(memcached_, kmer.c_str(), kmer.length(), reinterpret_cast<char *>(&value), sizeof(MemcacheKlookup::KmerCount),0,0);
  }

  MemcacheKlookup::KmerCount MemcacheKlookup::get_count(const std::string &kmer){
    size_t kmer_count_size = sizeof(MemcacheKlookup::KmerCount);
    uint32_t flags  = 0;
    char *data = memcached_get(memcached_, kmer.c_str(), kmer.length(), &kmer_count_size,&flags,&memcached_error_);
    if(NULL == data){
      return -1;
    }
    memcpy(&cached_count_,data,kmer_count_size);
    free(data);
    return cached_count_;
  }

  const memcached_return_t &MemcacheKlookup::get_last_error() const{
    return memcached_error_;
  }

  
}//namespace rollercoaster
