#!/usr/bin/env python3

import rclpy

from simple_node import Node
from std_srvs.srv import Empty
from adf_interfaces.srv import Motion, DogInVision, Record

from yasmin import CbState
from yasmin import State
from yasmin import StateMachine
from yasmin import Blackboard
from yasmin_ros import ServiceState
from yasmin_ros.basic_outcomes import SUCCEED, ABORT
from yasmin_viewer import YasminViewerPub

class CheckMovementState(ServiceState):
    def __init__(self, node: Node) -> None:
        super().__init__(
            node,
            Motion,
            "/motion",
            self.create_request_handler,
            ["motion_detected", "motion_not_detected"],
            self.response_handler
        )

    def create_request_handler(self, blackboard: Blackboard) -> Motion.Request:
        req = Motion.Request()
        return req

    def response_handler(
        self,
        blackboard: Blackboard,
        response: Motion.Response
    ) -> str:
        if response.motion_detected:
            return "motion_detected"
        return "motion_not_detected"

class CheckForDogState(ServiceState):
    def __init__(self, node: Node) -> None:
        super().__init__(
            node,
            DogInVision,
            "/dog_in_vision",
            self.create_request_handler,
            ["dog_seen", "dog_not_seen"],
            self.response_handler
        )

    def create_request_handler(self, blackboard: Blackboard) -> DogInVision.Request:
        req = DogInVision.Request()
        return req

    def response_handler(
        self,
        blackboard: Blackboard,
        response: DogInVision.Response
    ) -> str:
        if response.dog_in_vision:
            return "dog_seen"
        return "dog_not_seen"

class DispenseState(ServiceState):
    def __init__(self, node: Node) -> None:
        super().__init__(
            node,
            Empty,
            "/dispense",
            self.create_request_handler,
            ["dispensed"],
            self.response_handler
        )

    def create_request_handler(self, blackboard: Blackboard) -> Empty.Request:
        req = Empty.Request()
        return req

    def response_handler(
        self,
        blackboard: Blackboard,
        response: Empty.Response
    ) -> str:
        return "dispensed"
         
class RecordState(ServiceState):
    def __init__(self, node: Node) -> None:
        super().__init__(
            node,
            Record,
            "/record",
            self.create_request_handler,
            [],
            self.response_handler
        )

    def create_request_handler(self, blackboard: Blackboard) -> Record.Request:
        req = Record.Request()
        req.record = blackboard.record
        return req

    def response_handler(
        self,
        blackboard: Blackboard,
        response: Record.Response
    ) -> str:
        return SUCCEED 

def set_start_record(blackboard: Blackboard) -> str:
    blackboard.record = True
    return SUCCEED

def set_stop_record(blackboard: Blackboard) -> str:
    blackboard.record = False
    return SUCCEED

class TaskManager(Node):
    def __init__(self) -> None:
        super().__init__("task_manager_node")

        # decalre config parameters to be used
        # self.declare_parameters(
        #     namespace="",
        #     parameters=[
        #         ("food_servo_pin", 13),
        #         ("service_name", "/dispense"),
        #         ("food_full_threshold", 1000),
        #         ("water_full_threshold", 1000)
        #     ]
        # )

        # create the root FSM
        sm = StateMachine(outcomes=["succeeded", "aborted", "preempted"])

        # add state to check for any motion using PIR sensor
        sm.add_state("CHECK_MOVEMENT", CheckMovementState(self),
                     transitions={"motion_detected" : "CHECK_FOR_DOG",
                                  "motion_not_detected" : "CHECK_MOVEMENT",
                                  SUCCEED : "succeeded",
                                  ABORT: "aborted"})
        #  add state to check for a dog in the camera frame
        sm.add_state("CHECK_FOR_DOG", CheckForDogState(self),
                     transitions={"dog_seen" : "DISPENSE",
                                  "dog_not_seen" : "CHECK_FOR_MOVEMENT",
                                  SUCCEED : "succeeded",
                                  ABORT: "aborted"})
        # add state to dispense food and water if required
        sm.add_state("DISPENSE", DispenseState(self),
                     transitions={"dispensed" : "SET_START_RECORDING",
                                  SUCCEED : "succeeded",
                                  ABORT : "aborted"})
        # add state to set record parameter to start
        sm.add_state("SET_START_RECORDING", CbState([SUCCEED], set_start_record),
                     transitions={SUCCEED : "START_RECORDING"})
        # add state to start recording camera feed
        sm.add_state("START_RECORDING", RecordState(self),
                     transitions={SUCCEED : "CHECK_DOG_STILL_SEEN",
                                  ABORT : "aborted"})
        # add state to check for a dog in the camera frame
        sm.add_state("CHECK_DOG_STILL_SEEN", CheckForDogState(self),
                     transitions={"dog_seen" : "CHECK_DOG_STILL_SEEN",
                                  "dog_not_seen" : "SET_STOP_RECORDING",
                                  SUCCEED : "succeeded",
                                  ABORT : "aborted"})
        # add state to set record parameter to stop
        sm.add_state("SET_STOP_RECORDING", CbState([SUCCEED], set_stop_record),
                     transitions={SUCCEED : "STOP_RECORDING"})
        # add state to stop recording camera feed
        sm.add_state("STOP_RECORDING", RecordState(self),
                     transitions={SUCCEED : "CHECK_MOVEMENT",
                                  ABORT : "aborted"})

        # publisher for the introspective server
        YasminViewerPub(self, "ADF", sm)

        # execute
        outcome = sm()

def main(args=None):
    rclpy.init(args=args)
    task_manager = TaskManager()
    task_manager.join_spin()
    rclpy.shutdown()

if __name__ == "__main__":
    main()