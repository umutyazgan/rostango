cmake_minimum_required(VERSION 2.8.3)
project(online_tempo)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs rosbag tf message_generation 
    #message_filters
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

#######################################
## Declare ROS messages and services ##
#######################################

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  frame_message.msg
#  frame_message_multi.msg
  joint_message.msg
  frame_message_burst.msg
  scale_and_cast.msg
  imitated_tempo_message.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES beginner_tutorials
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
#  DEPENDS boost
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp library
# add_library(beginner_tutorials
#   src/${PROJECT_NAME}/beginner_tutorials.cpp
# )

## Declare a cpp executable
# add_executable(beginner_tutorials_node src/beginner_tutorials_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
#add_dependencies(abominationBroadcast msg_pkg_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(beginner_tutorials_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS beginner_tutorials beginner_tutorials_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )
# check c++11 / c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_beginner_tutorials.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)

#include_directories(include ${catkin_INCLUDE_DIRS})

#add_executable(harkMessage src/harkMessage.cpp)
#target_link_libraries(harkMessage ${catkin_LIBRARIES})

#add_executable(jointState src/jointState.cpp)
#target_link_libraries(jointState ${catkin_LIBRARIES})

add_executable(tempoMatch src/tempoMatch.cpp)
target_link_libraries(tempoMatch ${catkin_LIBRARIES})

add_executable(broadcast src/broadcast.cpp)
target_link_libraries(broadcast ${catkin_LIBRARIES})

#add_executable(broadcastPepper src/broadcastPepper.cpp)
#target_link_libraries(broadcastPepper ${catkin_LIBRARIES})

#add_executable(abominationBroadcast src/abominationBroadcast.cpp)
#target_link_libraries(abominationBroadcast ${catkin_LIBRARIES})

#add_executable(tempoMatchCustom src/tempoMatchCustom.cpp)
#target_link_libraries(tempoMatchCustom ${catkin_LIBRARIES})

#add_executable(elbowRotate src/elbowRotate.cpp)
#target_link_libraries(elbowRotate ${catkin_LIBRARIES})

#add_executable(naoHumanMapped src/naoHumanMapped.cpp)
#target_link_libraries(naoHumanMapped ${catkin_LIBRARIES})

#add_executable(naoIKTest src/naoKinematics/naoIKTest.cpp src/naoKinematics/NAOKinematics.cpp src/naoKinematics/NAOKinematicsJacobian.cpp )
#target_link_libraries(naoIKTest ${catkin_LIBRARIES})

#add_executable(elbowRotateKinematic src/elbowRotateKinematic.cpp src/naoKinematics/NAOKinematics.cpp src/naoKinematics/NAOKinematicsJacobian.cpp )
#target_link_libraries(elbowRotateKinematic ${catkin_LIBRARIES})

#add_executable(broadcastPrimitive src/broadcastPrimitive.cpp src/broadcastPrimitive.cpp  )
#target_link_libraries(broadcastPrimitive ${catkin_LIBRARIES})



#add_executable(broadcast_dance src/broadcast_dance.cpp)
#target_link_libraries(broadcast_dance ${catkin_LIBRARIES})

add_executable(imitated_tempo_publisher src/imitated_tempo_publisher.cpp)
target_link_libraries(imitated_tempo_publisher ${catkin_LIBRARIES})
add_dependencies(imitated_tempo_publisher online_tempo_generate_messages_cpp)

add_executable(imitated_tempo_input src/imitated_tempo_input.cpp)
target_link_libraries(imitated_tempo_input ${catkin_LIBRARIES})
add_dependencies(imitated_tempo_input online_tempo_generate_messages_cpp)
