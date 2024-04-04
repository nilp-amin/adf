import rclpy
from rclpy.node import Node
from adf_interfaces.srv import Motion

from gpiozero import MotionSensor

class ProximityService(Node):
    def __init__(self):
        super().__init__("proximity_service")

        # declare config parameters to be used
        self.declare_parameters(
            namespace="",
            parameters=[
                ("pir_pin", 4),
                ("pir_threshold", 0.5),
                ("service_name", "/motion")
            ]
        )

        # set config parameters for use
        pir_pin = self.get_parameter("pir_pin").value
        pir_threshold = self.get_parameter("pir_threshold").value
        service_name = self.get_parameter("service_name").value

        # setup the PIR sensor
        self.pir = MotionSensor(pin=pir_pin,
                                threshold=pir_threshold)

        # setup service to check if motion has been detected
        self.srv = self.create_service(Motion, service_name, self.motion_callback)

    def motion_callback(self, request, response):
        response.motion_detected = self.pir.motion_detected() 

        return response


def main(args=None):
    rclpy.init(args=args)

    proximity_service = ProximityService()

    rclpy.spin(proximity_service)

    rclpy.shutdown()


if __name__ == '__main__':
    main()
