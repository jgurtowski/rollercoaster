
#include "coverage_segment.h"


namespace rollercoaster{

  void segment_counts(const std::vector<int> &counts, float threshold,
                      std::vector<Segment> *segments_out){
    segments_out.clear();

    Segment cur;
    int left = 0, i = 1;
    uint64_t sum = counts[0];
    for(;i<counts.size();++i){
      if(std::abs(counts[i-1] - counts[i]) > threshold){
        cur.left = left;
        cur.right = i-1;
        cur.mean = sum / static_cast<double>(i-left);
        segments_out->push_back(cur);
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

}//namespace rollercoaster
