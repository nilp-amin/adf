import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory



def generate_launch_description():
    ld = LaunchDescription()

    vision_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("vision"),
                            "launch/vision.launch.py")
        )
    )
    proximity_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("proximity"),
                            "launch/proximity.launch.py")
        )
    )
    recorder_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("recorder"),
                            "launch/recorder.launch.py")
        )
    )
    dispense_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory("dispense"),
                            "launch/dispense.launch.py")
        )
    )

    task_manager_node = Node(
        package="task_manager",
        executable="task_manager_node",
        parameters = []
    )

    ld.add_action(vision_launch)
    ld.add_action(proximity_launch)
    ld.add_action(recorder_launch)
    ld.add_action(dispense_launch)
    ld.add_action(task_manager_node)

    return ld