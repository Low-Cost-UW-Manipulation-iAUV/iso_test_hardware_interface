#!/bin/bash
echo "I'll copy all ADIS_Interface files over to the BBB"

rsync -avzh ./src/*.cpp 						BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/src/
rsync -avzh ./include/test_robot/*.hpp 			BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/include/test_robot/
rsync -avzh CMakeLists.txt 						BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.xml 								BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.launch	 						BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.yaml		 						BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/
rsync -avzh *.md								BBB_BRLWIFI:/home/ubuntu/uwesub_catkin_workspace/src/test_robot/

echo "All done, Good Success!"