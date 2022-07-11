set(FILES # Code-Smell detector extensions
	
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()