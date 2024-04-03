#include <recorder/recorder.hpp>

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  std::string camera_topic{"/image"};
  rclcpp::spin(std::make_shared<Recorder>(camera_topic));

  rclcpp::shutdown();
  return 0;
}
