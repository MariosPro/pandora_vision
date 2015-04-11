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
 *   Chatzielefthriou Eirini <eirini.ch0@gmail.com>
 *   Tsirigotis Christos <tsirif@gmail.com>
 *********************************************************************/

#ifndef PANDORA_VISION_COMMON_VICTIM_CV_MAT_STAMPED_H
#define PANDORA_VISION_COMMON_VICTIM_CV_MAT_STAMPED_H

#include <opencv2/opencv.hpp>
#include "pandora_vision_common/cv_mat_stamped.h"

namespace pandora_vision
{
  class VictimCVMatStamped : public CVMatStamped  // TO FIX!!!!!!!!!!!!!!!
  {
    public:
      typedef boost::shared_ptr<VictimCVMatStamped> Ptr;
      typedef boost::shared_ptr<VictimCVMatStamped const> ConstPtr;

    public:
      cv::Rect boundingBox;
      cv::Point2f keypoint;

    public:
      void setBoundingBox(const cv::Rect&);
      void setKeypoint(const cv::Point2f&);
      
      cv::Rect& getBoundingBox() const;
      cv::Point2f getKeypoint() const;
  };

  void VictimCVMatStamped::setBoundingBox(const cv::Rect& boundingBoxArg)
  {
    boundingBox = boundingBoxArg;
  }
  void VictimCVMatStamped::setKeypoint(const cv::Point2f& keypointArg)
  {
    keypoint = keypointArg;
  }
  
  const cv::Rect& VictimCVMatStamped::getBoundingBox() const
  {
    return boundingBox;
  }
  cv::Point2f VictimCVMatStamped::getKeypoint() const
  {
    return keypoint;
  }

  typedef VictimCVMatStamped::Ptr VictimCVMatStampedPtr;
  typedef VictimCVMatStamped::ConstPtr VictimCVMatStampedConstPtr;
}  // namespace pandora_vision

#endif  // PANDORA_VISION_COMMON_VICTIM_CV_MAT_STAMPED_H
