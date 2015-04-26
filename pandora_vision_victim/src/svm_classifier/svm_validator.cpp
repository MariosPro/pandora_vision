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

#include <ros/console.h>

#include "pandora_vision_victim/svm_classifier/svm_validator.h"
#include "pandora_vision_victim/utilities/file_utilities.h"

/**
 * @namespace pandora_vision
 * @brief The main namespace for PANDORA vision
 */
namespace pandora_vision
{
  /**
   * @brief Constructor. Initializes SVM classifier parameters and loads
   * classifier model.
   * @param classifierPath [const std::string&] The path to the classifier
   * model.
   */
  SvmValidator::SvmValidator(const std::string& classifierPath)
  {
    ROS_INFO("Enter SVM Validator");
    classifierPath_ = classifierPath;

    svmParams_.svm_type = CvSVM::C_SVC;
    svmParams_.kernel_type = CvSVM::RBF;
    svmParams_.term_crit = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,
        10000, 1e-6);
    /// Load classifier model path.
    ROS_INFO_STREAM(classifierPath.c_str());
    svmValidator_.load(classifierPath.c_str());

    featureExtraction_ = new FeatureExtraction();
    featureExtractionUtilities_ = new FeatureExtractionUtilities();
  }

  /**
   * @brief Default Destructor.
   */
  SvmValidator::~SvmValidator()
  {
  }

  /**
   * @brief This function extracts features according to the predefined
   * feature extraction algorithms.
   * @param inImage [const cv::Mat&] Frame to extract features from.
   * @return void
   */
  void SvmValidator::extractFeatures(const cv::Mat& inImage)
  {
    featureExtraction_->extractFeatures(inImage);
  }

  /**
   * @brief This function extract features according to the
   * predifined features for the rgb image
   * @param inImage [cv::Mat] current rgb frame to be processed
   * @return void
  */
  float SvmValidator::calculatePredictionProbability(const cv::Mat& inImage)
  {
    extractFeatures(inImage);
    if (!featureVector_.empty())
      featureVector_.clear();
    featureVector_ = featureExtraction_->getFeatureVector();
    return predictionToProbability(predict());
  }

  /**
  @brief Function that loads the trained classifier and makes a prediction
  according to the featurevector given for each image
  @return void
  **/
  float SvmValidator::predict()
  {
    cv::Mat featuresMat = cv::Mat(featureVector_);
    // Make features matrix a row vector.
    transpose(featuresMat, featuresMat);
    ///Normalize the data
    featureExtractionUtilities_->performZScoreNormalization(
        &featuresMat, normalizationParamOneVec_, normalizationParamTwoVec_);
    featuresMat.convertTo(featuresMat, CV_32FC1);

    ROS_INFO_STREAM("SVM class label: " <<
                    svmValidator_.predict(featuresMat, false));
    return svmValidator_.predict(featuresMat, true);
  }

  /**
   * @brief This function calculates the classification probability according to
   * the SVM prediction.
   * @param prediction [float] The SVM prediction.
   * @return [float] The classification probability.
   */
  float SvmValidator::predictionToProbability(float prediction)
  {
    if(prediction < 0)
      prediction = fabs(prediction);

    // Normalize probability to [-1,1]
    float probability = tanh(probabilityScaling_ * prediction -
        probabilityTranslation_);
    // Normalize probability to [0,1]
    probability = (1.0 + probability) / 2.0;
    ROS_INFO_STREAM("SVM prediction: " << prediction);
    ROS_INFO_STREAM("SVM probability: " << probability);
    return probability;
  }
}// namespace pandora_vision
