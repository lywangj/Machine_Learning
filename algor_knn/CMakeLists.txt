set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NAME "GitHub_lywangj")
message("Thsi project has been created by ${NAME}.")

add_library(library
            src/knn.cpp)

target_include_directories(library PUBLIC include)