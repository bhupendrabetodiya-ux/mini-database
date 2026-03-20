cmake_minimum_required(VERSION 3.15)
project(SchoolDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(SchoolDB
    src/main.cpp
    src/Student.cpp
    src/Database.cpp
    src/MainFrame.cpp
)

target_include_directories(SchoolDB PRIVATE src)
target_link_libraries(SchoolDB ${wxWidgets_LIBRARIES})