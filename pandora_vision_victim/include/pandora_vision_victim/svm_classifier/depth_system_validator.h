/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
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
* Author: Despoina Paschalidou
*********************************************************************/
#ifndef PANDORA_VISION_VICTIM_DEPTH_SYSTEM_VALIDATOR_H 
#define PANDORA_VISION_VICTIM_DEPTH_SYSTEM_VALIDATOR_H 

#include "pandora_vision_victim/feature_extractors/channels_statistics_extractor.h"
#include "pandora_vision_victim/feature_extractors/haralickfeature_extractor.h"
#include "pandora_vision_victim/feature_extractors/edge_orientation_extractor.h"


namespace pandora_vision
{
  class DepthSystemValidator
  {
    public:
    
    ///Feature vector for depth features
    static std::vector<double> _depthFeatureVector;
    
    static std::string _depth_classifier_path;
    
     /// Svm classifier used for rgb subsystem
    static CvSVM _depthSvm;
    
    /// Set up SVM's parameters
    static CvSVMParams _params;
         
    static void initialize(const std::string& depth_classifier_path);
    
    /**
    @brief This function extract features according to the
    predifined features for the depth image
    @param inImage [cv::Mat] current depth frame to be processed
    @return void
    **/ 
    static float calculateSvmDepthProbability(const cv::Mat& inImage);
        
    /**
    @brief This function returns current feature vector according
    to the features found in rgb image
    @return [std::vector<double>] _rgbFeatureVector, feature vector 
    for current rgb image
    **/ 
    static std::vector<double> getDepthFeatureVector();
    
    /**
    @brief Function that loads the trained classifier and makes a prediction
    according to the featurevector given for each image
    @return void
    **/ 
    static float predict();
    
    /**
    @brief Function that converts a given vector of doubles
    in cv:Mat in order to use it to opencv function predict()
    @param data [std::vector <double>]: input vector to be 
    converted
    @return output [cv::Mat] : Mat of size size_of_vectorx1
    **/ 
    static cv::Mat vectorToMat(std::vector<double> data);
    
    /**
    @brief This function prediction according to the rgb classifier
    @return [float] prediction
    **/ 
    static float predictionToProbability(float prediction);
    
  };
}// namespace pandora_vision 
#endif  // PANDORA_VISION_VICTIM_DEPTH_SYSTEM_VALIDATOR_H
