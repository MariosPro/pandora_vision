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

#ifndef PANDORA_VISION_VICTIM_UTILITIES_BAG_OF_WORDS_TRAINER_H
#define PANDORA_VISION_VICTIM_UTILITIES_BAG_OF_WORDS_TRAINER_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "pandora_vision_victim/feature_extractors/feature_extractor_factory.h"

/**
 * @namespace pandora_vision
 * @brief The main namespace for PANDORA vision
 */
namespace pandora_vision
{
  /**
   * @class BagOfWordsTrainer
   * @brief This class applies Bag of words clustering at a set of features.
   */
  class BagOfWordsTrainer
  {
    public:
      /**
       * @brief Default Constructor
       */
      BagOfWordsTrainer();

      /**
       * @brief Constructor
       */
      BagOfWordsTrainer(const std::string& featureDetectorType,
          const std::string& descriptorExtractorType,
          const std::string& descriptorMatcherType,
          int dictionarySize);

      /**
       * @brief Destructor
       */
      ~BagOfWordsTrainer();

      /**
       * @brief
       */
      void addDescriptors(const cv::Mat& inImage);

      /**
       * @brief
       */
      void createVocabulary();

      /**
       * @brief
       */
      void setVocabulary(const cv::Mat& vocabulary);

      /**
       * @brief
       */
      void createBowRepresentation(const cv::Mat& inImage,
          cv::Mat* descriptors);

      /**
       * @brief : Plots the input descriptor.
       * @param descriptor[const cv::Mat&]: The descriptor to plot
       */
      void plotDescriptor(const cv::Mat& descriptor);

      /**
       * @brief
       */
      cv::Mat getVocabulary() const;

    private:
      ///
      FeatureExtractorFactory* factoryExtractor_;

      ///
      cv::Ptr<cv::BOWKMeansTrainer> bowKmeansTrainer_;

      ///
      cv::Ptr<cv::BOWImgDescriptorExtractor> bowDescriptorExtractor_;

      ///
      cv::Mat bowVocabulary_;

      ///
      std::string featureDetectorType_;

      ///
      std::string descriptorExtractorType_;

      ///
      std::string descriptorMatcherType_;

      ///
      cv::Ptr<cv::FeatureDetector> featureDetector_;

      ///
      cv::Ptr<cv::DescriptorExtractor> descriptorExtractor_;

      ///
      cv::Ptr<cv::DescriptorMatcher> descriptorMatcher_;
  };
}  // namespace pandora_vision
#endif  // PANDORA_VISION_VICTIM_UTILITIES_BAG_OF_WORDS_TRAINER_H
