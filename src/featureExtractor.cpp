/**
 * This software was developed at the National Institute of Standards and Technology (NIST) by
 * employees of the Federal Government in the course of their official duties. Pursuant to title
 * 17 Section 105 of the United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for its use by other
 * parties, and makes no guarantees, expressed or implied, about its quality, reliability, or any
 * other characteristic.
 */
#include "sift/featureExtractor.h"
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv::xfeatures2d;

namespace Sift
{
   const cv::Size FeatureExtractor::claheWindow = Size(10, 4);

   void FeatureExtractor::extract(const Mat& image, const IrisBoundary& pupil,
                                  const IrisBoundary& limbus, Features& features) const
   {
      Mat img;

      // If image contains multiple channels, utilize only the red channel.
      if (image.channels() > 1)
         extractChannel(image, img, 2);
      else
         img = image;

      // Convert from cartesian to pseudo-polar format.

      cv::Mat1b polar = cv::Mat1b::zeros(64, 768),
                mask  = cv::Mat1b::zeros(64, 768);

      const cv::Rect rect(0, 0, img.cols, img.rows);

      // For each pseudo-polar pixel coordinate.
      for (int y = 0; y < polar.rows; ++y)
         for (int x = 0; x < polar.cols; ++x)
         {
            const float theta = x * M_PI / 256 - (M_PI / 2.), 
                           dx = cos(theta),
                           dy = sin(theta),
                          rho = y / 64.;

            const Point2f p1( pupil.x +  pupil.a * dx,  pupil.y +  pupil.b * dy),
                          p2(limbus.x + limbus.a * dx, limbus.y + limbus.b * dy),
                          src = p1 + rho * (p2 - p1);

            if (rect.contains(src))
            {
               polar.at<uint8_t>(y, x) = img.at<uint8_t>(src);

               if (x >= 128 && x < 640)
                  mask.at<uint8_t>(y, x) = 255;
            }
         }

      // Apply localized histogram equalization.
      cv::Ptr<cv::CLAHE> clahe = createCLAHE(clipLimit, claheWindow);
      clahe->apply(polar, polar);

      // Slightly blur image (as recommended by Lowe).
      GaussianBlur(polar, polar, Size(), blurSigma, 0, cv::BORDER_WRAP);

      // Initialize SIFT feature extractor.
      cv::Ptr<cv::SIFT> detector = cv::SIFT::create(0, numOctaveLayers, Sift::FeatureExtractor::contrastThresh,
                                                    edgeThresh, sigma);

      Mat laplacian(polar.size(), CV_32F);
      cv::Laplacian(polar, laplacian, CV_32F); 

      // Extract features.
      vector<KeyPoint> keyPoints;
      detector->detectAndCompute(polar, mask, keyPoints, features.descriptors);

      for (auto& f : keyPoints)
      {
         // Classify feature as either light or dark.
         f.class_id = std::signbit(laplacian.at<float>(f.pt));

         // Convert to pseudo-polar coordinates.
         f.pt.x = fmod(M_PI * (f.pt.x / 256. + 1.5), 2 * M_PI);
         f.pt.y /= 64;

         features.keyPoints.push_back(f);
      }
   }
}

