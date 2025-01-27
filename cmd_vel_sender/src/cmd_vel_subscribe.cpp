/*
 * @Description: 
 * @Author: changfeng
 * @brief: 
 * @version: 
 * @Date: 2025-01-22 14:07:05
 * @LastEditors:  
 * @LastEditTime: 2025-01-27 12:17:57
 */
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "serial.hpp"

using namespace std;
using namespace geometry_msgs::msg;

class CmdVelSender : public rclcpp::Node
{
public:
  CmdVelSender()
  : Node("cmd_vel_sender")
  {
    // 创建串口对象
    s1.OpenSerial("/dev/ttyCH343USB0", E_BaudRate::_115200, E_DataSize::_8, E_Parity::None, E_StopBit::_1);

    // 订阅cmd_vel消息
    cmd_vel_sub_ = this->create_subscription<Twist>(
      "cmd_vel", 10, std::bind(&CmdVelSender::cmd_vel_callback, this, std::placeholders::_1));

     // 订阅turtle_teleop_key消息
    turtle_teleop_key_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "turtle_teleop_key", 10, std::bind(&CmdVelSender::turtle_teleop_key_callback, this, std::placeholders::_1));
  }

private:
  // 串口对象
  Serial s1;

  // 订阅者对象
  rclcpp::Subscription<Twist>::SharedPtr cmd_vel_sub_;

  // 处理cmd_vel消息的回调函数
  void cmd_vel_callback(const Twist::SharedPtr msg)
  {
    // 获取消息中的线速度和角速度
    float linear_x = msg->linear.x;
    float angular_z = msg->angular.z;

    // 创建数据，将线速度和角速度转换为字符串或其他适合的格式
    std::string serial_data = "Linear X: " + std::to_string(linear_x) + ", Angular Z: " + std::to_string(angular_z);

    // 发送数据
    s1.Send(serial_data.data(), serial_data.size());
  }

  // turtle_teleop_key 回调函数
  void turtle_teleop_key_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    RCLCPP_INFO(this->get_logger(), "Received turtle_teleop_key: Linear x: %f, Angular z: %f", msg->linear.x, msg->angular.z);
    // 处理来自 turtle_teleop_key 的消息（例如发送到串口）
  }

  // 订阅器
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr turtle_teleop_key_sub_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CmdVelSender>());
  rclcpp::shutdown();
  return 0;
}
