cmake_minimum_required(VERSION 3.10)

project(Thresholding)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(Thresholding thresholding.cxx)

target_link_libraries(Thresholding ${ITK_LIBRARIES})
