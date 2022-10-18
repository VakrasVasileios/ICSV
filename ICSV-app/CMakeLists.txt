set(ICSV-APP ICSV-app)

add_executable(${ICSV-APP} app.cpp)
target_link_libraries(${ICSV-APP} ${OGRE_LIBRARIES})