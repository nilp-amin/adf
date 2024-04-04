#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <rosbag2_cpp/writer.hpp>
#include <recorder/srv/record.hpp>

class Recorder : public rclcpp::Node
{
public:
    Recorder(const std::string& camera_topic, 
             const std::string& record_srv_name);

private:
    void camera_callback(const std::shared_ptr<rclcpp::SerializedMessage> msg) const;

    void record_callback(const std::shared_ptr<recorder::srv::Record::Request> request,
                         const std::shared_ptr<recorder::srv::Record::Response> response);

private:
    std::unique_ptr<rosbag2_cpp::Writer> writer_;   //!< the rosbag writer for the camera feed

    rclcpp::Service<recorder::srv::Record>::SharedPtr record_srv_;    //!< service which controls if to record camera feed or not 

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub_;   //!< sub for camera feed

private:
    std::string camera_topic_;  //!< the camera feed topic
    std::string bag_name_;      //!< current open ros bag's name
    bool recording_;    //!< if the camera feed is being recorded
};

#endif