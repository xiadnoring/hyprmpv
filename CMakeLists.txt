cmake_minimum_required(VERSION 3.28)
project(hyprmpv)

set(CMAKE_CXX_STANDARD 26)

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Svg Quick Multimedia)

include(CheckFunctionExists)
check_function_exists(getopt HAVE_GETOPT)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME} src/HyprMpvMain.cpp
        includes/HyprMpvUtils.h
        src/HyprMpvUtils.cpp
        src/HyprMpvSetup.cpp
        includes/HyprMpvSetup.h
        src/HyprMpvConfig.cpp
        includes/HyprMpvConfig.h src/HyprMvpCreateConfig.cpp includes/HyprMvpCreateConfig.h)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Svg
        Qt6::Quick
        Qt6::Multimedia
)
target_link_libraries(${PROJECT_NAME} ${GETOPT_LIBRARY})

target_include_directories(${PROJECT_NAME} PRIVATE ./includes)
