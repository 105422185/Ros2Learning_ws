#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2/LinearMath/Quaternion.h>
#include <my_robot_commander_cpp/move_to_target.hpp>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("test_moveit");
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  auto spinner = std::thread([&executor]() { executor.spin(); });

  auto arm = moveit::planning_interface::MoveGroupInterface(node, "arm");
  arm.setMaxVelocityScalingFactor(1.0);
  arm.setMaxAccelerationScalingFactor(1.0);

  auto gripper = moveit::planning_interface::MoveGroupInterface(node, "gripper");



  tf2::Quaternion q;
  q.setRPY(3.14, 0.0, 0.0);
  q = q.normalize();

  geometry_msgs::msg::PoseStamped target_pose;
  target_pose.header.frame_id = "base_link";
  target_pose.pose.position.x = 0.0;
  target_pose.pose.position.y = -0.7;
  target_pose.pose.position.z = 0.4;
  target_pose.pose.orientation.x = q.getX();
  target_pose.pose.orientation.y = q.getY();
  target_pose.pose.orientation.z = q.getZ();
  target_pose.pose.orientation.w = q.getW();

  
  arm.setStartStateToCurrentState();
  arm.setPoseTarget(target_pose);

  moveit::planning_interface::MoveGroupInterface::Plan plan_pose;
  bool success_pose =
    (arm.plan(plan_pose) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success_pose) {
    arm.execute(plan_pose);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to plan");
  }






  rclcpp::shutdown();
  spinner.join();
  return 0;
}

  // ==========================================================================
  // Named Target（SRDF group_state）
  // ==========================================================================
  /*
  arm.setStartStateToCurrentState();
  arm.setNamedTarget("pose_1");

  moveit::planning_interface::MoveGroupInterface::Plan plan1;
  bool success1 = (arm.plan(plan1) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success1) {
    arm.execute(plan1);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to plan");
  }

  arm.setStartStateToCurrentState();
  arm.setNamedTarget("home");

  moveit::planning_interface::MoveGroupInterface::Plan plan2;
  bool success2 = (arm.plan(plan2) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success2) {
    arm.execute(plan2);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to plan");
  }
  */

  // ==========================================================================
  // Joint Goal（关节角目标）
  // ==========================================================================
  /*
  std::vector<double> joints = {1.5, 0.5, 0.0, 1.5, 0.0, -0.7};
  arm.setStartStateToCurrentState();
  arm.setJointValueTarget(joints);

  moveit::planning_interface::MoveGroupInterface::Plan plan_joint;
  bool success_joint =
    (arm.plan(plan_joint) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success_joint) {
    arm.execute(plan_joint);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to plan");
  }
  */

  // ==========================================================================
  // Pose Goal（末端位姿目标）
  // ==========================================================================
  /*
  tf2::Quaternion q;
  q.setRPY(3.14, 0.0, 0.0);
  q = q.normalize();

  geometry_msgs::msg::PoseStamped target_pose;
  target_pose.header.frame_id = "base_link";
  target_pose.pose.position.x = 0.0;
  target_pose.pose.position.y = -0.7;
  target_pose.pose.position.z = 0.4;
  target_pose.pose.orientation.x = q.getX();
  target_pose.pose.orientation.y = q.getY();
  target_pose.pose.orientation.z = q.getZ();
  target_pose.pose.orientation.w = q.getW();

  
  arm.setStartStateToCurrentState();
  arm.setPoseTarget(target_pose);

  moveit::planning_interface::MoveGroupInterface::Plan plan_pose;
  bool success_pose =
    (arm.plan(plan_pose) == moveit::core::MoveItErrorCode::SUCCESS);
  if (success_pose) {
    arm.execute(plan_pose);
  } else {
    RCLCPP_ERROR(node->get_logger(), "Failed to plan");
  }
  */

  // ==========================================================================
  // 封装函数（实际执行）
  // ==========================================================================
  /*
  move_to_target(arm, node->get_logger(), "arm_reset", "home");
  move_to_target(gripper, node->get_logger(), "gripper_reset", "gripper_closed");

  move_to_target(arm, node->get_logger(), "plan1", "pose_1");
  move_to_target(gripper, node->get_logger(), "grasp1", "gripper_half_closed");
  move_to_target(arm, node->get_logger(), "plan2", "pose_2");
  move_to_target(gripper, node->get_logger(), "release1", "gripper_open");
  */