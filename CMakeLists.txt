cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ICSV CXX)

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-DNDEBUG")

if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic )
endif()

add_library(${PROJECT_NAME} )
 
set(JSONCPP_LIBS libjsoncpp.so)

add_library(imgui STATIC
        # Main Imgui files
        external/imgui/imgui.cpp
        external/imgui/imgui.h
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/backends/imgui_impl_opengl3.h)

add_library(imgui::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC 
  external/imgui/
  external/imgui/backends/
  external/googletest/googletest/include
  external/googletest/googlemock/include

)

# set(IMGUI_INCLUDE_DIR "/usr/include/imgui")
# include_directories(${IMGUI_INCLUDE_DIR})
# set(IMGUI_LIBS libimgui.a)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  ${JSONCPP_LIBS}
  # ${IMGUI_LIBS}
)

target_include_directories(${PROJECT_NAME} PUBLIC include libs/evaluator_impls)

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