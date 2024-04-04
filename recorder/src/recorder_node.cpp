#include <recorder/recorder.hpp>

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  std::string camera_topic{"/image"};
  std::string record_srv_name{"record"};
  rclcpp::spin(std::make_shared<Recorder>(camera_topic, record_srv_name));

  rclcpp::shutdown();
  return 0;
}
