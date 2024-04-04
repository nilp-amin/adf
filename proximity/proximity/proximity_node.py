import rclpy
from rclpy.node import Node
from example_interfaces.srv import Motion

from gpiozero import MotionSensor

class ProximityService(Node):
    def __init__(self):
        super().__init__("proximity_service")
        self.srv = self.create_service()


def main():
    print('Hi from proximity.')


if __name__ == '__main__':
    main()
