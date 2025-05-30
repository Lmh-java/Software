#include "software/ai/hl/stp/tactic/crease_defender/crease_defender_fsm.h"

#include "proto/message_translation/tbots_protobuf.h"
#include "software/ai/hl/stp/tactic/dribble/dribble_fsm.h"
#include "software/geom/algorithms/contains.h"
#include "software/geom/algorithms/distance.h"
#include "software/geom/stadium.h"

std::optional<Point> CreaseDefenderFSM::findBlockThreatPoint(
    const Field& field, const Point& enemy_threat_origin,
    const TbotsProto::CreaseDefenderAlignment& crease_defender_alignment,
    double robot_obstacle_inflation_factor)
{
    // We increment the angle to positive goalpost by 1/6, 3/6, or 5/6 of the shot
    // cone
    Angle shot_angle_sixth = convexAngle(field.friendlyGoalpostPos(), enemy_threat_origin,
                                         field.friendlyGoalpostNeg()) /
                             6.0;
    Angle angle_to_positive_goalpost =
        (field.friendlyGoalpostPos() - enemy_threat_origin).orientation();
    Angle angle_to_block = angle_to_positive_goalpost + shot_angle_sixth * 3.0;
    if (crease_defender_alignment == TbotsProto::CreaseDefenderAlignment::LEFT)
    {
        angle_to_block = angle_to_positive_goalpost + shot_angle_sixth * 1.0;
    }
    else if (crease_defender_alignment == TbotsProto::CreaseDefenderAlignment::RIGHT)
    {
        angle_to_block = angle_to_positive_goalpost + shot_angle_sixth * 5.0;
    }

    // Shot ray to block
    Ray ray(enemy_threat_origin, angle_to_block);

    return findDefenseAreaIntersection(field, ray, robot_obstacle_inflation_factor);
}

bool CreaseDefenderFSM::isAnyEnemyInZone(const Update& event, const Stadium& zone)
{
    std::vector<Robot> enemy_robots = event.common.world_ptr->enemyTeam().getAllRobots();
    return std::any_of(enemy_robots.begin(), enemy_robots.end(),
                       [zone, enemy_robots](const Robot& robot)
                       { return contains(zone, robot.position()); });
}

void CreaseDefenderFSM::blockThreat(
    const Update& event, boost::sml::back::process<MoveFSM::Update> processEvent)
{
    Point robot_position    = event.common.robot.position();
    Point destination       = event.common.robot.position();
    Angle robot_orientation = event.common.robot.orientation();
    // Use a slightly larger inflation factor to avoid the crease defenders from sitting
    double robot_obstacle_inflation_factor =
        robot_navigation_obstacle_config.robot_obstacle_inflation_factor() + 0.5;
    double robot_radius_expansion_amount =
        ROBOT_MAX_RADIUS_METERS * robot_obstacle_inflation_factor;
    Rectangle inflated_defense_area =
        event.common.world_ptr->field().friendlyDefenseArea().expand(
            robot_radius_expansion_amount);
    // right on the edge of the defense area obstacle.
    auto block_threat_point = findBlockThreatPoint(
        event.common.world_ptr->field(), event.control_params.enemy_threat_origin,
        event.control_params.crease_defender_alignment, robot_obstacle_inflation_factor);

    if (contains(inflated_defense_area, event.control_params.enemy_threat_origin))
    {
        // Check to see if ray is within goalie box to ignore and station bot along
        // inflated defense area
        auto crease_defender_alignment = event.control_params.crease_defender_alignment;
        if (crease_defender_alignment == TbotsProto::CreaseDefenderAlignment::LEFT)
        {
            destination = Point(inflated_defense_area.posXPosYCorner().x(),
                                ROBOT_MAX_RADIUS_METERS);
        }
        else if (crease_defender_alignment == TbotsProto::CreaseDefenderAlignment::RIGHT)
        {
            destination = Point(inflated_defense_area.posXPosYCorner().x(),
                                -ROBOT_MAX_RADIUS_METERS);
        }
    }
    else if (block_threat_point)
    {
        destination = block_threat_point.value();
    }
    else if (event.control_params.enemy_threat_origin.x() >=
             event.common.world_ptr->field().friendlyDefenseArea().negXNegYCorner().x())
    {
        // Ignore cases when the ball is behind the friendly goal baseline
        LOG(WARNING)
            << "Could not find a point on the defense area to block a potential shot";
    }
    Angle face_threat_orientation =
        (event.control_params.enemy_threat_origin - event.common.robot.position())
            .orientation();

    // Chip to the enemy half of the field
    double chip_distance = event.common.world_ptr->field().xLength() / 3.0;
    // If enemy threat is on the sides, then chip to near the edge of the field
    if (event.control_params.enemy_threat_origin.x() <
        event.common.world_ptr->field().friendlyDefenseArea().xMax())
    {
        chip_distance = event.common.world_ptr->field().yLength() / 3.0 -
                        event.common.world_ptr->field().friendlyDefenseArea().yMax();
    }

    TbotsProto::BallCollisionType ball_collision_type =
        TbotsProto::BallCollisionType::ALLOW;
    if ((event.common.world_ptr->ball().position() - destination).length() <
        (robot_position - destination).length())
    {
        ball_collision_type = TbotsProto::BallCollisionType::AVOID;
    }

    AutoChipOrKick auto_chip_or_kick{AutoChipOrKickMode::OFF, 0};
    auto goal_post_offset_vector =
        Vector(0, crease_defender_config.goal_post_offset_chipping());
    auto goal_line_segment =
        Segment(event.common.world_ptr->field().friendlyGoal().posXPosYCorner() +
                    goal_post_offset_vector,
                event.common.world_ptr->field().friendlyGoal().posXNegYCorner() -
                    goal_post_offset_vector);
    Ray robot_shoot_ray = Ray(robot_position, robot_orientation);
    std::vector<Point> goal_intersections =
        intersection(robot_shoot_ray, goal_line_segment);

    // Create threat zone ahead to determine safety to steal
    Stadium threat_zone = Stadium(robot_position,
                                  Vector::createFromAngle(robot_orientation)
                                      .normalize(DETECT_THREAT_AHEAD_SHAPE_LENGTH_M),
                                  DETECT_THREAT_AHEAD_SHAPE_RADIUS_M);

    double robot_to_net_m =
        distance(robot_position, event.common.world_ptr->field().friendlyGoal().centre());

    if (goal_intersections.empty() &&
        CreaseDefenderFSM::isAnyEnemyInZone(event, threat_zone) &&
        robot_to_net_m <= event.common.world_ptr->field().totalYLength() / 2)
    {
        // Autochip only if the robot is not facing the net, there is an enemy in front,
        // and robot is close to net
        auto_chip_or_kick = AutoChipOrKick{AutoChipOrKickMode::AUTOCHIP, chip_distance};
    }

    MoveFSM::ControlParams control_params{
        .destination             = destination,
        .final_orientation       = face_threat_orientation,
        .dribbler_mode           = TbotsProto::DribblerMode::OFF,
        .ball_collision_type     = ball_collision_type,
        .auto_chip_or_kick       = auto_chip_or_kick,
        .max_allowed_speed_mode  = event.control_params.max_allowed_speed_mode,
        .obstacle_avoidance_mode = TbotsProto::ObstacleAvoidanceMode::AGGRESSIVE};

    // Update the get behind ball fsm
    processEvent(MoveFSM::Update(control_params, event.common));
}

