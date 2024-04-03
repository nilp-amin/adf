#include <recorder/recorder.hpp>

Recorder::Recorder(const std::string& camera_topic) : Node{"recorder_node"}
{
    // create subscriber to camera feed
    camera_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        camera_topic, 10, std::bind(&Recorder::camera_callback, this, std::placeholders::_1)
    );
}

void Recorder::camera_callback(const sensor_msgs::msg::Image& msg)
{
    ;
}