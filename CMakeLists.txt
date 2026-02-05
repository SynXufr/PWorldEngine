cmake_minimum_required(VERSION 3.20)
project(PWorldEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# If it's in /lib, use: add_subdirectory(lib/raylib)
include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(PWorldEngine ${SOURCES} ${HEADERS}
        src/world/NoiseManager.cpp
        include/world/NoiseManager.h
        src/core/CameraManager.cpp
        include/core/CameraManager.h)

add_library(FastNoiseLite INTERFACE)

target_include_directories(PWorldEngine PUBLIC include)
target_include_directories(PWorldEngine PUBLIC include/external)

target_link_libraries(PWorldEngine PUBLIC raylib)
target_link_libraries(PWorldEngine PRIVATE FastNoiseLite)

# copy files to build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)