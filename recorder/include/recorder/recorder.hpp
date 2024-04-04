#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <rosbag2_cpp/writer.hpp>
#include <adf_interfaces/srv/record.hpp>

class Recorder : public rclcpp::Node
{
public:
    Recorder();

private:
    void camera_callback(const std::shared_ptr<rclcpp::SerializedMessage> msg) const;

    void record_callback(const std::shared_ptr<adf_interfaces::srv::Record::Request> request,
                         const std::shared_ptr<adf_interfaces::srv::Record::Response> response);

private:
    std::unique_ptr<rosbag2_cpp::Writer> writer_;   //!< the rosbag writer for the camera feed

    rclcpp::Service<adf_interfaces::srv::Record>::SharedPtr record_srv_;    //!< service which controls if to record camera feed or not 

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub_;   //!< sub for camera feed

private:
    std::string camera_topic_;  //!< the camera feed topic
    std::string record_srv_name_;   //!< the record service name
    std::string bag_name_;      //!< current open rosbag's name
    std::string recording_path_;    //!< the root path to store rosbags at
    bool recording_;    //!< if the camera feed is being recorded
};

#endif