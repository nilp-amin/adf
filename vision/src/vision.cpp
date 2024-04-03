#include "vision/vision.hpp"

Vision::Vision() : Node("vision_node") {
    // create subscriber to camera feed
    camera_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/image", 10, std::bind(&Vision::camera_callback, this, std::placeholders::_1)
    );

    // create action client to check for dog in camera feed
    check_object_action_ = rclcpp_action::create_client<darknet_ros_msgs::action::CheckForObjects>(
        this, "CheckForObjects"
    );

    // wait for the action server to become available
    if (!check_object_action_->wait_for_action_server(std::chrono::seconds(20)))
    {
        RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
        // rclcpp::shutdown();
    }
}

void Vision::camera_callback(const sensor_msgs::msg::Image& msg)
{
    curr_image_ = msg;
    RCLCPP_INFO(this->get_logger(), "dogs found %s", check_for_dog() ? "true" : "false");
} 

bool Vision::check_for_dog()
{
    auto goal = darknet_ros_msgs::action::CheckForObjects::Goal();
    goal.id = 17; // the id of the dog in darknet
    goal.image = curr_image_;
    // create a promise to get the result of the action
    auto goal_handle_future = check_object_action_->async_send_goal(goal);

    // wait until it is accepted
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), goal_handle_future) == 
        rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(this->get_logger(), "send goal call failed.");
        return false;
    }

    auto goal_handle = goal_handle_future.get();
    if (!goal_handle)
    {
        RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server.");
        return false;
    }
    
    // wait for the server to be done with goal
    auto result_future = check_object_action_->async_get_result(goal_handle);


    RCLCPP_INFO(this->get_logger(), "Waiting for result");
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result_future) !=
        rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(this->get_logger(), "get result call failed.");
        return false;
    }

    auto wrapped_result = result_future.get();

    switch (wrapped_result.code) {
        case rclcpp_action::ResultCode::SUCCEEDED:
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_ERROR(this->get_logger(), "Goal was aborted");
            return false;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
            return false;
        default:
            RCLCPP_ERROR(this->get_logger(), "Unknown result code");
            return false;
    }

    RCLCPP_INFO(this->get_logger(), "result received");
    return wrapped_result.result->bounding_boxes.bounding_boxes.size() > 0;
} 