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
 *   Chatzieleftheriou Eirini <eirini.ch0@gmail.com>
 *********************************************************************/

#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "pandora_vision_obstacle/barrel_detection/barrel_preprocessor.h"

namespace pandora_vision
{
  BarrelPreProcessor::BarrelPreProcessor(const std::string& ns, 
    sensor_processor::Handler* handler) : sensor_processor::PreProcessor<sensor_msgs::Image,
    CVMatIndexed>(ns, handler)
  {
    ROS_INFO_STREAM("[" + this->getName() + "] preprocessor nh processor : " +
      this->accessProcessorNh()->getNamespace());
  }
  
  BarrelPreProcessor::~BarrelPreProcessor() {}

  bool BarrelPreProcessor::preProcess(const ImageConstPtr& input, const CVMatIndexedPtr& output)
  {
    output->setHeader(input->header);
//    output->setImageType(true);  //TO FILL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    cv_bridge::CvImagePtr inMsg;

    inMsg = cv_bridge::toCvCopy(*input, input->encoding);
    if (input->encoding.compare(sensor_msgs::image_encodings::TYPE_32FC1))
    {
      output->setImageType(true);
      ROS_INFO("rgb");
    }
    else if (input->encoding.compare(sensor_msgs::image_encodings::BGR8))
    {
      output->setImageType(false);
      ROS_INFO("depth");
    }
    else
    {
      ROS_INFO("Error");
    }

    //~ if (output->getImageType())  //!< If the image type is depth
    //~ {
      //~ inMsg = cv_bridge::toCvCopy(*input, sensor_msgs::image_encodings::TYPE_32FC1);
    //~ }
    //~ else
    //~ {
      //~ inMsg = cv_bridge::toCvCopy(*input, sensor_msgs::image_encodings::BGR8);
    //~ }
    output->setImage(inMsg->image.clone());

    if (output->image.empty())
    {
      ROS_ERROR("[Node] No more Frames or something went wrong with bag file");
      // ros::shutdown();
      return false;
    }
    return true;
  }

}  // namespace pandora_vision
