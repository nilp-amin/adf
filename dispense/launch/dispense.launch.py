import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory



def generate_launch_description():
    ld = LaunchDescription()

    config = os.path.join(
        get_package_share_directory("dispense"),
        "config",
        "params.yaml"
    )

    dispense_node = Node(
        package="dispense",
        executable="dispense_node",
        parameters = [config]
    )

    ld.add_action(dispense_node)

    return ld