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

target_include_directories(${PROJECT_NAME} PUBLIC include)

set(SUBDIRECTORIES 
  # external
  include
  libs
  src
  unit-tests
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

# set(ALLEGRO_LIBRARIES 
# 	liballegro.so liballegro_dialog.so liballegro_image.so liballegro_color.so
# 	liballegro_audio.so liballegro_physfs.so liballegro_font.so 
# 	liballegro_acodec.so liballegro_main.so liballegro_memfile.so
# 	liballegro_primitives.so liballegro_ttf.so
# )


target_link_libraries(${PROJECT_NAME} PUBLIC ${ALLEGRO_LIBRARIES})