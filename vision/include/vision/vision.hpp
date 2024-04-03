#ifndef VISION_HPP
#define VISION_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>

class Vision : public rclcpp::Node
{
public:
    Vision();

private:
    void camera_callback(const sensor_msgs::msg::Image& msg);

private:
    sensor_msgs::msg::Image curr_image_;    //!< the current image from camera

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub_;   //!< sub camera feed
};


#endif