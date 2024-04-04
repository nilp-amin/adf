#include "vision/vision.hpp"

Vision::Vision() : Node("vision_node"), dog_in_vision_{false}
{
    // declare config parameters to be able to use them
    this->declare_parameter("bounding_box_topic", rclcpp::PARAMETER_STRING);
    this->declare_parameter("dog_srv_name", rclcpp::PARAMETER_STRING);
    this->declare_parameter("class_id", rclcpp::PARAMETER_STRING);

    // get config parameters
    bounding_box_topic_ =  this->get_parameter("bounding_box_topic").as_string();
    dog_srv_name_ = this->get_parameter("dog_srv_name").as_string();
    class_id_ = this->get_parameter("class_id").as_string();

    // create service for letting clients know if a dog is in the camera frame
    dog_in_vision_srv_ = this->create_service<adf_interfaces::srv::DogInVision>(
        dog_srv_name_, std::bind(&Vision::is_dog_in_vision, this, std::placeholders::_1, std::placeholders::_2)
    );

    // create subscriber to bounding box output of darknet_ros 
    bounding_box_sub_ = this->create_subscription<darknet_ros_msgs::msg::BoundingBoxes>(
        bounding_box_topic_, 1, std::bind(&Vision::bounding_box_callback, this, std::placeholders::_1)
    );
}

void Vision::bounding_box_callback(const darknet_ros_msgs::msg::BoundingBoxes& msg)
{
    // check if a dog is detected in the camera frame by darknet_ros
    dog_in_vision_ = false;
    for (auto bb : msg.bounding_boxes) {
        if (bb.class_id == class_id_) {
            dog_in_vision_ = true;
            break;
        }
    }
    RCLCPP_INFO(this->get_logger(), "is dog in vision: %s", dog_in_vision_ ? "true" : "false");
} 

void Vision::is_dog_in_vision(const std::shared_ptr<adf_interfaces::srv::DogInVision::Request> request,
                              const std::shared_ptr<adf_interfaces::srv::DogInVision::Response> response)
{
    response->dog_in_vision = dog_in_vision_;
}
