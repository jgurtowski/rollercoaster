#include <string.h>

#include "coverage_segment.h"


namespace rollercoaster{

  void segment_counts(const std::vector<int> &counts, float threshold,
                      std::vector<Segment> *segments_out){

    segments_out->clear();

    Segment cur;
    int left = 0, i = 1;
    int64_t sum = counts[0];
    for(;i<static_cast<int>(counts.size());++i){
      if(std::abs(static_cast<float>(counts[i-1] - counts[i])) > threshold){
        cur.left = left;
        cur.right = i-1;
        cur.mean = sum / static_cast<double>(i-left);
	segments_out->push_back(cur);
        
	left = i;
	sum = counts[i];
      }else{
        sum += counts[i];
      }
    }

    cur.left = left;
    cur.right = i-1;
    cur.mean = sum / static_cast<double>(i-left);
    segments_out->push_back(cur);

  }


  bool operator == (const Segment &lhs, const Segment &rhs){
    //simple structure, just byte compare
    return 0 == memcmp(&lhs,&rhs,sizeof(Segment));
  }


  std::ostream &operator << (std::ostream &out, const Segment &segment){
    out << "{l:" << segment.left << ",r:" << segment.right << ",m:" << segment.mean << "}";
    return out;
  }


}//namespace rollercoaster
