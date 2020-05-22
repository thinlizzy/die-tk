set(IMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)
add_definitions(-DIMAGE_DIR=${IMAGE_DIR})
add_executable(image-scaling
        imageScaling.cpp)

target_link_libraries(image-scaling die-tk die-tk-image)
