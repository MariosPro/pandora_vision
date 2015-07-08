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
 *   Bosdelekidis Vasilis <vasilis1bos@gmail.com>
 *********************************************************************/

#include <cmath>
#include <gtest/gtest.h>
#include "pandora_vision_obstacle/barrel_detection/barrel_processor.h"

namespace pandora_vision
{
  /**
    @class BarrelProcessorTest
    @brief Tests the integrity of methods of class BarrelProcessor
   **/
  class BarrelProcessorTest : public ::testing::Test
  {
    protected:

      BarrelProcessorTest () {}

      /**
        @brief Constructs a rectangle of width @param x and height of @param y.
        @param[in] upperLeft [const cv::Point2f&] The upper left vertex of the
        rectangle to be created
        @param[in] x [const int&] The rectangle's width
        @param[in] y [const int&] The rectangle's height
        @param[in] rgbIn [const unsigned char] The rgb value for all points i
        nside the rectangle
        @param[out] image [cv::Mat*] The image on which the rectangle will be
        imprinted on
        return void
       **/
      void generateRgbRectangle (
          const cv::Point2f& upperLeft,
          const int& x,
          const int& y,
          const unsigned char rgbIn,
          cv::Mat* image );

      /**
        @brief Constructs a rectangle of width @param x and height of @param y.
        @param[in] upperLeft [const cv::Point2f&] The upper left vertex of the
        rectangle to be created
        @param[in] x [const int&] The rectangle's width
        @param[in] y [const int&] The rectangle's height
        @param[in] depthIn [const float&] The depth value for all points inside
        the rectangle
        @param[out] image [cv::Mat*] The image on which the rectangle will be
        imprinted
        return void
       **/
      void generateDepthRectangle (
          const cv::Point2f& upperLeft,
          const int& x,
          const int& y,
          const float& depthIn,
          cv::Mat* image );

      /**
        @brief Constructs a rectangle of width @param x and height of @param y.
        where the depth firstly decrements and then decrements in equal sized
        steps
        @param[in] upperLeft [const cv::Point2f&] The upper left vertex of the
        rectangle to be created
        @param[in] x [const int&] The rectangle's width
        @param[in] y [const int&] The rectangle's height
        @param[in] depthLower [const float&] The lower depth value
        @param[in] depthUpper [const float&] The upper depth value
        @param[out] image [cv::Mat*] The image on which the rectangle will be
        imprinted
        return void
       **/
      void generateStepwiseDifferentiatingDepthRectangle (
          const cv::Point2f& upperLeft,
          const int& x,
          const int& y,
          const float& depthLower,
          const float& depthUpper,
          cv::Mat* image );

      virtual void SetUp()
      {
        WIDTH = 640;
        HEIGHT = 480;

      }

      // The images' width and height
      int WIDTH;
      int HEIGHT;

      // The images under processing
      cv::Mat depthImage;
      cv::Mat rgbImage;

  };



  /**
    @brief Constructs a rectangle of width @param x and height of @param y.
    All points inside the said rectangle are with a value of @param rgbIn.
    The outline of the rectangle is at a value of 1.
    @param[in] upperLeft [const cv::Point2f&] The upper left vertex of the
    rectangle to be created
    @param[in] x [const int&] The rectangle's width
    @param[in] y [const int&] The rectangle's height
    @param[in] rgbIn [const unsigned char] The rgb value for all points i
    nside the rectangle
    @param[out] image [cv::Mat*] The image on which the rectangle will be
    imprinted on
    return void
   **/
  void BarrelProcessorTest::generateRgbRectangle (
      const cv::Point2f& upperLeft,
      const int& x,
      const int& y,
      const unsigned char rgbIn,
      cv::Mat* image )
  {
    // Fill the inside of the desired rectangle with the @param rgbIn provided
    for( int rows = upperLeft.y; rows < upperLeft.y + y; rows++ )
    {
      for ( int cols = upperLeft.x; cols < upperLeft.x + x; cols++ )
      {
        image->at< cv::Vec3b >( rows, cols ).val[0] = rgbIn;
        image->at< cv::Vec3b >( rows, cols ).val[1] = rgbIn;
        image->at< cv::Vec3b >( rows, cols ).val[2] = rgbIn;
      }
    }
  }

  /**
    @brief Constructs a rectangle of width @param x and height of @param y.
    @param[in] upperLeft [const cv::Point2f&] The upper left vertex of the
    rectangle to be created
    @param[in] x [const int&] The rectangle's width
    @param[in] y [const int&] The rectangle's height
    @param[in] depthIn [const float&] The depth value for all points inside
    the rectangle
    @param[out] image [cv::Mat*] The image on which the rectangle will be
    imprinted on
    return void
   **/
  void BarrelProcessorTest::generateDepthRectangle (
      const cv::Point2f& upperLeft,
      const int& x,
      const int& y,
      const float& depthIn,
      cv::Mat* image )
  {
    // Fill the inside of the desired rectangle with the @param depthIn provided
    for( int rows = upperLeft.y; rows < upperLeft.y + y; rows++ )
    {
      for ( int cols = upperLeft.x; cols < upperLeft.x + x; cols++ )
      {
        image->at< float >( rows, cols ) = depthIn;
      }
    }
  }

