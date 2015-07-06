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
* Author:  Despoina Pascahlidou
*********************************************************************/

#ifndef PANDORA_VISION_MOTION_MOTION_DETECTOR_H
#define PANDORA_VISION_MOTION_MOTION_DETECTOR_H

#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include "pandora_vision_common/cv_mat_stamped.h"
#include "pandora_vision_common/pois_stamped.h"
#include "pandora_vision_common/bbox_poi.h"
#include "pandora_vision_motion/dbscan.h"

namespace pandora_vision
{
namespace pandora_vision_motion
{
  template <class T>
  inline std::string to_string (const T& t)
  {
      std::stringstream ss;
      ss << t;
      return ss.str();
  }

  class MotionDetector
  {
    public:
      //!< Background segmentation parameters
      int history;
      int varThreshold;
      bool bShadowDetection;
      int nmixtures;
      int indexFrame;

      //!< Threshold parameters
      int diff_threshold;
      double motion_high_thres;
      double motion_low_thres;
      bool visualization;
      bool show_image;
      bool show_background;
      bool show_diff_image;
      bool show_moving_objects_contours;

      //!< Enable dbscan
      bool dbscanEnable_;

      //!< Maximum deviation for calculation position of moving objects;
      int max_deviation_;

      int typeOfMovement_;


      /**
        @brief Class Constructor
        Initializes all varialbes for thresholding
      */
      MotionDetector(void);

      /**
        @brief Class Destructor
      */
      virtual ~MotionDetector();

      /**
      * @brief
      **/
      // void findMotionParameters(const cv::Mat& frame);

     std::vector<BBoxPOIPtr> getMotionPosition(void);

      void setMaxDeviation(int max_deviation);

      /**
        @brief Function that detects motion, according to substraction
        between background image and current frame. According to predifined
        thresholds motion is detected. According to the type of motion
        the suitable value is returned.
        @param frame [&cv::Mat] current frame to be processed
        @return [int] Index of evaluation of Motion in current frame.
      */
      void detectMotion(const cv::Mat& frame);


    protected:

      void setUpMotionDetector(void);

     
      /**
        @brief Function that defines the type of movement
        according to the number of pixels, that differ from current
        frame and background. In case insignificant motion 0 is detected
        0 is returned. If there is slight motion 1 is returned and last
        but not least in case extensive motion is detected 2 is returned
        @param thresholdedDifference: [&cv::Mat] frame that represents
          the thresholded difference between current frame and computed
          background
        @return typeOfMovement [int], where 2 corresponds to moving objects
        with greater probability whereas 0 corresponds to stationary objects
      */
      int motionIdentification(const cv::Mat& thresholdedDifference);

      /**
        @brief Function used for debug reasons, that shows background
        foreground and contours of motion trajectories in current frame
        @param thresholdedDifference: [&cv::Mat] frame that represents
          the thresholded difference between current frame and computed
          background.
        @param frame: [&cv::Mat] current frame, captured from camera
        @return void
      */
      void debugShow();

      /**
        @brief Function that calculates motion's position
        @param diff: [&cv::Mat] frame that represents
        the thresholded difference between current frame and computed
        background.
        @return void
      */
      void detectMotionPosition(const cv::Mat& diff);

      bool compareContourAreas(cv::Rect contour1, cv::Rect contour2);

     cv::Scalar HSVtoRGBcvScalar(int H, int S, int V);
     cv::Rect mergeBoundingBoxes(std::vector<cv::Point>& bbox);
     float calculateMotionProbability(const cv::Rect& bbox, const cv::Mat& img);


    private:
      //!< Current frame to be processed
      cv::Mat frame_;
      //!< Background image
      cv::Mat background_;
      //!< Foreground mask
      cv::Mat foreground_;
      cv::Mat movingObjects_;
      //!< diff image
      cv::Mat thresholdedDifference_;
      //!< Class instance for Gaussian Mixture-based backgound
      //!< and foreground segmentation
      cv::BackgroundSubtractorMOG2 bg_;
      //!< Erode kernel
      cv::Mat kernel_erode_;
      //!< Bounding box of moving objects.
      std::vector<BBoxPOIPtr> bounding_boxes_;
      BBoxPOIPtr bounding_box_;
      std::vector<cv::Rect> finalBoxes;
      std::vector<double> cohesion;
     friend class MotionDetectorTest;
  };
}  // namespace pandora_vision_motion
}  // namespace pandora_vision
#endif  // PANDORA_VISION_MOTION_MOTION_DETECTOR_H
