/******************************************************************************
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
* Authors: Manos Tsardoulias
*********************************************************************/

#include "pandora_vision_annotator/pandora_vision_annotator_controller.h"


namespace pandora_vision
{
  /**
  @brief Thread that performs the ros::spin functionality
  @return void
  **/
  void spinThreadFunction(void)
  {
    ros::spin();
  }

  /**
  @brief Default contructor
  @param argc [int] Number of input arguments
  @param argv [char **] Input arguments
  @return void
  **/
  CController::CController(int argc,char **argv):
    connector_(argc,argv),
    argc_(argc),
    argv_(argv)
  {
    PredatorNowOn == false;
    offset = 0;
  }

  /**
  @brief Default destructor
  @return void
  **/
  CController::~CController(void)
  {

  }

  /**
  @brief Initializes the Qt event connections and ROS subscribers and publishers
  @return void
  **/
  void CController::initializeCommunications(void)
  {

 /* QObject::connect(*/
    //&connector_,SIGNAL(rosTopicGiven()),
    /*this, SLOT(rosTopicGiven()));*/

     QObject::connect(
       &connector_,SIGNAL(offlineModeGiven()),
       this, SLOT(offlineModeGiven()));
  
    QObject::connect(
      &connector_,SIGNAL(onlineModeGiven()),
      this, SLOT(onlineModeGiven()));
  
    QObject::connect(
      this,SIGNAL(updateImage()),
      &connector_, SLOT(updateImage())); 
    
   
    annotationPublisher_ = n_.advertise<pandora_vision_msgs::AnnotationMsg>("/vision/annotator_output", 1000);
    predatorSubscriber_ = n_.subscribe("/vision/predator_alert",1, &CController::predatorCallback, this );

  }

  void CController::offlineModeGiven(void)
  {
     onlinemode = false;
     QObject::connect(
       &connector_,SIGNAL(rosTopicGiven()),
        this, SLOT(rosTopicGiven()));

     QObject::connect(
       &connector_,SIGNAL(predatorEnabled()),
        this, SLOT(predatorEnabled()));

     QObject::connect(
        &connector_,SIGNAL(appendToFile()),
        this, SLOT(appendToFile()));

    QObject::connect(
        &connector_,SIGNAL(removeFile()),
        this, SLOT(removeFile()));
  }      

  void CController::onlineModeGiven(void)
  {
    onlinemode = true;
    QObject::connect(
      &connector_,SIGNAL(rosTopicGiven()),
      this, SLOT(rosTopicGiven()));
  }

  void CController::removeFile(void)
  {
    std::string package_path = ros::package::getPath("pandora_vision_annotator");
    std::stringstream filename;
    filename << package_path << "/data/annotations.txt";
    ImgAnnotations::removeFile(filename.str());
  }
  void CController::appendToFile(void)
  {
    std::string package_path = ros::package::getPath("pandora_vision_annotator");
    std::stringstream filename;
    filename << package_path << "/data/annotations.txt";
    ImgAnnotations::getLastFrameIndex(filename.str(),offset);
    offset += 1;
    ROS_INFO_STREAM("Frame indexing starting at: " << offset);
  }

  void CController::predatorEnabled(void)
  {
    ROS_INFO("PREDATOR NOW ON");
    cv::Mat temp;
    PredatorNowOn == true;
    baseFrame = connector_.getFrameNumber();
    sendInitialFrame(baseFrame);
    enableBackwardTracking = false;

    /*pandora_vision_msgs::AnnotationMsg annotationMsg;
    currentFrameNo_ = connector_.getFrameNumber();
    connector_.getcurrentFrame(currentFrameNo_,&temp);
    baseFrame = currentFrameNo_;
    if(baseFrame > 0)
      enableBackwardTracking = true;
    annotationMsg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
    annotationMsg.header.stamp = msgHeader_[currentFrameNo_].stamp;
    annotationMsg.x = ImgAnnotations::annotations[0].x1;
    annotationMsg.y = ImgAnnotations::annotations[0].y1;
    annotationMsg.width =  ImgAnnotations::annotations[0].x2 - ImgAnnotations::annotations[0].x1;
    annotationMsg.height = ImgAnnotations::annotations[0].y2 - ImgAnnotations::annotations[0].y1;
    cv_bridge::CvImage out_msg;
    out_msg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
    out_msg.header.stamp = msgHeader_[currentFrameNo_].stamp;
    out_msg.encoding = "rgb8" ;     
    out_msg.image = temp;
    out_msg.toImageMsg(annotationMsg.img);
    annotationPublisher_.publish(annotationMsg);
    ROS_INFO_STREAM("send initial frame " << currentFrameNo_ 
                    << " " << annotationMsg.header.frame_id
                    << " " << annotationMsg.header.stamp 
                    << " " << annotationMsg.x 
                    << " " << annotationMsg.y 
                    << " " << annotationMsg.width 
                    << " " << annotationMsg.height);*/
  }

