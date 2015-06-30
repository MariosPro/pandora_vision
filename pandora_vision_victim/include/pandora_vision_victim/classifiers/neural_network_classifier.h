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
*   Choutas Vassilis <vasilis4ch@gmail.com>
*********************************************************************/

#ifndef PANDORA_VISION_VICTIM_CLASSIFIERS_NEURAL_NETWORK_CLASSIFIER_H
#define PANDORA_VISION_VICTIM_CLASSIFIERS_NEURAL_NETWORK_CLASSIFIER_H

#include <string>

#include <opencv2/opencv.hpp>

#include "pandora_vision_victim/classifiers/abstract_classifier.h"

#include "pandora_vision_victim/feature_extractors/feature_extraction.h"
#include "pandora_vision_victim/utilities/file_utilities.h"

namespace pandora_vision
{
  class NeuralNetworkClassifier : public AbstractClassifier
  {
    public:
      /**
       * @brief Constructor for the Neural Network Classifier Wrapper Class
       * @param ns[const std::string&] The namespace of the node
       * @param numFeatures[int] The number of input features to the classifier
       * @param datasetPath[const std::string&] The path to the training dataset
       * @param classifierType[const std::string&] The model used by the
       * classifier.
       * @param imageType[const std::string&] The type of input images given to
       * the classifier(RGB or Depth)
       */
      NeuralNetworkClassifier(const std::string& ns, int numFeatures,
          const std::string& datasetPath, const std::string& classifierType,
          const std::string& imageType);

      /**
       * @brief The Destructor
       */
      virtual ~NeuralNetworkClassifier();

      /**
       * @brief Trains the corresponding classifier using the input features and training labels.
       * @param trainingSetFeatures[const cv::Mat&] The matrix containing the features that describe the 
       * training set
       * @param trainingSetLabels[const cv::Mat&] The corresponding labels that define the class of each
       * training sample.
       * @return bool True on successfull completions, false otherwise. 
       */
      virtual bool train(const cv::Mat& trainingSetFeatures, const cv::Mat trainingSetLabels);

      /**
       * @brief Validates the resulting classifier using the given features 
       * extracted from the test set.
       * @param testSetFeatures[const cv::Mat&] The test set features matrix
       * @param validationResults[cv::Mat*] The results for the test set.
       * @return void
       */
      virtual void validate(const cv::Mat& testSetFeatures, cv::Mat* validationResults);

      /**
       * @brief Saves the classifier to a file 
       * @param classifierFile[const std::string&] The name of the file where the classifier will be stored
       * @return void
       */
      virtual void load(const std::string& classifierFile)
      {
        classifierPtr_->load(classifierFile.c_str());
      }

    protected:
      /// Parameters of the Random Forests Classifier.
      cv::ANN_MLP_TrainParams NeuralNetworkParams_;

      /// The Pointer to the classifier object
      boost::shared_ptr<CvANN_MLP> classifierPtr_;
  };
}  // namespace pandora_vision

#endif  // PANDORA_VISION_VICTIM_CLASSIFIERS_NEURAL_NETWORK_CLASSIFIER_H
