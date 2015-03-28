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
 * Authors:
 *   Tsirigotis Christos <tsirif@gmail.com>
 *********************************************************************/

#ifndef PANDORA_VISION_COMMON_CV_MAT_STAMPED_H
#define PANDORA_VISION_COMMON_CV_MAT_STAMPED_H

#include <boost/shared_ptr.hpp>
#include <opencv2/opencv.hpp>
#include "std_msgs/Header.h"

namespace pandora_vision
{
  class CVMatStamped {
    public:
      typedef boost::shared_ptr<CVMatStamped> Ptr;
      typedef boost::shared_ptr<CVMatStamped const> ConstPtr;

    public:
    std_msgs::Header header;
    cv::Mat image;

    public:
    void setHeader(const std_msgs::Header&);
    const std_msgs::Header& getHeader() const;

    void setImage(const cv::Mat&);
    cv::Mat getImage() const;
  };

  void CVMatStamped::setHeader(const std_msgs::Header& headerArg)
  {
    header = headerArg;
  }

  // mby delete
  const std_msgs::Header& CVMatStamped::getHeader() const
  {
    return header;
  }

  void CVMatStamped::setImage(const cv::Mat& imageArg)
  {
    image = imageArg;
  }

  cv::Mat CVMatStamped::getImage() const
  {
    return image;
  }

  typedef CVMatStamped::Ptr CVMatStampedPtr;
  typedef CVMatStamped::ConstPtr CVMatStampedConstPtr;
}  // namespace pandora_vision

#endif  // PANDORA_VISION_COMMON_CV_MAT_STAMPED_H
