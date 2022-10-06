/**
 * This software was developed at the National Institute of Standards and Technology (NIST) by
 * employees of the Federal Government in the course of their official duties. Pursuant to title
 * 17 Section 105 of the United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for its use by other
 * parties, and makes no guarantees, expressed or implied, about its quality, reliability, or any
 * other characteristic.
 */
#ifndef COMPARATOR_H_
#define COMPARATOR_H_

#include "feature.h"
#include <vector>
#include <fstream>

using cv::Mat;
using cv::DMatch;
using cv::KeyPoint;
using std::vector;
using std::istream;
using std::ostream;

namespace Sift
{
   class Comparator
   {
      public:
         void compare(const Features& features1,
                      const Features& features2,
                      vector<FeatureMatch>& pairs) const;

         float locTol     = 0.15,
               angleTol   = M_PI / 9.,         // max angle difference (degrees)
               scaleTol   = 0.8,               // max scale difference
               rotTol     = M_PI / 9.,         // max rotation difference
               matchRatio = 0.8;               // nearest neighbour matching ratio
   };
}

#endif // COMPARATOR_H_
