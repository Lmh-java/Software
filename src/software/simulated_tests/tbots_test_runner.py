from proto.import_all_protos import *
from software.logger.logger import create_logger
from software.thunderscope.thread_safe_buffer import ThreadSafeBuffer
from proto.ssl_gc_common_pb2 import Team
from abc import abstractmethod

logger = create_logger(__name__)


class TbotsTestRunner:
    """An abstract class that represents a test runner"""

    def __init__(
        self,
        test_name,
        thunderscope,
        blue_full_system_proto_unix_io,
        yellow_full_system_proto_unix_io,
        gamecontroller,
        is_yellow_friendly=False,
    ):
        """Initialize the TestRunner.

        :param test_name: The name of the test to run
        :param thunderscope: The Thunderscope to use, None if not used
        :param blue_full_system_proto_unix_io: The blue full system proto unix io to use
        :param yellow_full_system_proto_unix_io: The yellow full system proto unix io to use
        :param gamecontroller: The gamecontroller context managed instance
        :param: is_yellow_friendly: if yellow is the friendly team
        """
        self.test_name = test_name
        self.thunderscope = thunderscope
        self.blue_full_system_proto_unix_io = blue_full_system_proto_unix_io
        self.yellow_full_system_proto_unix_io = yellow_full_system_proto_unix_io
        self.gamecontroller = gamecontroller
        self.is_yellow_friendly = is_yellow_friendly
        self.world_buffer = ThreadSafeBuffer(buffer_size=20, protobuf_type=World)
        self.primitive_set_buffer = ThreadSafeBuffer(
            buffer_size=1, protobuf_type=PrimitiveSet
        )
        self.last_exception = None

        self.ssl_wrapper_buffer = ThreadSafeBuffer(
            buffer_size=1, protobuf_type=SSL_WrapperPacket
        )
        self.robot_status_buffer = ThreadSafeBuffer(
            buffer_size=1, protobuf_type=RobotStatus
        )

        self.blue_full_system_proto_unix_io.register_observer(
            SSL_WrapperPacket, self.ssl_wrapper_buffer
        )
        self.blue_full_system_proto_unix_io.register_observer(
            RobotStatus, self.robot_status_buffer
        )
        if self.is_yellow_friendly:
            self.yellow_full_system_proto_unix_io.register_observer(
                World, self.world_buffer
            )
            self.yellow_full_system_proto_unix_io.register_observer(
                PrimitiveSet, self.primitive_set_buffer
            )
        # Only validate on the blue worlds
        else:
            self.blue_full_system_proto_unix_io.register_observer(
                World, self.world_buffer
            )
            self.blue_full_system_proto_unix_io.register_observer(
                PrimitiveSet, self.primitive_set_buffer
            )

    def send_gamecontroller_command(
        self,
        gc_command: proto.ssl_gc_state_pb2.Command,
        is_friendly: bool,
        final_ball_placement_point=None,
    ):
        """Sends a gamecontroller command that is to be broadcasted to the given team

        :param gc_command: the gamecontroller command
        :param is_friendly: whether the command should be sent to the friendly team
        :param final_ball_placement_point: where to place the ball in ball placement
        """
        friendly_team, enemy_team = (
            (Team.YELLOW, Team.BLUE)
            if self.is_yellow_friendly
            else (Team.BLUE, Team.YELLOW)
        )
        team = friendly_team if is_friendly else enemy_team

        self.gamecontroller.send_gc_command(
            gc_command=gc_command,
            team=team,
            final_ball_placement_point=final_ball_placement_point,
        )

    def set_tactics(
        self,
        tactics: AssignedTacticPlayControlParams,
        is_friendly: bool,
    ):
        """Overrides current AI tactic for the given team

        :param tactic: the tactic params proto to use
        :param is_friendly: whether the play should be applied to the "friendly" team
        """
        fs_proto_unix_io = self.blue_full_system_proto_unix_io
        # If (friendly & yellow_friendly) or (~friendly & ~yellow_friendly), set command team to yellow
        if (is_friendly and self.is_yellow_friendly) or not (
            is_friendly or self.is_yellow_friendly
        ):
            fs_proto_unix_io = self.yellow_full_system_proto_unix_io

        fs_proto_unix_io.send_proto(AssignedTacticPlayControlParams, tactics)

    def set_play(self, play: Play, is_friendly: bool):
        """Overrides current AI play for the given team

        :param play: the play proto to use
        :param is_blue: whether the play should be applied to the blue team
        """
        fs_proto_unix_io = self.blue_full_system_proto_unix_io
        # If (friendly & yellow_friendly) or (~friendly & ~yellow_friendly), set command team to yellow
        if (is_friendly and self.is_yellow_friendly) or not (
            is_friendly or self.is_yellow_friendly
        ):
            fs_proto_unix_io = self.yellow_full_system_proto_unix_io

        fs_proto_unix_io.send_proto(Play, play)

    @abstractmethod
    def set_worldState(self, worldstate: WorldState):
        """Sets the worldstate for the given team

        :param worldstate: the worldstate proto to use
        """
        raise NotImplementedError("abstract class method called set_worldstate")

    @abstractmethod
    def run_test(
        self,
        always_validation_sequence_set=[[]],
        eventually_validation_sequence_set=[[]],
        test_timeout_s=3,
    ):
        """Begins validating a test based on incoming world protos

        :param always_validation_sequence_set: validation set that must always be true
        :param eventually_validation_sequence_set: validation set that must eventually be true
        :param test_timeout_s: how long the test will run
        """
        raise NotImplementedError("abstract method run_test called from base class")
