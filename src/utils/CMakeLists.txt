set(FILES 
  utils.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()