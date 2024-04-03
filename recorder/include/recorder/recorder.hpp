#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>

class Recorder : public rclcpp::Node
{
public:
    Recorder(const std::string& camera_topic);

private:
    void camera_callback(const sensor_msgs::msg::Image& msg);

private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub_;   //!< sub for camera feed
};

#endif