#ifndef VISION_HPP
#define VISION_HPP

#include <rclcpp/rclcpp.hpp>
#include <darknet_ros_msgs/msg/bounding_boxes.hpp>
#include <vision/srv/dog_in_vision.hpp>

class Vision : public rclcpp::Node
{
public:
    Vision();
           

private:
    void bounding_box_callback(const darknet_ros_msgs::msg::BoundingBoxes& msg);   //!< the bounding boxes callback

    void is_dog_in_vision(const std::shared_ptr<vision::srv::DogInVision::Request> request,
                          const std::shared_ptr<vision::srv::DogInVision::Response> response);

private:
    rclcpp::Service<vision::srv::DogInVision>::SharedPtr dog_in_vision_srv_;    //!< service which tells client if dog is in camera frame

    rclcpp::Subscription<darknet_ros_msgs::msg::BoundingBoxes>::SharedPtr bounding_box_sub_;   //!< sub to darknet_ros bounding boxes 

private:
    std::string bounding_box_topic_;    //!< the topic name for darknet_ros bounding boxes
    std::string dog_srv_name_;      //!< the dog in vision service name
    std::string class_id_;     //!< the class id to look for in bounding boxes
    bool dog_in_vision_;     //!< is a dog in the vision of the camera
};


#endif