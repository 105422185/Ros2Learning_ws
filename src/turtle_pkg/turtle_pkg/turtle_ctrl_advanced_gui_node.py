import sys

import rclpy
from geometry_msgs.msg import Twist
from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import (
    QApplication,
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QPushButton,
    QVBoxLayout,
    QWidget,
)
from rclpy.node import Node
from turtlesim.msg import Pose


class TurtleCtrlAdvancedGuiNode(Node):
    def __init__(self):
        super().__init__('turtle_ctrl_advanced_gui_node')
        self.publisher_ = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        self.subscription = self.create_subscription(
            Pose, '/turtle1/pose', self.pose_callback, 10)
        self.linear_x = 0.0
        self.angular_z = 0.0
        self.pose_callback_fn = None
        self.timer = self.create_timer(0.1, self.timer_callback)
        self.get_logger().info(
            'turtle_ctrl_advanced_gui_node started, '
            'pub /turtle1/cmd_vel, sub /turtle1/pose'
        )

    def set_velocity(self, linear_x, angular_z):
        self.linear_x = linear_x
        self.angular_z = angular_z
        self.get_logger().info(
            f'Send velocity: linear.x={linear_x:.2f}, angular.z={angular_z:.2f}'
        )

    def timer_callback(self):
        msg = Twist()
        msg.linear.x = self.linear_x
        msg.angular.z = self.angular_z
        self.publisher_.publish(msg)

    def pose_callback(self, msg):
        if self.pose_callback_fn is not None:
            self.pose_callback_fn(msg)


class TurtleCtrlWindow(QWidget):
    def __init__(self, node):
        super().__init__()
        self.node = node
        self.setWindowTitle('小乌龟控制器')
        self.setFixedSize(380, 320)
        self._build_ui()
        self.node.pose_callback_fn = self.update_pose

    def _build_ui(self):
        self.linear_edit = QLineEdit('0.0')
        self.angular_edit = QLineEdit('0.0')
        self.x_label = QLabel('0.000000')
        self.y_label = QLabel('0.000000')
        self.linear_label = QLabel('0.000000')
        self.angular_label = QLabel('0.000000')
        self.theta_label = QLabel('0.000000')

        send_button = QPushButton('发送')
        send_button.clicked.connect(self.on_send)

        layout = QVBoxLayout()
        layout.addLayout(self._row('线速度', self.linear_edit))
        layout.addLayout(self._row('角速度(rad/s)', self.angular_edit))
        layout.addLayout(self._row('当前X坐标', self.x_label))
        layout.addLayout(self._row('当前Y坐标', self.y_label))
        layout.addLayout(self._row('当前线速度', self.linear_label))
        layout.addLayout(self._row('当前角速度', self.angular_label))
        layout.addLayout(self._row('当前角度', self.theta_label))
        layout.addWidget(send_button)
        self.setLayout(layout)

    def _row(self, label_text, widget):
        row = QHBoxLayout()
        label = QLabel(label_text)
        label.setFixedWidth(120)
        row.addWidget(label)
        row.addWidget(widget)
        return row

    def update_pose(self, msg):
        self.x_label.setText(f'{msg.x:.6f}')
        self.y_label.setText(f'{msg.y:.6f}')
        self.linear_label.setText(f'{msg.linear_velocity:.6f}')
        self.angular_label.setText(f'{msg.angular_velocity:.6f}')
        self.theta_label.setText(f'{msg.theta:.6f}')

    def on_send(self):
        try:
            linear_x = float(self.linear_edit.text())
            angular_z = float(self.angular_edit.text())
        except ValueError:
            self.node.get_logger().warn('请输入有效数字，例如 1.0')
            return
        self.node.set_velocity(linear_x, angular_z)


def main(args=None):
    rclpy.init(args=args)
    app = QApplication(sys.argv)
    node = TurtleCtrlAdvancedGuiNode()
    window = TurtleCtrlWindow(node)
    window.show()

    ros_timer = QTimer()
    ros_timer.timeout.connect(lambda: rclpy.spin_once(node, timeout_sec=0))
    ros_timer.start(10)

    exit_code = 0
    try:
        exit_code = app.exec_()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()
    sys.exit(exit_code)


if __name__ == '__main__':
    main()
