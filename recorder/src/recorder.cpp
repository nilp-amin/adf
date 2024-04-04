#include <recorder/recorder.hpp>

Recorder::Recorder() : Node{"recorder_node"}, recording_{false}
{
    // declare config parameters to be able to use them
    this->declare_parameter("recording_path", rclcpp::PARAMETER_STRING);
    this->declare_parameter("camera_topic", rclcpp::PARAMETER_STRING);
    this->declare_parameter("record_srv_name", rclcpp::PARAMETER_STRING);

    // get config parameters
    recording_path_ =  this->get_parameter("recording_path").as_string();
    camera_topic_ = this->get_parameter("camera_topic").as_string();
    record_srv_name_ = this->get_parameter("record_srv_name").as_string();

    // create the camera feed rosbag writer
    writer_ = std::make_unique<rosbag2_cpp::Writer>();

    // create service for control of when to record the camera feed
    record_srv_ = this->create_service<adf_interfaces::srv::Record>(
        record_srv_name_, std::bind(&Recorder::record_callback, this, std::placeholders::_1, std::placeholders::_2)
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

void Recorder::record_callback(const std::shared_ptr<adf_interfaces::srv::Record::Request> request,
                               const std::shared_ptr<adf_interfaces::srv::Record::Response> response)
{
    if (recording_ == false && request->record == true)
    {
        // open new rosbag for recording
        bag_name_ = 
            "bag_" + std::to_string(this->now().nanoseconds()) + ".bag";
        writer_->open(recording_path_ + bag_name_);

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