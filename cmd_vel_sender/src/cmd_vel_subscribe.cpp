/*
 * @Description: 订阅 cmd_vel 并通过串口发送
 * @Author: changfeng
 * @Date: 2025-01-22 14:07:05
 * @LastEditTime: 2025-02-04 19:01:39
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
    RCLCPP_INFO(this->get_logger(), "CmdVelSender node started");

    // 打开串口
    s1.OpenSerial("/dev/ttyCH343USB0", E_BaudRate::_115200, E_DataSize::_8, E_Parity::None, E_StopBit::_1);

    // 订阅 cmd_vel
    cmd_vel_sub_ = this->create_subscription<Twist>(
      "cmd_vel", 10, std::bind(&CmdVelSender::cmd_vel_callback, this, std::placeholders::_1));

    // 订阅 turtle1/cmd_vel
    turtle_teleop_key_sub_ = this->create_subscription<Twist>(
      "turtle1/cmd_vel", 10, std::bind(&CmdVelSender::turtle_teleop_key_callback, this, std::placeholders::_1));
  }

private:
  Serial s1;
  rclcpp::Subscription<Twist>::SharedPtr cmd_vel_sub_;
  rclcpp::Subscription<Twist>::SharedPtr turtle_teleop_key_sub_;

  // 处理 cmd_vel 消息
  void cmd_vel_callback(const Twist::SharedPtr msg)
  {
    send_cmd_vel_data(msg);
  }

  // 处理 turtle1/cmd_vel 消息
  void turtle_teleop_key_callback(const Twist::SharedPtr msg)
  {
    send_cmd_vel_data(msg);
  }

  // **通用的发送函数**
  void send_cmd_vel_data(const Twist::SharedPtr msg)
  {
    // 数据包格式：[0xAA] [linear.x (4)] [linear.y (4)] [linear.z (4)] [angular.x (4)] [angular.y (4)] [angular.z (4)] [Checksum (1)] [0x55]
    uint8_t data_packet[27];
    data_packet[0] = 0xAA; // Start Byte

    // 将double转换为float
    float linear_x = static_cast<float>(msg->linear.x);
    float linear_y = static_cast<float>(msg->linear.y);
    float linear_z = static_cast<float>(msg->linear.z);
    float angular_x = static_cast<float>(msg->angular.x);
    float angular_y = static_cast<float>(msg->angular.y);
    float angular_z = static_cast<float>(msg->angular.z);

    // 拷贝转换后的float数据
    memcpy(&data_packet[1], &linear_x, sizeof(float));
    memcpy(&data_packet[5], &linear_y, sizeof(float));
    memcpy(&data_packet[9], &linear_z, sizeof(float));
    memcpy(&data_packet[13], &angular_x, sizeof(float));
    memcpy(&data_packet[17], &angular_y, sizeof(float));
    memcpy(&data_packet[21], &angular_z, sizeof(float));

    // 计算校验和（前 25 字节异或）
    uint8_t checksum = 0;
    for (int i = 0; i < 25; i++) {
        checksum ^= data_packet[i];
    }
    data_packet[25] = checksum;  // 存储校验和
    data_packet[26] = 0x55;      // End Byte

    // 发送数据
    s1.Send(data_packet, sizeof(data_packet));

    // 输出调试信息
    std::ostringstream oss;
    oss << "Sent data packet: ";
    for (int i = 0; i < sizeof(data_packet); i++) {
        oss << std::hex << std::uppercase << (int)data_packet[i] << " ";
    }
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());
  }
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CmdVelSender>());
  rclcpp::shutdown();
  return 0;
}
