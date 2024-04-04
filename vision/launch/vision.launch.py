import os

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python import get_package_share_directory


def generate_launch_description():
    ld = LaunchDescription()

    config = os.path.join(
        get_package_share_directory("vision"),
        "config",
        "params.yaml"
    )

    cam2image_node = Node(
        package="image_tools",
        executable="cam2image"
    )
    vision_node = Node(
        package="vision",
        executable="vision_node",
        parameters = [config]
    )

    darknet_ros_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("darknet_ros"),
                            "launch/darknet_ros.launch.py")
        )
    )

    ld.add_action(cam2image_node)
    ld.add_action(vision_node)
    ld.add_action(darknet_ros_launch)

    return ld