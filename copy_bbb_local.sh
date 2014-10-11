#!/bin/bash
echo "I'll copy all ADIS_Interface files over to the BBB"

rsync -avzh ./src/*.cpp 						BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/src/
rsync -avzh ./include/test_robot/*.hpp 			BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/include/test_robot/
rsync -avzh CMakeLists.txt 						BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.xml 								BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.launch	 						BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.yaml		 						BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.md								BBB_local:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/

echo "All done, Good Success!"