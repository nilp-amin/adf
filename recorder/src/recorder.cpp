#include <recorder/recorder.hpp>

Recorder::Recorder(const std::string& camera_topic,
                   const std::string& record_srv_name) : 
Node{"recorder_node"}, camera_topic_{camera_topic}, recording_{false}
{
    // create the camera feed rosbag writer
    writer_ = std::make_unique<rosbag2_cpp::Writer>();

    // create service for control of when to record the camera feed
    record_srv_ = this->create_service<recorder::srv::Record>(
        record_srv_name, std::bind(&Recorder::record_callback, this, std::placeholders::_1, std::placeholders::_2)
    );

    // create subscriber to camera feed
    camera_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        camera_topic_, 10, std::bind(&Recorder::camera_callback, this, std::placeholders::_1)
    );
}

void Recorder::camera_callback(const std::shared_ptr<rclcpp::SerializedMessage> msg) const
{
    rclcpp::Time time_stamp = this->now();

    if (recording_)
    {
        writer_->write(msg, camera_topic_, "sensor_msgs/msg/Image", time_stamp);
    }
}

void Recorder::record_callback(const std::shared_ptr<recorder::srv::Record::Request> request,
                               const std::shared_ptr<recorder::srv::Record::Response> response)
{
    if (recording_ == false && request->record == true)
    {
        // open new rosbag for recording
        bag_name_ = 
            "bag_" + std::to_string(this->now().nanoseconds()) + ".bag";
        writer_->open("recordings/" + bag_name_);

        recording_ = request->record;
        RCLCPP_INFO(this->get_logger(), "Opened ros bag with name: %s", bag_name_.c_str());
    } else if (recording_ == true && request->record == false)
    {
        recording_ = request->record;

        // close the currently open rosbag
        writer_->close();
        // reopen writer for next time when camera feed needs to be written 
        writer_ = std::make_unique<rosbag2_cpp::Writer>();
        RCLCPP_INFO(this->get_logger(), "Closed ros bag with name: %s", bag_name_.c_str());
    }
}