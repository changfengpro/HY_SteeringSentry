/*
 * @Description: 
 * @Author: changfeng
 * @brief: 
 * @version: 
 * @Date: 2025-01-22 14:07:05
 * @LastEditors:  
 * @LastEditTime: 2025-02-01 20:23:29
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
    // 输出节点启动信息
    RCLCPP_INFO(this->get_logger(), "CmdVelSender node started");
    // 创建串口对象
    s1.OpenSerial("/dev/ttyCH343USB0", E_BaudRate::_115200, E_DataSize::_8, E_Parity::None, E_StopBit::_1);

    // 订阅cmd_vel消息
    cmd_vel_sub_ = this->create_subscription<Twist>(
      "cmd_vel", 10, std::bind(&CmdVelSender::cmd_vel_callback, this, std::placeholders::_1));

     // 订阅新的 turtle1/teleop_key 话题，修改为小乌龟相关的控制话题
    turtle_teleop_key_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "turtle1/cmd_vel", 10, std::bind(&CmdVelSender::turtle_teleop_key_callback, this, std::placeholders::_1));
  }

private:
  // 串口对象
  Serial s1;

  // 订阅者对象
  rclcpp::Subscription<Twist>::SharedPtr cmd_vel_sub_;

  // 处理cmd_vel消息的回调函数
  void cmd_vel_callback(const Twist::SharedPtr msg)
  {
    // 输出接收到的消息
    RCLCPP_INFO(this->get_logger(), "Received cmd_vel: Linear X: %f, Angular Z: %f", msg->linear.x, msg->angular.z);
    // 获取消息中的线速度和角速度
    // float linear_x = msg->linear.x;
    // float angular_z = msg->angular.z;

    // 创建数据，将线速度和角速度转换为字符串或其他适合的格式
    // std::string serial_data = "Linear X: " + std::to_string(linear_x) + ", Angular Z: " + std::to_string(angular_z);

    // 发送数据
    // s1.Send(serial_data.data(), serial_data.size());
  }

  // turtle_teleop_key 回调函数
//   void turtle_teleop_key_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
//   {
//     // 输出接收到的小乌龟控制信息
//     RCLCPP_INFO(this->get_logger(), "Received turtle_teleop_key: Linear x: %f, Angular z: %f", msg->linear.x, msg->angular.z);
//  // 获取消息中的线速度和角速度
//     float linear_x = msg->linear.x;
//     float angular_z = msg->angular.z;

//     // 创建数据，将线速度和角速度转换为字符串或其他适合的格式
//     std::string serial_data = "Linear X: " + std::to_string(linear_x) + ", Angular Z: " + std::to_string(angular_z);
//     // 发送数据
//     s1.Send(serial_data.data(), serial_data.size());

//     // 处理来自 turtle_teleop_key 的消息（例如发送到串口）
//   }
    void turtle_teleop_key_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
    // 输出接收到的小乌龟控制信息
    // RCLCPP_INFO(this->get_logger(), "Received turtle_teleop_key: Linear x: %f, Angular z: %f", msg->linear.x, msg->angular.z);

    // 获取消息中的线速度和角速度
    float linear_x = msg->linear.x;
    float angular_z = msg->angular.z;

    // 创建数据包（协议格式：[Start Byte] [linear_x (4 bytes)] [angular_z (4 bytes)] [Checksum (1 byte)] [End Byte]）
    uint8_t data_packet[11]; // 数据包大小
    data_packet[0] = 0xAA; // Start Byte

    // 将 linear_x 和 angular_z 转换为字节流（float -> 4 字节）
    memcpy(&data_packet[1], &linear_x, sizeof(float));  // 将 linear_x 存入字节流
    memcpy(&data_packet[5], &angular_z, sizeof(float)); // 将 angular_z 存入字节流

    // 计算校验和（按字节异或）
    uint8_t checksum = 0;
    for (int i = 0; i < 9; i++) {
        checksum ^= data_packet[i];  // 对前9个字节异或计算校验和
    }
    data_packet[9] = checksum; // 存储校验和

    // 结束字节
    data_packet[10] = 0x55; // End Byte

    // 发送数据包
    s1.Send(data_packet, sizeof(data_packet)); // 通过串口发送数据包

    // 输出调试信息
    // RCLCPP_INFO(this->get_logger(), "Sending data: Linear X: %f, Angular Z: %f", linear_x, angular_z);
     std::ostringstream oss;
    oss << "Sending data packet: ";
    for (int i = 0; i < sizeof(data_packet); i++)
    {
        oss << std::hex << std::uppercase << (int)data_packet[i] << " ";
    }
    RCLCPP_INFO(this->get_logger(), "%s", oss.str().c_str());


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
