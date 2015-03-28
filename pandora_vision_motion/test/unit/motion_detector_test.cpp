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
* Author: Despoina Paschalidou
*         Miltiadis Kofinas, <mkofinas@gmail.com>
*********************************************************************/
#include "pandora_vision_motion/motion_detector.h"

#include "gtest/gtest.h"
namespace pandora_vision
{
  /**
  @class MotionDetectorTest
  @brief Tests the integrity of methods of class MotionDetector
  **/
  class MotionDetectorTest : public ::testing::Test
  {
    public:
      MotionDetectorTest() {}
    
    protected:
      virtual void SetUp()
      {
        WIDTH = 640;
        HEIGHT = 480;
      }
      
      /* accessors to private functions */
      int* detectMotionPosition(cv::Mat& frame);
      
      int detectMotion(cv::Mat& frame);
      int* getMotionPosition();
      
    protected:
      int WIDTH;
      int HEIGHT;
      
    private:
      MotionDetector motionDetector_;
  };
  
  int* MotionDetectorTest::detectMotionPosition(cv::Mat& frame)
  {
    motionDetector_.detectMotionPosition(frame);
    return motionDetector_.getMotionPosition();
  }
  
  int* MotionDetectorTest::getMotionPosition()
  {
    return motionDetector_.getMotionPosition();
  }
  
  int MotionDetectorTest::detectMotion(cv::Mat& frame)
  {
    return motionDetector_.detectMotion(frame);
  }
  
  /* Unit Tests */
  //! Tests MotionDetector::detectMotionPosition
  TEST_F(MotionDetectorTest, detectMotionPositionImageNoData)
  {
    cv::Mat frame;
    int* bounding_boxes  = detectMotionPosition(frame);
    for(int i = 0; i < 4; i++)
      EXPECT_EQ(0, bounding_boxes[i]);
  }  
  
  TEST_F(MotionDetectorTest, detectMotionPositionBlackImage)
  {
    cv::Mat blackFrame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
    int* bounding_boxes  = detectMotionPosition(blackFrame);
    for(int i = 0; i < 4; i++)
      EXPECT_EQ(0, bounding_boxes[i]);
  }
  
  TEST_F(MotionDetectorTest, detectMotionPositionImageWithRectangle)
  {
    cv::Mat frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
    frame(cv::Rect(17, 63, 8, 8)) = 255;

    int* bounding_boxes  = detectMotionPosition(frame);
    EXPECT_EQ(21, bounding_boxes[0]);
    EXPECT_EQ(67, bounding_boxes[1]);
    EXPECT_EQ(8, bounding_boxes[2]);
    EXPECT_EQ(8, bounding_boxes[3]);
  }
  
  TEST_F(MotionDetectorTest, detectMotionPositionImageWithRectangle2)
  {
    cv::Mat frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
    frame(cv::Rect(100, 50, 75, 45)) = 255;

    int* bounding_boxes  = detectMotionPosition(frame);
    EXPECT_EQ(137, bounding_boxes[0]);
    EXPECT_EQ(72, bounding_boxes[1]);
    EXPECT_EQ(75, bounding_boxes[2]);
    EXPECT_EQ(45, bounding_boxes[3]);
  }
  
  TEST_F(MotionDetectorTest, detectMotionPositionImageWithRotatedRectangle)
  {
    cv::Mat frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);

    std::vector<cv::Point> rectangleVertices(4);
    rectangleVertices[0] = cv::Point(100, 200);
    rectangleVertices[1] = cv::Point(100, 300);
    rectangleVertices[2] = cv::Point(150, 200);
    rectangleVertices[3] = cv::Point(150, 300);
  
    cv::fillConvexPoly(frame, &rectangleVertices[0], rectangleVertices.size(), 255);
    
