/* Author: Raphael Nagel
	Desc: ros_control_interface for testing purpose of the ros_control_iso controller
	Date: 02/Sept/2014
*/

#ifndef _ROS_CONTROL_ISO_TEST_ROBOT_
#define _ROS_CONTROL_ISO_TEST_ROBOT_

#include <controller_manager/controller_manager.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>

#include "geometry_msgs/Vector3.h"
#include "test_robot/versatile_dynamics.hpp"
	

#include "ros/ros.h"
#include <sstream>
#include <stdio.h>

namespace test_robot
{

	class phoenix_test : public hardware_interface::RobotHW
	{
	public:
		phoenix_test();
		~phoenix_test();
		int read(void);
		int write(void);
		void update(const ros::TimerEvent&);
		void sub_callback1(const geometry_msgs::Vector3::ConstPtr&);
		void sub_callback2(const geometry_msgs::Vector3::ConstPtr&);

	private:
		int set_a_b_from_rosparam(void);
		// versatile_dynamics coefficients and object
		std::vector<double> coeff_b, coeff_a;
		versatile_dynamics dynamics_x;
		int simulation_rate;



		ros::NodeHandle nh_;

		hardware_interface::JointStateInterface joint_state_interface;		
		hardware_interface::EffortJointInterface jnt_eff_interface;

		boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;

		ros::Duration control_period_;
		ros::Duration elapsed_time_;
		double loop_hz_;

		ros::Timer non_realtime_loop_;

		double cmd[6];
		double pos[6];
		double vel[6];
		double eff[6];

		double state_x_position, state_x_velocity;

		ros::Publisher shoutout;
		ros::Subscriber subber1;
		ros::Subscriber subber2;


	};


}

#endif