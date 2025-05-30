package(default_visibility = ["//visibility:public"])

load("@thunderscope_deps//:requirements.bzl", "requirement")
load("@rules_python//python:pip.bzl", "compile_pip_requirements")

compile_pip_requirements(
    name = "requirements",
    src = "requirements.in",
    requirements_txt = "requirements_lock.txt",
)

py_binary(
    name = "thunderscope_main",
    srcs = ["thunderscope_main.py"],
    deps = [
        ":config",
        ":constants",
        ":estop_helpers",
        ":thunderscope",
        ":util",
        "//software/thunderscope/binary_context_managers:full_system",
        "//software/thunderscope/binary_context_managers:game_controller",
        "//software/thunderscope/binary_context_managers:simulator",
    ],
)

py_library(
    name = "thunderscope",
    srcs = ["thunderscope.py"],
    data = [
        "thunderscope-logo.png",
        "//software:py_constants.so",
        "//software:python_bindings.so",
    ],
    deps = [
        ":config",
        "//extlibs/er_force_sim/src/protobuf:erforce_py_proto",
        "//proto:software_py_proto",
        "//proto/message_translation:py_tbots_protobuf",
        "//software/networking/unix:threaded_unix_listener_py",
        "//software/networking/unix:threaded_unix_sender_py",
        "//software/thunderscope:constants",
        "//software/thunderscope:dock_style",
        "//software/thunderscope:proto_unix_io",
        "//software/thunderscope:robot_communication",
        "//software/thunderscope/binary_context_managers:full_system",
        "//software/thunderscope/binary_context_managers:game_controller",
        "//software/thunderscope/binary_context_managers:simulator",
        "//software/thunderscope/binary_context_managers:tigers_autoref",
        requirement("numpy"),
        requirement("pyqtdarktheme-fork"),
        requirement("pyqtgraph"),
    ],
)

py_library(
    name = "time_provider",
    srcs = ["time_provider.py"],
)

py_library(
    name = "util",
    srcs = ["util.py"],
    data = ["//software:py_constants.so"],
    deps = [
        ":constants",
        ":thread_safe_buffer",
        ":time_provider",
        "//proto/message_translation:py_tbots_protobuf",
    ],
)

py_library(
    name = "widget_names_to_setup",
    srcs = ["widget_setup_functions.py"],
    data = [
        "//software:py_constants.so",
        "//software:python_bindings.so",
    ],
    deps = [
        "//software/thunderscope/binary_context_managers:game_controller",
        "//software/thunderscope/common:common_widgets",
        "//software/thunderscope/common:fps_widget",
        "//software/thunderscope/common:proto_configuration_widget",
        "//software/thunderscope/common:proto_plotter",
        "//software/thunderscope/gl:gl_widget",
        "//software/thunderscope/gl/layers:gl_attacker_layer",
        "//software/thunderscope/gl/layers:gl_cost_vis_layer",
        "//software/thunderscope/gl/layers:gl_debug_shapes_layer",
        "//software/thunderscope/gl/layers:gl_draw_polygon_obstacle",
        "//software/thunderscope/gl/layers:gl_max_dribble_layer",
        "//software/thunderscope/gl/layers:gl_movement_field_test_layer",
        "//software/thunderscope/gl/layers:gl_obstacle_layer",
        "//software/thunderscope/gl/layers:gl_passing_layer",
        "//software/thunderscope/gl/layers:gl_path_layer",
        "//software/thunderscope/gl/layers:gl_referee_info_layer",
        "//software/thunderscope/gl/layers:gl_sandbox_world_layer",
        "//software/thunderscope/gl/layers:gl_simulator_layer",
        "//software/thunderscope/gl/layers:gl_tactic_layer",
        "//software/thunderscope/gl/layers:gl_trail_layer",
        "//software/thunderscope/gl/layers:gl_validation_layer",
        "//software/thunderscope/gl/layers:gl_world_layer",
        "//software/thunderscope/log:g3log_checkboxes",
        "//software/thunderscope/log:g3log_widget",
        "//software/thunderscope/play:playinfo_widget",
        "//software/thunderscope/play:refereeinfo_widget",
        "//software/thunderscope/replay:proto_player",
        "//software/thunderscope/robot_diagnostics:chicker",
        "//software/thunderscope/robot_diagnostics:diagnostics_widget",
        "//software/thunderscope/robot_diagnostics:drive_and_dribbler_widget",
        "//software/thunderscope/robot_diagnostics:estop_view",
        "//software/thunderscope/robot_diagnostics:robot_error_log",
        "//software/thunderscope/robot_diagnostics:robot_info",
        "//software/thunderscope/robot_diagnostics:robot_view",
    ],
)

py_library(
    name = "thunderscope_types",
    srcs = ["thunderscope_types.py"],
    deps = [
        ":constants",
    ],
)

py_library(
    name = "config",
    srcs = ["thunderscope_config.py"],
    deps = [
        ":constants",
        ":proto_unix_io",
        ":thunderscope_types",
        ":widget_names_to_setup",
        requirement("pyqtdarktheme-fork"),
    ],
)

py_library(
    name = "proto_unix_io",
    srcs = ["proto_unix_io.py"],
    deps = [
        ":thread_safe_buffer",
        "//software/networking/unix:threaded_unix_listener_py",
        "//software/networking/unix:threaded_unix_sender_py",
    ],
)

py_library(
    name = "constants",
    srcs = ["constants.py"],
    deps = [
        requirement("protobuf"),
    ],
)

py_library(
    name = "dock_style",
    srcs = ["dock_style.py"],
)

py_library(
    name = "thread_safe_buffer",
    srcs = ["thread_safe_buffer.py"],
)

py_library(
    name = "robot_communication",
    srcs = ["robot_communication.py"],
    data = [
        "//software:py_constants.so",
    ],
    deps = [
        ":wifi_communication_manager",
        "//software/logger:py_logger",
        "//software/thunderscope:constants",
    ],
)

py_library(
    name = "wifi_communication_manager",
    srcs = ["wifi_communication_manager.py"],
    deps = [
        requirement("colorama"),
    ],
)

py_library(
    name = "estop_helpers",
    srcs = ["estop_helpers.py"],
    deps = [
        "//software/thunderscope:constants",
    ],
)
