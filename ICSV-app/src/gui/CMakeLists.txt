set(FILES 
  gui.cpp
  gui.hpp
  imguifilebrowser.hpp
)

if(FILES)
	target_sources(${ICSVAPP} PUBLIC ${FILES})
endif()