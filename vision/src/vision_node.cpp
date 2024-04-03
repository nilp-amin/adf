#include "vision/vision.hpp"

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);

  std::string bounding_box_topic{"/darknet_ros/bounding_boxes"};
  std::string dog_srv_name{"dog_in_vision"};
  rclcpp::spin(std::make_shared<Vision>(bounding_box_topic, dog_srv_name));

  rclcpp::shutdown();
  return 0;
}