#ifndef COVERAGE_SEGMENT_H
#define COVERAGE_SEGMENT_H

#include <stdint.h>

#include <iostream>
#include <vector>
#include <cmath>

namespace rollercoaster{

  /**
   *Data container for a Segment
   */
  struct Segment{
    int left;
    int right;
    double mean;

    inline int length() const{ return right - left + 1; }
  };

  
  /**
   *Free Functions
   */
  void segment_counts(const std::vector<int> &counts, float threshold,
                      std::vector<Segment> *segments_out);

  
  bool operator == (const Segment &lhs, const Segment &rhs);

  std::ostream &operator << (std::ostream &out, const Segment &segment);

}//namespace rollercoaster


#endif
