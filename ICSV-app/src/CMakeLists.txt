set(ICSVAPP ICSVapp)
set(FILES
  app.cpp
  app.hpp
  entity_manager.cpp
  entity_manager.hpp
  icsv_entity.cpp
  icsv_entity.hpp
  main.cpp
)

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites Overlay CONFIG)

set(THIRD_PARTY_LIBS 
  ${OGRE_LIBRARIES}
)

add_executable(${ICSVAPP} ${FILES})

target_link_libraries(${ICSVAPP} PUBLIC
  ${PROJECT_NAME}
  ${THIRD_PARTY_LIBS}
)
