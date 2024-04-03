#include "vision/vision.hpp"

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Vision>());
  rclcpp::shutdown();
  return 0;
}