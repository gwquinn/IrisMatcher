/**
 * This software was developed at the National Institute of Standards and Technology (NIST) by
 * employees of the Federal Government in the course of their official duties. Pursuant to title
 * 17 Section 105 of the United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for its use by other
 * parties, and makes no guarantees, expressed or implied, about its quality, reliability, or any
 * other characteristic.
 */
#ifndef FEATURES_H_
#define FEATURES_H_

#include <opencv2/opencv.hpp>
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
   struct Feature : public KeyPoint
   {
      Feature() = default;
      Feature(const KeyPoint& kp) : KeyPoint(kp) {};
   };

   struct FeatureMatch : public DMatch
   {
      float distanceRatio;
      FeatureMatch(const DMatch& m, const float r) : DMatch(m), distanceRatio(r) {};
   };

   struct Features
   {
      Features() = default;
      Features(const Features&) = default;

      vector<Feature> keyPoints;
      Mat descriptors;
   };
}

#endif // FEATURES_H_

