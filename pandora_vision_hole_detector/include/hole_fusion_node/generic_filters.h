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
 * Authors: Alexandros Filotheou, Manos Tsardoulias
 *********************************************************************/
#ifndef HOLE_FUSION_NODE_GENERIC_FILTERS_H
#define HOLE_FUSION_NODE_GENERIC_FILTERS_H

#include "depth_node/hole_filters.h"
#include <math.h>

/**
  @namespace vision
  @brief The main namespace for PANDORA vision
 **/
namespace pandora_vision
{
  /**
    @class GenericFilters
    @brief Provides functionalities of hole filters independently of depth
    or RGB images
   **/
  class GenericFilters
  {
    private:

      /**
        @brief Assimilates the fragmented holes of @param assimilable into the
        existing whole ones of @param assimilator. It checks whether each
        entry of the set of assimilable's outline points reside inside
        the assimilator's set of outlines. If so, the latter keypoint
        etc are kept unchanged and the former one is deleted.
        @param[in][out] assimilator [const HoleFilters::HolesConveyor&]
        The candidate holes conveyor that will potentially assimilate the
        assimimable's holes
        @param[in][out] assimilable [HoleFilters::HolesConveyor*]
        The candidate holes conveyor whose holes will potentially by assimilated
        by the assimilator
        @return void
       **/
      static void assimilateUnilaterally(
        const HoleFilters::HolesConveyor& assimilator,
        HoleFilters::HolesConveyor* assimilable);

      /**
        @brief Given two HoleFilters::HolesConveyor* structs, one with the
        potential of assimilating the other (assimilator) and the other with the
        potential of being assimilated by the other (assimilable), the purpose
        of this function is to identify blobs that are overlapping each other
        but none of them is entirely inside the other, while the assimilator's
        bounding box is greater than that of the assimilable's.
        If this is true for a given assimilator and assimilable,
        the assimilator will grow rectangle-wise and outline-wise
        by the size of the assimilator, while the assimilator's
        new keypoint will be the mean of the two keypoints. The assimilable
        then is rendered useless and deleted.
        @param[in][out] assimilator [HoleFilters::HolesConveyor*]
        The holes conveyor whose candidate holes will potentially
        assimilate candidate holes of @param assimilable
        @param[in][out] assimilable [HoleFilters::HolesConveyor*]
        The holes conveyor whose candidate holes will potentially
        will be assimilated by candidate holes of @param assimilator
        @return void
       **/
      static void mergeUnilaterally(
        HoleFilters::HolesConveyor* assimilator,
        HoleFilters::HolesConveyor* assimilable);

    public:
      /**
        @brief Assimilates fragmented holes into existing whole ones
        from either source (RGB or Depth).
        @param[in][out] depthHolesConveyor [HoleFilters::HolesConveyor*]
        The candidate holes conveyor originated from the depth node
        @param[in][out] rgbHolesConveyor [HoleFilters::HolesConveyor*]
        The candidate holes conveyor originated from the rgb node
        @return void
       **/
      static void assimilateBilaterally(
        HoleFilters::HolesConveyor* depthHolesConveyor,
        HoleFilters::HolesConveyor* rgbHolesConveyor);

      /**
        @brief Given the RGB and Depth HoleFilters::HolesConveyor* structs,
        the purpose of this function is to identify blobs that are overlapping
        each other but none of them is entirely inside the other, and merge
        them in one candidate hole: the one whose bounding rectangle has
        the greater area.
        @param[in][out] depthHolesConveyor [HoleFilters::HolesConveyor*]
        The candidate holes conveyor originated from the depth node
        @param[in][out] rgbHolesConveyor [HoleFilters::HolesConveyor*]
        The candidate holes conveyor originated from the rgb node
        @return void
       **/
      static void mergeBilaterally(
        HoleFilters::HolesConveyor* depthHolesConveyor,
        HoleFilters::HolesConveyor* rgbHolesConveyor);
  };

} // namespace pandora_vision

#endif  // HOLE_FUSION_NODE_GENERIC_FILTERS_H
