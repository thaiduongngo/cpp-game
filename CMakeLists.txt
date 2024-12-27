cmake_minimum_required(VERSION 3.31)

project(DragonQuest LANGUAGES CXX VERSION 1.0.1)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 COMPONENTS Graphics Window System Network Audio REQUIRED)

file(GLOB SOURCES
     "src/*.cpp"
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
    SFML::Graphics
    SFML::Audio
)

# Copy the font to the build directory
file(COPY res/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)
