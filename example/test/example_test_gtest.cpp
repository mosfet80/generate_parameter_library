// Copyright 2022 Stogl Robotics Consulting
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the PickNik Inc. nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Author: Denis Štogl
//

#include "generate_parameter_library_example/admittance_controller_parameters.hpp"
#include "gtest/gtest.h"
#include "rclcpp/rclcpp.hpp"

#include <memory>

class ExampleTest : public ::testing::Test {
 public:
  void SetUp() {
    example_test_node_ = std::make_shared<rclcpp::Node>("example_test_node");

    param_listener_ = std::make_shared<admittance_controller::ParamListener>(
        example_test_node_->get_node_parameters_interface());
    params_ = param_listener_->get_params();
  }

  void TearDown() { example_test_node_.reset(); }

 protected:
  std::shared_ptr<rclcpp::Node> example_test_node_;
  admittance_controller::Params params_;
  std::shared_ptr<admittance_controller::ParamListener> param_listener_;
};

TEST_F(ExampleTest, check_parameters) {
  ASSERT_EQ(params_.interpolation_mode, "spline");  // default value

  ASSERT_EQ(params_.joints.size(), 3);
  EXPECT_EQ(params_.joints[0], "joint4");
  EXPECT_EQ(params_.joints[1], "joint5");
  EXPECT_EQ(params_.joints[2], "joint6");

  ASSERT_EQ(params_.ft_sensor.filter_coefficient, 0.1);
}

TEST_F(ExampleTest, try_update_params) {
  ASSERT_FALSE(param_listener_->try_update_params(params_));

  const rclcpp ::Parameter new_param("interpolation_mode", "linear");
  example_test_node_->set_parameter(new_param);
  ASSERT_TRUE(param_listener_->try_update_params(params_));
  ASSERT_EQ(params_.interpolation_mode, "linear");
}

TEST_F(ExampleTest, try_get_params) {
  ASSERT_TRUE(param_listener_->try_get_params(params_));

  const rclcpp ::Parameter new_param("interpolation_mode", "linear");
  example_test_node_->set_parameter(new_param);
  ASSERT_TRUE(param_listener_->try_get_params(params_));
  ASSERT_EQ(params_.interpolation_mode, "linear");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  rclcpp::init(argc, argv);
  int result = RUN_ALL_TESTS();
  rclcpp::shutdown();
  return result;
}