  void CController::rosTopicGiven(void)
  {
    QString ros_topic = connector_.getRosTopic();
    if(onlinemode)
    {
      img_subscriber_ = n_.subscribe(
      ros_topic.toStdString(),
      1,
      &CController::receiveImage,
      this);
    }

    else
    { 
      QString bag_name = connector_.getBagName();
      std::string  package_path = ros::package::getPath("pandora_vision_annotator");
      std::stringstream bag_path;
      count = 0;
      currentFrameNo_ = 0;
      bag_path << package_path << "/data/" <<bag_name.toStdString();
      loadBag(bag_path.str(),ros_topic.toStdString());
    }
  }

  void CController::loadBag(const std::string& filename, const std::string& topic)
  {     
    ROS_INFO("Loading bag...");
    rosbag::Bag bag;
    bag.open(filename, rosbag::bagmode::Read);
    rosbag::View view(bag, rosbag::TopicQuery(topic));
    // Load all messages 
    int count = 0;
    BOOST_FOREACH(rosbag::MessageInstance const m, view)
    {
      sensor_msgs::Image::ConstPtr img = m.instantiate<sensor_msgs::Image>();
      sensor_msgs::PointCloud2::ConstPtr pc =m.instantiate<sensor_msgs::PointCloud2>();
      
      if(img != NULL)
      {
     
       receiveImage(img);
       count++;
      }

      if (pc != NULL)
      {
        receivePointCloud(pc);
        count++;
      }
    }
    connector_.setFrames(frames,offset);
    connector_.setcurrentFrame(0);
    frames.clear();
  }

  void CController::receivePointCloud(const sensor_msgs::PointCloud2ConstPtr& msg)
  {
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::fromROSMsg (*msg, *cloud);
    cv::Mat imageFrame;
    if (cloud->isOrganized())
    {       
      imageFrame = cv::Mat(cloud->height, cloud->width, CV_8UC3);
      for (int h = 0; h < imageFrame.rows; h++)
      {
        for (int w = 0; w < imageFrame.cols; w++)
        {
          pcl::PointXYZRGB point = cloud->at(w, h);
          Eigen::Vector3i rgb = point.getRGBVector3i();
          imageFrame.at<cv::Vec3b>(h,w)[2] = rgb[2];
          imageFrame.at<cv::Vec3b>(h,w)[1] = rgb[1];
          imageFrame.at<cv::Vec3b>(h,w)[0] = rgb[0];
        }
      }
    } 
    frames.push_back(imageFrame);
  }

  void CController::predatorCallback(const pandora_vision_msgs::PredatorMsg& msg)
  { 
 
    if(msg.header.stamp == msgHeader_[currentFrameNo_ ].stamp && msg.x != 0 && msg.y != 0 
      && msg.width != 0 && msg.height != 0 )
    {
       connector_.setPredatorValues(msg.x, msg.y, msg.width, msg.height);
       ROS_INFO_STREAM("predator alert for frame " << currentFrameNo_);
     
    }
    bool x;
    if(currentFrameNo_ < msgHeader_.size()-1 && !enableBackwardTracking )
    {
      x= false;
      ROS_INFO("SENDING NEXT FRAME ++");
      sendNextFrame(x);
    }
    
    if(currentFrameNo_ == msgHeader_.size()-1 )
    {
      if(baseFrame == 0)
      {
        sendFinalFrame();
      } 
      else
      {
      sendInitialFrame(baseFrame);
      ROS_INFO_STREAM("ENABLE BACKWARD REINITIALIZING");
      enableBackwardTracking = true;
      }
    }
    
    if(currentFrameNo_ > 0 && enableBackwardTracking)
    {
      x= true;
      ROS_INFO("SEND NEXT FRAME --");
      sendNextFrame(x);
    }

    if(currentFrameNo_ == 0  && enableBackwardTracking)
    {
      ROS_INFO_STREAM("SEND FINAL FRAME");
      sendFinalFrame();
    }



    
  }

