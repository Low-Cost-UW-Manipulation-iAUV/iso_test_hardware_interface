/* Author: Raphael Nagel
	Desc: ros_control_interface for testing purpose of the ros_control_iso controller
	Date: 02/Sept/2014
*/
#include <controller_manager/controller_manager.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>

#include "ros/ros.h"
#include <sstream>
#include <stdio.h>

#include <test_robot/test_robot.hpp>

namespace test_robot{
	phoenix_test::phoenix_test(){
		///set the controller output to 0
		cmd[0] = 0.0;
		cmd[1] = 0.0;
		cmd[2] = 0.0;
		cmd[3] = 0.0;
		cmd[4] = 0.0;
		cmd[5] = 0.0;

		// and the controller input
		pos[0] = 0.0;
		pos[1] = 0.0;
		pos[2] = 0.0;
		pos[3] = 0.0;
		pos[4] = 0.0;
		pos[5] = 0.0;

		/// connect and register the joint state interface for the x DOF
		hardware_interface::JointStateHandle state_handle_x("x", &pos[0], &vel[0], &eff[0]);
		joint_state_interface.registerHandle(state_handle_x);	
        
        /// connect and register the joint state interface for the y DOF
        hardware_interface::JointStateHandle state_handle_y("yaw", &pos[1], &vel[1], &eff[1]);
        joint_state_interface.registerHandle(state_handle_y); 
		
        /// register the joint_state_interface
        registerInterface(&joint_state_interface);

		/// connect and register the joint position interface 
		hardware_interface::JointHandle pos_handle_x(joint_state_interface.getHandle("x"), &cmd[0]);
		jnt_eff_interface.registerHandle(pos_handle_x);

        /// connect and register the joint position interface
        hardware_interface::JointHandle pos_handle_y(joint_state_interface.getHandle("yaw"), &cmd[1]);
        jnt_eff_interface.registerHandle(pos_handle_y);

        /// register the joint effort interface
		registerInterface(&jnt_eff_interface);	

		


		///Advertise the status topic
		shoutout = nh_.advertise<geometry_msgs::Vector3>("test_robot/out", 100);
		subber1 = nh_.subscribe<geometry_msgs::Vector3>("test_robot/in1",20, &phoenix_test::sub_callback1, this);
		subber2 = nh_.subscribe<geometry_msgs::Vector3>("test_robot/in2",20, &phoenix_test::sub_callback2, this);

		///Initialise the controller manager
		ROS_INFO("test_robot: Loading the controller manager");
		controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));

		/// Get the required variables from the parameter server and set standard values if not available
		if (!nh_.getParam("/ros_control_iso/relay_with_hysteresis/publish_rate", loop_hz_)){
			  
			ROS_ERROR("test_robot: Could not find update rate, assuming 50. \n");
			loop_hz_ = 50;
			nh_.setParam("/ros_control_iso/relay_with_hysteresis/publish_rate", loop_hz_);
		}	

		/// setup the simulator
		set_a_b_from_rosparam();			

		/// Set up the control loop by creating a timer and a connected callback
		ros::Duration update_freq = ros::Duration(1.0/loop_hz_);
		non_realtime_loop_ = nh_.createTimer(update_freq, &phoenix_test::update, this);

		/// Reset the joint state
		state_x_position = 0;
		state_x_velocity = 0;
	}

	phoenix_test::~phoenix_test() {}


	void phoenix_test::sub_callback1(const geometry_msgs::Vector3::ConstPtr& message) {
		pos[0] = message->x;
		pos[1] = message->y;
		pos[2] = message->z;
	}
	void phoenix_test::sub_callback2(const geometry_msgs::Vector3::ConstPtr& message) {
		pos[3] = message->x;
		pos[4] = message->y;
		pos[5] = message->z;
	}
	/** Update(): read the current joint status, run the controller and update the actuator output
	*/
	void phoenix_test::update(const ros::TimerEvent& event) {
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
	*	This does read the current state of the dynamics simulation.
	*/
	int phoenix_test::read(void) {

		pos[0] = dynamics_x.get_out();
		pos[1] = dynamics_y.get_out();

	}

	/** Write(): testing - publish the control output
	*		For testing purpose we only publish the output, in real this would change the PWMs of the motor, etc.
	*	This inputs the newest data into the simulator and calculates the output.
	*/
	int phoenix_test::write() {

		dynamics_x.put_in(cmd[0], (simulation_rate / loop_hz_));
		dynamics_y.put_in(cmd[1], (simulation_rate / loop_hz_));

		geometry_msgs::Vector3 msg;
		msg.x = cmd[0];
		msg.y = pos[0];
		msg.z = 0;

		shoutout.publish(msg);
	}


	int phoenix_test::set_a_b_from_rosparam(void) {
		double vector_size;
		/// get the dynamics parameters
		if (!nh_.getParam("/ros_control_iso/simulation/x/vector_size", vector_size)) {

			ROS_ERROR("test_robot: Could not find vector_size\n");
			return EXIT_FAILURE;
		}

		/// prepare the vector to hold them
		coeff_b.resize(vector_size);
		coeff_a.resize(vector_size);

		if (!nh_.getParam("/ros_control_iso/simulation/x/coeff_b", coeff_b)) {
			  
			ROS_ERROR("test_robot: Could not find coeff_b\n");
			return EXIT_FAILURE;
		}
		if (!nh_.getParam("/ros_control_iso/simulation/x/coeff_a", coeff_a)) {
			  
			ROS_ERROR("test_robot: Could not find coeff_a\n");
			return EXIT_FAILURE;
		}

		dynamics_x.reset(coeff_b, coeff_a);
		dynamics_y.reset(coeff_b, coeff_a);

		if (!nh_.getParam("/ros_control_iso/simulation/x/simulation_rate", simulation_rate)) {
			  
			ROS_ERROR("test_robot: Could not find simulation_rate\n");
			return EXIT_FAILURE;
		}

	}

} // namespace



int main(int argc, char **argv) {
	ros::init(argc, argv, "test_robot");
	ros::NodeHandle nh;

	/// An Async spinner creates another thread which will handle the event of this node being executed.
	ros::AsyncSpinner spinner(4);
	spinner.start();
	test_robot::phoenix_test robot;
	ros::spin();

	ROS_INFO("test_robot: Shutting down hardware interface");

}
