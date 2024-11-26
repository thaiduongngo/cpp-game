cmake_minimum_required(VERSION 3.31)

project(DragonQuest LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.6 COMPONENTS graphics window system network audio REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/Game.h
    src/CtrChar.h
    src/Cloud.h
    src/Clouds.h
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    "sfml-graphics"
    "sfml-audio"
)

# Copy the font to the build directory
file(COPY res/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)
