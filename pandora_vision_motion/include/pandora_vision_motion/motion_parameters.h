/*********************************************************************
*
* Software License Agreement (BSD License)
*
* Copyright (c) 2014, P.A.N.D.O.R.A. Team.
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
* Authors: Despoina Paschalidou
*********************************************************************/

#ifndef PANDORA_VISION_MOTION_MOTION_PARAMETERS_H
#define PANDORA_VISION_MOTION_MOTION_PARAMETERS_H

#include <dynamic_reconfigure/server.h>
#include <ros/ros.h>
#include <pandora_vision_motion/motion_cfgConfig.h>

namespace pandora_vision
{
  // TODO remove static variables and define/implement methods to get params
  // from yaml
  struct MotionParameters
  {
    //!< Background segmentation parameters
    int history;
    int varThreshold;
    bool bShadowDetection;
    int nmixtures;
    //!< Threshold parameters
    int diff_threshold;
    double motion_high_thres;
    double motion_low_thres;
    bool visualization;
    bool show_image;
    bool show_background;
    bool show_diff_image;
    bool show_moving_objects_contours;
    
    void configMotion(const ros::NodeHandle& nh);
  };
}  // namespace pandora_vision

#endif  // PANDORA_VISION_MOTION_MOTION_PARAMETERS_H
