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
* Authors:
*   Kofinas Miltiadis <mkofinas@gmail.com>
*   Protopapas Marios <protopapas_marios@hotmail.com>
*********************************************************************/

#include <string>

#include <ros/console.h>

#include "pandora_vision_victim/svm_classifier/rgb_svm_validator.h"
#include "pandora_vision_victim/victim_parameters.h"
#include "pandora_vision_victim/feature_extractors/rgb_feature_extraction.h"
#include "pandora_vision_victim/utilities/file_utilities.h"

/**
 * @namespace pandora_vision
 * @brief The main namespace for PANDORA vision
 */
namespace pandora_vision
{
  /**
   * @brief Constructor. Initializes SVM classifier parameters and loads
   * classifier model. The classifier is to be used with RGB images.
   * @param classifierPath [const std::string&] The path to the classifier
   * model.
   */
  RgbSvmValidator::RgbSvmValidator(const std::string& classifierPath)
    : SvmValidator()
  {
    ROS_INFO("Enter RGB SVM Validator");
    classifierPath_ = classifierPath;

    ROS_INFO_STREAM(classifierPath.c_str());
    svmValidator_.load(classifierPath.c_str());

    probabilityScaling_ = VictimParameters::rgb_svm_prob_scaling;
    probabilityTranslation_ = VictimParameters::rgb_svm_prob_translation;

    svmParams_.C = VictimParameters::rgb_svm_C;
    svmParams_.gamma = VictimParameters::rgb_svm_gamma;

    packagePath_ = ros::package::getPath("pandora_vision_victim");
    std::string filesDirectory = packagePath_ + "/data/";

    std::string normalizationParamOne = "rgb_mean_values.xml";
    std::stringstream normalizationParamOnePath;
    normalizationParamOnePath << filesDirectory << normalizationParamOne;
    std::string normalizationParamTwo = "rgb_standard_deviation_values.xml";
    std::stringstream normalizationParamTwoPath;
    normalizationParamTwoPath << filesDirectory << normalizationParamTwo;

    normalizationParamOneVec_ = file_utilities::loadFiles(
        normalizationParamOnePath.str(), "mean");
    normalizationParamTwoVec_ = file_utilities::loadFiles(
        normalizationParamTwoPath.str(), "std_dev");

    featureExtraction_ = new RgbFeatureExtraction();
    bool vocabularyNeeded = featureExtraction_->bagOfWordsVocabularyNeeded();
    if (vocabularyNeeded)
    {
      const std::string bagOfWordsFile = "rgb_bag_of_words.xml";
      const std::string bagOfWordsFilePath = filesDirectory + bagOfWordsFile;
      cv::Mat vocabulary = file_utilities::loadFiles(bagOfWordsFilePath,
          "bag_of_words");
      featureExtraction_->setBagOfWordsVocabulary(vocabulary);
    }
    ROS_INFO("Initialized RGB SVM Validator");
  }

  /**
   * @brief Default Destructor.
   */
  RgbSvmValidator::~RgbSvmValidator()
  {
  }
}  // namespace pandora_vision

