cmake_minimum_required(VERSION 3.10)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 14)
#find_package(imgui CONFIG)

include_directories(include)

#　
aux_source_directory(src  source_files)
aux_source_directory(imgui imgui_files)
#
add_executable(${PROJECT_NAME} ${imgui_files} ${source_files}  )
target_link_libraries(${PROJECT_NAME} glfw3 GL Xi X11 pthread dl assimp)