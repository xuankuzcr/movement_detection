project(judge_movement)
SET( CMAKE_CXX_FLAGS "-std=c++11 -O3")
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
find_package( OpenCV REQUIRED )
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS}  )
target_link_libraries( main ${OpenCV_LIBS} )
