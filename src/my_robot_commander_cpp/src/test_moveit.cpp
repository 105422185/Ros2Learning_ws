#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <my_robot_commander_cpp/move_to_target.hpp>

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("test_moveit");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    auto spinner = std::thread([&executor]() {executor.spin();});

    auto arm = moveit::planning_interface::MoveGroupInterface(node, "arm");
    arm.setMaxVelocityScalingFactor(1.0);
    arm.setMaxAccelerationScalingFactor(1.0);

    // 夹爪组：同一套函数直接复用
    auto gripper = moveit::planning_interface::MoveGroupInterface(node, "gripper");

    move_to_target(arm, node->get_logger(), "arm_reset", "home");
    move_to_target(gripper, node->get_logger(), "gripper_reset", "gripper_closed");
    
    move_to_target(arm, node->get_logger(), "plan1", "pose_1");
    move_to_target(gripper, node->get_logger(), "grasp1", "gripper_half_closed");
    move_to_target(arm, node->get_logger(), "plan2", "pose_2");
    move_to_target(gripper, node->get_logger(), "release1", "gripper_open");


    rclcpp::shutdown();
    spinner.join();
    return 0;
}


   /* //named target
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
    // ========== 用封装好的函数控制夹爪（一行顶上面一整段）==========
    // 以后想加新动作，只需要一行，例如：
    //   move_to_target(gripper, node->get_logger(), "grasp", "gripper_half_closed");
    //   move_to_target(arm,    node->get_logger(), "plan3", "pose_2");