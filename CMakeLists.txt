cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(PROJECT_NAME ComputerClub)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/time.cpp 
    src/computer_club.cpp 
    src/input_validator.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)