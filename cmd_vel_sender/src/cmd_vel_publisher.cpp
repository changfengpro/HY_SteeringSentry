/*
 * @Description: 
 * @Author: changfeng
 * @brief: 
 * @version: 
 * @Date: 2025-01-22 23:14:01
 * @LastEditors:  
 * @LastEditTime: 2025-01-22 23:23:51
 */
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std;
using namespace geometry_msgs::msg;

class CmdVelPublisher : public rclcpp::Node
{
public:
  CmdVelPublisher()
  : Node("cmd_vel_publisher")
  {
    // 创建发布者，发布到 "cmd_vel" 话题
    cmd_vel_pub_ = this->create_publisher<Twist>("cmd_vel", 10);

    // 设置定时器，定期发布消息
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500), // 每500ms发布一次
      std::bind(&CmdVelPublisher::publish_cmd_vel, this)
    );
  }

private:
  // 发布者对象
  rclcpp::Publisher<Twist>::SharedPtr cmd_vel_pub_;

  // 定时器
  rclcpp::TimerBase::SharedPtr timer_;

  // 发布cmd_vel消息的函数
  void publish_cmd_vel()
  {
    // 创建一个Twist消息
    Twist msg;

    // 设置线速度和角速度（可以根据需要调整这些值）
    msg.linear.x = 0.5;  // 前进
    msg.angular.z = 0.1; // 轻微旋转

    // 发布消息
    RCLCPP_INFO(this->get_logger(), "Publishing: linear.x = %.2f, angular.z = %.2f", msg.linear.x, msg.angular.z);
    cmd_vel_pub_->publish(msg);
  }
};

int main(int argc, char * argv[])
{
  // 初始化ROS 2
  rclcpp::init(argc, argv);

  // 创建并运行发布者节点
  rclcpp::spin(std::make_shared<CmdVelPublisher>());

  // 关闭ROS 2
  rclcpp::shutdown();
  return 0;
}
