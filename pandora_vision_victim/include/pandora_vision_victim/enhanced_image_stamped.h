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

#ifndef PANDORA_VISION_VICTIM_ENHANCED_IMAGE_STAMPED_H
#define PANDORA_VISION_VICTIM_ENHANCED_IMAGE_STAMPED_H

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>
#include "std_msgs/Header.h"

namespace pandora_vision
{
  class EnhancedImageStamped
  {
    public:
      typedef boost::shared_ptr<EnhancedImageStamped> Ptr;
      typedef boost::shared_ptr<EnhancedImageStamped const> ConstPtr;
      typedef cv::Rect_<float> Rect2f;
      
    public:
      std_msgs::Header header;
      bool isDepth;
      
      cv::Mat depthImage;
      cv::Mat rgbImage;
      
      std::vector<Rect2f> areasOfInterest;
      
    public:
      void setHeader(const std_msgs::Header&);
      const std_msgs::Header& getHeader() const;

      void setDepth(bool depth);
      bool getDepth() const;

      void setRgbImage(const cv::Mat&);
      cv::Mat getRgbImage() const;
      
      void setDepthImage(const cv::Mat&);
      cv::Mat getDepthImage() const;
      
      void setAreas(const std::vector<Rect2f>&);
      std::vector<Rect2f> getAreas() const;
      
      void setArea(int , const Rect2f&);
      Rect2f getArea(int it) const;
  };
  
  void EnhancedImageStamped::setHeader(const std_msgs::Header& headerArg)
  {
    header = headerArg;
  }
  const std_msgs::Header& EnhancedImageStamped::getHeader() const
  {
    return header;
  }
  
  void EnhancedImageStamped::setDepth(bool depth)
  {
    isDepth = depth;
  }
  bool EnhancedImageStamped::getDepth() const
  {
    return isDepth;
  }
  
  void EnhancedImageStamped::setRgbImage(const cv::Mat& imageArg)
  {
    rgbImage = imageArg;
  }
  cv::Mat EnhancedImageStamped::getRgbImage() const
  {
    return rgbImage;
  }
  
  void EnhancedImageStamped::setDepthImage(const cv::Mat& imageArg)
  {
    depthImage = imageArg;
  }
  cv::Mat EnhancedImageStamped::getDepthImage() const
  {
    return depthImage;
  }
  
  void EnhancedImageStamped::setAreas(const std::vector<EnhancedImageStamped::Rect2f>& areas)
  {
    areasOfInterest = areas;
  }
  std::vector<EnhancedImageStamped::Rect2f> EnhancedImageStamped::getAreas() const
  {
    return areasOfInterest;
  }
  
  void EnhancedImageStamped::setArea(int it, const Rect2f& area)
  {
    areasOfInterest[it] = area;
  }
  EnhancedImageStamped::Rect2f EnhancedImageStamped::getArea(int it) const
  {
    return areasOfInterest[it];
  }
  
  typedef EnhancedImageStamped::Rect2f Rect2f;
  typedef EnhancedImageStamped::Ptr EnhancedImageStampedPtr;
  typedef EnhancedImageStamped::ConstPtr EnhancedImageStampedConstPtr;
  
}  // namespace pandora_vision

#endif  // PANDORA_VISION_VICTIM_ENHANCED_IMAGE_STAMPED_H
