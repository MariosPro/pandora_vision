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
* Author:  Marios Protopapas
*********************************************************************/

#ifndef PANDORA_VISION_COLOR_COLOR_PROCESSOR_H
#define PANDORA_VISION_COLOR_COLOR_PROCESSOR_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "pandora_vision_common/pandora_vision_interface/vision_processor.h"
#include "pandora_vision_common/cv_mat_stamped.h"
#include "pandora_vision_common/pois_stamped.h"
#include "pandora_vision_common/bbox_poi.h"
#include "pandora_vision_color/color_detector.h"
#include <pandora_vision_color/color_cfgConfig.h>
#include <dynamic_reconfigure/server.h>


namespace pandora_vision
{
namespace pandora_vision_color
{
  class ColorProcessor : public VisionProcessor
  {
    public:

      /**
       * @brief: The Main constructor for the Color code Processor objects.
       * @param ns[const std::string&]: The namespace for the node.
       * @param handler[sensor_processor::Handler*]: A pointer to the handler
       * of the processor used to access the nodehandle of the node
       */
      virtual void
      initialize(const std::string& ns, sensor_processor::Handler* handler);

      /**
        @brief Class Constructor
      */
      ColorProcessor(void);

    protected:
      /**
       * @brief
       **/
      virtual bool
        process(const CVMatStampedConstPtr& input, const POIsStampedPtr& output);

    private:
      BBoxPOIPtr bounding_box_;

      boost::shared_ptr<ColorDetector> colorDetectorPtr_;

      dynamic_reconfigure::Server< ::pandora_vision_color::color_cfgConfig>::CallbackType f; 

      //!< The dynamic reconfigure (color) parameters' server
      boost::shared_ptr< dynamic_reconfigure::Server< ::pandora_vision_color::color_cfgConfig> > server_; 

      void parametersCallback(
        const ::pandora_vision_color::color_cfgConfig& config,
        const uint32_t& level);
  };
}  // namespace pandora_vision_color
}  // namespace pandora_vision
#endif  // PANDORA_VISION_COLOR_COLOR_PROCESSOR_H
