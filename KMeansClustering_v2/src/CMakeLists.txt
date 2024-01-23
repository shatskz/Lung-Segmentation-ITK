CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

project (Final)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(Final Final.cxx)

target_link_libraries(Final ${ITK_LIBRARIES})