std::optional<Point> CreaseDefenderFSM::findDefenseAreaIntersection(
    const Field& field, const Ray& ray, double robot_obstacle_inflation_factor)
{
    // Return the segments that form the path around the crease that the
    // defenders must follow. It's basically the crease inflated by one robot radius
    // multiplied by a factor
    double robot_radius_expansion_amount =
        ROBOT_MAX_RADIUS_METERS * robot_obstacle_inflation_factor;
    Rectangle inflated_defense_area =
        field.friendlyDefenseArea().expand(robot_radius_expansion_amount);

    auto front_segment = Segment(inflated_defense_area.posXPosYCorner(),
                                 inflated_defense_area.posXNegYCorner());
    auto left_segment  = Segment(inflated_defense_area.posXPosYCorner(),
                                 inflated_defense_area.negXPosYCorner());
    auto right_segment = Segment(inflated_defense_area.posXNegYCorner(),
                                 inflated_defense_area.negXNegYCorner());
    std::vector<Point> front_intersections = intersection(ray, front_segment);
    if (!front_intersections.empty() && ray.getStart().x() > front_segment.getStart().x())
    {
        return front_intersections[0];
    }
    if (ray.getStart().y() > 0)
    {
        // Check left segment if ray start point is in positive y half
        std::vector<Point> left_intersections = intersection(ray, left_segment);
        if (!left_intersections.empty())
        {
            return left_intersections[0];
        }
    }
    else
    {
        // Check right segment if ray start point is in negative y half
        std::vector<Point> right_intersections = intersection(ray, right_segment);
        if (!right_intersections.empty())
        {
            return right_intersections[0];
        }
    }

    return std::nullopt;
}

bool CreaseDefenderFSM::ballNearbyWithoutThreat(const Update& event)
{
    bool ball_on_friendly_side = event.common.world_ptr->ball().position().x() < 0;
    return ball_on_friendly_side && DefenderFSMBase::ballNearbyWithoutThreat(
                                        event.common.world_ptr, event.common.robot,
                                        event.control_params.ball_steal_mode,
                                        crease_defender_config.defender_steal_config());
}

void CreaseDefenderFSM::prepareGetPossession(
    const Update& event, boost::sml::back::process<DribbleFSM::Update> processEvent)
{
    DefenderFSMBase::prepareGetPossession(event.common, processEvent);
}
