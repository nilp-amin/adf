import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory



def generate_launch_description():
    ld = LaunchDescription()

    config = os.path.join(
        get_package_share_directory("recorder"),
        "config",
        "params.yaml"
    )

    recorder_node = Node(
        package="recorder",
        executable="recorder_node",
        parameters = [config]
    )

    ld.add_action(recorder_node)

    return ld