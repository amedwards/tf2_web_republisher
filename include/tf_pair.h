/*********************************************************************
 *
 *  Copyright (c) 2009, Willow Garage, Inc.
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
 *   * Neither the name of the Willow Garage nor the names of its
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
 * Author: Julius Kammerl (jkammerl@willowgarage.com)
 *
 */


#include <tf/tfMessage.h>
#include <tf/transform_datatypes.h>


class TFPair
{
public:

  TFPair() :
      angular_thres_(0.0f), trans_thres_(0.0f), updated_(false)
  {
  }

  TFPair(const std::string& source_frame,
         const std::string& target_frame,
         float angular_thres = 0.0f,
         float trans_thres = 0.0f) :
      source_frame_(source_frame),
      target_frame_(target_frame),
      angular_thres_(angular_thres),
      trans_thres_(trans_thres),
      updated_(false)
  {
  }

  ~TFPair()
  {
  }

  void setSourceFrame(const std::string& source_frame)
  {
    source_frame_ = source_frame;
    updated_ = true;
  }

  const std::string& getSourceFrame() const
  {
    return source_frame_;
  }

  void setTargetFrame(const std::string& target_frame)
  {
    target_frame_ = target_frame;
    updated_ = true;
  }

  const std::string& getTargetFrame() const
  {
    return target_frame_;
  }

  void setAngularThres(float angular_thres)
  {
    angular_thres_ = angular_thres;
    updated_ = true;
  }

  float getAngularThres()
  {
    return angular_thres_;
  }

  void setTransThres(float trans_thres)
  {
    trans_thres_ = trans_thres;
    updated_ = true;
  }

  float getTransThres()
  {
    return trans_thres_;
  }

  void setTransform(geometry_msgs::TransformStampedConstPtr update)
  {
    tf_received_ = update;
    updated_ = true;
  }

  geometry_msgs::TransformStampedConstPtr getTransform() const
  {
    return tf_received_;
  }

  bool updateNeeded()
  {
    bool result = false;

    if (updated_)
    {
      tf::Transform tf1, tf2;
      tf::transformMsgToTF(tf_transmitted_->transform, tf1);
      tf::transformMsgToTF(tf_received_->transform, tf2);

      if (trans_thres_ == 0.0 || angular_thres_ == 0.0
          || tf1.getOrigin().distance(tf2.getOrigin()) > trans_thres_
          || tf1.getRotation().angle(tf2.getRotation()) > angular_thres_)
        result = true;
    }

    updated_ = false;

    return result;
  }

  const std::string getID()
  {
    return source_frame_+"-"+target_frame_;
  }


private:
  std::string source_frame_;
  std::string target_frame_;

  float angular_thres_;
  float trans_thres_;

  geometry_msgs::TransformStampedConstPtr tf_transmitted_;
  geometry_msgs::TransformStampedConstPtr tf_received_;

  bool updated_;

};

typedef boost::shared_ptr<TFPair> TFPairPtr;