  void CController::sendInitialFrame(const int& initialFrame)
  {
      pandora_vision_msgs::AnnotationMsg annotationMsg;
      cv::Mat temp;
      currentFrameNo_= initialFrame ; 
      connector_.getcurrentFrame(currentFrameNo_, &temp);
      annotationMsg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
      annotationMsg.header.stamp = msgHeader_[currentFrameNo_].stamp;
      annotationMsg.x = ImgAnnotations::annotations[0].x1;
      annotationMsg.y = ImgAnnotations::annotations[0].y1;
      annotationMsg.width =  ImgAnnotations::annotations[0].x2 - ImgAnnotations::annotations[0].x1;
      annotationMsg.height = ImgAnnotations::annotations[0].y2 - ImgAnnotations::annotations[0].y1;
      cv_bridge::CvImage out_msg;
      out_msg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
      out_msg.header.stamp = msgHeader_[currentFrameNo_].stamp;
      out_msg.encoding = "rgb8" ;     
      out_msg.image = temp;
      out_msg.toImageMsg(annotationMsg.img);
      annotationPublisher_.publish(annotationMsg);
      ROS_INFO_STREAM("send initial frame" << currentFrameNo_ 
                    << " " << annotationMsg.header.frame_id
                    << " " << annotationMsg.header.stamp 
                    << " " << annotationMsg.x 
                    << " " << annotationMsg.y 
                    << " " << annotationMsg.width 
                    << " " << annotationMsg.height);

  }
  void CController::sendNextFrame(bool enableBackward)
  {  
      pandora_vision_msgs::AnnotationMsg annotationMsg;
      cv::Mat temp;
      if(enableBackward)
        currentFrameNo_--;
      else
        currentFrameNo_+= 1; 
      connector_.getcurrentFrame(currentFrameNo_, &temp);
      annotationMsg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
      annotationMsg.header.stamp = msgHeader_[currentFrameNo_].stamp;
      annotationMsg.x = -1;
      annotationMsg.y = -1;
      annotationMsg.width =  -1;
      annotationMsg.height = -1;
      cv_bridge::CvImage out_msg;
      out_msg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
      out_msg.header.stamp = msgHeader_[currentFrameNo_].stamp;
      out_msg.encoding = "rgb8"; 
      out_msg.image = temp;
      out_msg.toImageMsg(annotationMsg.img);
      annotationPublisher_.publish(annotationMsg);
      connector_.setcurrentFrame(currentFrameNo_);
      ROS_INFO_STREAM("send next frame "<< currentFrameNo_ 
                       << " " << annotationMsg.header.frame_id 
                       << " " << annotationMsg.header.stamp 
                       << " " << annotationMsg.x 
                       << " " << annotationMsg.y 
                       << " " << annotationMsg.width 
                       << " " << annotationMsg.height);

  }

  void CController::sendFinalFrame()
  {
    pandora_vision_msgs::AnnotationMsg annotationMsg;
    cv::Mat temp;
    currentFrameNo_= 0 ; 
    connector_.getcurrentFrame(currentFrameNo_, &temp);
    annotationMsg.header.frame_id = "close";
    annotationMsg.header.stamp = msgHeader_[currentFrameNo_].stamp;
    annotationMsg.x = 2;
    annotationMsg.y = 2;
    annotationMsg.width =  2;
    annotationMsg.height = 2;
    cv_bridge::CvImage out_msg;
    out_msg.header.frame_id = msgHeader_[currentFrameNo_].frame_id;
    out_msg.header.stamp = msgHeader_[currentFrameNo_].stamp;
    out_msg.encoding = "rgb8"; 
    out_msg.image = temp;
    out_msg.toImageMsg(annotationMsg.img);
    annotationPublisher_.publish(annotationMsg);
    connector_.setcurrentFrame(currentFrameNo_);
    ROS_INFO_STREAM("send 'closing' frame " << currentFrameNo_ 
                     << " " << annotationMsg.header.frame_id 
                     << " " << annotationMsg.header.stamp 
                     << " " << annotationMsg.x 
                     << " " << annotationMsg.y 
                     << " " << annotationMsg.width 
                     << " " << annotationMsg.height);

  }
  void CController::receiveImage(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImageConstPtr in_msg;
    cv::Mat temp;
    try
    {
      in_msg = cv_bridge::toCvCopy(msg);
      if(msg->encoding == "8UC1")
      {
        cv::cvtColor(in_msg->image, temp, CV_GRAY2RGB);
      }
      else if (msg->encoding == "16UC1" || msg->encoding == "32FC1")
      {
      
        double min, max;
        cv::minMaxLoc(in_msg->image, &min, &max);
   	cv::Mat img_scaled_8u;
        cv::Mat(in_msg->image-min).convertTo(img_scaled_8u, CV_8UC1, 255. / (max - min));
        cv::cvtColor(img_scaled_8u, temp, CV_GRAY2RGB);
      }
      else if(msg->encoding == "rgb8" || msg->encoding == "bgr8" )
      {
        in_msg = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
        temp = in_msg->image;
        //cv::cvtColor(in_msg->image, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
      }

    }

    catch(cv_bridge::Exception& e)
    {
      qWarning("CController::receiveImage() while trying to convert image from '%s' to 'rgb8' an exception was thrown ((%s)", msg->encoding.c_str(), e.what());
      return;
    }

    if(!onlinemode)
    {
      count++;
      frames.push_back(temp);
      msgHeader_.push_back(msg->header);
    }

    else
    {
      QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
      dest.bits(); // enforce deep copy, see documentation
      connector_.setImage(dest);
      connector_.msgTimeStamp(msg->header);
      Q_EMIT updateImage();
    }
  }   
 
 
  /**
  @brief Initializes the ROS spin and Qt threads
  @return bool
  **/

  bool CController::init(void)
  {
    if ( ! ros::master::check() )
    {
      return false;
    }
    connector_.show();

    initializeCommunications();
    boost::thread spinThread(&spinThreadFunction);
    return true;
  }
}


