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
 *   Tsirigotis Christos <tsirif@gmail.com>
 *********************************************************************/

#ifndef PANDORA_VISION_COMMON_VISION_POSTPROCESSOR_H
#define PANDORA_VISION_COMMON_VISION_POSTPROCESSOR_H

#include <map>
#include <string>
#include <utility>

#include <boost/shared_ptr.hpp>
#include <urdf_parser/urdf_parser.h>

#include "sensor_processor/postprocessor.h"
#include "pandora_common_msgs/GeneralAlertVector.h"
#include "pandora_common_msgs/GeneralAlertInfo.h"

#include "pandora_vision_common/pois_stamped.h"
#include "pandora_vision_common/pandora_vision_interface/vision_exceptions.h"

namespace pandora_vision
{
  template <class VisionAlertMsg>
  class VisionPostProcessor : public sensor_processor::PostProcessor<POIsStamped, VisionAlertMsg>
  {
  private:
    typedef boost::shared_ptr<VisionAlertMsg> VisionAlertMsgPtr;

  public:
    /**
     * @brief Constructor
     * @param ns [const std::string&] The namespace of this postprocessor's nodeHandle
     * @param handler [sensor_processor::AbstractHandler*] A pointer of the class that
     * handles this postprocessor
     **/
    VisionPostProcessor(const std::string& ns, sensor_processor::Handler* handler);

    /**
     * @brief Virtual Destructor
     **/
    virtual
      ~VisionPostProcessor() {}

    /**
     * @brief Function that gets the Points of Interest with their timestamp and converts them
     * to a ROS message type in order to be published by a vision node
     * @param input [const POIsStampedConstPtr&] A constant reference to a constant shared pointer
     * of a POI with timestamp
     * @param output [const VisionAlertMsgPtr&] A constant reference to a shared pointer of a
     * template class that signifies the output ROS message type of each vision node
     * @return [bool] whether postprocessing finished
     **/
    virtual bool
      postProcess(const POIsStampedConstPtr& input, const VisionAlertMsgPtr& output) = 0;

  protected:
    /**
     * @brief Function that calculates parameters yaw and pitch for every POI, given its
     * coordinates, and puts them in a structure with POI's probability and timestamp
     * @param result [const POIsStampedConstPtr&] A constant reference to a constant shared
     * pointer of a POI with timestamp
     * @return [pandora_common_msgs::GeneralAlertInfoVector] ROS message type that contains
     * yaw, pitch and probability of every POI in the processed frame and the frame's header
     **/
    pandora_common_msgs::GeneralAlertVector
      getGeneralAlertInfo(const POIsStampedConstPtr& result);

  private:
    /**
     * @brief Function that finds in a dictionary a parameter of type T with the frame id
     * as key. If the parameter is not found there, external files (yaml files, launchers)
     * are searched and, when found, the parameter is inserted to the dictionary
     * @param dict [std::map<std::string, T>*] Pointer to the dictionary to be searched and
     * possibly altered
     * @param key [const std::string&] The key used to search in the dictionary
     * @return [T] The parameter that is found
     **/
    template <class T>
      T
      findParam(std::map<std::string, T>* dict, const std::string& key);

    /**
     * @brief Function that finds in a dictionary the parent frame id with the frame id
     * as key. If the parameter is not found there, the robot model is searched and when
     * the connection to the frame id is found, it is inserted to the dictionary
     * @param dict [std::map<std::string, std::string>*] Pointer to the dictionary to be
     * searched and possibly altered
     * @param key [std::string&] The key used to search in the dictionary
     * @param model_param_name [std::string&] The model parameter name
     * @return [std::string] The parent frame id
     **/
    std::string
      findParentFrameId(std::map<std::string, std::string>* dict,
        const std::string& key,
        const std::string& model_param_name);

  protected:
    /// A dictionary that includes every parent frame id that the node uses
    /// with frame id as key
    std::map<std::string, std::string> parentFrameDict_;

    /// A dictionary that includes Horizontal Fields Of View for every camera
    /// with frame id as key
    std::map<std::string, double> hfovDict_;

    /// A dictionary that includes Vertical Fields Of View for every camera
    /// with frame id as key
    std::map<std::string, double> vfovDict_;
  };

  template <class VisionAlertMsg>
    VisionPostProcessor<VisionAlertMsg>::
    VisionPostProcessor(const std::string& ns, sensor_processor::Handler* handler) :
      sensor_processor::PostProcessor<POIsStamped, VisionAlertMsg>(ns, handler),
      converter_(new GeneralAlertConverter)
    {
    }

  template <class VisionAlertMsg>
    pandora_common_msgs::GeneralAlertVector
    VisionPostProcessor<VisionAlertMsg>::
    getGeneralAlertInfo(const POIsStampedConstPtr& result)
    {
      return converter_->getGeneralAlertInfo(this->getName(), *this->accessPublicNh(), result);
    }

  template <class VisionAlertMsg>
    template <class T>
      T
      VisionPostProcessor<VisionAlertMsg>::
      findParam(std::map<std::string, T>* dict, const std::string& key)
      {
        typename std::map<std::string, T>::iterator iter;
        if ((iter = dict->find(key)) != dict->end())
        {
          return iter->second;
        }
        else
        {
          ROS_DEBUG("[%s] First at: %s", this->getName().c_str(), key.c_str());

          std::string true_key;
          if (key[0] == '/') {
            true_key = key;
          }
          else {
            true_key = '/' + key;
          }

          T param;

          if (!this->accessPublicNh()->getParam(true_key, param))
          {
            ROS_ERROR_NAMED(this->getName(),
                "Params couldn't be retrieved for %s", true_key.c_str());
            throw vision_config_error(key + " : not found");
          }
          ROS_DEBUG("[%s] Got: %f", this->getName().c_str(), param);
          dict->insert(std::make_pair(key, param));
          return param;
        }
      }

  template <class VisionAlertMsg>
    std::string
    VisionPostProcessor<VisionAlertMsg>::
    findParentFrameId(std::map<std::string, std::string>* dict,
        const std::string& key,
        const std::string& model_param_name)
    {
      typename std::map<std::string, std::string>::iterator iter;
      if ((iter = dict->find(key)) != dict->end())
      {
        return iter->second;
      }
      else
      {
        ROS_DEBUG("[%s] First at: %s", this->getName().c_str(), key.c_str());
          
        std::string true_key;
        if (key[0] == '/') {
          true_key = key;
          true_key.erase(0, 1);
        }
        else {
          true_key = key;
        }

        std::string robot_description;

        if (!this->accessPublicNh()->getParam(model_param_name, robot_description))
        {
          ROS_ERROR_NAMED(this->getName(),
              "Robot description couldn't be retrieved from the parameter server.");
          throw vision_config_error(model_param_name + " : not found");
        }

        std::string parent_frame_id;

        boost::shared_ptr<urdf::ModelInterface> model(
          urdf::parseURDF(robot_description));
        // Get current link and its parent
        boost::shared_ptr<const urdf::Link> currentLink( model->getLink(true_key) );
        boost::shared_ptr<const urdf::Link> parentLink( currentLink->getParent() );
        // Set the parent frame_id to the parent of the frame_id
        parent_frame_id = parentLink->name;
        ROS_DEBUG("[%s] Got: %s", this->getName().c_str(), parent_frame_id.c_str());
        dict->insert(std::make_pair(key, parent_frame_id));
        return parent_frame_id;
      }
    }
}  // namespace pandora_vision

#endif  // PANDORA_VISION_COMMON_VISION_POSTPROCESSOR_H
