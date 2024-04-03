#include "vision/vision.hpp"

Vision::Vision() : Node("vision_node") {
    camera_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/image", 10, std::bind(&Vision::camera_callback, this, std::placeholders::_1)
    );
}

void Vision::camera_callback(const sensor_msgs::msg::Image& msg)
{
    curr_image_ = msg;
    RCLCPP_INFO(this->get_logger(), "I heard %i", msg.height);
} 