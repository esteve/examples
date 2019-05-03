// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

template <typename MessageT>
struct PollingSubscription
{
  using SharedPtr = std::shared_ptr<PollingSubscription<MessageT>>;
};

struct PollingSubscriptionFactory
{
};

namespace rclcpp
{

template <
    typename CallbackT,
    typename AllocatorT,
    typename CallbackMessageT,
    typename SubscriptionT>
struct subscription_t<
    std_msgs::msg::String,
    CallbackT,
    AllocatorT,
    CallbackMessageT,
    SubscriptionT>
{
  static typename PollingSubscription<std_msgs::msg::String>::SharedPtr
  create(
      rclcpp::node_interfaces::NodeTopicsInterface *,
      const std::string &,
      CallbackT &&,
      const rmw_qos_profile_t &,
      rclcpp::callback_group::CallbackGroup::SharedPtr,
      bool,
      bool,
      typename rclcpp::message_memory_strategy::MessageMemoryStrategy<
          CallbackMessageT, AllocatorT>::SharedPtr,
      typename std::shared_ptr<AllocatorT>)
  {
    typename PollingSubscription<std_msgs::msg::String>::SharedPtr sub;
    return sub;
  }
};

/// Return a SubscriptionFactory with functions for creating a SubscriptionT<MessageT, Alloc>.
template <
    typename CallbackT,
    typename Alloc,
    typename CallbackMessageT,
    typename SubscriptionT>
struct subscription_factory_t<
    std_msgs::msg::String,
    CallbackT,
    Alloc,
    CallbackMessageT,
    SubscriptionT>
{
  static PollingSubscriptionFactory
  create(
      CallbackT &&callback,
      typename rclcpp::message_memory_strategy::MessageMemoryStrategy<
          CallbackMessageT, Alloc>::SharedPtr
          msg_mem_strat,
      std::shared_ptr<Alloc> allocator)
  {
    PollingSubscriptionFactory factory;
    return factory;
  }
};
} // namespace rclcpp

class MinimalPollingSubscriber : public rclcpp::Node
{
public:
  MinimalPollingSubscriber()
      : Node("minimal_subscriber")
  {
    subscription_ = this->create_subscription<std_msgs::msg::String, std::allocator<void>, PollingSubscription<std_msgs::msg::String>>(
        "topic",
        [this](std_msgs::msg::String::UniquePtr msg) {
          RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
        });
  }

private:
  PollingSubscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPollingSubscriber>());
  rclcpp::shutdown();
  return 0;
}
