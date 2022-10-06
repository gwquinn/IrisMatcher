/**
 * This software was developed at the National Institute of Standards and Technology (NIST) by
 * employees of the Federal Government in the course of their official duties. Pursuant to title
 * 17 Section 105 of the United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for its use by other
 * parties, and makes no guarantees, expressed or implied, about its quality, reliability, or any
 * other characteristic.
 */
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#include "sift/feature.h"
#include "irisBoundary.h"
#include <opencv2/opencv.hpp>

using cv::Size;
using cv::Mat;

namespace Sift
{
   class FeatureExtractor
   {
      public:
         FeatureExtractor() = default;

         void extract(const Mat& image, const IrisBoundary& pupil,
                      const IrisBoundary& limbus, Features& features) const;

         // Image preprocessing parameters.
         static constexpr float blurSigma = 1.5;       // pre-blur applied to the image

         // Size of window used for CLAHE adaptive histogram equalization.
         static const cv::Size claheWindow;

         // Clip limit for histogram equalization.
         static constexpr float clipLimit = 40.0;

         // Sift paramaters.
         static constexpr int numOctaveLayers  = 3;    // number of layers per octave
         static constexpr float sigma          = 1.6,  // blur applied at each SIFT level
                                contrastThresh = 0.04, // min contrast to qualify as a feature
                                edgeThresh     = 10;   // max edgeness to qualify as a feature
   };
}

#endif // FEATURE_EXTRACTOR_H_

