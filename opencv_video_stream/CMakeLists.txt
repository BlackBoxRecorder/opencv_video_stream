
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cpp-mjpeg-streamer)

add_executable (opencv_video_stream "main.cpp"  "cvstream.h" "cvstream.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET opencv_video_stream PROPERTY CXX_STANDARD 20)
endif()

find_package(OpenCV REQUIRED)	
include_directories(${OpenCV_INCLUDE_DIRS})
# add_executable(${PROJECT_NAME} *.cpp)	
target_link_libraries(
    ${PROJECT_NAME} 
    ${OpenCV_LIBRARIES}
    nadjieb_mjpeg_streamer::nadjieb_mjpeg_streamer
)
