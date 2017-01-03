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
set(CMAKE_CXX_STANDARD 11)
project( SetSolver )
find_package( OpenCV REQUIRED )
add_executable( SetSolver set_solver/main.cpp set_solver/PossibleCards.cpp set_solver/PossibleCards.hpp set_solver/Card.cpp set_solver/Card.hpp set_solver/Image.cpp set_solver/Image.hpp)
target_link_libraries( SetSolver ${OpenCV_LIBS} )
