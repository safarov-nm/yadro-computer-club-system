cmake_minimum_required(VERSION 3.10)
project(ComputerClub)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options(/MD$<$<CONFIG:Debug>:d>)
endif()

add_executable(ComputerClub
    src/main.cpp
    src/computer_club.cpp
    src/time.cpp
    src/input_validator.cpp
)
target_include_directories(ComputerClub PUBLIC include)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(third_party/googletest)

add_executable(ComputerClubTests
    tests/test_time.cpp
    tests/test_input_validator.cpp
    tests/test_computer_club.cpp
    src/time.cpp
    src/input_validator.cpp       
    src/computer_club.cpp         
)

target_include_directories(ComputerClubTests PUBLIC include)
target_link_libraries(ComputerClubTests PRIVATE gtest_main)

enable_testing()
add_test(NAME ComputerClubTests COMMAND ComputerClubTests)