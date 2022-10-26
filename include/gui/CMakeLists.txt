set(FILES 
  gui.cpp
  gui.hpp
  imguifilebrowser.hpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()