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

# if(EXISTS "/usr/local/lib/OGRE/cmake")

# 	set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
# 	set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

# elseif(EXISTS "/usr/lib/OGRE/cmake")

# 	set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
# 	set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

# else ()
# 	message(SEND_ERROR "Failed to find module path.")
# endif(EXISTS "/usr/local/lib/OGRE/cmake")

# find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)
 
# # add the source files as usual
# add_executable(0_Bootstrap Bootstrap.cpp)
 
# # this also sets the includes and pulls third party dependencies
# target_link_libraries(0_Bootstrap OgreBites)

set(JSONCPP_LIBS libjsoncpp.so)

target_link_libraries(${PROJECT_NAME} PRIVATE ${JSONCPP_LIBS} )

target_include_directories(${PROJECT_NAME} PUBLIC include)

set(SUBDIRECTORIES 
  external
  include
  libs
  src
  unit-tests
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()