package(default_visibility = ["//visibility:public"])

load("@simulated_tests_deps//:requirements.bzl", "requirement")

# We force linking for all plays so that the static variables required for the
# "factory" design pattern to work are linked in
# https://www.bfilipek.com/2018/02/static-vars-static-lib.html

cc_library(
    name = "kickoff_enemy_play",
    srcs = ["kickoff_enemy_play.cpp"],
    hdrs = ["kickoff_enemy_play.h"],
    deps = [
        ":play",
        "//shared:constants",
        "//software/ai/evaluation:enemy_threat",
        "//software/ai/evaluation:possession",
        "//software/ai/hl/stp/tactic/goalie:goalie_tactic",
        "//software/ai/hl/stp/tactic/move:move_tactic",
        "//software/ai/hl/stp/tactic/shadow_enemy:shadow_enemy_tactic",
        "//software/logger",
        "//software/util/generic_factory",
    ],
    alwayslink = True,
)

cc_library(
    name = "kickoff_friendly_play",
    srcs = ["kickoff_friendly_play.cpp"],
    hdrs = ["kickoff_friendly_play.h"],
    deps = [
        ":play",
        "//shared:constants",
        "//software/ai/evaluation:enemy_threat",
        "//software/ai/hl/stp/tactic/chip:chip_tactic",
        "//software/ai/hl/stp/tactic/move:move_tactic",
        "//software/logger",
        "//software/util/generic_factory",
    ],
    alwayslink = True,
)

cc_library(
    name = "shoot_or_chip_play",
    srcs = ["shoot_or_chip_play.cpp"],
    hdrs = ["shoot_or_chip_play.h"],
    deps = [
        ":play",
        "//proto/message_translation:tbots_protobuf",
        "//shared:constants",
        "//software/ai/evaluation:enemy_threat",
        "//software/ai/evaluation:find_open_areas",
        "//software/ai/evaluation:possession",
        "//software/ai/hl/stp/tactic/attacker:attacker_tactic",
        "//software/ai/hl/stp/tactic/crease_defender:crease_defender_tactic",
        "//software/ai/hl/stp/tactic/goalie:goalie_tactic",
        "//software/ai/hl/stp/tactic/halt:halt_tactic",
        "//software/ai/hl/stp/tactic/move:move_tactic",
        "//software/ai/hl/stp/tactic/shadow_enemy:shadow_enemy_tactic",
        "//software/logger",
        "//software/util/generic_factory",
        "//software/world:game_state",
    ],
    alwayslink = True,
)

cc_library(
    name = "stop_play",
    srcs = ["stop_play.cpp"],
    hdrs = ["stop_play.h"],
    deps = [
        ":play",
        "//shared:constants",
        "//software/ai/evaluation:enemy_threat",
        "//software/ai/hl/stp/tactic/crease_defender:crease_defender_tactic",
        "//software/ai/hl/stp/tactic/goalie:goalie_tactic",
        "//software/ai/hl/stp/tactic/move:move_tactic",
        "//software/logger",
        "//software/util/generic_factory",
    ],
    alwayslink = True,
)

cc_library(
    name = "play",
    srcs = ["play.cpp"],
    hdrs = [
        "play.h",
        "play_fsm.h",
    ],
    deps = [
        "//software/ai/hl/stp/tactic",
        "//software/ai/hl/stp/tactic/goalie:goalie_tactic",
        "//software/ai/hl/stp/tactic/halt:halt_tactic",
        "//software/ai/motion_constraint:motion_constraint_set_builder",
        "//software/ai/navigator/trajectory:trajectory_planner",
        "//software/ai/passing:pass_with_rating",
        "//software/util/sml_fsm",
        "@boost//:coroutine2",
        "@munkres_cpp",
        "@tracy",
    ],
)

cc_library(
    name = "all_plays",
    deps = [
        ":kickoff_enemy_play",
        ":kickoff_friendly_play",
        ":shoot_or_chip_play",
        ":stop_play",
        "//software/ai/hl/stp/play/ball_placement:ball_placement_play",
        "//software/ai/hl/stp/play/crease_defense:crease_defense_play",
        "//software/ai/hl/stp/play/defense:defense_play",
        "//software/ai/hl/stp/play/enemy_ball_placement:enemy_ball_placement_play",
        "//software/ai/hl/stp/play/enemy_free_kick:enemy_free_kick_play",
        "//software/ai/hl/stp/play/example:example_play",
        "//software/ai/hl/stp/play/free_kick:free_kick_play",
        "//software/ai/hl/stp/play/halt_play",
        "//software/ai/hl/stp/play/hardware_challenge_plays:dribbling_parcour_play",
        "//software/ai/hl/stp/play/hardware_challenge_plays:pass_endurance_play",
        "//software/ai/hl/stp/play/hardware_challenge_plays:scoring_from_contested_possession_play",
        "//software/ai/hl/stp/play/hardware_challenge_plays:scoring_with_static_defenders_play",
        "//software/ai/hl/stp/play/offense:offense_play",
        "//software/ai/hl/stp/play/penalty_kick:penalty_kick_play",
        "//software/ai/hl/stp/play/penalty_kick_enemy:penalty_kick_enemy_play",
        "//software/ai/hl/stp/play/shoot_or_pass:shoot_or_pass_play",
    ],
)

