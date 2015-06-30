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
 * Authors: Choutas Vassilis
 *********************************************************************/


#include "pandora_vision_hazmat/filters/image_signature.h"
#include "gtest/gtest.h"

namespace pandora_vision
{
  namespace pandora_vision_hazmat
  {

    TEST( ImageSignatureTest , imageSign )
    {
      // Create a small image with positive elements .
      cv::Mat testImage( 20 , 20 , CV_32SC3 );
      testImage.setTo(100);

      cv::Mat signs;

      ImageSignature::signFunction(testImage , &signs );

      // Since we pass a multi channel matrix the function must return
      // an empty matrix.
      ASSERT_TRUE(signs.data == NULL);

      // Create a matrix with positive elements.
      testImage = cv::Mat( 20 , 20, CV_32FC1 );
      testImage.setTo(100);

      ImageSignature::signFunction( testImage , &signs );
      ASSERT_TRUE(signs.data != NULL);

      // Create a matrix with positive elements.
      testImage = cv::Mat(20, 20, CV_32FC1);
      testImage.setTo(100);

      ImageSignature::signFunction(testImage , &signs );

      // Check that every value is positive.

      for (int i = 0 ; i < testImage.rows ; i++)
      {
        for (int j = 0 ; j < testImage.cols ; j++ )
        {
          int val = signs.at<float>( i , j );
          ASSERT_GT(  val , 0 );
        }
      }


      // Create a matrix with negative elements.
      testImage = cv::Mat(20, 20, CV_32FC1);
      testImage.setTo(-100);

      ImageSignature::signFunction(testImage , &signs );

      // Check that every value is negative.

      for (int i = 0 ; i < testImage.rows ; i++)
      {
        for (int j = 0 ; j < testImage.cols ; j++ )
        {
          int val = signs.at<float>( i , j);
          ASSERT_LT( val , 0 );
        }
      }

    }

} // namespace pandora_vision_hazmat
} // namespace pandora_vision
