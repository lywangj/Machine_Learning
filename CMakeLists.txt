cmake_minimum_required(VERSION 3.0.0)
project(ML_PROJ VERSION 1.0.0)
project(ML_PROJ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NAME "GitHub_lywangj")
message("Thsi project has been created by ${NAME}.")

add_library(libraries
            src/data.cpp
            src/data_handler.cpp)

add_executable(main
                src/main.cpp)

target_include_directories(libraries PUBLIC include)
target_link_libraries(main PUBLIC libraries)