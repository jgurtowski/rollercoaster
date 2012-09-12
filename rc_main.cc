#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>

#include "memcache_klookup.h"

int main(void){
  
  std::vector<rollercoaster::MemcacheHost> server_vec;
  server_vec.push_back(rollercoaster::MemcacheHost("slippery.cshl.edu",11211));
  
  rollercoaster::MemcacheKlookup kmer_lookup(server_vec);

  std::cout<<kmer_lookup.set_kmer("ACGTACGTAGCAGTA",13)<<std::endl;
  std::cout<<kmer_lookup.set_kmer("ACGTACGTAGCAGTC",23)<<std::endl;
  std::cout << "value:" << kmer_lookup.get_count("ACGTACGTAGCAGTA")<<std::endl;
  std::cout << "value:" << kmer_lookup.get_count("ACGTACGTAGCAGTC")<<std::endl;
  std::cout << "value:" << kmer_lookup.get_count("ACGTACGTAGCAGTG")<<std::endl;
  
  return 0;
}
