cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ICSV CXX)

if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic )
endif()

add_library(${PROJECT_NAME} )
 
set(JSONCPP_LIBS libjsoncpp.so)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  ${JSONCPP_LIBS}
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

set(SUBDIRECTORIES 
  ICSV-app
  external
  include
  libs
  src
  unit-tests
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()