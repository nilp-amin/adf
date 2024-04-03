#ifndef VISION_HPP
#define VISION_HPP

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <sensor_msgs/msg/image.hpp>

#include "darknet_ros_msgs/action/check_for_objects.hpp"

class Vision : public rclcpp::Node
{
public:
    Vision();

private:
    void camera_callback(const sensor_msgs::msg::Image& msg);   //!< the camera feed callback

    bool check_for_dog();    //!< used to trigger darknet action server

private:
    sensor_msgs::msg::Image curr_image_;    //!< the current image from camera

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub_;   //!< sub camera feed

    rclcpp_action::Client<darknet_ros_msgs::action::CheckForObjects>::SharedPtr check_object_action_;     //!< action client to check for dog
};


#endif