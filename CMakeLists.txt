cmake_minimum_required(VERSION 3.0)
project(jsonformatter)

set(CMAKE_CXX_STANDARD 17)
add_executable(jf jf.cpp json.hpp)

if(MSVC)
  target_compile_options(jf PRIVATE /W4 /WX)
else()
  target_compile_options(jf PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