cc_test(
    name = "kickoff_friendly_play_cpp_test",
    srcs = ["kickoff_friendly_play_test.cpp"],
    deps = [
        "//shared/test_util:tbots_gtest_main",
        "//software/ai/hl/stp/play:kickoff_friendly_play",
        "//software/simulated_tests:simulated_er_force_sim_play_test_fixture",
        "//software/simulated_tests/non_terminating_validation_functions",
        "//software/simulated_tests/terminating_validation_functions",
        "//software/simulated_tests/validation:validation_function",
        "//software/test_util",
        "//software/time:duration",
        "//software/world",
    ],
)

cc_test(
    name = "kickoff_enemy_play_cpp_test",
    srcs = ["kickoff_enemy_play_test.cpp"],
    deps = [
        "//shared/test_util:tbots_gtest_main",
        "//software/ai/hl/stp/play:kickoff_enemy_play",
        "//software/geom/algorithms",
        "//software/simulated_tests:simulated_er_force_sim_play_test_fixture",
        "//software/simulated_tests/non_terminating_validation_functions",
        "//software/simulated_tests/terminating_validation_functions",
        "//software/simulated_tests/validation:validation_function",
        "//software/test_util",
        "//software/time:duration",
        "//software/world",
    ],
)

py_test(
    name = "kickoff_play_test",
    srcs = [
        "kickoff_play_test.py",
    ],
    # TODO (#2619) Remove tag to run in parallel
    tags = [
        "exclusive",
    ],
    deps = [
        "//software:conftest",
        "//software/simulated_tests:validation",
        requirement("pytest"),
    ],
)

cc_test(
    name = "stop_play_test",
    srcs = ["stop_play_test.cpp"],
    deps = [
        "//shared/test_util:tbots_gtest_main",
        "//software/ai/hl/stp/play:stop_play",
        "//software/simulated_tests:simulated_er_force_sim_play_test_fixture",
        "//software/simulated_tests/non_terminating_validation_functions",
        "//software/simulated_tests/validation:validation_function",
        "//software/test_util",
        "//software/time:duration",
        "//software/world",
    ],
)

cc_test(
    name = "shoot_or_chip_play_cpp_test",
    srcs = ["shoot_or_chip_play_test.cpp"],
    deps = [
        "//shared/test_util:tbots_gtest_main",
        "//software/ai/hl/stp/play:shoot_or_chip_play",
        "//software/simulated_tests:simulated_er_force_sim_play_test_fixture",
        "//software/simulated_tests/validation:validation_function",
        "//software/test_util",
        "//software/time:duration",
        "//software/world",
    ],
)

py_test(
    name = "shoot_or_chip_play_test",
    srcs = [
        "shoot_or_chip_play_test.py",
    ],
    # TODO (#2619) Remove tag to run in parallel
    tags = [
        "exclusive",
    ],
    deps = [
        "//software:conftest",
        "//software/simulated_tests:validation",
        requirement("pytest"),
    ],
)

cc_library(
    name = "assigned_tactics_play",
    srcs = ["assigned_tactics_play.cpp"],
    hdrs = ["assigned_tactics_play.h"],
    deps = [
        ":play",
        "//shared:constants",
        "//software/ai/hl/stp/tactic/halt:halt_tactic",
        "//software/logger",
        "//software/util/generic_factory",
    ],
    alwayslink = True,
)

cc_library(
    name = "play_factory",
    srcs = ["play_factory.cpp"],
    hdrs = [
        "play_factory.h",
    ],
    deps = [
        ":all_plays",
        "//proto:tbots_cc_proto",
        "//proto/message_translation:tbots_protobuf",
        "//software/util/generic_factory",
    ],
)

cc_test(
    name = "play_factory_test",
    srcs = ["play_factory_test.cpp"],
    deps = [
        "//shared/test_util:tbots_gtest_main",
        "//software/ai/hl/stp/play:play_factory",
    ],
)

py_test(
    name = "passing_sim_test",
    srcs = [
        "passing_sim_test.py",
    ],
    data = [
        "//software:py_constants.so",
    ],
    tags = ["exclusive"],
    deps = [
        "//proto:import_all_protos",
        "//proto:software_py_proto",
        "//proto:tbots_py_proto",
        "//software:conftest",
        "//software/simulated_tests:validation",
        requirement("pytest"),
    ],
)
