CMAKE_MINIMUM_REQUIRED (VERSION 3.22)
project (ImagingProject)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable (ImagingProject ImagingProject.cxx)

target_link_libraries(ImagingProject ${ITK_LIBRARIES})
