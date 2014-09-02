#include <controller_manager/controller_manager.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <stdio.h>

#include <test_robot/test_robot.hpp>

namespace test_robot{
	phoenix_test::phoenix_test(){

		///set the output to 0
		cmd[0] = 0;
		cmd[1] = 0;
		cmd[2] = 0;
		cmd[3] = 0;
		cmd[4] = 0;
		cmd[5] = 0;

		///connect and register the joint state interface
		hardware_interface::JointStateHandle state_handle_x("x", &pos[0], &vel[0], &eff[0]);
		joint_state_interface.registerHandle(state_handle_x);	

		registerInterface(&joint_state_interface);


		///connect and register the joint position interface
		hardware_interface::JointHandle pos_handle_x(joint_state_interface.getHandle("x"), &cmd[0]);
		jnt_eff_interface.registerHandle(pos_handle_x);

		registerInterface(&jnt_eff_interface);	


		///Advertise the status topic
		shoutout = nh_.advertise<std_msgs::String>("test_robot", 100);
		

		///Initialise the controller manager
		ROS_INFO("test_robot: Loading the controller manager");
		controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));

		/// Get the required variables from the parameter server and set standard values if not available
		if (!nh_.getParam("/ros_control_iso/parameters/update_rate", loop_hz_)){
			  
			ROS_ERROR("test_robot: Could not find update rate, assuming 50. \n");
			loop_hz_ = 50;
			nh_.setParam("/ros_control_iso/parameters/update_rate", loop_hz_);
		}		

		///Set up the control loop by creating a timer and a connected callback
		ros::Duration update_freq = ros::Duration(1.0/loop_hz_);
		non_realtime_loop_ = nh_.createTimer(update_freq, &phoenix_test::update, this);

		/// Reset the joint state
		state_x_position = 0;
		state_x_velocity = 0;
	}

	phoenix_test::~phoenix_test(){}

	/** Update(): read the current joint status, run the controller and update the actuator output
	*/
	void phoenix_test::update(const ros::TimerEvent& event){
		/// Update the time since the last update
		elapsed_time_ = ros::Duration(event.current_real - event.last_real);

		/// Read the current status
		read();

		/// Let the controller do its work
		controller_manager_->update(ros::Time::now(), elapsed_time_);

		// Write the new command to the motor drivers
		write();
	}

	/** Read(): testing - increment the joint state
	*		For testing purpose we only increment the joint state. In real this would read relevant data from sensors or encoders.
	*/
	int phoenix_test::read(void){
		state_x_velocity ++;
		state_x_position ++;

	}

	/** Write(): testing - publish the control output
	*		For testing purpose we only publish the output, in real this would change the PWMs of the motor, etc.
	*/
	int phoenix_test::write(){

		std_msgs::String msg;
		std::stringstream ss;
		ss << "test_robot: cc= " << cmd[0] << ", out= " << state_x_position; 
		msg.data = ss.str();

		shoutout.publish(msg);
	}
} //namespace



int main(int argc, char **argv){
	ros::init(argc, argv, "test_robot");
	ros::NodeHandle nh;

	/// An Async spinner creates another thread which will handle the event of this node being executed.
	ros::AsyncSpinner spinner(4);
	spinner.start();
	test_robot::phoenix_test robot;
	ros::spin();

	ROS_INFO("test_robot: Shutting down hardware interface");

}














