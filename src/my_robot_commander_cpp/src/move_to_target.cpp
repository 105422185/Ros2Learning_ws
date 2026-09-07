#include "my_robot_commander_cpp/move_to_target.hpp"

bool move_to_target(moveit::planning_interface::MoveGroupInterface& group,
                    const rclcpp::Logger& logger,
                    const std::string& task_name,
                    const std::string& target_name)
{
    RCLCPP_INFO(logger, "[%s] moving to target '%s' ...",
                task_name.c_str(), target_name.c_str());

    group.setStartStateToCurrentState();
    group.setNamedTarget(target_name);

    moveit::planning_interface::MoveGroupInterface::Plan plan;
    if (group.plan(plan) != moveit::core::MoveItErrorCode::SUCCESS) {
        RCLCPP_ERROR(logger, "[%s] failed to plan to '%s'",
                     task_name.c_str(), target_name.c_str());
        return false;
    }

    group.execute(plan);
    RCLCPP_INFO(logger, "[%s] reached '%s'", task_name.c_str(), target_name.c_str());
    return true;
}
