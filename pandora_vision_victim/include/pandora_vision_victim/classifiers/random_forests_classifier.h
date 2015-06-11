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
*********************************************************************/

#ifndef PANDORA_VISION_VICTIM_CLASSIFIERS_RANDOM_FORESTS_CLASSIFIER_H
#define PANDORA_VISION_VICTIM_CLASSIFIERS_RANDOM_FORESTS_CLASSIFIER_H

#include <string>

#include <opencv2/opencv.hpp>

#include "pandora_vision_victim/classifiers/abstract_classifier.h"

namespace pandora_vision
{
  class RandomForestsClassifier : public AbstractClassifier
  {
    public:
      /**
       * @brief The Constructor
       */
      RandomForestsClassifier(const std::string& ns, int numFeatures,
          const std::string& datasetPath, const std::string& classifierType,
          const std::string& imageType);

      /**
       * @brief The Destructor
       */
      virtual ~RandomForestsClassifier();

      /**
       * @brief Function that implements the training for the subsystems
       * according to the given training sets. It applies SVM and extracts
       * a suitable model.
       * @return void
       */
      virtual void trainSubSystem();

    protected:
      /// Parameters of the Random Forests Classifier.
      CvRTParams randomForestsParams_;
      /// Train the Random Forests
      boost::shared_ptr<CvRTrees> randomForestsClassifierPtr_;
  };
}  // namespace pandora_vision
#endif  // PANDORA_VISION_VICTIM_CLASSIFIERS_RANDOM_FORESTS_CLASSIFIER_H
