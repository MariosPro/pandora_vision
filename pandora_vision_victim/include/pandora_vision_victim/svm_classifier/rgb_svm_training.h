/*********************************************************************
*
* Software License Agreement (BSD License)
*
* Copyright (c) 2015, P.A.N.D.O.R.A. Team.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following
* disclaimer in the documentation and/or other materials provided
* with the distribution.
* * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* Authors:
*   Marios Protopapas
*   Kofinas Miltiadis <mkofinas@gmail.com>
*********************************************************************/

#ifndef PANDORA_VISION_VICTIM_SVM_CLASSIFIER_RGB_SVM_TRAINING_H
#define PANDORA_VISION_VICTIM_SVM_CLASSIFIER_RGB_SVM_TRAINING_H

#include <math.h>

#include <iostream>
#include <fstream>
#include <string>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <ros/ros.h>
#include <ros/package.h>

#include "pandora_vision_victim/svm_classifier/svm_training.h"
#include "pandora_vision_victim/svm_classifier/training_parameters.h"
#include "pandora_vision_victim/victim_parameters.h"
#include "pandora_vision_victim/feature_extractors/rgb_feature_extraction.h"
#include "pandora_vision_victim/feature_extractors/feature_extraction.h"
#include "pandora_vision_victim/utilities/file_utilities.h"

#define USE_OPENCV_GRID_SEARCH_AUTOTRAIN 1

namespace pandora_vision
{
  class RgbSvmTraining : public SvmTraining
  {
    public:
      /**
       * @brief The Constructor
       */
      RgbSvmTraining(const std::string& ns, int _num_feat, const std::string& datasetPath);

      /**
       * @brief The Destructor
       */
      virtual ~RgbSvmTraining();

      /**
       * @brief Function that implements the training for the subsystems
       * according to the given training sets. It applies SVM and extracts
       * a suitable model.
       * @return void
       */
      virtual void trainSubSystem();
  };
}  // namespace pandora_vision
#endif  // PANDORA_VISION_VICTIM_SVM_CLASSIFIER_RGB_SVM_TRAINING_H

