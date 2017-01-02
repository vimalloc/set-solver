#cmake_minimum_required(VERSION 3.6)
#project(Set)
#
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c")
#find_package( OpenCV HINTS /usr/local/opt/opencv3 )
#
#set(SOURCE_FILES main.cpp)
#add_executable(Set ${SOURCE_FILES})

cmake_minimum_required(VERSION 3.6)
project( Set )
find_package( OpenCV REQUIRED )
add_executable( SetSolver SetSolver.cpp )
target_link_libraries( SetSolver ${OpenCV_LIBS} )
