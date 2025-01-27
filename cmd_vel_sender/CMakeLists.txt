cmake_minimum_required(VERSION 3.10)
project(cmd_vel_sender)

# 查找 ROS 2 依赖项
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/include/cmd_vel_sender/*.cpp)

# 添加可执行文件
add_executable(cmd_vel_publisher ${SRC} src/cmd_vel_publisher.cpp)
add_executable(cmd_vel_subscribe ${SRC} src/cmd_vel_subscribe.cpp)

target_link_libraries(cmd_vel_publisher udev)
target_link_libraries(cmd_vel_subscribe udev)

include_directories("/home/rmer/ros2/sentry/cmd_vel_sender/include/cmd_vel_sender")


# 链接 ROS 2 库
ament_target_dependencies(cmd_vel_publisher rclcpp geometry_msgs)
ament_target_dependencies(cmd_vel_subscribe rclcpp geometry_msgs)

# 安装可执行文件
install(TARGETS
cmd_vel_publisher
  DESTINATION lib/${PROJECT_NAME}
)
install(TARGETS
cmd_vel_subscribe
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
