cmake_minimum_required(VERSION 2.8.3)
project(test_robot)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS 
	roscpp
	rospy
	geometry_msgs
	message_generation
	controller_manager
	hardware_interface )

generate_messages(
	DEPENDENCIES
	geometry_msgs
	)


## Declare a catkin package
catkin_package(
	 CATKIN_DEPENDS 
	 roscpp
	 controller_manager
	 hardware_interface
	 INCLUDE_DIRS
	 )

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(test_robot src/test_robot.cpp)
target_link_libraries(test_robot ${catkin_LIBRARIES})
add_dependencies(test_robot geometry_msgs_generate_messages_cpp)
