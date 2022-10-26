set(ICSVAPP ICSVapp)
set(FILES
  app.cpp
  render_smells.cpp
  render_smells.hpp
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
