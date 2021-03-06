/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015, P.A.N.D.O.R.A. Team.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Kofinas Miltiadis <mkofinas@gmail.com>
 *********************************************************************/

#include <string>
#include <vector>

#include "pandora_vision_victim/feature_extractors/sift_extractor.h"

/**
 * @namespace pandora_vision
 * @brief The main namespace for PANDORA vision
 */
namespace pandora_vision
{
namespace pandora_vision_victim
{
  /**
   * @brief Default Constructor
   */
  SiftExtractor::SiftExtractor() : featureDetectorType_("SIFT"),
      descriptorExtractorType_("SIFT")
  {
    cv::initModule_nonfree();
    featureDetector_ = cv::FeatureDetector::create(featureDetectorType_);
    descriptorExtractor_ = cv::DescriptorExtractor::create(
        descriptorExtractorType_);
  }

  /**
   * @brief Default Constructor
   */
  SiftExtractor::SiftExtractor(const std::string& featureDetectorType,
      const std::string& descriptorExtractorType)
        : featureDetectorType_(featureDetectorType),
          descriptorExtractorType_(descriptorExtractorType)
  {
    cv::initModule_nonfree();
    featureDetector_ = cv::FeatureDetector::create(featureDetectorType_);
    descriptorExtractor_ = cv::DescriptorExtractor::create(
        descriptorExtractorType_);
  }

  /**
   * @brief Destructor
   */
  SiftExtractor::~SiftExtractor()
  {
  }

  /**
   *
   */
  void SiftExtractor::extractFeatures(const cv::Mat& inImage,
      cv::Mat* descriptors)
  {
    std::vector<cv::KeyPoint> keyPoints;
    featureDetector_->detect(inImage, keyPoints);

    // cv::Mat imageWithKeyPoints;
    // cv::drawKeypoints(inImage, keyPoints, imageWithKeyPoints, cv::Scalar::all(-1),
        // cv::DrawMatchesFlags::DEFAULT);
    // cv::imshow("Image Keypoints", imageWithKeyPoints);
    // cvWaitKey(0);
    descriptorExtractor_->compute(inImage, keyPoints, *descriptors);
  }
}  // namespace pandora_vision_victim
}  // namespace pandora_vision
