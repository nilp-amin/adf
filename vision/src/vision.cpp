#include "vision/vision.hpp"

Vision::Vision(const std::string& bounding_box_topic,
               const std::string& dog_srv_name) : 
Node("vision_node"), dog_in_vision_{false}
{
    // create service for letting clients know if a dog is in the camera frame
    dog_in_vision_srv_ = this->create_service<vision::srv::DogInVision>(
        dog_srv_name, std::bind(&Vision::is_dog_in_vision, this, std::placeholders::_1, std::placeholders::_2)
    );

    // create subscriber to camera feed
    bounding_box_sub_ = this->create_subscription<darknet_ros_msgs::msg::BoundingBoxes>(
        bounding_box_topic, 1, std::bind(&Vision::bounding_box_callback, this, std::placeholders::_1)
    );
}

void Vision::bounding_box_callback(const darknet_ros_msgs::msg::BoundingBoxes& msg)
{
    // check if a dog is detected in the camera frame by darknet_ros
    dog_in_vision_ = false;
    for (auto bb : msg.bounding_boxes) {
        if (bb.class_id == "dog") {
            dog_in_vision_ = true;
            break;
        }
    }
    RCLCPP_INFO(this->get_logger(), "is dog in vision: %s", dog_in_vision_ ? "true" : "false");
} 

void Vision::is_dog_in_vision(const std::shared_ptr<vision::srv::DogInVision::Request> request,
                              const std::shared_ptr<vision::srv::DogInVision::Response> response)
{
    response->dog_in_vision = dog_in_vision_;
}
