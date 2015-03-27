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
* Author: Marios Protopapas
*********************************************************************/

#include "pandora_vision_victim/feature_extractors/dominant_color.h"



namespace pandora_vision
{
  DominantColorExtractor::DominantColorExtractor(cv::Mat* img)
    : BaseFeatureExtractor(img)
  {

  }

  std::vector<double> DominantColorExtractor::extract(void)
  {
    std::vector<double> ret;

    double maxVal = 0;
    double val = 0;
    unsigned int size = cv::Size(_img->size()).height;
/*    ROS_INFO_STREAM("HIST SIZE" <<_img->size());*/
    /*ROS_INFO_STREAM("HEIGHT" << size);*/
    for( int i = 0 ; i < size ; i++ )
    {
      double binVal = static_cast<double>(_img->at<float>(i));
      if(binVal > maxVal)
      {
        maxVal = binVal;
        val = i;
      }
    }

    ret.push_back(val);
    if(maxVal != 0)
      ret.push_back(maxVal/(640 * 480));
    else
      ret.push_back(0);

    return ret;
  }
}// namespace pandora_vision


