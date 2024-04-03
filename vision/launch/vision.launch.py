import os

from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python import get_package_share_directory


def generate_launch_description():
    return LaunchDescription([
        Node(
            package="image_tools",
            executable="cam2image"
        ),
        Node(
            package="vision",
            executable="vision_node"
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                os.path.join(get_package_share_directory("darknet_ros"),
                             "launch/darknet_ros.launch.py")
            )
        )
    ])