    int* bounding_boxes  = detectMotionPosition(frame);
    EXPECT_EQ(125, bounding_boxes[0]);
    EXPECT_EQ(250, bounding_boxes[1]);
    EXPECT_EQ(51, bounding_boxes[2]);
    EXPECT_EQ(101, bounding_boxes[3]);
  }
  
  TEST_F(MotionDetectorTest, detectMotionPositionImageWithHighStdDeviation)
  {
    cv::Mat frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
    frame(cv::Rect(50, 50, 400, 400)) = 255;

    int* bounding_boxes  = detectMotionPosition(frame);
    EXPECT_EQ(0, bounding_boxes[0]);
    EXPECT_EQ(0, bounding_boxes[1]);
    EXPECT_EQ(0, bounding_boxes[2]);
    EXPECT_EQ(0, bounding_boxes[3]);
  }
  
  //! Tests MotionDetector::detectMotion
  TEST_F(MotionDetectorTest, detectMotionNoMotion)
  {
    cv::Mat frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
 
    int motionEvaluation  = detectMotion(frame);
    EXPECT_EQ(0, motionEvaluation);
  }
  
  TEST_F(MotionDetectorTest, detectMotionNoData)
  {
    cv::Mat frame;
 
    int motionEvaluation  = detectMotion(frame);
    EXPECT_EQ(0, motionEvaluation);
  }
  
  TEST_F(MotionDetectorTest, detectMotionSingleChannelImage)
  {
    cv::Mat frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
 
    int motionEvaluation  = detectMotion(frame);
    EXPECT_EQ(0, motionEvaluation);
  }
  
  TEST_F(MotionDetectorTest, detectMotionRectangleLinearMovementSlowly)
  {
    cv::Mat frameBackground = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
    cv::Mat frame = frameBackground.clone();
    
    int objectWidth = 50;
    int objectHeight = 50;
    int topLeftXCoordinate = 10;
    int topLeftYCoordinate = 10;
    int stepXCoordinate = 20;
    int stepYCoordinate = 20;
    
    cv::Scalar rectColor = cv::Scalar(255, 255, 255);
    cv::Point startingPoint = cv::Point(topLeftXCoordinate, topLeftYCoordinate);
    cv::Point endingPoint = cv::Point(topLeftXCoordinate + objectWidth, 
      topLeftYCoordinate + objectHeight);
      
    cv::rectangle(frame, startingPoint, endingPoint, rectColor, -1); 
    
    int motionEvaluation  = detectMotion(frame);
    EXPECT_EQ(0, motionEvaluation);
    for (int ii = 0; ii < 10; ii++)
    {
      topLeftXCoordinate += stepXCoordinate;
      topLeftYCoordinate += stepYCoordinate;
      startingPoint = cv::Point(topLeftXCoordinate, topLeftYCoordinate);
      endingPoint = cv::Point(topLeftXCoordinate + objectWidth, 
        topLeftYCoordinate + objectHeight);
        
      frame = frameBackground.clone();
      cv::rectangle(frame, startingPoint, endingPoint, rectColor, -1);       
      
      motionEvaluation  = detectMotion(frame);
      EXPECT_EQ(1, motionEvaluation);
    }
  }
  
  TEST_F(MotionDetectorTest, detectMotionRectangleLinearMovementFastNoOverlaps)
  {
    cv::Mat frameBackground = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
    cv::Mat frame = frameBackground.clone();
    
    int objectWidth = 100;
    int objectHeight = 100;
    int topLeftXCoordinate = 1;
    int topLeftYCoordinate = 1;
    int stepXCoordinate = 100;
    int stepYCoordinate = 100;
    
    cv::Scalar rectColor = cv::Scalar(255, 255, 255);
    cv::Point startingPoint = cv::Point(topLeftXCoordinate, topLeftYCoordinate);
    cv::Point endingPoint = cv::Point(topLeftXCoordinate + objectWidth - 1, 
      topLeftYCoordinate + objectHeight - 1);
      
    cv::rectangle(frame, startingPoint, endingPoint, rectColor, -1); 
    
    int motionEvaluation  = detectMotion(frame);
    EXPECT_EQ(0, motionEvaluation);
    for (int ii = 0; ii < 3; ii++)
    {
      topLeftXCoordinate += stepXCoordinate;
      topLeftYCoordinate += stepYCoordinate;
      startingPoint = cv::Point(topLeftXCoordinate, topLeftYCoordinate);
      endingPoint = cv::Point(topLeftXCoordinate + objectWidth - 1, 
        topLeftYCoordinate + objectHeight - 1);
        
      frame = frameBackground.clone();
      cv::rectangle(frame, startingPoint, endingPoint, rectColor, -1);       
      
      motionEvaluation  = detectMotion(frame);
      EXPECT_EQ(2, motionEvaluation);
      
      int* bounding_boxes  = getMotionPosition();
      EXPECT_EQ(topLeftXCoordinate + 50, bounding_boxes[0]);
      EXPECT_EQ(topLeftYCoordinate + 50, bounding_boxes[1]);
      EXPECT_EQ(objectWidth, bounding_boxes[2]);
      EXPECT_EQ(objectHeight, bounding_boxes[3]);
    }
  }

  TEST_F(MotionDetectorTest, detectMotionRectangleAngularMovement)
  {
    cv::Mat frameBackground = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
    cv::Mat frame = frameBackground.clone();
    
    int objectWidth = 200;
    int objectHeight = 200;
    int centerXCoordinate = 240;
    int centerYCoordinate = 240;
    int objectHalfWidth = ceil(static_cast<double>(objectWidth) / 2);
    int objectHalfHeight = ceil(static_cast<double>(objectHeight) / 2);
    
    cv::Point rectangleCenter = cv::Point(centerXCoordinate, centerYCoordinate);
    
    std::vector<cv::Point> rectangleVertices;
    rectangleVertices.push_back(cv::Point(centerXCoordinate - objectHalfWidth,
      centerYCoordinate - objectHalfHeight));
    rectangleVertices.push_back(cv::Point(centerXCoordinate - objectHalfWidth,
      centerYCoordinate + objectHalfHeight - 1));
    rectangleVertices.push_back(cv::Point(centerXCoordinate + objectHalfWidth - 1,
      centerYCoordinate + objectHalfHeight - 1));
    rectangleVertices.push_back(cv::Point(centerXCoordinate + objectHalfWidth - 1,
      centerYCoordinate - objectHalfHeight));
  
    cv::Scalar rectColor = cv::Scalar(255, 255, 255);
    cv::fillConvexPoly(frame, &rectangleVertices[0], rectangleVertices.size(), rectColor);
    
    int motionEvaluation  = detectMotion(frame);
    EXPECT_EQ(0, motionEvaluation);
    
    int* bounding_boxes  = getMotionPosition();
    EXPECT_EQ(0, bounding_boxes[0]);
    EXPECT_EQ(0, bounding_boxes[1]);
    EXPECT_EQ(0, bounding_boxes[2]);
    EXPECT_EQ(0, bounding_boxes[3]);
    
    double angle = 45.0;
    double scale = 1.0;
    cv::Mat rotationMatrix = getRotationMatrix2D(rectangleCenter, angle, scale);
    
    for (int ii = 0; ii < 4; ii++)
    {
      cv::transform(rectangleVertices, rectangleVertices, rotationMatrix);
      
      frame = frameBackground.clone();
      cv::fillConvexPoly(frame, &rectangleVertices[0], rectangleVertices.size(), rectColor);
      
      motionEvaluation = detectMotion(frame);
      EXPECT_EQ(1, motionEvaluation);
      
      bounding_boxes  = getMotionPosition();
      EXPECT_NEAR(centerXCoordinate, bounding_boxes[0], 1);
      EXPECT_NEAR(centerYCoordinate, bounding_boxes[1], 1);
      
      if (ii % 2)
      {
        EXPECT_NEAR(objectWidth, bounding_boxes[2], 1);
        EXPECT_NEAR(objectHeight, bounding_boxes[3], 1);

      }
      else
      {
        EXPECT_NEAR(sqrt(2) * objectWidth, bounding_boxes[2], 1);
        EXPECT_NEAR(sqrt(2) * objectHeight, bounding_boxes[3], 1);
      }
    }
  }
} // namespace pandora_vision
