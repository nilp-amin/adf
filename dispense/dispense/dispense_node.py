import rclpy
import Adafruit_ADS1x15
from rclpy.node import Node
from std_srvs.srv import Empty

from gpiozero import Servo

class DispenseService(Node):
    def __init__(self):
        super().__init__("dispense_service")

        # decalre config parameters to be used
        self.declare_parameters(
            namespace="",
            parameters=[
                ("food_servo_pin", 13),
                ("water_servo_pin", 18),
                ("food_adc_pin", 0),
                ("water_adc_pin", 1),
                ("adc_gain", 1),
                ("min_pulse_width", 0.001),
                ("max_pulse_width", 0.002),
                ("service_name", "/dispense"),
                ("food_full_threshold", 1000),
                ("water_full_threshold", 1000)
            ]
        )

        # set config parameters for use
        food_servo_pin = self.get_parameter("food_servo_pin").value
        water_servo_pin = self.get_parameter("water_servo_pin").value
        min_pulse_width = self.get_parameter("min_pulse_width").value
        max_pulse_width = self.get_parameter("max_pulse_width").value
        service_name = self.get_parameter("service_name").value
        self.food_full_threshold = self.get_parameter("food_full_threshold").value
        self.water_full_threshold = self.get_parameter("water_full_threshold").value
        self.food_adc_pin = self.get_parameter("food_adc_pin").value
        self.water_adc_pin = self.get_parameter("water_adc_pin").value
        self.adc_gain = self.get_parameter("adc_gain").value

        # setup the food and water servos
        self.food_servo = Servo(pin=food_servo_pin,
                                min_pulse_width=min_pulse_width,
                                max_pulse_width=max_pulse_width) 
        self.water_servo = Servo(pin=water_servo_pin,
                                 min_pulse_width=min_pulse_width,
                                 max_pulse_width=max_pulse_width)

        # close food and water servo
        self.food_servo.min()
        self.water_servo.min()

        # setup the ADC to read from load sensors
        self.adc = Adafruit_ADS1x15.ADS1015(address=0x48, busnum=1) 

        # setup serive to dispense food and water
        self.srv = self.create_service(Empty, service_name, self.dispense_callback)

    def dispense_callback(self, request, response):
        # add food to food bowl until full
        while (self.adc.read_adc(self.food_adc_pin, gain=self.adc_gain) < self.food_full_threshold):
            self.food_servo.max()
        self.food_servo.min()

        # add water to water bowl until full
        while (self.adc.read_adc(self.water_adc_pin, gain=self.adc_gain) < self.water_full_threshold):
            self.water_servo.max()
        self.water_servo.min()

        return response

def main(args=None):
    rclpy.init(args=args)

    dispense_service = DispenseService()

    rclpy.spin(dispense_service)
    
    rclpy.shutdown()


if __name__ == '__main__':
    main()
