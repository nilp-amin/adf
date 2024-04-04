#include <recorder/recorder.hpp>

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Recorder>());
  rclcpp::shutdown();
  return 0;
}
