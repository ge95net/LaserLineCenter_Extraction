
# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("tracking")

#-------------------------------------
# opencv
#-------------------------------------
find_package(OpenCV REQUIRED PATHS C:/opencv/opencv/build)
include_directories(${OpenCV_INCLUDE_DIRS})


#-------------------------------------
# project name
#-------------------------------------
include_directories("${CMAKE_SOURCE_DIR}/src")
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src DIR_SRCS)
FILE(GLOB DIR_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
add_executable(${PROJECT_NAME} ${DIR_SRCS} ${DIR_HEARDERS}  "src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
#-------------------------------------
#set groups
#-------------------------------------
source_group("head files" FILES ${DIR_HEADERS})
source_group("source files" FILES ${DIR_SRCS})