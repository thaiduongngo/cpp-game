cmake_minimum_required(VERSION 3.31)

project(DragonQuest LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.6 COMPONENTS graphics window system network audio REQUIRED)

set(SOURCES
    src/main.cpp
    src/Leaderboard.cpp
    src/Pipe.cpp
    src/Pipes.cpp
    src/Cloud.cpp
    src/Clouds.cpp
    src/CtrChar.cpp
    src/Game.cpp
)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC 
    "src/include"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    "sfml-graphics"
    "sfml-audio"
)

# Copy the font to the build directory
file(COPY res/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)
