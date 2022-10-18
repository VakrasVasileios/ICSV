set(FILES 
  utils.hpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()