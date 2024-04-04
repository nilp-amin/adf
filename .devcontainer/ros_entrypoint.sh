#!/bin/bash
set -e

# Source the setup.bash file
source /opt/ros/$ROS_DISTRO/setup.bash
exec "$@"