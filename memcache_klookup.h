#ifndef MEMCACHE_KLOOKUP_H
#define MEMCACHE_KLOOKUP_H

#include <vector>
#include <string>

#include <libmemcached/memcached.h>

namespace rollercoaster{

  /**
   *Simple data class that holds hostname and port of memcache servers
   */
  class MemcacheHost{

  public:
  MemcacheHost(const std::string &host, int port):hostname_(host),port_(port){}
    inline const std::string &hostname() const{return hostname_;}
    inline int port() const{ return port_;}
    
  private:
    std::string hostname_;
    int port_;
  };


  /**
   *class MemcacheKlookup
   *Looks up kmers in Memcached cluster
   */
  class MemcacheKlookup{

  public:
    /**
     *Type to hold counts
     */
    typedef int KmerCount;

    /**
     *Provide a list of servers in the form host:port to connect with
     */
    MemcacheKlookup(const std::vector<MemcacheHost> &server_list);
    
    ~MemcacheKlookup();

    /**
     *set a kmer in memcached given a specific count
     */
    bool set_kmer(const std::string &kmer, int value);

    /**
     *Looks up kmer in memcached and returns the associated count
     */
    KmerCount get_count(const std::string &kmer);

    /**
     *Get Last error
     */
    const memcached_return_t &get_last_error() const;

  private:
    memcached_st *memcached_;
    memcached_return_t memcached_error_;
    KmerCount cached_count_;
    
  };//class MemcacheKlookup

}//namespace rollercoaster


#endif
