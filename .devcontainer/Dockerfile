FROM ros:iron
ARG USERNAME=USERNAME
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create the user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && usermod -aG video $USERNAME \
    #
    # [Optional] Add sudo support. Omit if you don't need to install software after connecting.
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y python3-pip
ENV SHELL /bin/bash

# ********************************************************
# * Anything else you want to do like clean up goes here *
# ********************************************************
# Install ros packages
RUN apt-get update && apt-get install -y \
    ros-$ROS_DISTRO-image-tools \
    ros-$ROS_DISTRO-image-transport \
    ros-$ROS_DISTRO-v4l2-camera \
    libx11-dev \
    pciutils \
    python3-gpiozero

# Install Adafruit ADC driver
RUN pip3 install --no-cache-dir Adafruit-ADS1x15

# Setup scripts
RUN echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> /root/.bashrc

# Set the entry point
COPY ./ros_entrypoint.sh /
RUN chmod +x /ros_entrypoint.sh

# [Optional] Set the default user. Omit if you want to keep the default as root.
USER $USERNAME
# ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["/bin/bash"]