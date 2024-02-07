set(ICSVAPP ICSVapp)
set(FILES
  app.cpp
  app.hpp
  chart.cpp
  chart.hpp
  entity_manager.cpp
  entity_manager.hpp
  icsv_entity.cpp
  icsv_entity.hpp
  main.cpp
  material_factory.cpp
  material_factory.hpp
  movable_text.cpp
  movable_text.hpp
  statistics.cpp
  statistics.hpp
)
set(SUBDIRECTORIES
  gui
)

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites Overlay CONFIG)

set(IMGUI_INCLUDE_DIR "/usr/include/imgui")
include_directories(${IMGUI_INCLUDE_DIR})
set(IMGUI_LIBS libimgui.a)

set(THIRD_PARTY_LIBS 
  ${OGRE_LIBRARIES}
  ${IMGUI_LIBS}
)

add_executable(${ICSVAPP} main.cpp)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${ICSVAPP} PUBLIC ${FILES})
endif()

target_link_libraries(${ICSVAPP} PUBLIC
  ${PROJECT_NAME}
  ${THIRD_PARTY_LIBS}
)
