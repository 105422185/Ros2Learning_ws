#ifndef MY_ROBOT_COMMANDER_CPP__MOVE_TO_TARGET_HPP_
#define MY_ROBOT_COMMANDER_CPP__MOVE_TO_TARGET_HPP_

#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <string>

// 封装"设目标 -> 规划 -> 执行"整个流程，可对任意控制组复用
//   group      : 控制组（arm / gripper 的 MoveGroupInterface）
//   logger     : 日志输出用
//   task_name  : 任务名称（仅用于日志区分，如 "plan3"、"grasp"）
//   target_name: 目标点名称（SRDF 里的 group_state，如 "pose_1"/"home"/"gripper_open"）
// 返回 true 表示规划并执行成功，false 表示规划失败
bool move_to_target(moveit::planning_interface::MoveGroupInterface& group,
                    const rclcpp::Logger& logger,
                    const std::string& task_name,
                    const std::string& target_name);

#endif  // MY_ROBOT_COMMANDER_CPP__MOVE_TO_TARGET_HPP_
