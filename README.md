iso_test_hardware_interface
===========================
This is a hardware interface for testing a ros_control effort interface controller.

It does nothing other than publish the demanded thrust on /test_robot/out when updating.
This will soon turn to be 2 topics to cater for 5/6 thrusters

It subscribes to 2 topics: /test_robot/in1 and /test_robot/in2
These two take geometry::msgs and 1(x,y,z) 2(x,y,z) map to pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]
This encodes the position feedback from an external (matlab?!) simulation.

If Matlab ros permits I might use the std_msgs::Float32MultiArray