  /**
    @brief Constructs a rectangle of width @param x and height of @param y.
    where the depth firstly decrements and then increments in equal sized
    steps
    @param[in] upperLeft [const cv::Point2f&] The upper left vertex of the
    rectangle to be created
    @param[in] x [const int&] The rectangle's width
    @param[in] y [const int&] The rectangle's height
    @param[in] depthLower [const float&] The lower depth value
    @param[in] depthUpper [const float&] The upper depth value
    @param[out] image [cv::Mat*] The image on which the rectangle will be
    imprinted
    return void
   **/
  void BarrelProcessorTest::generateStepwiseDifferentiatingDepthRectangle (
      const cv::Point2f& upperLeft,
      const int& x,
      const int& y,
      const float& depthLower,
      const float& depthUpper,
      cv::Mat* image )
  {
    float step = 2 * (depthUpper - depthLower) / x;
    for( int rows = upperLeft.y; rows < upperLeft.y + y; rows++ )
    {
      // First decrement depth
      for ( int cols = upperLeft.x; cols < upperLeft.x + (x / 2); cols++ )
      {
        image->at< float >( rows, cols ) = depthUpper - (cols - upperLeft.x) * step;
      }
      // Then increment depth
      for ( int cols = upperLeft.x + (x / 2); cols < upperLeft.x + x; cols++ )
      {
        image->at< float >( rows, cols ) = depthLower + (cols - upperLeft.x - (x / 2)) * step;
      }
    }
  }

  //! Tests BarrelProcessor::getSymmertyObject
  TEST_F ( BarrelProcessorTest, getSymmetryObjectTest )
  {
    // The image upon which the squares will be inprinted
    depthImage = cv::Mat::zeros( HEIGHT, WIDTH, CV_32FC1 );

    // Construct the squares_ image

    // Set the depth for each point of the depthImage to constant value
    for ( int rows = 0; rows < depthImage.rows; rows++ )
    {
      for ( int cols = 0; cols < depthImage.cols; cols++ )
      {
        depthImage.at<float>(rows, cols) = 0.2;
      }
    }

    // Construct a square
    cv::Mat square = cv::Mat::zeros(HEIGHT, WIDTH, CV_32FC1 );

    BarrelProcessorTest::generateDepthRectangle
      ( cv::Point2f ( WIDTH - 350, HEIGHT - 350 ),
        310,
        310,
        0.9,
        &square );

    depthImage += square;

    cv::Rect roi;
    cv::Point symmetricStartPoint;
    cv::Point symmetricEndPoint;

    BarrelProcessor barrelProcessor;

    barrelProcessor.getSymmetryObject(
        depthImage, 
        &roi, 
        &symmetricStartPoint, 
        &symmetricEndPoint);

    // It is expected that the bounding box surrounds the square
    EXPECT_NEAR(290, roi.x, 50);
    EXPECT_NEAR(310, roi.width, 100);
  }

  //! Tests BarrelProcessor::validateROI
  TEST_F ( BarrelProcessorTest, validateROITest )
  {
    // The image upon which the squares will be inprinted
    depthImage = cv::Mat::zeros( HEIGHT, WIDTH, CV_32FC1 );

    // Construct the squares_ image

    // Set the depth for each point of the depthImage to constant value
    for ( int rows = 0; rows < depthImage.rows; rows++ )
    {
      for ( int cols = 0; cols < depthImage.cols; cols++ )
      {
        depthImage.at<float>(rows, cols) = 0.1;
      }
    }

    rgbImage = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);

    // Construct the rgbImage. The entire image is at a colour of
    // value approximate the the colour value of the images of walls
    for ( int rows = 0; rows < HEIGHT; rows++ )
    {
      for ( int cols = 0; cols < WIDTH; cols++ )
      {
        if ( rgbImage.at< cv::Vec3b >( rows, cols ).val[0] == 0)
        {
          rgbImage.at< cv::Vec3b >( rows, cols ).val[0] = 116;
          rgbImage.at< cv::Vec3b >( rows, cols ).val[1] = 163;
          rgbImage.at< cv::Vec3b >( rows, cols ).val[2] = 171;
        }
      }
    }

    // Construct a symmetric square with firstly decreasing and then increasing
    // depth
    cv::Mat square = cv::Mat::zeros(HEIGHT, WIDTH, CV_32FC1);

    BarrelProcessorTest::generateStepwiseDifferentiatingDepthRectangle
      ( cv::Point2f ( WIDTH - 350, HEIGHT - 350 ),
        310,
        310,
        0.3,
        1.0,
        &square );


    // Construct the homogeneous RGB square
    cv::Mat homogeneousSquare = cv::Mat::zeros( HEIGHT, WIDTH, CV_8UC3 );

    BarrelProcessorTest::generateRgbRectangle
      ( cv::Point2f ( WIDTH - 350, HEIGHT - 350 ),
        310,
        310,
        100,
        &homogeneousSquare );


    depthImage += square;

    cv::Rect roi = cv::Rect(290, 130, 310, 310 );
    cv::Point symmetricStartPoint = cv::Point(445, 130);
    cv::Point symmetricEndPoint(445, 285);

    BarrelProcessor barrelProcessor;

    bool valid = true; 

    valid = barrelProcessor.validateRoi(
        rgbImage, 
        depthImage, 
        roi, 
        symmetricStartPoint, 
        symmetricEndPoint); 

    // It is expected that this object is verified as positive
    EXPECT_EQ(true, valid);
  }

} // namespace pandora_vision

