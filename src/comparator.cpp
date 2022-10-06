/**
 * This software was developed at the National Institute of Standards and Technology (NIST) by
 * employees of the Federal Government in the course of their official duties. Pursuant to title
 * 17 Section 105 of the United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for its use by other
 * parties, and makes no guarantees, expressed or implied, about its quality, reliability, or any
 * other characteristic.
 */
#include "sift/comparator.h"
#include <iostream> // -GW
using std::endl;
using std::cerr;
using Sift::Features;

// Ensure angle is within the range [-pi,pi).
static inline float wrapAngle(const float angle) {
   return fmod(angle + 3 * M_PI, 2 * M_PI) - M_PI;
}

// Smallest difference between two angles.
static inline float angleDiff(const float angle1, const float angle2) {
   return fabs(wrapAngle(angle2 - angle1));
}

namespace Sift
{
   // Compare features and return matches.
   void Comparator::compare(const Features& features1,
                            const Features& features2,
                            vector<FeatureMatch>& pairs) const
   {
      pairs.clear();

std::cerr << "here 0" << std::endl; // -GW

      if (features1.keyPoints.size() == 0 || features2.keyPoints.size() == 0)
         return;

      // Full cross comparison of all key points.
      vector<vector<DMatch>> matches;

      cv::BFMatcher matcher;
      matcher.knnMatch(features1.descriptors, features2.descriptors, matches,
                       features2.keyPoints.size());

      vector<FeatureMatch> prunnedMatches;

      // Remove pairings outside of tolerances.
      for (auto& m : matches)
         if (m.size() > 1)
         {
            // Get matched keypoints.
            const Feature& f1 = features1.keyPoints[m[0].queryIdx],
                         & f2 = features2.keyPoints[m[0].trainIdx];

            const float angleDisp = angleDiff(f1.angle * M_PI / 180.,
                                              f2.angle * M_PI / 180.);
            if (angleDisp > angleTol)
               continue;

            const float scaleDiff = (f1.size > f2.size) ? (f2.size / f1.size) :
                                                          (f1.size / f2.size);
            if (scaleDiff < scaleTol)
                  continue;
               
            // Compute distance between points.
            const float dx      = angleDiff(f1.pt.x, f2.pt.x),
                        dy      = f1.pt.y - f2.pt.y,
                        locDiff = sqrt(dx * dx + dy * dy);

            if (locDiff > locTol + rotTol)
               continue;

            // Fulfilled all first-pass criteria.

            // Distance relative to next most similar (as recommended in Lowe paper).
            const float ratio = m[0].distance / m[1].distance;
            prunnedMatches.push_back(FeatureMatch(m[0], ratio));
         }

      // Estimate rotational disparity.
      vector<float> rotations;

      // Save rotation difference for strongly matched points.
      for (auto& m : prunnedMatches)
         if (m.distanceRatio <= matchRatio)
         {
            // Estimate rotation.
            const float rotation1 = features1.keyPoints[m.queryIdx].pt.x,
                        rotation2 = features2.keyPoints[m.trainIdx].pt.x,
                        rotation = wrapAngle(rotation2 - rotation1);

            // Save rotation estimate.
            rotations.push_back(rotation);
         }

      float rotation = 0;

      // Estimate rotation (using median to reduce impact of outliers).
      if (rotations.size())
      {
         const auto ptr = rotations.begin() + rotations.size() / 2;

         nth_element(rotations.begin(), ptr, rotations.end());
      }


      // Step 4: Retain all pairings that meet location requirements.
      for (const auto& m : prunnedMatches)
      {
         const cv::Point2f p1 = features1.keyPoints[m.queryIdx].pt,
                           p2 = features2.keyPoints[m.trainIdx].pt;
         
         // Compute distance between points.
         const float dx      = angleDiff(p1.x + rotation, p2.x),
                     dy      = p2.y - p1.y,
                     locDiff = sqrt(dx * dx + dy * dy);

         if (locDiff > locTol)
            continue;

         pairs.push_back(m);
      }
   }
}

